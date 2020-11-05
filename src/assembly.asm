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

global read_cr4
read_cr4:
    mov rax, cr4
    ret
global write_cr4
write_cr4:
    mov cr4, rdi
    ret

global read_cr0
read_cr0:
    mov rax, cr0
    ret

global write_cr0
write_cr0:
    mov cr0, rdi
    ret

global io_in8
io_in8:
    mov rdx, rdi
    in al, dx
    ret

global io_out8
io_out8:
    mov rdx, rdi
    mov rax, rsi
    out dx, al
    ret

global io_in32
io_in32:
    mov dx, di
    in eax, dx
    ret

global io_out32
io_out32:
    mov dx, di
    mov eax, esi
    out dx, eax
    ret
global load_gdt
load_gdt:
    push rbp
    mov rbp, rsp
    sub rsp, 0x10
    mov rdx, rsp
    mov [rdx], si
    mov [rdx + 2], rdi
    lgdt [rdx]
    add rsp, 0x10
    mov rsp, rbp
    pop rbp
    ret

global set_ds
set_ds:
    mov ds, di
    mov es, di
    mov ss, di
    mov gs, di
    mov fs, di
    ret

global set_cs
set_cs:
    push rbp
    mov rbp,rsp
    push rdi 
    push label
    o64 retf

global label
label:
    mov     rsp,rbp
    pop     rbp
    ret

global load_idt
load_idt:
    push rbp
    mov rbp, rsp
    sub rsp, 10
    mov [rsp], di
    mov [rsp + 2], rsi
    lidt [rsp]
    mov rsp, rbp
    pop rbp
    ret

global cpuidf
cpuidf:
    mov rax, 0x1
    cpuid
    mov rax, rcx
    ret

global read_msr
read_msr:
    mov ecx, edi
    rdmsr
    shl rdx, 32
    or rax, rdx
    ret

global write_msr
write_msr:
    mov rdx, rsi
    shr rdx, 32
    mov eax, esi
    mov ecx, edi
    wrmsr
    ret

global exec_vmxon
exec_vmxon:
    vmxon [rdi]
    jbe vmx_fail
    jmp vmx_success
    ret

global vmx_fail
vmx_fail:
    mov rax, 0x0
    ret

global vmx_success
vmx_success:
    mov rax, 0x01
    ret

global exec_vmxoff
exec_vmxoff:
    vmxoff
    ret

global disable_a20
disable_a20:
    out 0xee, al
    ret

global check_cf
check_cf:
    xor rax, rax
    setc al
    ret

global check_zf
check_zf:
    xor rax, rax
    sete al
    ret

global check_pf
check_pf:
    xor rax, rax
    setp al
    ret

global check_sf
check_sf:
    xor rax, rax
    setno al
    ret

global read_flags
read_flags:
    xor rax, rax
    lahf
    ret


    

