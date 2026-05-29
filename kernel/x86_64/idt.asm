/* x86_64 Interrupt Descriptor Table setup */

.section .text
.global idt_init
idt_init:
    lidt idt_desc
    ret

/* ISR stubs for common exceptions */
.section .text
.global isr0
isr0:
    push $0
    push $0
    jmp isr_common

.global isr1
isr1:
    push $0
    push $1
    jmp isr_common

.global isr2
isr2:
    push $0
    push $2
    jmp isr_common

.global isr3
isr3:
    push $0
    push $3
    jmp isr_common

.global isr4
isr4:
    push $0
    push $4
    jmp isr_common

.global isr5
isr5:
    push $0
    push $5
    jmp isr_common

.global isr6
isr6:
    push $0
    push $6
    jmp isr_common

.global isr7
isr7:
    push $0
    push $7
    jmp isr_common

.global isr8
isr8:
    push $8
    jmp isr_common

.global isr10
isr10:
    push $0
    push $10
    jmp isr_common

.global isr11
isr11:
    push $0
    push $11
    jmp isr_common

.global isr12
isr12:
    push $0
    push $12
    jmp isr_common

.global isr13
isr13:
    push $0
    push $13
    jmp isr_common

.global isr14
isr14:
    push $0
    push $14
    jmp isr_common

.global isr16
isr16:
    push $0
    push $16
    jmp isr_common

.global isr17
isr17:
    push $0
    push $17
    jmp isr_common

.global isr18
isr18:
    push $0
    push $18
    jmp isr_common

.global isr19
isr19:
    push $0
    push $19
    jmp isr_common

.global isr20
isr20:
    push $0
    push $20
    jmp isr_common

/* IRQ handler stubs (PIC remapped to 32-47) */
.global irq0
irq0:
    push $0
    push $32
    jmp irq_common

.global irq1
irq1:
    push $0
    push $33
    jmp irq_common

/* Common ISR handler */
.section .text
.global isr_common
isr_common:
    push %rax
    push %rbx
    push %rcx
    push %rdx
    push %rsi
    push %rdi
    push %r8
    push %r9
    push %r10
    push %r11
    push %r12
    push %r13
    push %r14
    push %r15

    mov %rsp, %rdi
    call handle_exception

    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %r11
    pop %r10
    pop %r9
    pop %r8
    pop %rdi
    pop %rsi
    pop %rdx
    pop %rcx
    pop %rbx
    pop %rax
    add $16, %rsp
    iretq

/* Common IRQ handler */
.section .text
.global irq_common
irq_common:
    push %rax
    push %rbx
    push %rcx
    push %rdx
    push %rsi
    push %rdi
    push %r8
    push %r9
    push %r10
    push %r11
    push %r12
    push %r13
    push %r14
    push %r15

    mov %rsp, %rdi
    call handle_irq

    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %r11
    pop %r10
    pop %r9
    pop %r8
    pop %rdi
    pop %rsi
    pop %rdx
    pop %rcx
    pop %rbx
    pop %rax
    add $16, %rsp
    iretq

/* IDT structure */
.section .data
.align 8
idt:
    .space 256 * 16
idt_desc:
    .short 256 * 16 - 1
    .quad idt

/* Stub exception handler */
.section .text
.global handle_exception
handle_exception:
    ret

/* Stub IRQ handler */
.section .text
.global handle_irq
handle_irq:
    ret
