#ifndef MINI_OS_LOAD_MODULE_H_
#define MINI_OS_LOAD_MODULE_H_

void init_func_table(void);
void* get_func(int);
void set_ptr(void* p);

#define SET_FUNC_PTR(P, A) \
    (*(void**)&(P) = (A))

#endif /* MINI_OS_LOAD_MODULE_H_ */