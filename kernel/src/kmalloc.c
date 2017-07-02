#include <kmalloc.h>

extern uint32_t end;
uint32_t placement_address = (uint32_t)&end;

static void* kmalloc_generic(uint32_t size, int align, uint32_t* phys) {
    uint32_t tmp = placement_address;

    if (align == 1 && (placement_address & 0xFFFFF000) ) {
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }

    if (phys) {
        *phys = placement_address;
    }

    placement_address += size;
    return (void*)tmp;
}

void* kmalloc_a(uint32_t size) {
    return kmalloc_generic(size, 1, 0);
}

void* kmalloc_p(uint32_t size, uint32_t* phys) {
    return kmalloc_generic(size, 0, phys);
}

void* kmalloc_ap(uint32_t size, uint32_t* phys) {
    return kmalloc_generic(size, 1, phys);
}

void* kmalloc(uint32_t size) {
    return kmalloc_generic(size, 0, 0);
}