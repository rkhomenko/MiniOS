#ifndef MINI_OS_LOAD_MODULE_H_
#define MINI_OS_LOAD_MODULE_H_

#include <module.h>

void init_module(struct func_table* ft);
void* get_func(int);
void set_ptr(void* p);

#define SET_FUNC_PTR(P, A) \
    (*(void**)&(P) = (A))

#define SET_FUNC(N, F) \
    (ft->func_ptrs[(N)] = (void*)(&(F)))

#endif /* MINI_OS_LOAD_MODULE_H_ */