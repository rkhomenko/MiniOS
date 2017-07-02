#ifndef MINI_OS_MEMORY_H_
#define MINI_OS_MEMORY_H_

#include <stddef.h>
#include <isr.h>

#define PAGES_COUNT 1024
#define TABLES_COUNT 1024

struct page {
    uint32_t present : 1; // Страница представленна в памяти?
    uint32_t rw : 1; // Если выставлен, то rw
    uint32_t user : 1;
    uint32_t accessed : 1; // Был доступ после крайнего обновления?
    uint32_t dirty : 1; // Была запись после крайнего обновления?
    uint32_t unused : 7; // Неиспользуемые биты
    uint32_t frame : 20; // Адрес фрейма
};

struct page_table {
    struct page pages[PAGES_COUNT];
};

struct page_directory {
    struct page_table* tables[TABLES_COUNT];
    uint32_t tables_phys_addr[TABLES_COUNT];
    uint32_t phys_addr;
};

void initialise_paging();
void switch_page_directory(struct page_directory *pd);
struct page *get_page(uint32_t address,
                      int make,
                      struct page_directory *pd);
void page_fault(struct registers regs);

#endif /* MINI_OS_MEMORY_H_ */