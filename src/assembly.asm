bits 64
section .text

global read_cr3
read_cr3:
    mov rax, cr3
    ret

global write_cr3
write_cr3:
    mov cr3, rdi
    ret

global read_cr0
read_cr0:
    mov rax, cr0
    ret

global write_cr0
write_cr0:
    mov cr0, rdi
    ret

global io_in
io_in:
    mov dx, di
    in eax, dx
    ret

global io_out
io_out:
    mov dx, di
    mov eax, esi
    out dx, eax
    ret
