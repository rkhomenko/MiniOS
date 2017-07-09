#include <common.h>
#include <monitor.h>
#include <multiboot.h>
#include <module.h>
#include <descriptor_tables.h>
#include <load_module.h>
#include <timer.h>

static struct func_table ft;

#pragma GCC diagnostic ignored "-Wpedantic"
void init_out_func_table(struct func_table* ft) {
    int i = 0;

    SET_FUNC(MONITOR_WRITE, monitor_write);
}
#pragma GCC diagnostic push

create_proc_ptr create_proc;
delete_proc_ptr delete_proc;
process_dispatcher_ptr process_dispatcher;
get_current_process_ptr get_current_process;

int kernel_main(struct multiboot* mboot_ptr) {
    struct process* proc;

    monitor_write_ptr mw;

    set_ptr(mboot_ptr);
    init_descriptor_tables();

    monitor_clear();
    monitor_write("Hello modules world!\n");

    init_out_func_table(&ft);
    init_module(&ft);

    SET_FUNC_PTR(create_proc, get_func(CREATE_PROCESS));
    SET_FUNC_PTR(delete_proc, get_func(DELETE_PROCESS));
    SET_FUNC_PTR(process_dispatcher, get_func(PROCESS_DISPATCHER));
    SET_FUNC_PTR(get_current_process, get_func(GET_CURRENT_PROCESS));

    proc = create_proc(1);
    proc = create_proc(3);
    proc = create_proc(2);
    proc = create_proc(1);

    asm volatile("sti");
    init_timer(10);

    return 0;
}