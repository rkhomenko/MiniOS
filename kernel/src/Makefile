SOURCES_BUILD_DIR=$(BUILD_DIR)/src

CFLAGS=-c -Wall -Wextra -pedantic-errors -fno-builtin -fno-stack-protector \
		-nostdinc -nostdlib -ggdb3 -m32 -I$(INCLUDE_DIR)

SOURCES_C=common.c descriptor_tables.c isr.c main.c \
			monitor.c timer.c string.c load_disk.c load_module.c
OBJECTS_C=$(patsubst %.c, $(SOURCES_BUILD_DIR)/%.o,$(SOURCES_C))

all: $(OBJECTS_C)

$(OBJECTS_C): $(SOURCES_BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(SOURCES_BUILD_DIR)/*.o

.PHONY: all clean