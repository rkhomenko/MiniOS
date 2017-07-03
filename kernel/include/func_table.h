#ifndef MINI_OS_FUNC_TABLE_H_
#define MINI_OS_FUNC_TABLE_H_

#define FUNC_MAX 1024

struct func_table {
    void* func_ptrs[FUNC_MAX];
};

/* Список предопределенных функций */
enum {
    MONITOR_CLEAR = 0,
    MONITOR_PUT,
    MONITOR_WRITE,
    MONITOR_WRITE_DEC,
    MONITOR_WRITE_HEX,
    MULTIPLAY
};

void init_func_table();
void* get_func_addr(int func_no);
struct func_table* get_func_table_addr();

#define SET_FUNC_PTR(P, A) \
    (*(void**)&(P) = (A))

#endif /* MINI_OS_FUNC_TABLE_H_ */