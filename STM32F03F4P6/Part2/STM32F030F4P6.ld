/* Program's entry point */
ENTRY(reset_handler)

/* End of the stack */
_estack = 0x20001000;

/* Minimum size of dynamic allocation memory (stack and heap)
  1KB */
_Min_Leftover_RAM = 0x400;

MEMORY
{
  FLASH ( rx )      : ORIGIN = 0x08000000, LENGTH = 16K
  RAM ( rxw )       : ORIGIN = 0x20000000, LENGTH = 4K
}

SECTIONS
{
    .vector_table :
    {
        /*
        '.' represents the location counter.
        the location counter sets the address of the next symbol

        In this case, the location counter is aligned with the value 4
        so the vector table will start in address ending with the value 4
        */
        . = ALIGN(4);
        
        /* The keep statement forces the linker to mantain the specified section
         The vector_table must not be pruned out from the out object. */
        KEEP (*(.vector_table))
        . = ALIGN(4);
    } >FLASH

    .text :
    {
        . = ALIGN(4);
        /* selection all *.text sections found on the linking process */
        *(.text)
        *(.text*)
        . = ALIGN(4);
    } > FLASH

    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
    } > FLASH
    
    _sidata = .;
    .data : AT(_sidata)
    {
        . = ALIGN(4);
        /* Mark start/end locatin for the 'data' section. */
        _sdata = .;
        *(.data)
        *(.data*)
        _edata = .;
        . = ALIGN(4);
    } > RAM

    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } > RAM

    .dynamic_allocations :
    {
        . = ALIGN(4);
        _ssystem_ram = .;
        . = . + _Min_Leftover_RAM;
        . = ALIGN(4);
        _esystem_ram = .;
    } > RAM
}