ASM_BUILD_DIR=$(BUILD_DIR)/asm

AS=nasm
ASFLAGS= -felf

SOURCES_ASM=boot.s gdt.s interrupt.s process.s
OBJECTS_ASM=$(patsubst %.s, $(ASM_BUILD_DIR)/%.o,$(SOURCES_ASM))

all: $(OBJECTS_ASM)

$(OBJECTS_ASM): $(ASM_BUILD_DIR)/%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	@rm -f $(ASM_BUILD_DIR)/*.o

.PHONY: all clean