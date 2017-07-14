#include <stddef.h>
#include <module.h>
#include <monitor.h>

static struct func_table ft;

void init_module(struct multiboot* mboot, struct func_table* func_table) {
    module_main_ptr module_main = NULL;
    int i = 0;

    module_main = (module_main_ptr)(*((uint32_t*)(mboot)->mods_addr) + 0x1000);

    module_main(&ft, func_table);

    for (i = 0; i < FUNC_MAX; i++) {
        ft.func_ptrs[i] = (void*)((uint32_t)ft.func_ptrs[i] +
                                  (uint32_t)module_main - 0x1000);
    }
}

void* get_func(int n) {
    return ft.func_ptrs[n];
}