#include <stddef.h>
#include <module.h>
#include <load_disk.h>
#include <monitor.h>

static m_t* ptr;
static struct func_table ft;

void init_func_table(void) {
    module_main_ptr module_main = NULL;
    void* p = (void*)get_module();
    int i = 0;

    SET_MODULE_MAIN(module_main, p, ptr);

    module_main(&ft);

    for (i = 0; i < FUNC_MAX; i++) {
        ft.func_ptrs[i] = (void*)((uint32_t)ft.func_ptrs[i] +
                                  (uint32_t)module_main - 0x1000);
    }
}

void* get_func(int n) {
    return ft.func_ptrs[n];
}

void set_ptr(void* p) {
    ptr = (m_t*)p;
}