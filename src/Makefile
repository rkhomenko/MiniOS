BUILD_DIR=.
SOURCES_C=common.c descriptor_tables.c isr.c main.c monitor.c timer.c
SOURCES_ASM=boot.s gdt.s interrupt.s
OBJS=$(SOURCES_C:.c=.o) $(SOURCES_ASM:.s=.o)
OBJFILES=$(addprefix $(BUILD_DIR)/, $(OBJS))

CFLAGS= -c -Wall -Wextra -pedantic-errors -fno-builtin -fno-stack-protector -nostdinc -nostdlib -ggdb3 -m32
ASFLAGS= -felf
LDFLAGS= -Tlink.ld -melf_i386

BINARY=$(BUILD_DIR)/MiniOS.bin
BINARY_DBG=$(BUILD_DIR)/MiniOS.bin.dbg

all: $(OBJS) link

.c.o:
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

clean:
	@rm -f $(BUILD_DIR)/*.o $(BINARY) $(BINARY_DBG)

link:
	@ld $(LDFLAGS) -o $(BINARY) $(OBJFILES)
	@cp $(BINARY) $(BINARY_DBG) 
	@strip $(BINARY)

.s.o:
	@nasm $(ASFLAGS) $< -o $(BUILD_DIR)/$@