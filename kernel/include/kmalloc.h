#ifndef MINI_OS_KMALLOC_H_
#define MINI_OS_KMALLOC_H_

#include <stddef.h>

uint32_t kmalloc(uint32_t size);
uint32_t kmalloc_ap(uint32_t size, uint32_t* phys);
uint32_t kmalloc_p(uint32_t size, uint32_t* phys);
uint32_t kmalloc_a(uint32_t size);

#endif /* MINI_OS_KMALLOC_H_ */