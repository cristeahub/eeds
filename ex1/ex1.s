.syntax unified

.include "efm32gg.s"

/////////////////////////////////////////////////////////////////////////////
//
// Exception vector table
// This table contains addresses for all exception handlers
//
/////////////////////////////////////////////////////////////////////////////

.section .vectors

.long   stack_top               /* Top of Stack                 */
.long   _reset                  /* Reset Handler                */
.long   dummy_handler           /* NMI Handler                  */
.long   dummy_handler           /* Hard Fault Handler           */
.long   dummy_handler           /* MPU Fault Handler            */
.long   dummy_handler           /* Bus Fault Handler            */
.long   dummy_handler           /* Usage Fault Handler          */
.long   dummy_handler           /* Reserved                     */
.long   dummy_handler           /* Reserved                     */
.long   dummy_handler           /* Reserved                     */
.long   dummy_handler           /* Reserved                     */
.long   dummy_handler           /* SVCall Handler               */
.long   dummy_handler           /* Debug Monitor Handler        */
.long   dummy_handler           /* Reserved                     */
.long   dummy_handler           /* PendSV Handler               */
.long   dummy_handler           /* SysTick Handler              */

/* External Interrupts */
.long   dummy_handler
.long   gpio_handler            /* GPIO even handler */
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   gpio_handler            /* GPIO odd handler */
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler

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

cmu_base_addr:
    .long CMU_BASE

gpio_pa_base_addr:
    .long GPIO_PA_BASE

// LOAD CMU BASE ADDRESS
ldr r1, cmu_base_addr

// load current value of HFPERCLK ENABLE
ldr r2, [r1, #CMU_HFPERCLKEN0]

// set bit for GPIO clk
mov r3, #1
lsl r3, r3, #CMU_HFPERCLKEN0_GPIO
orr r2, r2, r3

// store new value
str r2, [r1, #CMU_HFPERCLKEN0]


// ---> set high drive strength by writing 0x2 to gpio_pa_ctrl
// load gpio pa base
ldr r1, gpio_pa_base_addr

// set bit for GPIO_PA_CTRL
mov r2, #0x2

// store value
str r2, [r1, #GPIO_CTRL]

// --->
// r1 still has gpio_pa_base_addr in it
// set bit
mov r2, #0x55555555

// store value
str r2, [r1, #GPIO_MODEH]

// boot sequence
mov r2, #0x0100
str r2, [r1, #GPIO_DOUT]

mov r2, #0x0200
str r2, [r1, #GPIO_DOUT]

mov r2, #0x0400
str r2, [r1, #GPIO_DOUT]

mov r2, #0x0800
str r2, [r1, #GPIO_DOUT]

mov r2, #0x1000
str r2, [r1, #GPIO_DOUT]

mov r2, #0x2000
str r2, [r1, #GPIO_DOUT]

mov r2, #0x4000
str r2, [r1, #GPIO_DOUT]

mov r2, #0x8000
str r2, [r1, #GPIO_DOUT]

mov r2, #0xff00
str r2, [r1, #GPIO_DOUT]

mov r2, #0x0000
str r2, [r1, #GPIO_DOUT]

mov r2, #0xff00
str r2, [r1, #GPIO_DOUT]

mov r2, #0x0000
str r2, [r1, #GPIO_DOUT]

mov r2, #0x0000
str r2, [r1, #GPIO_DOUT]

// Setup for input

/////////////////////////////////////////////////////////////////////////////
//
// GPIO handler
// The CPU will jump here when there is a GPIO interrupt
//
/////////////////////////////////////////////////////////////////////////////

.thumb_func
gpio_handler:
b .  // do nothing

/////////////////////////////////////////////////////////////////////////////

.thumb_func
dummy_handler:  
b .  // do nothing

