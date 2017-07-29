#include "maxminddb.h"

#include <ruby.h>
#include <ruby/encoding.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define INT2BOOL(x)  ((x)?Qtrue:Qfalse)

VALUE mMaxmindGeoIP2 = Qnil;

VALUE locate_by_path(MMDB_lookup_result_s *result, char *lookup_path)
{
    VALUE return_value = Qnil;

    MMDB_entry_data_s entry_data;
    int status = MMDB_aget_value(&result->entry, &entry_data, lookup_path);
    if (MMDB_SUCCESS == status)
    {
        if (entry_data.offset)
        {
            if (entry_data.has_data) {
                if (entry_data.type == MMDB_DATA_TYPE_UTF8_STRING)
                    return_value = rb_enc_str_new((char *)entry_data.utf8_string, entry_data.data_size, rb_utf8_encoding());
                if (entry_data.type == MMDB_DATA_TYPE_UINT32)
                    return_value = rb_int_new(entry_data.uint32);
                if (entry_data.type == MMDB_DATA_TYPE_DOUBLE)
                    return_value = rb_float_new(entry_data.double_value);
            }
        }
    }
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
            const char *city_path[] = { "city", "names", lang, NULL };
            rb_hash_aset(locate_result, rb_str_new2("city"), locate_by_path(&result, city_path));
            const char *city_geoname_id_path[] = { "city", "geoname_id", NULL };
            rb_hash_aset(locate_result, rb_str_new2("city_geoname_id"), locate_by_path(&result, city_geoname_id_path));
            const char *country_path[] = { "country", "names", lang, NULL };
            rb_hash_aset(locate_result, rb_str_new2("country"), locate_by_path(&result, country_path));
            const char *country_geoname_id_path[] = { "country", "geoname_id", NULL };
            rb_hash_aset(locate_result, rb_str_new2("country_geoname_id"), locate_by_path(&result, country_geoname_id_path));
            const char *country_code_path[] = { "country", "iso_code", NULL };
            rb_hash_aset(locate_result, rb_str_new2("country_code"), locate_by_path(&result, country_code_path));
            const char *continent_path[] = { "continent", "names", lang, NULL };
            rb_hash_aset(locate_result, rb_str_new2("continent"), locate_by_path(&result, continent_path));
            const char *continent_code_path[] = { "continent", "code", NULL };
            rb_hash_aset(locate_result, rb_str_new2("continent_code"), locate_by_path(&result, continent_code_path));
            const char *continent_geoname_id_path[] = { "continent", "geoname_id", NULL };
            rb_hash_aset(locate_result, rb_str_new2("continent_geoname_id"), locate_by_path(&result, continent_geoname_id_path));
            const char *subdivision_path[] = { "subdivisions", "0", "names", lang, NULL };
            rb_hash_aset(locate_result, rb_str_new2("subdivision"), locate_by_path(&result, subdivision_path));
            const char *subdivision_code_path[] = { "subdivisions", "0", "iso_code", NULL };
            rb_hash_aset(locate_result, rb_str_new2("subdivision_code"), locate_by_path(&result, subdivision_code_path));
            const char *subdivision_geoname_id_path[] = { "subdivisions", "0", "geoname_id", NULL };
            rb_hash_aset(locate_result, rb_str_new2("subdivision_geoname_id"), locate_by_path(&result, subdivision_geoname_id_path));
            const char *postal_code_path[] = { "postal", "code", NULL };
            rb_hash_aset(locate_result, rb_str_new2("postal_code"), locate_by_path(&result, postal_code_path));
            const char *latitude_path[] = { "location", "latitude", NULL };
            rb_hash_aset(locate_result, rb_str_new2("latitude"), locate_by_path(&result, latitude_path));
            const char *longitude_path[] = { "location", "longitude", NULL };
            rb_hash_aset(locate_result, rb_str_new2("longitude"), locate_by_path(&result, longitude_path));
            const char *time_zone_path[] = { "location", "time_zone", NULL };
            rb_hash_aset(locate_result, rb_str_new2("time_zone"), locate_by_path(&result, time_zone_path));
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
