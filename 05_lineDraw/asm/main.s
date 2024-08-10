.arm
    .text
    .global main
    .align 4
main:
    ldr r3, =0x4000000       // Load display control base address
    ldr r2, =0x403           // Load display mode settings
    str r2, [r3]             // Set display control

    ldr r3, =0x6000000       // Load VRAM base address
    ldr r2, =0x035E0         // Load pixel color

    bl initialize            // Initialize line drawing parameters
    bl drawLine
    b plotEndPixel
    b end

initialize:
    mov r4, #0      // r4 = start.x =   0
    mov r5, #80     // r5 = end.x   =   0
    mov r6, #0      // r6 = start.y =   0
    mov r7, #120    // r7 = end.y   = 120

    sub r8, r5, r4  // r8 = dx      =  80
    sub r9, r7, r6  // r9 = dy      = 120 

    cmp r4, r5     // r10 = sx = 1
    movgt r10, #1 // if start.x < end.x sx = 1
    movlt r10, #-1// if start.x > end.x sx =-1
    
    cmp r6, r7     // r11 = sy = 1
    movgt r11, #1
    movlt r11, #-1

    sub r12, r8, r9 // r12 = err    = -40
    
    bx lr

drawLine:    
    bl plotPixel 

    mov r0, #0
    lsl r0, r12, #1 // r0 = e2      = -80
    neg r9, r9      // dy = -dy     =-120 
    cmp r0, r9      // r0 = -80 r9  =-120
    neg r9, r9      // r9 = dy      = 120
    bgt updateX
    //cmp r0, r8
    b updateY
    bx lr
    

updateX: 
    sub r12, r12, r9 // r12 = err - dy = -40 - 120 = -160
    add r4, r4, r10 // r4 start.x += sx = 0 + 1 = 1

    cmp r0, r8
    blt updateY 
updateY:
    add r12, r12, r8 //err = err + dx = -160 + 80
    add r6, r6, r11  // start.y += sy = 0 + 1 = 1

    bl checkX
    bl checkY
    
    bx lr
checkX:
    cmp r4, r5
    beq plotEndPixel
    bx lr
checkY:
    cmp r6, r7
    beq plotEndPixel
    bx lr
plotPixel:
    mov r0, #240
    mul r0, r6, r0
    add r0, r0, r4
    strh r2, [r3, r0]
    bx lr
plotEndPixel: 
    mov r0, #240
    mul r0, r7, r0
    add r0, r0, r5
    strh r2, [r3, r0]
    b end
end:
    b end
