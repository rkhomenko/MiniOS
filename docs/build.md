## Build
Сборка может осуществляться в любом дистрибутиве GNU/Linux.
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

### Запуск виртуальной машины QEMU
```bash
qemu-system-i386 -s -hda "$BUILD_DIR/disk.img"
```

### Отладка с помощью gdb
1. Запускаем QEMU с -S (машина должна быть приостановлена)
```bash
qemu-system-i386 -s -S -hda "$BUILD_DIR/disk.img"
```
2. Запускаем gdb
```bash
gdb $BUILD_DIR/mini_os.elf.dbg
```
3. В gdb
```gdb
target remote localhost:1234
break kernel_main
continue
```

Для удобства можно создать .gdbinit файл. При запуске
gdb автоматически выполнит все инструкции в нем. Для этого
нужно:
1. Прописать путь до .gdbinit в файл /home/$USER/.gdbinit
```gdb
add-auto-load-safe-path /absolute/path/to/.gdbinit
```
2. Создать сам файл .gdbinit с таким содержимым
```gdb
target remote localhost:1234
break kernel_main
continue
```

Также иногда удобно создавать точки останова
на какой-то строке файла, например
```gdb
target remote localhost:1234
break main.c:45
continue
```