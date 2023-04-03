/*
 * (C) Copyright 2005- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 *
 * In applying this licence, ECMWF does not waive the privileges and immunities granted to it by
 * virtue of its status as an intergovernmental organisation nor does it submit to any jurisdiction.
 */

#include "grib_api_internal.h"

/*
   This is used by make_class.pl

   START_CLASS_DEF
   CLASS      = accessor
   SUPER      = grib_accessor_class_data_g1simple_packing
   IMPLEMENTS = init
   IMPLEMENTS = unpack_double
   IMPLEMENTS = value_count
   IMPLEMENTS = pack_double
   MEMBERS=const char*  missing_value
   MEMBERS=const char*  numberOfPoints
   MEMBERS=const char*  bitmap
   END_CLASS_DEF

 */

/* START_CLASS_IMP */

/*

Don't edit anything between START_CLASS_IMP and END_CLASS_IMP
Instead edit values between START_CLASS_DEF and END_CLASS_DEF
or edit "accessor.class" and rerun ./make_class.pl

*/

static int pack_double(grib_accessor*, const double* val, size_t* len);
static int unpack_double(grib_accessor*, double* val, size_t* len);
static int value_count(grib_accessor*, long*);
static void init(grib_accessor*, const long, grib_arguments*);
static void init_class(grib_accessor_class*);

typedef struct grib_accessor_data_dummy_field
{
    grib_accessor att;
    /* Members defined in gen */
    /* Members defined in values */
    int  carg;
    const char* seclen;
    const char* offsetdata;
    const char* offsetsection;
    int dirty;
    /* Members defined in data_simple_packing */
    int edition;
    const char*  units_factor;
    const char*  units_bias;
    const char*  changing_precision;
    const char*  number_of_values;
    const char*  bits_per_value;
    const char*  reference_value;
    const char*  binary_scale_factor;
    const char*  decimal_scale_factor;
    const char*  optimize_scaling_factor;
    /* Members defined in data_g1simple_packing */
    const char*  half_byte;
    const char*  packingType;
    const char*  ieee_packing;
    const char*  precision;
    /* Members defined in data_dummy_field */
    const char*  missing_value;
    const char*  numberOfPoints;
    const char*  bitmap;
} grib_accessor_data_dummy_field;

extern grib_accessor_class* grib_accessor_class_data_g1simple_packing;

static grib_accessor_class _grib_accessor_class_data_dummy_field = {
    &grib_accessor_class_data_g1simple_packing,                      /* super */
    "data_dummy_field",                      /* name */
    sizeof(grib_accessor_data_dummy_field),  /* size */
    0,                           /* inited */
    &init_class,                 /* init_class */
    &init,                       /* init */
    0,                  /* post_init */
    0,                    /* free mem */
    0,                       /* describes himself */
    0,                /* get length of section */
    0,              /* get length of string */
    &value_count,                /* get number of values */
    0,                 /* get number of bytes */
    0,                /* get offset to bytes */
    0,            /* get native type */
    0,                /* get sub_section */
    0,               /* grib_pack procedures long */
    0,                 /* grib_pack procedures long */
    0,                  /* grib_pack procedures long */
    0,                /* grib_unpack procedures long */
    &pack_double,                /* grib_pack procedures double */
    0,                 /* grib_pack procedures float */
    &unpack_double,              /* grib_unpack procedures double */
    0,               /* grib_unpack procedures float */
    0,                /* grib_pack procedures string */
    0,              /* grib_unpack procedures string */
    0,          /* grib_pack array procedures string */
    0,        /* grib_unpack array procedures string */
    0,                 /* grib_pack procedures bytes */
    0,               /* grib_unpack procedures bytes */
    0,            /* pack_expression */
    0,              /* notify_change */
    0,                /* update_size */
    0,             /* preferred_size */
    0,                     /* resize */
    0,      /* nearest_smaller_value */
    0,                       /* next accessor */
    0,                    /* compare vs. another accessor */
    0,      /* unpack only ith value */
    0,       /* unpack only ith value */
    0,  /* unpack a given set of elements */
    0,   /* unpack a given set of elements */
    0,     /* unpack a subarray */
    0,                      /* clear */
    0,                 /* clone accessor */
};


grib_accessor_class* grib_accessor_class_data_dummy_field = &_grib_accessor_class_data_dummy_field;


static void init_class(grib_accessor_class* c)
{
    c->dump    =    (*(c->super))->dump;
    c->next_offset    =    (*(c->super))->next_offset;
    c->string_length    =    (*(c->super))->string_length;
    c->byte_count    =    (*(c->super))->byte_count;
    c->byte_offset    =    (*(c->super))->byte_offset;
    c->get_native_type    =    (*(c->super))->get_native_type;
    c->sub_section    =    (*(c->super))->sub_section;
    c->pack_missing    =    (*(c->super))->pack_missing;
    c->is_missing    =    (*(c->super))->is_missing;
    c->pack_long    =    (*(c->super))->pack_long;
    c->unpack_long    =    (*(c->super))->unpack_long;
    c->pack_float    =    (*(c->super))->pack_float;
    c->unpack_float    =    (*(c->super))->unpack_float;
    c->pack_string    =    (*(c->super))->pack_string;
    c->unpack_string    =    (*(c->super))->unpack_string;
    c->pack_string_array    =    (*(c->super))->pack_string_array;
    c->unpack_string_array    =    (*(c->super))->unpack_string_array;
    c->pack_bytes    =    (*(c->super))->pack_bytes;
    c->unpack_bytes    =    (*(c->super))->unpack_bytes;
    c->pack_expression    =    (*(c->super))->pack_expression;
    c->notify_change    =    (*(c->super))->notify_change;
    c->update_size    =    (*(c->super))->update_size;
    c->preferred_size    =    (*(c->super))->preferred_size;
    c->resize    =    (*(c->super))->resize;
    c->nearest_smaller_value    =    (*(c->super))->nearest_smaller_value;
    c->next    =    (*(c->super))->next;
    c->compare    =    (*(c->super))->compare;
    c->unpack_double_element    =    (*(c->super))->unpack_double_element;
    c->unpack_float_element    =    (*(c->super))->unpack_float_element;
    c->unpack_double_element_set    =    (*(c->super))->unpack_double_element_set;
    c->unpack_float_element_set    =    (*(c->super))->unpack_float_element_set;
    c->unpack_double_subarray    =    (*(c->super))->unpack_double_subarray;
    c->clear    =    (*(c->super))->clear;
    c->make_clone    =    (*(c->super))->make_clone;
}

/* END_CLASS_IMP */

static void init(grib_accessor* a, const long v, grib_arguments* args)
{
    grib_accessor_data_dummy_field* self = (grib_accessor_data_dummy_field*)a;
    self->missing_value                  = grib_arguments_get_name(grib_handle_of_accessor(a), args, self->carg++);
    self->numberOfPoints                 = grib_arguments_get_name(grib_handle_of_accessor(a), args, self->carg++);
    self->bitmap                         = grib_arguments_get_name(grib_handle_of_accessor(a), args, self->carg++);
}

static int unpack_double(grib_accessor* a, double* val, size_t* len)
{
    grib_accessor_data_dummy_field* self = (grib_accessor_data_dummy_field*)a;
    size_t i                             = 0;
    size_t n_vals                        = 0;
    long numberOfPoints;
    double missing_value = 0;
    int err              = 0;

    if ((err = grib_get_long_internal(grib_handle_of_accessor(a), self->numberOfPoints, &numberOfPoints)) != GRIB_SUCCESS)
        return err;
    n_vals = numberOfPoints;

    if ((err = grib_get_double_internal(grib_handle_of_accessor(a), self->missing_value, &missing_value)) != GRIB_SUCCESS)
        return err;

    if (*len < n_vals) {
        *len = n_vals;
        return GRIB_ARRAY_TOO_SMALL;
    }

    for (i = 0; i < n_vals; i++)
        val[i] = missing_value;

    if (grib_find_accessor(grib_handle_of_accessor(a), self->bitmap)) {
        if ((err = grib_set_double_array_internal(grib_handle_of_accessor(a), self->bitmap, val, n_vals)) != GRIB_SUCCESS)
            return err;
    }

    *len = (long)n_vals;
    return err;
}

static int pack_double(grib_accessor* a, const double* val, size_t* len)
{
    grib_accessor_data_dummy_field* self = (grib_accessor_data_dummy_field*)a;


    size_t n_vals = *len;
    int err       = 0;

    long bits_per_value = 0;

    long half_byte = 0;

    size_t buflen      = 0;
    unsigned char* buf = NULL;

    if (*len == 0)
        return GRIB_NO_VALUES;

    if ((err = grib_get_long_internal(grib_handle_of_accessor(a), self->bits_per_value, &bits_per_value)) != GRIB_SUCCESS)
        return err;

    buflen = (1 + ((bits_per_value * n_vals) / 8)) * sizeof(unsigned char);

    buf = (unsigned char*)grib_context_malloc_clear(a->context, buflen);
    if (!buf)
        return GRIB_OUT_OF_MEMORY;

    half_byte = (buflen * 8) - ((*len) * bits_per_value);

    if ((err = grib_set_long_internal(grib_handle_of_accessor(a), self->half_byte, half_byte)) != GRIB_SUCCESS) {
        grib_context_free(a->context, buf);
        return err;
    }
    grib_buffer_replace(a, buf, buflen, 1, 1);

    grib_context_free(a->context, buf);

    return GRIB_SUCCESS;
}

static int value_count(grib_accessor* a, long* numberOfPoints)
{
    grib_accessor_data_dummy_field* self = (grib_accessor_data_dummy_field*)a;
    int err                              = 0;
    *numberOfPoints                      = 0;

    if ((err = grib_get_long_internal(grib_handle_of_accessor(a), self->numberOfPoints, numberOfPoints)) != GRIB_SUCCESS) {
        grib_context_log(a->context, GRIB_LOG_ERROR, "unable to get count of %s (%s)", a->name, grib_get_error_message(err));
    }

    return err;
}
