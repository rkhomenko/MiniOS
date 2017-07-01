#include <string.h>

void* memset(void* dest, int ch, size_t size) {
    uint8_t* ptr_dest = dest;

    if (dest == NULL) {
        return NULL;
    }

    while (size--) {
        *ptr_dest++ = (uint8_t)ch;
    }

    return dest;
}

void* memcpy(void* dest, const void* src, size_t count) {
    uint8_t* dest_ptr = dest;
    const uint8_t* src_ptr = src;

    if (dest == NULL || src == NULL) {
        return NULL;
    }

    while (count--) {
        *dest_ptr++ = *src_ptr++;
    }

    return dest;
}

size_t strlen(const char* str) {
    size_t lenght = 0;

    if (str == NULL) {
        return 0;
    }

    while (*str++ != '\0') {
        lenght++;
    }

    return lenght;
}

int strcmp(const char* lhs, const char* rhs) {
    if (lhs == NULL || rhs == NULL) {
        return UINT32_MAX;
    }

    while (*lhs && (*lhs == *rhs)) {
        lhs++;
        rhs++;
    }

    return *(const uint8_t*)lhs - *(const uint8_t*)rhs;
}

char* strcpy(char* dest, const char* src) {
    char* dest_copy = dest;

    if (dest == NULL || src == NULL) {
        return NULL;
    }

    while ((*src != '\0') && (*dest++ = *src++)) {}
    *dest = '\0';

    return dest_copy;
}

char* strcat(char* dest, const char* src) {
    char* dest_copy = dest;

    if (dest == NULL || src == NULL) {
        return NULL;
    }

    while (*dest++ != '\0') {}
    while ((*src++ != '\0') && (*dest++ = *src++)) {}

    return dest_copy;
}