.syntax unified
.cpu cortex-m0
//The microcontroller doesn't have floating point hardware
.fpu softvfp
//Selection of the instruction set
.thumb

//Global memory allocation
//.global enusres that the variables are accessible
//in other files
.global vtable
.global reset_handler

.type vtable, %object
vtable:
    //End of stack
    .word _estack
    .word reset_handler
.size vtable, .-vtable


.type reset_handler, %function
reset_handler:
    LDR r0, =_estack
    MOV sp, r0

    //The symbol '=' tells the assembler that we are loading a literal value into the register
    LDR r7, =0xDEADBEEF
    MOVS r0, #0
    main_loop:
        ADDS r0, r0, #1
    B main_loop
.size reset_handler, .-reset_handler