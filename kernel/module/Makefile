MODULE_BUILD_DIR=$(BUILD_DIR)/module
MODULE=$(MODULE_BUILD_DIR)/multi_proc.ko

CFLAGS=-c -Wall -Wextra -pedantic-errors -fno-builtin -fno-stack-protector \
		-nostdinc -nostdlib -ggdb3 -m32 -I$(INCLUDE_DIR)
LDFLAGS= -Tlink.ld -melf_i386

SOURCES_C=multi_proc.c
OBJECTS_C=$(patsubst %.c, $(MODULE_BUILD_DIR)/%.o,$(SOURCES_C))

all: dirs $(MODULE)

dirs:
	@mkdir -p "$(MODULE_BUILD_DIR)"

$(MODULE): $(OBJECTS_C)
	ld $(LDFLAGS) -o $(MODULE) $(OBJECTS_C)
	strip $(MODULE)

$(OBJECTS_C): $(MODULE_BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(MODULE_BUILD_DIR)/*.o $(MODULE)

.PHONY: all clean