#ifndef KERNEL_H
#define KERNEL_H

#include "types.h"

size_t  strlen(const char   *str);
int     strcmp(const char   *str1, const char   *str2);

void    *memset(void    *ptr, int   value, size_t   num);
void    *memcpy(void    *dest, const void    *src, size_t   num);

#endif
