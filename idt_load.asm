; idt_load.asm

global idt_load
idt_load:
    lidt [idt_ptr]  ; Load the IDT
    ret
