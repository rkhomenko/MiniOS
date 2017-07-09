#ifndef MINI_OS_MODULE_H_
#define MINI_OS_MODULE_H_

#include <stddef.h>
#include <multiboot.h>

#define FUNC_MAX 1024

struct func_table {
    void* func_ptrs[FUNC_MAX];
};

#define PROC_MAX 1024

enum process_state {
    PROCESS_EXECD,
    PROCESS_PAUSED,
    PROCESS_STOPED,
    PROCESS_DEFAULT
};

struct process {
    uint32_t id;
    size_t work_count;
    int state;
};

enum module_funcs {
    CREATE_PROCESS,
    DELETE_PROCESS,
    PROCESS_DISPATCHER,
    GET_CURRENT_PROCESS
};

typedef struct process* (*create_proc_ptr)(size_t work_count);
typedef void (*delete_proc_ptr)(struct process* proc);
typedef void (*process_dispatcher_ptr)(void);
typedef struct process* (*get_current_process_ptr)(void);

typedef void (*module_main_ptr)(struct func_table* out, struct func_table* in);

#endif /* MINI_OS_MODULE_H_ */