#include "maxminddb.h"

#include <ruby.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

VALUE mGeoIP2 = Qnil;

char **lookup_path_parse(char *lookup_path, char *lang)
{

    char **result;
    if (NULL != lang)
        result = malloc(sizeof(char *) * (strlen(lookup_path) + strlen(lang) + 1 ));
    else
        result = malloc(sizeof(char *) * (strlen(lookup_path) + 1));

    char *token;
    char *string = strdup(lookup_path);

    token = strtok(string, " ");
    int i = 0;
    while (token != NULL) {
        result[i++] = token;
        token = strtok(NULL, " ");
    }
    if (NULL != lang) {
        result[i++] = lang;
    }
    result[i] = NULL;
    return result;
}

VALUE locate_by_path(MMDB_lookup_result_s *result, char *lookup_path, char *lang)
{
    VALUE return_value = Qnil;

    MMDB_entry_data_s entry_data;
    char **lp = lookup_path_parse(lookup_path, lang);
    int status = MMDB_aget_value(&result->entry, &entry_data, lp);
    if (MMDB_SUCCESS == status)
    {
        if (entry_data.offset)
        {
            if (entry_data.has_data) {
                if (entry_data.type == MMDB_DATA_TYPE_UTF8_STRING)
                    return_value = rb_str_new2(strndup((char *)entry_data.utf8_string, entry_data.data_size));
                if (entry_data.type == MMDB_DATA_TYPE_DOUBLE)
                    return_value = rb_float_new(entry_data.double_value);
            }
        }
    }
    free(lp);
    return return_value;
}

VALUE mGeoIP2_locate(int argc, VALUE *argv, VALUE self)
{
    VALUE locate_result = Qnil;

    if (argc > 4 || argc == 0) {
        rb_raise(rb_eArgError, "wrong number of arguments");
    }

    char *lang;
    if (argc == 3) {
        Check_Type(argv[2], T_STRING);
        lang = StringValuePtr(argv[2]);
    } else
    { lang = "ru"; }

    VALUE filename = argv[0];
    VALUE ipaddr = argv[1];

    Check_Type(filename, T_STRING);
    char *fname = StringValuePtr(filename);
    Check_Type(ipaddr, T_STRING);
    char *ip_address = StringValuePtr(ipaddr);

    MMDB_s mmdb;
    int status = MMDB_open(fname, MMDB_MODE_MMAP, &mmdb);
    if (MMDB_SUCCESS == status)
    {
        int gai_error, mmdb_error;
        MMDB_lookup_result_s result =
            MMDB_lookup_string(&mmdb, ip_address, &gai_error, &mmdb_error);

        if (result.found_entry)
        {
            locate_result = rb_hash_new();
            rb_hash_aset(locate_result, rb_str_new2("city"), locate_by_path(&result, "city names", lang));
            rb_hash_aset(locate_result, rb_str_new2("country"), locate_by_path(&result, "country names", lang));
            rb_hash_aset(locate_result, rb_str_new2("country_code"), locate_by_path(&result, "country iso_code", NULL));
            rb_hash_aset(locate_result, rb_str_new2("continent"), locate_by_path(&result, "continent names", lang));
            //rb_hash_aset(locate_result, rb_str_new2("subdivision"), locate_by_path(&result, "subdivisions names", lang));
            rb_hash_aset(locate_result, rb_str_new2("latitude"), locate_by_path(&result, "location latitude", NULL));
            rb_hash_aset(locate_result, rb_str_new2("longitude"), locate_by_path(&result, "location longitude", NULL));
        }
        MMDB_close(&mmdb);
    } else {
        rb_raise(rb_eIOError, "unable to open file %s", fname);
    }
    return locate_result;
}

void Init_GeoIP2()
{
      mGeoIP2 = rb_define_module("GeoIP2");
      rb_define_module_function(mGeoIP2, "locate", mGeoIP2_locate, -1);
}