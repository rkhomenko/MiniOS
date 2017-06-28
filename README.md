# MiniOS
## Build
Сборка может осуществляться в любом дистрибудиве GNU/Linux.
Для сборки необходимы: make, gcc, nasm. Для создания загрузочного образа
необходимо наличие в системе следующих утилит: mktemp, sudo, losetup,
parted, mkfs.ext2, partx, grub-install.
Во всех примерах BUILD_DIR - директория сборки.

### Сборка и создание загрузочного образа
```bash
make BUILD_DIR=/path/to/build/dir
```

### Запись загрузочного образа на диск
```bash
sudo dd if="$BUILD_DIR/disk.img" of=/dev/your_device bs=1M count=64
```

### Запуск виртуальной машины qemu
```bash
qemu-system-i386 -s -S -hda "$BUILD_DIR/disk.img"
```