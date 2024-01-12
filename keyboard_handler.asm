global keyboard_interrupt_handler

extern keyboard_handler  ; Declare the C function

section .text
keyboard_interrupt_handler:
    pusha                   ; Push all general-purpose registers
    call keyboard_handler   ; Call the C keyboard handler
    popa                    ; Pop all general-purpose registers
    iret                    ; Return from interrupt
