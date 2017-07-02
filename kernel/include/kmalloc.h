#ifndef MINI_OS_KMALLOC_H_
#define MINI_OS_KMALLOC_H_

#include <stddef.h>

void* kmalloc(uint32_t size);
void* kmalloc_ap(uint32_t size, uint32_t* phys);
void* kmalloc_p(uint32_t size, uint32_t* phys);
void* kmalloc_a(uint32_t size);

#endif /* MINI_OS_KMALLOC_H_ */