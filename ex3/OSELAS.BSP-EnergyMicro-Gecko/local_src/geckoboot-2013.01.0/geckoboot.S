#define UARTBASE	0x4000e400
#define UARTLOCATION	2
#define DTB_SRC		0x80000
#define DTB_DST		0x10000000
#define LINUX_ENTRY	0x18000000

#define BC_REGISTER	0x80000000

	.syntax unified
	.thumb
	.int 0x10020000			@ Initial SP value
	.int reset + 1

#define UARTn_STATUS            0x0010
#define UARTn_STATUS_TXC                0x0020
#define UARTn_STATUS_TXBL               0x0040

#define UARTn_TXDATA            0x0034

	@ load UART base address into \rx
	.macro	addruart, rx, tmp
	ldr	\rx, =(0x4000e400)
	.endm

	@ send char \rd with \rx holding the UART base address
	.macro	senduart, rd, rx
	strb	\rd, [\rx, #UARTn_TXDATA]
	.endm

	@ wait for the UART to become ready to take another char
	.macro	waituart, rd, rx
1001:	ldr	\rd, [\rx, #UARTn_STATUS]
	tst	\rd, #UARTn_STATUS_TXBL
	beq	1001b
	.endm

	@ wait for the fifo and shifter to become idle
	.macro	busyuart, rd, rx
1001:	ldr	\rd, [\rx, #UARTn_STATUS]
	tst	\rd, #UARTn_STATUS_TXC
	bne	1001b
	.endm

/* printascii and printch are mostly copied from Linux v3.12, arch/arm/kernel/debug.S */

@ prints a zero-terminated string pointed to by r0
printascii:
	addruart r3, r1
	b	2f
1:	/* send char in r1 */
	waituart r2, r3
	senduart r1, r3
	teq	r1, #'\n'
	moveq	r1, #'\r'
	beq	1b
2:	/* load next char from r0 */
	teq	r0, #0
	ldrbne	r1, [r0], #1
	teqne	r1, #0
	bne	1b
	busyuart r2, r3
	bx	lr

@ prints a single char contained in r0
printch:
	addruart r3, r1
	mov	r1, r0
	mov	r0, #0
	b	1b

	.macro	putc, val, cond=al
	push	{r0-r4}
	mrs	r4, APSR		/* save cond flags */
	mov	r0, \val
	bl\cond printch
	msr	APSR_nzcvq, r4		/* restore cond flags */
	pop	{r0-r4}
	.endm

reset:
	/* init external RAM, serial port, EBI and stuff */
	adr	r0, reginit
1:
	ldr	r1, [r0]
	ldr	r2, [r0, #4]
	str	r2, [r1]
	add	r0, r0, #8
	cmp	r0, #(reginit_end)
	blo	1b

	/* init some BC registers */
	adr	r0, bcinit
1:
	ldrh	r1, [r0]
	ldrh	r2, [r0, #2]
	add	r1, r1, #BC_REGISTER
	strh	r2, [r1]
	add	r0, r0, #4
	cmp	r0, #(bcinit_end)
	blo	1b

	/* give mux some time to enable the level shifter */
	ldr	r0, =0x4000
1:	subs	r0, r0, #1
	bne	1b

	/* First sign of life */
	putc	#'G'

	/* if PB1 is pressed setup for SWO */
	ldr	r0, =(BC_REGISTER)
	ldr	r1, [r0, #8]		@ BC_REGISTER->UIF_PB
	tst	r1, #1
	putc	#'e', ne
	bne	wait_boot_linux
	putc	#'E'

	/* poll for CMU_STATUS_AUXHFRCORDY */
	ldr	r0, =(0x400c8000)
1:
	ldr	r1, [r0, 0x2c]		@ CMU_STATUS
	tst	r1, #0x20
	beq	1b

	adr	r0, swoinit
1:
	ldmia	r0!, {r1, r2, r3} /* load addr, mask, value */
	teq	r2, #0xffffffff
	ldrne	r4, [r1]
	bicne	r4, r2
	orrne	r3, r4
	str	r3, [r1]
	cmp	r0, #(swoinit_end)
	blo	1b

wait_boot_linux:
#ifdef CONFIG_WAIT_FOR_PB2
	putc	#'C'
	/* only boot into Linux when PB2 is pressed */
	ldr	r0, =(BC_REGISTER)
1:	ldr	r1, [r0, #8]		@ BC_REGISTER->UIF_PB
	tst	r1, #2
	bne	1b
#else
	putc	#'c'
#endif

boot_linux:
	putc	#'k'

	/* Copy oftree to RAM */
	ldr	r0, =(DTB_DST)
	ldr	r1, =(DTB_SRC)
	ldr	r2, [r1, #4]
	rev	r2, r2

	bl	memcpy

	putc	#'o'
	putc	#'\r'
	putc	#'\n'

	/* boot Linux */
	mov	r0, #0
	ldr	r1, =#0xf11
	ldr	r2, =(DTB_DST)
	mov	r3, #0
	mov	r4, #0
	mov	r5, #0
	mov	r6, #0

	ldr	r7, =(LINUX_ENTRY + 1)
	bx	r7

memcpy:
	@ copies r2 bytes from r1 to r0 with r2 > 0
	ldrb	r3, [r1], #1
	strb	r3, [r0], #1
	subs	r2, r2, #1
	bhi	memcpy
	bx	lr

	.ltorg

	.align 3
	.type reginit, %object
reginit:
	.int	0x400c8020, 0x00000014		@ CMU_OSCENCMD = HFXOEN | AUXHFRCOEN

	.int	0x400c8008, 0x00000100		@ CMU_HFPERCLKDIV, reset default
	.int	0x43900814, 0x00000001		@ CMU_HFCORECLKEN0 |= EBI via bitband
	.int	0x43900800, 0x00000001		@ CMU_HFCORECLKEN0 |= DMA via bitband
	.int	0x439008b4, 0x00000001		@ CMU_HFPERCLKEN0 |= GPIO via bitband
	.int	0x43900890, 0x00000001		@ CMU_HFPERCLKEN0 |= UART1 via bitband

	/* pinmuxing */
	/*  A0: EBI_AD09
	 *  A1: EBI_AD10
	 *  A2: EBI_AD11
	 *  A3: EBI_AD12
	 *  A4: EBI_AD13
	 *  A5: EBI_AD14
	 *  A6: EBI_AD15
	 * A15: EBI_AD08
	 */
	.int	0x40006014, 0x0000807f		@ GPIO_PA_DOUTCLR
	.int	0x40006004, 0x04444444		@ GPIO_PA_MODEL
	.int	0x40006008, 0x40000000		@ GPIO_PA_MODEH

	/*  B0: EBI_A16
	 *  B1: EBI_A17
	 *  B2: EBI_A18
	 *  B3: EBI_A19
	 *  B4: EBI_A20
	 *  B5: EBI_A21
	 *  B6: EBI_A22
	 *  B9: UART1_TX#2
	 * B10: UART1_RX#2
	 * B15: MCU_EBI_CONNECT, output, 0
	 */
	.int	0x40006038, 0x0000807f		@ GPIO_PB_DOUTCLR
	.int	0x40006034, 0x00000200		@ GPIO_PB_DOUTSET; set UART_TX to avoid false start
	.int	0x40006028, 0x04444444		@ GPIO_PB_MODEL
	.int	0x4000602c, 0x40000140		@ GPIO_PB_MODEH

	/* C11:	EBI_ALE
	 */
	.int	0x4000605c, 0x00000800		@ GPIO_PC_DOUTCLR
	.int	0x40006050, 0x00004000		@ GPIO_PC_MODEH

	/*  D0: USART1_TX#1 -> Ethernet (KSZ8851SNL)
	 *  D1: USART1_RX#1
	 *  D2: USART1_CLK#1
	 *  D3: USART1_CS#1
	 *  D9: EBI_CS0
	 * D10: EBI_CS1
	 * D11: EBI_CS2
	 * D12: EBI_CS3
	 * D13: #MCU_SPI_CONNECT, output, 1
	 * D14: I2C0_SDA#3
	 * D15: I2C0_SCL#3
	 */
	.int	0x4000607c, 0x0000fe08		@ GPIO_PD_DOUTSET
	.int	0x40006080, 0x00000007		@ GPIO_PD_DOUTCLR
	.int	0x40006070, 0x00004414		@ GPIO_PD_MODEL
	.int	0x40006074, 0x88444440		@ GPIO_PD_MODEH

	/*  E0: irq line from Board controller
	 *  E4: USART0_CS#1 -> Micro SD
	 *  E5: USART0_CLK#1
	 *  E6: USART0_RX#1
	 *  E7: USART0_TX#1
	 *  E8: EBI_AD00
	 *  E9: EBI_AD01
	 * E10: EBI_AD02
	 * E11: EBI_AD03
	 * E12: EBI_AD04
	 * E13: EBI_AD05
	 * E14: EBI_AD06
	 * E15: EBI_AD07
	 */
	.int	0x400060a0, 0x00000011		@ GPIO_PE_DOUTSET
	.int	0x400060a4, 0x0000ffe0		@ GPIO_PE_DOUTCLR
	.int	0x40006094, 0x41440002		@ GPIO_PE_MODEL
	.int	0x40006098, 0x44444444		@ GPIO_PE_MODEH

	/*  F6: EBI_BL0
	 *  F7: EBI_BL1
	 *  F8: EBI_WEn
	 *  F9: EBI_REn
	 */
	.int	0x400060c8, 0x000003c0		@ GPIO_PF_DOUTCLR
	.int	0x400060b8, 0x44000000		@ GPIO_PF_MODEL
	.int	0x400060bc, 0x00000044		@ GPIO_PF_MODEH

	.int	0x40006100, 0x00000004		@ GPIO_EXTIPSELL: select port E for irq 0
	.int	0x4000610c, 0x00000001		@ GPIO_EXTIFALL: trigger for falling FPGA irq line
	.int	0x4000611c, 0x0000ffff		@ GPIO_IFC: clear all irqs
	.int	0x40006110, 0x00000001		@ GPIO_IEN: enable irq 0

	/* EBI */
	.int	0x40008000, 0x4f00d051		@ EBI_CTRL, enable ITS, mode0=mode2=mode3=D16A16ALE, bl0-3, noidle[023]
	
	/* EBI PSRAM bank 2 */
	.int	0x40008028, 0x10000000		@ EBI_ADDRTIMING2; HALFALE
	.int	0x4000802c, 0x20000400		@ EBI_RDTIMING2; Prefetch, StrobeCycles = 4, HoldCycles = SetupCycles = 0
	.int	0x40008030, 0x00000200		@ EBI_WRTIMING2; StrobeCycles = 2, HoldCycles = SetupCycles = 0
	.int	0x40008034, 0x00000008		@ EBI_POLARITY2, ARDY_, ALE, WE_, RE_, CS_, BL_

	/* Board Control FPGA bank 0 */
	.int	0x40008004, 0x10000303		@ EBI_ADDRTIMING; HALFALE, HoldCycles = SetupCycles = 3
	.int	0x40008008, 0x00030703		@ EBI_RDTIMING; StrobeCycles = 7, HoldCycles = SetupCycles = 3
	.int	0x4000800c, 0x00030703		@ EBI_WRTIMING; StrobeCycles = 7, HoldCycles = SetupCycles = 3
	.int	0x40008010, 0x00000008		@ EBI_POLARITY, ARDY_, ALE, WE_, RE_, CS_, BL_

	/* TFT bank 1 */
	.int	0x40008018, 0x10000101		@ EBI_ADDRTIMING1; HALFALE, HoldCycles = SetupCycles = 1
	.int	0x4000801c, 0x00030703		@ EBI_RDTIMING1; StrobeCycles = 7, HoldCycles = SetupCycles = 3
	.int	0x40008020, 0x00010201		@ EBI_WRTIMING1; StrobeCycles = 2, HoldCycles = SetupCycles = 1
	.int	0x40008024, 0x00000008		@ EBI_POLARITY1, ARDY_, ALE, WE_, RE_, CS_, BL_

	/* external NOR flash bank 3 */
	.int	0x40008038, 0x10000000		@ EBI_ADDRTIMING3; HALFALE, HoldCycles = SetupCycles = 0
#define PRE
#ifdef PRE
	.int	0x4000803c, 0x00000700		@ EBI_RDTIMING3; StrobeCycles = 7, HoldCycles = SetupCycles = 0
	.int	0x40008040, 0x00000200		@ EBI_WRTIMING3; StrobeCycles = 2, HoldCycles = SetupCycles =0
#else
	/* reference: kits/common/bsp/bsp_dk_3201.c: 5 strobe cycles, hold=setup=0 */
	.int	0x4000803c, 0x00000700		@ EBI_RDTIMING3; StrobeCycles = 7, HoldCycles = SetupCycles = 0
	.int	0x40008040, 0x00000700		@ EBI_WRTIMING3; StrobeCycles = 7, HoldCycles = SetupCycles =0
#endif
	.int	0x40008044, 0x00000008		@ EBI_POLARITY3, ARDY_, ALE, WE_, RE_, CS_, BL_

	.int	0x40008014, 0x105e00bf		@ EBI_ROUTE, location=1
	.int	0x40008000, 0x4f00ff55		@ EBI_CTRL, enable ITS, mode0=mode1=mode2=mode3=D16A16ALE, bl0-3, noidle[0123], bank[0123]en

	.int	UARTBASE + 0x00, 0x00000000		@ UART1_CTRL
	.int	UARTBASE + 0x04, 0x00001005		@ UART1_FRAME
	.int	UARTBASE + 0x14, 0x00001900		@ UART1_CLKDIV
	.int	UARTBASE + 0x0c, 0x00000c04		@ UART1_CMD
	.int	UARTBASE + 0x54, 0x00000003 + (UARTLOCATION << 8)		@ UART1_ROUTE
	.int	0x400c8024, 0x00000002		@ CMU_CMD = HFCLKSEL_HFXO

reginit_end:
	.size reginit, . - reginit

	.align	3
	.type	bcinit, %object
bcinit:
	.short	0x0018, 0x1701			@ enable UART mux, I2C, ETH and spi
	.short	0x0014, 0x0001			@ /
	.short	0x001a, 0x0001			@ ETH
	.short	0x0042, 0x0010			@ workaround bug in BC's irq detection
	.short	0x0040, 0x0010

bcinit_end:
	.size	bcinit, . - bcinit

	.align 3
	.type	swoinit, %object
swoinit:
	.int	0x40006120, 0x00000304, 0x00000004	@ GPIO_ROUTE: SWOPEN + LOCATION = 0
	.int	0x400060b8, 0x00000f00, 0x00000400	@ configure F2 as pushpull output
	.int	0xe000edf0, 0x00000001, 0x00000001	@ CoreDebug->DHCSR |= 1
	.int	0xe000edfc, 0x01000000, 0x01000000	@ CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk
	.int	0xe0001000, 0xffffffff, 0x400113ff	@ Enable PC and IRQ sampling output
	.int 	0xe0040010, 0xffffffff, 0x0000000f	@ Set TPIU prescaler to 16.
	.int	0xe00400f0, 0xffffffff, 0x00000002	@ Set protocol to NRZ
	.int	0xe0000fb0, 0xffffffff, 0xc5acce55	@ Unlock ITM
	.int	0xe0000e80, 0xffffffff, 0x00010009	@ output data
swoinit_end:
	.size	swoinit, . - swoinit
