#ifndef _STRING_H
#define _STRING_H 1

#include <sys/cdefs.h>

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

char* itoa(int num, char* str, int base);
int memcmp(const void*, const void*, size_t);
void* memcpy(void* __restrict, const void* __restrict, size_t);
void* memmove(void*, const void*, size_t);
void* memset(void*, int, size_t);
size_t strlen(const char*);
char * strncpy (char *s1, const char *s2, size_t n);
char *strchr(const char *p, int ch);
int strcmp (const char *p1, const char *p2);
char* uint64_to_string(uint64_t value);

#ifdef __cplusplus
}
#endif

#endif
