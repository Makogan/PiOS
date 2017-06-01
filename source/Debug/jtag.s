/*
	Filename:	main.s
	Author:	Dmitry	Zavyalov
	Date:	05/19/2013
	Description:	Select	correct	GPIO	ALT	functions	to	enable	JTAG	and	loop	forever
	Platform:	BCM2835	on	Raspberry	PI	2.0
	Reference:	http://www.raspberrypi.org/wp-content/uploads/2012/02/BCM2835-ARM-Peripherals.pdf
*/

//	define	some	hardware	registers	necessary
.equ	GPFSEL0,	0x3F200000	//	GPIO	Function	Select	0
.equ	GPFSEL2,	0x3F200008	//	GPIO	Function	Select	2

.equ	GPPUD,		0x3F200094	//	GPIO	Pull-up/down	Register
.equ	GPPUDCLK0,	0x3F200098	//	GPIO	Pull-up/down	Clock

.section	.text

.globl EnableJTAG
EnableJTAG:
	push	{lr}
	//	disable	all	pull	ups/downs
	ldr		r0,	=GPPUD
	eor		r1,	r1
	str		r1,[r0]
	bl		dowait		//	wait	150	cycles	as	per	datasheet
	ldr		r0,	=GPPUDCLK0
	ldr		r1,	=0x0BC00010
	str		r1,[r0]
	bl		dowait		//	wait	150	cycles	again
	eor		r1,r1
	str		r1,[r0]
	
	//	define	ALT5	function	for	GPIO4	(JTAG	TDI	line)
	ldr		r0,	=GPFSEL0
	ldr		r1,[r0]
	bic		r1,	r1,	#0x7000
	orr		r1,	r1,	#0x2000
	str		r1,[r0]

	//	define	ALT4	functions	for	GPIOs	22,24,25,27
	ldr		r0,	=GPFSEL2
	ldr		r1,[r0]
	ldr		r2,	=0x00E3FFC0
	bic		r1,	r1,	r2
	ldr		r2,	=0x0061B6C0
	orr		r1,	r1,	r2
	str		r1,[r0]

	pop		{pc}

dowait:
	mov	r2,	#300
dowaitloop:
	subs	r2,	#1
	bne	dowaitloop
	bx	lr
