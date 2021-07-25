.syntax unified
.cpu cortex-m0
.fpu softvfp
.thumb

.global Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
    //set stack pointer to the end of SRAM
ldr   r0, =_estack
    //Set the stack pointer to point to the end of the stack
    LDR r0, =_estack
    MOV sp, r0

    MOVS r0, #0
    LDR r1, =_sdata
    LDR r2, =_edata
    LDR r3, =_sidata
    B copy_sidata_loop

    copy_sidata:
        LDR r4, [r3, r0]
        STR r4, [r1, r0]
        ADDS r0, r0, #4

    //Copying data from flash to ram
    copy_sidata_loop:
        ADDS r4, r0, r1
        CMP r4, r2
        bcc copy_sidata

    MOVS r0, #0
    LDR r1, =_sbss
    LDR r2, =_ebss
    B reset_bss_loop

    
    reset_bss:
        STR r0, [r1]
        ADDS r1, r1, #4

    //setting all positions in BSS section to '0'
    reset_bss_loop:
        CMP r1, r2
        bcc reset_bss

    //branch to main
    B main

.size Reset_Handler, .-Reset_Handler