#include <common.h>
#include <monitor.h>
#include <multiboot.h>
#include <module.h>
#include <descriptor_tables.h>
#include <load_module.h>

static struct func_table ft;

#pragma GCC diagnostic ignored "-Wpedantic"
void init_out_func_table(struct func_table* ft) {
    int i = 0;

    SET_FUNC(MONITOR_WRITE, monitor_write);

    /*for (i = 0; i < FUNC_MAX; i++) {
        ft->func_ptrs[i] = (void*)((uint32_t)ft->func_ptrs[i] + 0x100000);
    }*/
}
#pragma GCC diagnostic push

int kernel_main(struct multiboot* mboot_ptr) {
    monitor_write_ptr mw;

    set_ptr(mboot_ptr);
    init_descriptor_tables();

    monitor_clear();
    monitor_write("Hello modules world!\n");

    init_out_func_table(&ft);

    monitor_write_hex((uint32_t)ft.func_ptrs[MONITOR_WRITE]);
    monitor_put('\n');
    monitor_write_hex((uint32_t)kernel_main);
    monitor_put('\n');

    SET_FUNC_PTR(mw, ft.func_ptrs[MONITOR_WRITE]);
    mw("It works!\n");

    init_module(&ft);

    monitor_write("Life after call!\n");
    return 0;
}