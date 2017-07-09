#ifndef MINI_OS_MONITOR_H_
#define MINI_OS_MONITOR_H_

#include <stddef.h>

void monitor_clear();
void monitor_put(char c);
void monitor_write(const char* str);
void monitor_write_hex(uint32_t n);
void monitor_write_dec(uint32_t n);

enum monitor_funcs {
    MONITOR_WRITE
};

typedef void (*monitor_clear_ptr)();
typedef void (*monitor_put_ptr)(char c);
typedef void (*monitor_write_ptr)(const char* str);
typedef void (*monitor_write_hex_ptr)(uint32_t n);
typedef void (*monitor_write_dec_ptr)(uint32_t n);

#endif /* MINI_OS_MONITOR_H_ */