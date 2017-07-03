#include <common.h>
#include <memory.h>
#include <monitor.h>
#include <multiboot.h>
#include <descriptor_tables.h>
#include <timer.h>
#include <kmalloc.h>
#include <string.h>
#include <process.h>
#include <func_table.h>

uint32_t initial_esp;

typedef int (*module_main_ptr)(struct func_table* ft);

module_main_ptr get_module_main_addr(struct multiboot* mboot_ptr) {
    uint32_t module_start = *((uint32_t*)mboot_ptr->mods_addr);
    uint32_t module_end = *(uint32_t*)(mboot_ptr->mods_addr + 4);
    module_main_ptr ptr = NULL;

    set_placement_addr(module_end);

    SET_FUNC_PTR(ptr, (void*)(module_start + 0x1000));

    return ptr;
}

int kernel_main(struct multiboot* mboot_ptr) {
    uint32_t ret = 0;
    module_main_ptr module_main = NULL;

    init_descriptor_tables();

    monitor_clear();
    monitor_write("Hello modules world!\n");

    init_func_table();

    module_main = get_module_main_addr(mboot_ptr);
    ret = module_main(get_func_table_addr());

    monitor_write("Life after call!\n");
    monitor_write_hex(ret);
    monitor_put('\n');

    return 0;
}