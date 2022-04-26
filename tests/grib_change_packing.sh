#!/bin/sh

# --- check nothing bombs when changing packing
# ---  for now limited to just a few packing schemes

. ./include.ctest.sh

label="grib_change_packing_test"
temp=temp.$label.grib
rm -f $temp

grib1=${data_dir}/reduced_latlon_surface_constant.grib1
grib2=${data_dir}/reduced_latlon_surface_constant.grib2

packing1="
    grid_ieee
    grid_simple
    grid_simple_matrix"

packing2="
    grid_ieee
    grid_simple
    grid_simple_matrix
    grid_simple_log_preprocessing"

if [ $HAVE_JPEG -eq 1 ]; then
    packing2="grid_jpeg "$packing2
fi


# --- test changing the packing
# arg 1    : input grib file
# arg 2-n  : packing types to test
test_packing() {
    grib=$1

    if [ ! -f $grib ]; then
        echo "Input data missing"
        exit 1
    fi

    shift
    while [ $# -gt 0 ]; do
        packing=$1

        ${tools_dir}/grib_set -r -s packingType=$packing $grib $temp
        result=`${tools_dir}/grib_get -p packingType $temp`

        if [ "$result" != "$packing" ]; then
            echo "Setting packing did not go right"
            exit 1
        fi

        result=`${tools_dir}/grib_get -p accuracy $temp`
        [ $result -eq 0 -o $result -eq 32 -o $result -eq 64 ]

        shift
    done

    rm -f $temp
}

# ECC-774
# ------------
input=${data_dir}/spherical_model_level.grib2
output=`${tools_dir}/grib_set -r -s packingType=spectral_simple $input $temp 2>&1`
grib_check_key_equals $temp packingType 'spectral_simple'
grib_check_key_equals $temp accuracy 16
# Check no error was posted i.e. output string is empty
[ -z "$output" ]
res1=`${tools_dir}/grib_get '-F%.1f' -p avg,enorm $input`
res2=`${tools_dir}/grib_get '-F%.1f' -p avg,enorm $temp`
[ "$res1" = "$res2" ]
rm -f $temp


# IEEE to Simple Packing
# -----------------------
input=${data_dir}/grid_ieee.grib
${tools_dir}/grib_set -r -s packingType=grid_simple $input $temp
grib_check_key_equals $input accuracy 32
grib_check_key_equals $temp packingType 'grid_simple'
grib_check_key_equals $temp accuracy 24
stats1=`${tools_dir}/grib_get -F%.2f -p skew,kurt $input`
stats2=`${tools_dir}/grib_get -F%.2f -p skew,kurt $temp`
[ "$stats1" = "$stats2" ]


# Test 'accuracy' key
# -----------------------
input=${data_dir}/reduced_gaussian_model_level.grib1
${tools_dir}/grib_set -r -s packingType=grid_ieee,accuracy=64 $input $temp
grib_check_key_equals $temp precision '2'
stats1=`${tools_dir}/grib_get -F%.2f -p skew,kurt $input`
stats2=`${tools_dir}/grib_get -F%.2f -p skew,kurt $temp`
[ "$stats1" = "$stats2" ]


# Change grib packing test
# -------------------------
test_packing $grib1 $packing1
test_packing $grib2 $packing2


# spectral_simple for GRIB1
# -------------------------
input=${data_dir}/spherical_model_level.grib1
${tools_dir}/grib_set -r -s packingType=spectral_simple $input $temp
grib_check_key_equals $temp packingType 'spectral_simple'
${tools_dir}/grib_ls -p numberOfCodedValues $temp
${tools_dir}/grib_ls -n statistics $temp
stats=`${tools_dir}/grib_get -F%.1f -p average,standardDeviation $temp`
[ "$stats" = "195.1 12.0" ]


# ECC-1352: Check CCSDS
# ----------------------------
temp_err=temp.$label.err
if [ $HAVE_AEC -eq 0 ]; then
    # Check we get an error if we try to decode this packing
    [ -f "${data_dir}/ccsds.grib2" ]
    set +e
    ${tools_dir}/grib_get -p min,max ${data_dir}/ccsds.grib2 2>$temp_err
    status=$?
    set -e
    [ $status -ne 0 ]
    grep -q "CCSDS support not enabled. Please rebuild with -DENABLE_AEC=ON" $temp_err
fi


rm -f $temp $temp_err
