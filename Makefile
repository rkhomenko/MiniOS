BUILD_DIR=.
BINARY=$(BUILD_DIR)/mini_os.elf
IMAGE=$(BUILD_DIR)/disk.img
MODULE=$(BUILD_DIR)/module/multi_proc.ko

create_or_update_image:
	make BUILD_DIR=$(BUILD_DIR) --directory=kernel
	@./image_util.sh -i "$(IMAGE)" -e "$(BINARY)" -m "$(MODULE)"

clean:
	make BUILD_DIR=$(BUILD_DIR) --directory=kernel clean
	@rm -f $(IMAGE) $(MODULE)