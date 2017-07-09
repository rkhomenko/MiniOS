SECTIONS
{
    . = 0x100;
    .signature :
    {
        *(.signature)
    }
    . = 0x1000;
    .start :
    {
        *(.start)
    }
}