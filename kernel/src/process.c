#include <process.h>
#include <memory.h>
#include <kmalloc.h>
#include <string.h>

extern struct page_directory* kernel_dir;
extern struct page_directory* current_dir;
extern void alloc_frame(struct page* page, int, int);
extern uint32_t initial_esp;
extern uint32_t read_eip();

uint32_t next_pid = 1;
volatile struct process* current_proc;
volatile struct process* ready_queue;

void move_stack(void* new_stack, uint32_t size) {
    uint32_t pd_addr = 0;
    uint32_t old_stack_ptr = 0;
    uint32_t old_base_ptr = 0;
    uint32_t new_stack_ptr = 0;
    uint32_t new_base_ptr = 0;
    uint32_t offset = 0;
    uint32_t tmp = 0;
    uint32_t *tmp2 = NULL;
    uint32_t i = 0;

    for (i = (uint32_t)new_stack;
         i >= (uint32_t) new_stack - size;
         i -= 0x1000) {
        alloc_frame(get_page(i, 1, current_dir), 0, 1);
    }

    asm volatile("mov %%cr3, %0" : "=r"(pd_addr));
    asm volatile("mov %0, %%cr3" :: "r"(pd_addr));

    asm volatile("mov %%esp, %0" : "=r"(old_stack_ptr));
    asm volatile("mov %%ebp, %0" : "=r"(old_base_ptr));

    offset = (uint32_t)new_stack - initial_esp;

    new_stack_ptr = old_stack_ptr + offset;
    new_base_ptr = old_base_ptr + offset;

    memcpy((void*)new_stack_ptr,
           (void*)old_stack_ptr,
           initial_esp - old_stack_ptr);

    for (i = (uint32_t)new_stack_ptr;
         i > (uint32_t)new_stack - size;
         i -= 4) {
        tmp = *(uint32_t*)i;
        if ((old_stack_ptr < tmp) && (tmp < initial_esp)) {
            tmp += offset;
            tmp2 = (uint32_t*)i;
            *tmp2 = tmp;
        }
    }

    asm volatile("mov %0, %%esp" :: "r"(new_stack_ptr));
    asm volatile("mov %0, %%esp" :: "r"(new_base_ptr));
}

void initialise_procs() {
    asm volatile("cli");

    move_stack((void*)0xE0000000, 0x2000);

    current_proc = ready_queue =
        (struct process *)kmalloc(sizeof(struct process));
    current_proc->id = next_pid++;
    current_proc->esp = current_proc->ebp = 0;
    current_proc->eip = 0;
    current_proc->pd = current_dir;
    current_proc->next = 0;

    asm volatile("sti");
}

int fork() {
    struct process* parent_proc = NULL;
    struct page_directory* dir = NULL;
    struct process* new_proc = NULL;
    struct process* tmp_proc = NULL;
    uint32_t eip = 0;
    uint32_t esp = 0;
    uint32_t ebp = 0;
    int ret = 0;

    asm volatile("cli");

    parent_proc = (struct process*)current_proc;
    dir = clone_directory(current_dir);
    new_proc = (struct process*)kmalloc(sizeof(struct process));

    new_proc->id = next_pid++;
    new_proc->esp = new_proc->ebp = 0;
    new_proc->eip = 0;
    new_proc->pd = dir;
    new_proc->next = 0;

    tmp_proc = (struct process*)ready_queue;

    while (tmp_proc->next) {
        tmp_proc = tmp_proc->next;
    }
    tmp_proc->next = new_proc;

    eip = read_eip();

    if (current_proc == parent_proc) {
        asm volatile("mov %%esp, %0" : "=r"(esp));
        asm volatile("mov %%ebp, %0" : "=r"(ebp));

        new_proc->esp = esp;
        new_proc->ebp = ebp;
        new_proc->eip = eip;

        asm volatile("sti");

        ret = new_proc->id;
    }

    return ret;
}

void proc_switch() {
    uint32_t esp = 0;
    uint32_t ebp = 0;
    uint32_t eip = 0;

    if (!current_proc) {
        return;
    }

    asm volatile("mov %%esp, %0" : "=r"(esp));
    asm volatile("mov %%ebp, %0" : "=r"(ebp));

    eip = read_eip();

    if (eip == 0x12345) {
        return;
    }

    current_proc->eip = eip;
    current_proc->esp = esp;
    current_proc->ebp = ebp;

    current_proc = current_proc->next;

    if (!current_proc) {
        current_proc = ready_queue;
    }

    eip = current_proc->eip;
    esp = current_proc->esp;
    ebp = current_proc->ebp;

    current_dir = current_proc->pd;

    asm volatile(" \
        cli; \
        mov %0, %%ecx; \
        mov %1, %%esp; \
        mov %2, %%ebp; \
        mov %3, %%cr3; \
        mov $0x12345, %%eax; \
        sti; \
        jmp *%%ecx"
        :: "r"(eip), "r"(esp), "r"(ebp), "r"(current_dir->phys_addr)
    );
}