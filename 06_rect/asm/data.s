.arm
.global main

.section .rodata  @ Read-only data section
color_green: 
    .hword 0x03E0 @ Green color value (RGB 0, 31, 0)

.section .data  @ Initialized data section
square_offset:
    .word 0x9680  @ Top-left corner offset in VRAM
square_size:
    .word 20      @ Height and width of the square
y_offset:
    .word 9600    @ Y offset for drawing

.section .bss  @ Uninitialized data section
    .comm drawFlag, 4  @ Example uninitialized global variable (not used here, but to show .bss usage)

.section .text  @ Code section
main:
    ldr r0, =0x4000000            @ Load base address for display control
    ldr r1, =0x403                @ Set Mode 3 and enable BG2
    str r1, [r0]                  @ Write to display control register

    ldr r0, =0x6000000            @ Load base address for VRAM
    ldr r1, =color_green          @ Load the address of the color from .rodata
    ldrh r1, [r1]                 @ Load the actual color value (green) into r1

    ldr r2, =square_offset        @ Load the address of the square offset
    ldr r2, [r2]                  @ Load the actual offset value into r2

    mov r3, #20                   @ Load square size into r3 (height/width)
    ldr r5, =y_offset             @ Load address of the y offset
    ldr r5, [r5]                  @ Load the actual y offset into r5

    add r4, r0, r2                @ Calculate starting point in VRAM (base + offset)
    add r6, r4, r5                @ Calculate end of vertical line (base + offset + y offset)

draw_horizontal:
    strh r1, [r4], #2             @ Draw pixel horizontally, increment by 2 (16-bit)
    strh r1, [r6], #2             @ Draw pixel horizontally at lower line
    subs r3, r3, #1               @ Decrement the loop counter (width/height)
    bne draw_horizontal           @ Repeat until r3 reaches zero
    strh r1, [r4]
setup_v_draw:
    add r4, r0, r2                @ Reset starting corner
    mov r3, #20                   @ Reset the counter for vertical drawing
    mov r5, #240                  @ Set row size (240 pixels per row)
    add r5, r5, r5                @ Multiply by 2 for 16-bit pixels (240 * 2)

draw_vertical:
    strh r1, [r4]                 @ Draw pixel vertically
    add r4, r4, r5                @ Move down one row (240 pixels forward)
    strh r1, [r6]                 @ Draw corresponding pixel at the opposite side
    sub r6, r6, r5                @ Move back one row for opposite side

    subs r3, r3, #1               @ Decrement the loop counter
    bne draw_vertical             @ Repeat until r3 reaches zero

infin:
    b infin                       @ Infinite loop to keep the program running
