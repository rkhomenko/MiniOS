#include <func_table.h>
#include <monitor.h>

int module_main(struct func_table* ft) __attribute__((section(".start")));

typedef uint32_t (*multiplay_ptr)(uint32_t x, uint32_t y);

int module_main(struct func_table* ft) {
    const uint32_t DEADBEAF = 0xdeadbeef;
    multiplay_ptr multiplay = NULL;

    SET_FUNC_PTR(multiplay, ft->func_ptrs[MULTIPLAY]);

    return multiplay(DEADBEAF / 2, 2) + DEADBEAF % 2;
}