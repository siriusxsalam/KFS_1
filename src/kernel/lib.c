#include "kernel.h"

size_t  strlen(const char   *str) {
    size_t l = 0;
    
    while (str[l] != '\0')
        l++;
    
    return l;
}

int     strcmp(const char   *str1, const char   *str2) {
    while (*str1 && (*str1 == *str2))
    {
        str1++;
        str2++;
    }

    return (unsigned char)*str1 - (unsigned char)*str2;
}

void    *memset(void    *ptr, int   value, size_t   num) {
    u8  *byte_ptr = (u8 *)ptr;

    for (size_t i = 0; i < num; i++)
        byte_ptr[i] = (u8)value;

    return ptr;
}
void    *memcpy(void    *dest, const void    *src, size_t   num) {
    u8  *dst = (u8 *)dest;
    const u8    *source = (const u8 *)src;

    for (size_t i = 0; i < num; i++)
        dst[i] = source[i];

    return dest;
}
