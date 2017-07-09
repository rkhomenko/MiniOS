#include <module.h>
#include <load_module.h>
#include <monitor.h>

extern uint8_t MODULE_SIGNATURE[1024] __attribute__((section(".signature")));
uint8_t MODULE_SIGNATURE[1024] = "I'M MINI_OS MODULE!";
int module_main(struct func_table* out,
                struct func_table* in) __attribute__((section(".start")));

static int32_t index;
static size_t id;
static struct process procs[PROC_MAX];
static struct process* current_proc;
static struct process* prev_proc;

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
    proc->state = PROCESS_STOPED;
}

static void dec_work_count(struct process* proc) {
    proc->work_count--;
    if (!proc->work_count) {
        delete_proc(proc);
    }
}

void process_dispatcher(void) {
    int i = 0;

    if (current_proc == NULL && prev_proc == NULL) {
        for (i = 0; i < PROC_MAX; i++) {
            if (procs[i].id != 0) {
                current_proc = &procs[i];
                index = i;
                break;
            }
        }
        current_proc->state = PROCESS_EXECD;
        return;
    }

    if (current_proc != NULL) {
        prev_proc = current_proc;
        prev_proc->state = PROCESS_PAUSED;
        dec_work_count(prev_proc);

        current_proc = NULL;
        for (i = index + 1; i < PROC_MAX; i++) {
            if (procs[i].id != 0) {
                current_proc = &procs[i];
                index = i;
                break;
            }
        }
        if (current_proc != NULL) {
            current_proc->state = PROCESS_EXECD;
            return;
        }
        for (i = 0; i <= index; i++) {
            if (procs[i].id != 0) {
                current_proc = &procs[i];
                index = i;
                break;
            }
        }
        if (current_proc != NULL) {
            current_proc->state = PROCESS_EXECD;
            return;
        }
        current_proc = prev_proc = NULL;
    }
}

struct process* get_current_process(void) {
    return current_proc;
}

#pragma GCC diagnostic ignored "-Wpedantic"

void init_func_table(struct func_table* ft) {
    SET_FUNC(CREATE_PROCESS, create_proc);
    SET_FUNC(DELETE_PROCESS, delete_proc);
    SET_FUNC(PROCESS_DISPATCHER, process_dispatcher);
    SET_FUNC(GET_CURRENT_PROCESS, get_current_process);
}

#pragma GCC diagnostic push

void init_procs(void) {
    int i = 0;

    for (i = 0; i < PROC_MAX; i++) {
        procs[i].id = 0;
        procs[i].work_count = 0;
        procs[i].state = PROCESS_DEFAULT;
    }

    id = 1;
    index = -1;
    current_proc = NULL;
    prev_proc = NULL;
}

int module_main(struct func_table* out, struct func_table* in) {
    init_procs();
    init_func_table(out);
    return 0;
}