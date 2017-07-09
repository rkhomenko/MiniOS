#include <module.h>
#include <load_module.h>
#include <monitor.h>

extern uint8_t MODULE_SIGNATURE[1024] __attribute__((section(".signature")));
uint8_t MODULE_SIGNATURE[1024] = "I'M MINI_OS MODULE!";
int module_main(struct func_table* out,
                struct func_table* in) __attribute__((section(".start")));

static size_t index = 0;
static size_t id = 1;
static struct process procs[PROC_MAX];

static monitor_write_ptr monitor_write_func;

struct process* create_proc(size_t work_count) {
    struct process* proc = NULL;
    int i = 0;

    for (i = 0; i < PROC_MAX; i++) {
        if (procs[i].id == 0) {
            proc = &procs[i];
            break;
        }
    }

    proc->id = id++;
    proc->work_count = work_count;

    return proc;
}

void delete_proc(struct process* proc) {
    proc->id = 0;
    proc->work_count = 0;
}

void process_dispatcher(void) {
    struct process* proc = &procs[index];
    size_t new_index = 0;
    size_t i = 0;

    proc->work_count--;
    if (!proc->work_count) {
        delete_proc(proc);
    }

    for (i = index + 1; i < PROC_MAX; i++) {
        if (procs[i].id != 0) {
            new_index = i;
            break;
        }
    }

    if (new_index != 0) {
        index = new_index;
        return;
    }

    for (i = 0; i <= index; i++) {
        if (procs[i].id != 0) {
            new_index = i;
            break;
        }
    }

    if (new_index != 0) {
        index = new_index;
    }
}

#pragma GCC diagnostic ignored "-Wpedantic"

void init_func_table(struct func_table* ft) {
    SET_FUNC(CREATE_PROCESS, create_proc);
    SET_FUNC(DELETE_PROCESS, delete_proc);
    SET_FUNC(PROCESS_DISPATCHER, process_dispatcher);
}

#pragma GCC diagnostic push

int module_main(struct func_table* out, struct func_table* in) {
    init_func_table(out);
    SET_FUNC_PTR(monitor_write_func, in->func_ptrs[MONITOR_WRITE]);
    monitor_write_func("Hello from module!\n");
    return 0;
}