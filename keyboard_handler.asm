global keyboard_interrupt_handler

section .text
keyboard_interrupt_handler:
    pusha                   ; Push all general-purpose registers
    call keyboard_handler   ; Call your C handler
    popa                    ; Pop all general-purpose registers
    iret                    ; Return from interrupt
