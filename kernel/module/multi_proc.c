#include <module.h>

extern uint8_t MODULE_SIGNATURE[1024] __attribute__((section(".signature")));
uint8_t MODULE_SIGNATURE[1024] = "I'M MINI_OS MODULE!";

int module_main(struct func_table* ft) __attribute__((section(".start")));

int32_t add(int32_t x, int32_t y) {
    return x + y;
}

int32_t sub(int32_t x, int32_t y) {
    return x - y;
}

int32_t mul(int32_t x, int32_t y) {
    return x * y;
}

int32_t div(int32_t x, int32_t y) {
    return x / y;
}

#define SET_FUNC(N, F) \
    (ft->func_ptrs[(N)] = (void*)(&(F)))

#pragma GCC diagnostic ignored "-Wpedantic"

void init_func_table(struct func_table* ft) {
    SET_FUNC(ADD, add);
    SET_FUNC(SUB, sub);
    SET_FUNC(MUL, mul);
    SET_FUNC(DIV, div);
}

#pragma GCC diagnostic push

int module_main(struct func_table* ft) {
    init_func_table(ft);
    return 0;
}