bits 32
section .text
	align 4
	dd 0x1BADB002
	dd 0x00
	dd - (0x1BADB002 + 0x00)

global start
extern k_main
extern idt_load

start:
	cli
	call k_main
	;call idt_load
	hlt
