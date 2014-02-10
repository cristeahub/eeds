.syntax unified

.include "lib/efm32gg.s"
.include "lib/vectors.s"

.section .text

/////////////////////////////////////////////////////////////////////////////
//
// Reset handler
// The CPU will start executing here after a reset
//
/////////////////////////////////////////////////////////////////////////////

.globl  _reset
.type   _reset, %function
.thumb_func
_reset:

    // aliases for constants, don't change these
    gpio .req r11
    gpio_o .req r10
    gpio_i .req r9
    cw .req r8
    ldr r11, =GPIO_BASE
    ldr r10, =GPIO_PA_BASE
    ldr r9, =GPIO_PC_BASE

    // LOAD CMU BASE ADDRESS
    ldr r1, =CMU_BASE
    ldr r2, [r1, #CMU_HFPERCLKEN0]
    mov r3, #1
    lsl r3, r3, #CMU_HFPERCLKEN0_GPIO
    orr r2, r2, r3
    str r2, [r1, #CMU_HFPERCLKEN0]

    // DISABLE LFA AND LFB
    ldr r2, [r1, #CMU_LFCLKSEL]
    mov r3, #1
    lsl r3, r3, #CMU_LFCLKSEL_LFAE
    orr r2, r2, r3
    lsl r3, r3, #CMU_LFCLKSEL_LFBE
    orr r2, r2, r3
    and r2, r2, #0x0
    str r2, [r1, #CMU_LFCLKSEL]


    // set high drive_strength
    mov r2, #0x2
    str r2, [gpio_o, #GPIO_CTRL]

    // set pins to output
    mov r2, #0x55555555
    str r2, [gpio_o, #GPIO_MODEH]

    // boot sequence
    bl boot_sequence

    // set up pins for input (gamepad)
    mov r2, #0x33333333
    str r2, [gpio_i, #GPIO_MODEL]
    mov r2, #0xff
    str r2, [gpio_i, #GPIO_DOUT]

    bl main


/////////////////////////////////////////////////////////////////////////////
//
// Main
// The main function that uses interrupts
// to check what buttons are being pressed
//
/////////////////////////////////////////////////////////////////////////////

.thumb_func
main:
    // set up automatic deep sleep
    ldr r2, =0x6
    ldr r3, =SCR
    str r2, [r3, #0]

    // set up interrupts
    mov r2, #0x22222222
    str r2, [gpio, #GPIO_EXTIPSELL]
    mov r2, #0xff
    str r2, [gpio, #GPIO_EXTIFALL]
    str r2, [gpio, #GPIO_EXTIRISE]
    str r2, [gpio, #GPIO_IEN]
    ldr r2, =0x802
    ldr r3, =ISER0
    str r2, [r3, #0]

    // input loop
    LOOP:
        wfi
        b LOOP

/////////////////////////////////////////////////////////////////////////////
//
// GPIO handler
// The CPU will jump here when there is a GPIO interrupt
//
/////////////////////////////////////////////////////////////////////////////

.thumb_func
gpio_handler:
    mov r2, #0xff
    str r2, [gpio, #GPIO_IFC]

    ldr r2, [gpio_i, #GPIO_DIN]
    lsl r2, r2, #8
    str r2, [gpio_o, #GPIO_DOUT]

    bx lr

/////////////////////////////////////////////////////////////////////////////
//
// Other
// Other functions as the result of procrastination
//
/////////////////////////////////////////////////////////////////////////////

.thumb_func
casual_wait:
    mov cw, #0x00a0000
cwait_loop:
    subs cw, #1
    bne cwait_loop
    bx lr

.thumb_func
start_seq:
    push {lr}

    bl high_rise

    // high rise
high_rise:
    ror r1, r2
    str r1, [gpio_o, #GPIO_DOUT]
    bl casual_wait
    subs r3, #1
    bne high_rise

    // reset constants
    mov r3, #8      // see line below with the same constant
    bl low_cooling

    // low cooling
low_cooling:
    lsl r1, r2
    str r1, [gpio_o, #GPIO_DOUT]
    bl casual_wait
    subs r3, #1
    bne low_cooling

    // reset constants prepare for ye olde big effect
    mov r3, #4
    bl ye_olde_big_effect

ye_olde_big_effect:
    // ((r1 & 0xf000) << 1 | (r1 & 0x0f00) >> 1) & 0xff00
    and r4, r1, #0xf000
    and r5, r1, #0x0f00
    lsl r4, r4, #1
    lsr r5, r5, #1
    orr r1, r4, r5
    and r1, r1, #0xff00
    str r1, [gpio_o, #GPIO_DOUT]
    bl casual_wait
    subs r3, #1
    bne ye_olde_big_effect

    pop {pc}

.thumb_func
boot_sequence:
    push {lr}

    mov r1, #0xff00 // start position for lights
    mov r2, #1      // positions rotated
    mov r3, #8      // jumps made by the led
    bl start_seq

    // reset lights, we are now ready for usage
    // at least the user thinks so
    // which doesn't make sense, since the users of this
    // board know exactly what's going on and are like
    // "why in gods name did they waste all that time??"
    // well to you we say: energy consumption over time!
    // as long as the board is on for long enough,
    // we are safe!

    mov r1, #0xff00
    str r1, [gpio_o, #GPIO_DOUT]

    pop {pc}

/////////////////////////////////////////////////////////////////////////////

.thumb_func
dummy_handler:
    b .  // do nothing

