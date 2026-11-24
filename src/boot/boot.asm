; ==============================
; KFS_1 - Boot Entry Point
; ==============================

section .multiboot
align   4

MB_MAGIC    equ 0x1BADB002
MB_FLAGS    equ 0x00000003
MB_CHECKSUM equ -(MB_MAGIC + MB_FLAGS)

dd  MB_MAGIC
dd  MB_FLAGS
dd  MB_CHECKSUM

; ==============================
; Kernel Stack
; ==============================

section .bss
align   16

stack_bottom:
    resb    16384
stack_top:

; ==============================
; Kernel Entry Point
; ==============================

section .text
global  _start
extern  kernel_main

; ==============================
; Kernel Starting Point
; ==============================

_start:
    cli
    mov esp, stack_top
    cld
    call kernel_main

.hang:
    hlt
    jmp .hang
