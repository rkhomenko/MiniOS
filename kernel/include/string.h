#ifndef MINI_OS_STRING_H_
#define MINI_OS_STRING_H_

#include <stddef.h>

void* memset(void* dest, int ch, size_t size);
void* memcpy(void* dest, const void* src, size_t count);

size_t strlen(const char* str);
int strcmp(const char* lhs, const char* rhs);
char* strcpy(char* dest, const char* src);
char* strcat(char* dest, const char* src);

#endif /* MINI_OS_STRING_H_ */