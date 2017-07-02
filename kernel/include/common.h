#ifndef MINI_OS_COMMON_H_
#define MINI_OS_COMMON_H_

#include <stddef.h>

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);

#define PANIC(msg) panic((msg), __FILE__, __LINE__)
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

extern void panic(const char *message, const char *file, uint32_t line);
extern void panic_assert(const char *file, uint32_t line, const char *desc);

#endif /* MINI_OS_COMMON_H_ */
