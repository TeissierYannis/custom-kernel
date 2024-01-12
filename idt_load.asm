; Loads the IDT defined in '_idtp' into the processor.
; This is declared in C as 'extern void idt_load();'
global idt_load
extern idt_ptr

idt_load:
    lidt [idt_ptr]  ; Load the IDT
    ret

