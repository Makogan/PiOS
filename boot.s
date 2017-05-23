.section .init
.global _start

_start:
	ldr sp, =8000
	b kernel_main
