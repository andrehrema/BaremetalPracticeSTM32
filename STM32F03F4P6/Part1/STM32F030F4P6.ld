/* Define the end of RAM and limit of stack memory */
/* (4KB SRAM on the STM32F030F4P6 line, 4096 = 0x1000) */
/* (RAM starts at address 0x20000000) */

_estack = 0x20001000;


/*types of memory and available space*/
MEMORY
{
    /*The tutorial sets a flash size of 16K*/
    FLASH ( rx ) : ORIGIN = 0x08000000, LENGTH = 16K
    RAM ( rxw ) : ORIGIN = 0x20000000, LENGTH = 4K
}