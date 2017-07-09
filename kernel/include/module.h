#ifndef MINI_OS_MODULE_H_
#define MINI_OS_MODULE_H_

#include <stddef.h>
#include <multiboot.h>

#define FUNC_MAX 1024

struct func_table {
    void* func_ptrs[FUNC_MAX];
};

enum module_funcs {
    ADD,
    SUB,
    MUL,
    DIV
};

typedef int32_t (*add_ptr)(int32_t, int32_t);
typedef int32_t (*sub_ptr)(int32_t, int32_t);
typedef int32_t (*mul_ptr)(int32_t, int32_t);
typedef int32_t (*div_ptr)(int32_t, int32_t);

typedef void (*module_main_ptr)(struct func_table* func_table);

#endif /* MINI_OS_MODULE_H_ */