#include <memory.h>
#include <monitor.h>
#include <multiboot.h>
#include <descriptor_tables.h>
#include <timer.h>
#include <kmalloc.h>
#include <string.h>

int kernel_main(struct multiboot *mboot_ptr)
{
    uint32_t* ptr = (uint32_t*)0xA0000000;
    uint32_t do_page_fault = 0;
    // Initialise all the ISRs and segmentation
    init_descriptor_tables();
    // Initialise the screen (by clearing it)
    monitor_clear();

    asm volatile("sti");
    initialise_paging();
    monitor_write("Hello, world!\n");

    do_page_fault = *ptr;
    char* buffer = kmalloc(32);
    memset(buffer, 'a', 31);
    buffer[31] = '\0';

    monitor_write(buffer);

    /*asm volatile("int $0x3");
    asm volatile("int $0x4");

    asm volatile("sti");
    init_timer(10);*/

    return 0;
}