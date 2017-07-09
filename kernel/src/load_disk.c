#include <stddef.h>
#include <string.h>

extern void load_disk(void); /* load_disk function from load_disk.s */

uint32_t get_module(void) {
    const char* MODULE_SIGNUTURE = "I'M MINI_OS MODULE!";
    const uint32_t MODULE_SIGNATURE_OFFSET = 0x100;
    const uint32_t MODULE_MAIN_OFFSET = 0x1000;
    uint32_t i = 0;
    uint32_t signature_start = 0;

    load_disk(); /* load disk at address (0x1000:0x7c00) */

    for (i = 0x1000; i < 0x7c00; i++) {
        if (strcmp(MODULE_SIGNUTURE, (char*)i) == 0) {
            signature_start = i;
            break;
        }
    }

    return signature_start + (MODULE_MAIN_OFFSET - MODULE_SIGNATURE_OFFSET);
}