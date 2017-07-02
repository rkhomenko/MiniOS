#ifndef MINI_OS_MONITOR_H_
#define MINI_OS_MONITOR_H_

#include <stddef.h>

void monitor_put(char c);
void monitor_clear();
void monitor_write(const char *c);
void monitor_write_hex(uint32_t n);
void monitor_write_dec(uint32_t n);

#endif /* MINI_OS_MONITOR_H_ */