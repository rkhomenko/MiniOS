#include <common.h>
#include <isr.h>
#include <kmalloc.h>
#include <memory.h>
#include <monitor.h>
#include <string.h>

#define ONE (0x1)

#define INDEX_FROM_BIT(X) ((X) / (8 * 4))
#define OFFSET_FROM_BIT(X) ((X) % (8 * 4))
#define IF_SET(X, Y) ((Y) = (X) ? 1 : 0)

struct page_directory* kernel_dir = NULL;
struct page_directory* current_dir = NULL;

static uint32_t* frames;
static uint32_t nframes;

extern uint32_t placement_address;

static void set_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr / 0x1000;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);
    frames[idx] |= (ONE << off);
}

static void clear_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr / 0x1000;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);
    frames[idx] &= ~(ONE << off);
}

/*
static uint32_t test_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr / 0x1000;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);
    return (frames[idx] & (ONE << off));
}
*/

static uint32_t first_frame() {
    uint32_t i = 0;
    uint32_t j = 0;
    uint32_t value = 0;
    for (i = 0; i < INDEX_FROM_BIT(nframes); i++) {
        if (frames[i] != 0xFFFFFFFF) {
            for (j = 0; j < 32; j++) {
                uint32_t to_test = ONE << j;
                if (!(frames[i] & to_test)) {
                    value = i * 4 * 8 + j;
                    goto ret;
                }
            }
        }
    }
ret:
    return value;
}

void alloc_frame(struct page* page, int is_kernel, int is_writeable) {
    if (page->frame != 0) {
        return;
    }

    uint32_t idx = first_frame();
    if (idx == UINT32_MAX) {
        PANIC("No free frames!");
    }
    set_frame(idx * 0x1000);

    page->present = 1;
    page->rw = (is_writeable) ? 1 : 0;
    page->user = (is_kernel) ? 0 : 1;
    page->frame = idx;
}

void free_frame(struct page* page) {
    uint32_t frame = page->frame;
    if (!frame) {
        return;
    }

    clear_frame(frame);
    page->frame = 0;
}

void initialise_paging() {
    const uint32_t MEM_END_PAGE = 0x1000000;
    uint32_t i = 0;

    nframes = MEM_END_PAGE / 0x1000;
    frames = (uint32_t*)kmalloc(INDEX_FROM_BIT(nframes));
    memset(frames, 0, INDEX_FROM_BIT(nframes));

    kernel_dir = (struct page_directory*)
                     kmalloc_a(sizeof(struct page_directory));
    memset(kernel_dir, 0, sizeof(struct page_directory));
    current_dir = kernel_dir;

    while (i < placement_address) {
        alloc_frame(get_page(i, 1, kernel_dir), 0, 0);
        i += 0x1000;
    }

    register_interrupt_handler(14, page_fault);
    switch_page_directory(kernel_dir);
}

void switch_page_directory(struct page_directory* dir) {
    uint32_t cr0 = 0;
    current_dir = dir;
    asm volatile("mov %0, %%cr3" :: "r"(&dir->tables_phys_addr));
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x800000000;
    asm volatile("mov %0, %%cr0" :: "r"(cr0));
}

struct page* get_page(uint32_t address,
                      int make,
                      struct page_directory* dir) {
    uint32_t table_idx = 0;
    uint32_t tmp = 0;

    address /= 0x1000;
    table_idx = address / 1024;
    if (dir->tables[table_idx]) {
        return &dir->tables[table_idx]->pages[address % 1024];
    }
    else if (make) {
        dir->tables[table_idx] =
            (struct page_table*)kmalloc_ap(sizeof(struct page_table), &tmp);
        memset(dir->tables[table_idx], 0, 0x1000);
        dir->tables_phys_addr[table_idx] = tmp | 0x7; // presen, rw, user
        return &dir->tables[table_idx]->pages[address % 1024];
    }
    return 0;
}

void page_fault(struct registers regs) {
    uint32_t faulting_address = 0;
    int present = !(regs.err_code & ONE);
    int rw = regs.err_code & 0x2;
    int us = regs.err_code & 0x4;
    int reserved = regs.err_code & 0x8;

    monitor_write("Page fault handler!\n");

    asm volatile("mov %%cr2, %0" : "=r"(faulting_address));

    monitor_write("Page fault: ");
    if (present) {
        monitor_write("present, ");
    }
    if (rw) {
        monitor_write("read only, ");
    }
    if (us) {
        monitor_write("user mode, ");
    }
    if (reserved) {
        monitor_write("reserved");
    }
    monitor_write(") at 0x");
    monitor_write_hex(faulting_address);
    monitor_write("\n");
    PANIC("Page fault");
}

void copy_page_physical(uint32_t, uint32_t);

static struct page_table* clone_table(struct page_table* src,
                                      uint32_t* phys) {
    struct page_table* table =
        (struct page_table *)kmalloc_ap(sizeof(struct page_table), phys);
    int i = 0;

    memset(table, 0, sizeof(struct page_directory));
    for (i = 0; i < PAGES_COUNT; i++) {
        if (src->pages[i].frame) {
            alloc_frame(&table->pages[i], 0, 0);
            IF_SET(src->pages[i].present, table->pages[i].present);
            IF_SET(src->pages[i].rw, table->pages[i].rw);
            IF_SET(src->pages[i].user, table->pages[i].user);
            IF_SET(src->pages[i].accessed, table->pages[i].accessed);
            IF_SET(src->pages[i].dirty, table->pages[i].dirty);

            copy_page_physical(src->pages[i].frame * 0x1000,
                               table->pages[i].frame * 0x1000);
        }
    }

    return table;
}

struct page_directory* clone_directory(struct page_directory* src) {
    uint32_t phys = 0;
    uint32_t offset = 0;
    struct page_directory* dir = kmalloc_ap(sizeof(struct page_directory), &phys);
    int i = 0;

    memset(dir, 0, sizeof(struct page_directory));
    offset = (uint32_t)dir->phys_addr - (uint32_t)dir;
    dir->phys_addr = phys + offset;

    for (i = 0; i < TABLES_COUNT; i++) {
        if (!src->tables[i]) {
            continue;
        }

        if (kernel_dir->tables[i] == src->tables[i]) {
            dir->tables[i] = src->tables[i];
            dir->tables_phys_addr[i] = src->tables_phys_addr[i];
        }
        else {
            dir->tables[i] = clone_table(src->tables[i], &phys);
            dir->tables_phys_addr[i] = phys | 0x07;
        }
    }
    return dir;
}

void set_placement_addr(uint32_t addr) {
    placement_address = addr;
}