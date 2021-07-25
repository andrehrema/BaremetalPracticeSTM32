/*
    O objetivo do script de ligação é o de receber múltiplos arquivos de entrada (códigos objeto)
    e gerar um único arquivo de saída (binário).
*/


/*
    A instrução ENTRY define o ponto de entrada do código durante a execução.
    O ligador executará a seguinte ordem a fim de setar o entry point.

    The -e entry command-line option;
    The ENTRY (symbol) command in a linker script;
    The value of the symbol, start, if defined;
    The address of the first byte of the .text section, if present;
    The address, 0.
*/
ENTRY (Reset_Handler)

/*  initial stack pointer value
    It points to the end of the RAM memory
*/
_estack = 0x200017FF;

/* Memory spaced dedicated to SRAM and HEAP memory */
_Min_Leftover_RAM = 0x400;


/*
    Memory define o local e tamanho dos blocos de memória no dispositivo alvo.
    origin é o endereço inicial
*/
MEMORY
{
    FLASH ( rx ) : ORIGIN = 0x08000000, LENGTH = 16K
    RAM ( rw ) : ORIGIN = 0x20000000, LENGTH = 6K
}

SECTIONS
{
    /* . = location counter, específica o endereço inicial que o script de linking está montando */
    /* A seção vector_table será posicionada no endereço 0x0800 0000 -  início da memória flash*/
    .vector_table :
    {
        . = ALIGN(4);
        KEEP(*(.vector_table))
        . = ALIGN(4);
    } >FLASH

    /* .text = indica que o arquivo de saída tem que ter uma seção de nome text. O início da seção text
        começa no endereço definido no location counter + vector table.

        O text de saída (antes dos ':') abrange as seções de entrada (definidas dentro das chaves).

        O símbolo '*' indica que o nome escrito deve abranger as seções de todos os arquivos de entrada,
        i.e. *(.text): todas as seções text de entradas

    */
    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        . = ALIGN(4);
    } >FLASH

    /* A próxima seção inicia no fim da seção .text, a não ser que um novo location counter seja definido. */
    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);

    } >FLASH

    _sidata = .;
    .data :
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