#include <func_table.h>
#include <monitor.h>

#pragma GCC diagnostic ignored "-Wpedantic"

struct func_table func_table;

void* get_func_addr(int func_no) {
    return func_table.func_ptrs[func_no];
}

#define SET_FUNC(N, F) \
    (func_table.func_ptrs[(N)] = (void*)(&(F)))

uint32_t multiplay(uint32_t x, uint32_t y) {
    return x * y;
}

void init_func_table() {
    SET_FUNC(MONITOR_CLEAR, monitor_clear);
    SET_FUNC(MONITOR_PUT, monitor_put);
    SET_FUNC(MONITOR_WRITE, monitor_write);
    SET_FUNC(MONITOR_WRITE_DEC, monitor_write_dec);
    SET_FUNC(MONITOR_WRITE_HEX, monitor_write_hex);
    SET_FUNC(MULTIPLAY, multiplay);
}

struct func_table* get_func_table_addr() {
    return &func_table;
}

#undef SET_FUNC