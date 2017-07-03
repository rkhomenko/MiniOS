# Реализация
## Структура загрузочного диска
На загрузочном диске имеется файловая система ext2. Содержимое:
```text
|
+--- boot/
|   +--- grub/
|           +--- grub.cfg - файл конфигурации grub2
|           +--- background.jpg - нескучный фон
|   +--- kernel/
|           +--- mini_os.elf - непосредственно ядро
|   +--- mods/
|           +--- multi_proc.ko - модули ядра
```
## Модули ядра
Так как из ядра операционной системы мы хотим вызывать
процедуры из модулей, то мы должны знать их смещение от начала
модуля. Следующий файл конфигурации линкера ld позволяет задать
смещение 0x1000 для процедуры module_main
```ld
SECTIONS
{
    . = 0x1000;
    .start :
    {
        *(.start)
    }
}
```
Сама процедура module_main должна быть предварительно
объявлена следующим образом
```C
int module_main() __attribute__((section(".start")));
```

## Загрузка модулей ядра
Само ядро не умеет работать с диском. Для это нужно было бы
писать реализацию работы с SATA. В случае успеха всё равно нужно
уметь работать с ext2 или fat (самые простые файловые системы).

В связи с невозможностью реализации работы с диском в разумные
сроки, было принято решение использовать функционал загрузчика
GRUB2. Следующий файл конфигурации позволяет загрузить ядро
и модули в оперативную память.
```text
menuentry "MiniOS" {
    insmod part_msdos
    insmod ext2
    insmod elf

    multiboot /boot/kernel/mini_os.elf
    module /boot/mods/multi_proc.ko
}
```
Адрес загруженного модуля предается в процедуру kernel_main
через указатель на структуру multiboot
[см. документацию multiboot](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html).

## Вызов процедур ядра из модуля
В процедуру module_main будем передавать указатель на структуру,
```C
struct func_table {
    void* func_ptrs[FUNC_MAX];
};
```
которая будет хранить адреса всех необходимых процедур ядра.
Файл func_table.h содержит индексы массива для этих структур
```C
enum {
    MULTIPLAY = 0,
    DIV,
    ADD
    // ...
};
```
Далее легко можно получить указатель на необходимую процедуру
и вызвать её
```C
typedef uint32_t (*multiplay_ptr)(uint32_t x, uint32_t y);

int module_main(struct func_table* ft) {
    const uint32_t DEADBEAF = 0xdeadbeef;
    multiplay_ptr multiplay = NULL;

    SET_FUNC_PTR(multiplay, ft->func_ptrs[MULTIPLAY]);

    return multiplay(DEADBEAF / 2, 2) + DEADBEAF % 2;
}
```