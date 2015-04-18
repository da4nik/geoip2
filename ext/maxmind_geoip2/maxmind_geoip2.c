#include "maxminddb.h"

#include <ruby.h>
#include <ruby/encoding.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

VALUE mMaxmindGeoIP2 = Qnil;

const char **lookup_path_parse(char *lookup_path, char *lang)
{

    const char **result;
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
    const char **lp = lookup_path_parse(lookup_path, lang);
    int status = MMDB_aget_value(&result->entry, &entry_data, lp);
    if (MMDB_SUCCESS == status)
    {
        if (entry_data.offset)
        {
            if (entry_data.has_data) {
                if (entry_data.type == MMDB_DATA_TYPE_UTF8_STRING)
                    return_value = rb_enc_str_new(strndup((char *)entry_data.utf8_string, entry_data.data_size), entry_data.data_size, rb_utf8_encoding());
                if (entry_data.type == MMDB_DATA_TYPE_UINT32)
                    return_value = rb_int_new(entry_data.uint32);
                if (entry_data.type == MMDB_DATA_TYPE_DOUBLE)
                    return_value = rb_float_new(entry_data.double_value);
            }
        }
    }
    free(lp);
    return return_value;
}

VALUE mMaxmindGeoIP2_locate(int argc, VALUE *argv, VALUE self)
{
    VALUE locate_result = Qnil;

    if (argc > 3 || argc == 0) {
        rb_raise(rb_eArgError, "wrong number of arguments");
    }


    char *lang;
    if (argc == 2) {
        Check_Type(argv[1], T_STRING);
        lang = StringValuePtr(argv[1]);
    } else {
        VALUE  loc = rb_iv_get(self, "@_locale");
        Check_Type(loc, T_STRING);
        lang = StringValuePtr(loc);
    }

    VALUE ipaddr = argv[0];
    Check_Type(ipaddr, T_STRING);
    char *ip_address = StringValuePtr(ipaddr);

    char *filename;
    VALUE file = rb_iv_get(self, "@_file");
    Check_Type(file, T_STRING);
    filename = StringValuePtr(file);


    MMDB_s mmdb;
    int status = MMDB_open(filename, MMDB_MODE_MMAP, &mmdb);
    if (MMDB_SUCCESS == status)
    {
        int gai_error, mmdb_error;
        MMDB_lookup_result_s result =
            MMDB_lookup_string(&mmdb, ip_address, &gai_error, &mmdb_error);

        if (result.found_entry)
        {
            locate_result = rb_hash_new();
            rb_hash_aset(locate_result, rb_str_new2("city"), locate_by_path(&result, "city names", lang));
            rb_hash_aset(locate_result, rb_str_new2("city_geoname_id"), locate_by_path(&result, "city geoname_id", NULL));
            rb_hash_aset(locate_result, rb_str_new2("country"), locate_by_path(&result, "country names", lang));
            rb_hash_aset(locate_result, rb_str_new2("country_geoname_id"), locate_by_path(&result, "country geoname_id", NULL));
            rb_hash_aset(locate_result, rb_str_new2("country_code"), locate_by_path(&result, "country iso_code", NULL));
            rb_hash_aset(locate_result, rb_str_new2("continent"), locate_by_path(&result, "continent names", lang));
            rb_hash_aset(locate_result, rb_str_new2("continent_code"), locate_by_path(&result, "continent code", NULL));
            rb_hash_aset(locate_result, rb_str_new2("continent_geoname_id"), locate_by_path(&result, "continent geoname_id", NULL));
            rb_hash_aset(locate_result, rb_str_new2("subdivision"), locate_by_path(&result, "subdivisions 0 names", lang));
            rb_hash_aset(locate_result, rb_str_new2("subdivision_code"), locate_by_path(&result, "subdivisions 0 iso_code", NULL));
            rb_hash_aset(locate_result, rb_str_new2("subdivision_geoname_id"), locate_by_path(&result, "subdivisions 0 geoname_id", NULL));
            rb_hash_aset(locate_result, rb_str_new2("postal_code"), locate_by_path(&result, "postal code", NULL));
            rb_hash_aset(locate_result, rb_str_new2("latitude"), locate_by_path(&result, "location latitude", NULL));
            rb_hash_aset(locate_result, rb_str_new2("longitude"), locate_by_path(&result, "location longitude", NULL));
            rb_hash_aset(locate_result, rb_str_new2("time_zone"), locate_by_path(&result, "location time_zone", NULL));
        }
        MMDB_close(&mmdb);
    } else {
        rb_raise(rb_eIOError, "unable to open file %s", filename);
    }
    return locate_result;
}

VALUE mMaxmindGeoIP2_file(VALUE self, VALUE filepath)
{
    rb_iv_set(self, "@_file", filepath);
    return Qtrue;
}

VALUE mMaxmindGeoIP2_locale(VALUE self, VALUE language)
{
    rb_iv_set(self, "@_locale", language);
    return Qtrue;
}

void Init_maxmind_geoip2()
{
      mMaxmindGeoIP2 = rb_define_module("MaxmindGeoIP2");
      rb_define_module_function(mMaxmindGeoIP2, "locate", mMaxmindGeoIP2_locate, -1);
      rb_define_module_function(mMaxmindGeoIP2, "file", mMaxmindGeoIP2_file, 1);
      rb_define_module_function(mMaxmindGeoIP2, "locale", mMaxmindGeoIP2_locale, 1);

      rb_define_attr(mMaxmindGeoIP2, "_file", 1, 1);
      rb_define_attr(mMaxmindGeoIP2, "_locale", 1, 1);

      rb_iv_set(mMaxmindGeoIP2, "@_file", rb_str_new2("GeoLite2-City.mmdb"));
      rb_iv_set(mMaxmindGeoIP2, "@_locale", rb_str_new2("ru"));
}
