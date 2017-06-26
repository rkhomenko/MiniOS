BUILD_DIR=.
BINARY=$(BUILD_DIR)/MiniOS.bin
IMAGE=$(BUILD_DIR)/hdd.img
TMP_DIR=$(BUILD_DIR)/tmp

update_image: compile
	@losetup /dev/loop1 ./hdd.img
	@echo "Mounting partition to /dev/loop2..."
	@losetup /dev/loop2 $(IMAGE) --offset `./find_offset.sh` --sizelimit `./find_size_limit.sh`
	@losetup -d /dev/loop1

	@echo "Write new MiniOS.bin to hdd.img..."
	@mkdir -p $(TMP_DIR)
	@mount /dev/loop2 $(TMP_DIR)
	@cp src/MiniOS.bin $(TMP_DIR)
	@umount /dev/loop2
	@rm -r $(TMP_DIR)
	@losetup -d /dev/loop2

compile:
	make BUILD_DIR=$(BUILD_DIR) --directory=src

image:
	@echo "Creating hdd.img..."
	@dd if=/dev/zero of=$(IMAGE) bs=512 count=32130 1>/dev/null 2>&1

	@echo "Creating bootable first FAT32 partition..."
	@losetup /dev/loop1 $(IMAGE)
	@(echo c; echo u; echo n; echo p; echo 1; echo ;  echo ; echo a; echo 1; echo t; echo c; echo w;) | fdisk /dev/loop1 1>/dev/null 2>&1 || true

	@echo "Mounting partition to /dev/loop2..."
	@losetup /dev/loop2 $(IMAGE) --offset `./find_offset.sh` --sizelimit `./find_size_limit.sh`
	@losetup -d /dev/loop1

	@echo "Format partition..."
	@mkdosfs -I -n 'MiniOS' /dev/loop2

	@echo "Installing GRUB..."
	@mkdir -p $(TMP_DIR)
	@mount /dev/loop2 $(TMP_DIR)
	@mkdir $(TMP_DIR)/boot
	@cp $(BINARY) $(TMP_DIR)
	@cp -r grub $(TMP_DIR)/boot
	@grub-install \
				--modules=part_msdos \
				--boot-directory=$(TMP_DIR)/boot \
				--force /dev/loop2
	@losetup -d /dev/loop2
	@rm -rf $(TMP_DIR)
	@echo "Done!"

clean:
	make BUILD_DIR=$(BUILD_DIR) clean --directory=src

umount:
	@umount /dev/loop2
	@losetup -d /dev/loop1
	@losetup -d /dev/loop2