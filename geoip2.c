#include <ruby.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "MMDB.h"


VALUE mGeoIP2 = Qnil;

char* result_json = "{";

char *replace_str(const char *str, const char *old, const char *new)
{
    char *ret, *r;
    const char *p, *q;
    size_t oldlen = strlen(old);
    size_t count, retlen, newlen = strlen(new);

    if (oldlen != newlen) {
        for (count = 0, p = str; (q = strstr(p, old)) != NULL; p = q + oldlen)
            count++;
        /* this is undefined if p - str > PTRDIFF_MAX */
        retlen = p - str + strlen(p) + count * (newlen - oldlen);
    } else
        retlen = strlen(str);

    if ((ret = malloc(retlen + 1)) == NULL)
        return NULL;

    for (r = ret, p = str; (q = strstr(p, old)) != NULL; p = q + oldlen) {
        /* this is undefined if q - p > PTRDIFF_MAX */
        ptrdiff_t l = q - p;
        memcpy(r, p, l);
        r += l;
        memcpy(r, new, newlen);
        r += newlen;
    }
    strcpy(r, p);

    return ret;
}


char *my_strcat(char *str1, char *str2)
{
    char *ret;
    if ((ret = malloc(strlen(str1)+strlen(str2)+1)) != NULL)
    {
        ret[0] = '\0';
        strcat(ret, str1);
        strcat(ret, str2);
        return ret;
    }
    return NULL;
}

int is_ipv4(MMDB_s * mmdb)
{
    return mmdb->depth == 32;
}

int int_pread(int fd, uint8_t * buffer, ssize_t to_read, off_t offset)
{
    while (to_read > 0) {
        ssize_t have_read = pread(fd, buffer, to_read, offset);
        if (have_read <= 0) {
            return MMDB_IOERROR;
        }
        to_read -= have_read;
        if (to_read == 0) {
            break;
        }
        offset += have_read;
        buffer += have_read;
    }
    return MMDB_SUCCESS;
}

char* decode_key(MMDB_s * mmdb, MMDB_return_s * data)
{
    uint8_t str[256];
    int len = data->data_size > 255 ? 255 : data->data_size;

    if (mmdb && mmdb->fd >= 0) {
        uint32_t segments = mmdb->full_record_size_bytes * mmdb->node_count;
        int_pread(mmdb->fd, str, len, segments + (uintptr_t) data->ptr);
    } else {
        memcpy(str, data->ptr, len);
    }

    str[len] = '\0';
    //fprintf(stderr, "%s\n", str);
    return strdup((char*) str);
}

void silly_pindent(int i)
{
    char buffer[1024];
    int size = i >= 1024 ? 1023 : i;
    memset(buffer, 32, size);
    buffer[size] = '\0';
    fputs(buffer, stderr);
}

int key = 0;
char *current_key = "location";
char tmp_str[255];

MMDB_decode_all_s * dump_hash(MMDB_s * mmdb, MMDB_decode_all_s *decode_all,
                              int indent)
{
    switch (decode_all->decode.data.type) {
    case MMDB_DTYPE_MAP:
        {

            if (key == 0 ) {
                //silly_pindent(indent);
                //fprintf(stdout, "\"%s\": { \n", current_key);
                sprintf(tmp_str, "\"%s\": { ", current_key);
                result_json = my_strcat(result_json, tmp_str);
            }

            int size = decode_all->decode.data.data_size;
            for (decode_all = decode_all->next; size && decode_all; size--) {
                //fprintf(stdout, "11 -> \n");
                key = 1;
                decode_all = dump_hash(mmdb, decode_all, indent + 2);

                //fprintf(stdout, "22 -> \n");
                key = 0;
                decode_all = dump_hash(mmdb, decode_all, indent + 2);

                //silly_pindent(indent);
            }

            result_json = my_strcat(result_json, "},");
            //fprintf(stdout, "}\n");
        }
        break;
    case MMDB_DTYPE_ARRAY:
        {
            int size = decode_all->decode.data.data_size;
            for (decode_all = decode_all->next; size && decode_all; size--) {
                decode_all = dump_hash(mmdb, decode_all, indent + 2);
            }
        }
        break;
    case MMDB_DTYPE_UTF8_STRING:
    case MMDB_DTYPE_BYTES:
        //silly_pindent(indent);
        {
            char* str = decode_key(mmdb, &decode_all->decode.data);

            if (key == 1) {
                current_key = str;
            } else {
                //silly_pindent(indent);
                //fprintf(stdout, "\"%s\": \"%s\",\n", current_key, str);
                sprintf(tmp_str, "\"%s\": \"%s\",", current_key, str);
                result_json = my_strcat(result_json, tmp_str);
            }

            decode_all = decode_all->next;
        }
        break;
    case MMDB_DTYPE_IEEE754_DOUBLE:
        //silly_pindent(indent);
        //fprintf(stdout, "%f\n", decode_all->decode.data.double_value);
        decode_all = decode_all->next;
        break;
    case MMDB_DTYPE_IEEE754_FLOAT:
        //silly_pindent(indent);
        //fprintf(stdout, "%f\n", decode_all->decode.data.float_value);
        decode_all = decode_all->next;
        break;
    case MMDB_DTYPE_UINT16:
    case MMDB_DTYPE_UINT32:
    case MMDB_DTYPE_BOOLEAN:
        //silly_pindent(indent);
        //fprintf(stdout, "%u\n", decode_all->decode.data.uinteger);
        {
                char res[10];
                sprintf(res, "%d", decode_all->decode.data.uinteger);

                if (key == 1) {
                    current_key = res;
                } else {
                    // silly_pindent(indent);
                    // fprintf(stdout, "\"%s\": \"%s\",\n", current_key, res);
                    sprintf(tmp_str, "\"%s\": \"%s\",", current_key, res);
                    result_json = my_strcat(result_json, tmp_str);
                }

                decode_all = decode_all->next;
        }
        break;
    case MMDB_DTYPE_UINT64:
    case MMDB_DTYPE_UINT128:
        //silly_pindent(indent);
        //fprintf(stdout, "Some UINT64 or UINT128 data\n");
        //fprintf(stderr, "%u\n", decode_all->decode.data.uinteger);
        decode_all = decode_all->next;
        break;
    case MMDB_DTYPE_INT32:
        //silly_pindent(indent);
        //fprintf(stdout, "%d\n", decode_all->decode.data.sinteger);
        decode_all = decode_all->next;
        break;
    default:
        MMDB_DBG_CARP("decode_one UNIPLEMENTED type:%d\n",
                      decode_all->decode.data.type);
        assert(0);
    }
    return decode_all;

}

//VALUE mGeoIP2_locate(VALUE self, VALUE filename, VALUE ipaddr)
VALUE mGeoIP2_locate(int argc, VALUE *argv, VALUE self)
{

    if (argc > 2 || argc == 0) {
        rb_raise(rb_eArgError, "wrong number of arguments");
    }

    VALUE filename = argv[0];
    VALUE ipaddr = argv[1];

    Check_Type(filename, T_STRING);
    char* fname = StringValuePtr(filename);

    // //MMDB_s *mmdb = MMDB_open(fname, MMDB_MODE_MEMORY_CACHE);
    MMDB_s *mmdb = MMDB_open(fname, MMDB_MODE_STANDARD);

    if (!mmdb) {
          fprintf(stderr, "Can't open %s\n", fname);
          exit(1);
    }

    // free(fname);


    Check_Type(ipaddr, T_STRING);
    char *ipstr = StringValuePtr(ipaddr);
    union {
        struct in_addr v4;
        struct in6_addr v6;
    } ip;

    //return rb_str_new2(ipstr);

    int ai_family = is_ipv4(mmdb) ? AF_INET : AF_INET6;
    int ai_flags = AI_V4MAPPED;

    if (ipstr == NULL || 0 != MMDB_resolve_address(ipstr, ai_family, ai_flags,
                                                  &ip)) {
        fprintf(stderr, "Invalid IP\n");
        exit(1);
    }

    MMDB_root_entry_s root = {.entry.mmdb = mmdb };

    int status = is_ipv4(mmdb)
        ? MMDB_lookup_by_ipnum(htonl(ip.v4.s_addr), &root)
        : MMDB_lookup_by_ipnum_128(ip.v6, &root);

    if (status == MMDB_SUCCESS) {
        if (root.entry.offset > 0) {
            MMDB_decode_all_s *decode_all;
            int err = MMDB_get_tree(&root.entry, &decode_all);
            if (err == MMDB_SUCCESS) {
                if (decode_all != NULL)
                    dump_hash(mmdb, decode_all, 0);
                MMDB_free_decode_all(decode_all);
            }

        } else {
            return Qnil;       // not found
        }
    }

    result_json = my_strcat(result_json, "}");
    result_json = replace_str(result_json, ",}", "}");
    return rb_str_new2(result_json);
}

void Init_GeoIP2()
{
      mGeoIP2 = rb_define_module("GeoIP2");
      rb_define_module_function(mGeoIP2, "locate", mGeoIP2_locate, -1);
}