ROOT_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

INCLUDE_DIR=$(ROOT_DIR)/include
SOURCES_DIR=src
ASM_DIR=asm

LDFLAGS= -Tlink.ld -melf_i386

BINARY=$(BUILD_DIR)/mini_os.elf
BINARY_DBG=$(BUILD_DIR)/mini_os.elf.dbg

all: link

dirs:
	@mkdir -p $(BUILD_DIR)/$(ASM_DIR)
	@mkdir -p $(BUILD_DIR)/$(SOURCES_DIR)

objects: dirs
	make BUILD_DIR=$(BUILD_DIR) --directory=asm
	make BUILD_DIR=$(BUILD_DIR) INCLUDE_DIR=$(INCLUDE_DIR) --directory=src

link: objects
	@ld $(LDFLAGS) -o $(BINARY) \
		$(shell find $(BUILD_DIR)/$(ASM_DIR) -type f -name '*.o') \
		$(shell find $(BUILD_DIR)/$(SOURCES_DIR) -type f -name '*.o')
	@cp $(BINARY) $(BINARY_DBG)
	@strip $(BINARY)

clean:
	make BUILD_DIR=$(BUILD_DIR) --directory=asm clean
	make BUILD_DIR=$(BUILD_DIR) INCLUDE_DIR=$(INCLUDE_DIR) \
		--directory=src clean
	@rm -f $(BINARY) $(BINARY_DBG)