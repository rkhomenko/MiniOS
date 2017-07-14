#ifndef MINI_OS_LOAD_MODULE_H_
#define MINI_OS_LOAD_MODULE_H_

#include <multiboot.h>
#include <module.h>

void init_module(struct multiboot* mboot, struct func_table* ft);
void* get_func(int);

#define SET_FUNC_PTR(P, A) \
    (*(void**)&(P) = (A))

#define SET_FUNC(N, F) \
    (ft->func_ptrs[(N)] = (void*)(&(F)))

#endif /* MINI_OS_LOAD_MODULE_H_ */