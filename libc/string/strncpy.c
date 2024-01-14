#include <string.h>

char * strncpy(char * dst_str, const char * src_str, size_t len) {
    const size_t srclen = strlen(src_str);
    if (srclen < len) {
        memcpy(dst_str, src_str, srclen);
        memset(dst_str+srclen, 0, len - srclen);
    } else {
        memcpy(dst_str, src_str, len);
    }
    return dst_str;
}