#include <memory.h>
#include <monitor.h>
#include <multiboot.h>
#include <descriptor_tables.h>
#include <timer.h>
#include <kmalloc.h>
#include <string.h>
#include <process.h>

extern uint32_t placement_address;
uint32_t initial_esp;

int kernel_main(struct multiboot* mboot_ptr, uint32_t initial_stack) {
    int ret = 0;

    initial_esp = initial_stack;

    monitor_clear();
    monitor_write("Hello multitasking world!\n");

    init_descriptor_tables();

    asm volatile("sti");
    init_timer(50);

    initialise_paging();
    initialise_procs();

    ret = fork();
    monitor_write("fork(): pid = ");
    monitor_write_hex(ret);
    monitor_write("\n");
    if (ret == 0) {
        monitor_write("In child process!\n");
    }
    else {
        monitor_write("In parent process!\n");
    }

    return 0;
}