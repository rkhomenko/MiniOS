#ifndef MINI_OS_PROCESS_H_
#define MINI_OS_PROCESS_H_

#include <stddef.h>
#include <memory.h>

struct process {
    int id;
    uint32_t esp;
    uint32_t ebp;
    uint32_t eip;
    struct page_directory* pd;
    struct process* next;
};

void initialise_procs();
void proc_switch();
int fork();
void move_stack(void* new_stack_start, uint32_t size);
int getpid();

#endif /* MINI_OS_PROCESS_H_ */