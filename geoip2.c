#include "maxminddb.h"

#include <ruby.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

VALUE mGeoIP2 = Qnil;

VALUE mGeoIP2_locate_city(int argc, VALUE *argv, VALUE self)
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
        char *lookup_path[] = {"city", "names", lang, NULL};

        int gai_error, mmdb_error;
        MMDB_lookup_result_s result =
            MMDB_lookup_string(&mmdb, ip_address, &gai_error, &mmdb_error);

        if (result.found_entry)
        {
            MMDB_entry_data_s entry_data;
            status = MMDB_aget_value(&result.entry, &entry_data, lookup_path);
            if (MMDB_SUCCESS == status)
            {
                if (entry_data.offset)
                {
                    if (entry_data.has_data &&
                        entry_data.type == MMDB_DATA_TYPE_UTF8_STRING) {
                        char *string = strndup((char *)entry_data.utf8_string, entry_data.data_size );
                        locate_result = rb_str_new2(string);
                        free(string);
                    }
                }
            }
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
      rb_define_module_function(mGeoIP2, "locate_city", mGeoIP2_locate_city, -1);
}