; idt_load.asm

global idt_load
extern idt_ptr
idt_load:
    lidt [idt_ptr]  ; Load the IDT
    ret
