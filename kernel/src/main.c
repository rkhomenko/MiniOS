#include <common.h>
#include <monitor.h>
#include <multiboot.h>
#include <module.h>
#include <descriptor_tables.h>
#include <load_module.h>

int kernel_main(struct multiboot* mboot_ptr) {
    add_ptr add = NULL;
    div_ptr div = NULL;

    set_ptr(mboot_ptr);
    init_descriptor_tables();

    monitor_clear();
    monitor_write("Hello modules world!\n");

    init_func_table();
    monitor_write("Call functions\n");

    SET_FUNC_PTR(add, get_func(ADD));
    SET_FUNC_PTR(div, get_func(DIV));
    monitor_write_dec(add(1, 2));
    monitor_put('\n');
    monitor_write_dec(div(36, 9));
    monitor_put('\n');

    return 0;
}