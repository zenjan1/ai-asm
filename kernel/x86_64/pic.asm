/* x86_64 8259 PIC driver - IRQ remapping */

.equ PIC1_CMD, 0x20
.equ PIC1_DATA, 0x21
.equ PIC2_CMD, 0xa0
.equ PIC2_DATA, 0xa1
.equ PIC_EOI, 0x20

/* Remap PIC IRQs to 32-47 (above CPU exceptions) */
.section .text
.global pic_remap
pic_remap:
    push %rax

    /* Save masks */
    in $PIC1_DATA, %al
    push %rax
    in $PIC2_DATA, %al
    push %rax

    /* Start init sequence (ICW1) */
    mov $0x11, %al
    out %al, $PIC1_CMD
    out %al, $PIC2_CMD

    /* ICW2: Set vector offsets */
    mov $0x20, %al
    out %al, $PIC1_DATA
    mov $0x28, %al
    out %al, $PIC2_DATA

    /* ICW3: Cascade identity */
    mov $4, %al
    out %al, $PIC1_DATA
    mov $2, %al
    out %al, $PIC2_DATA

    /* ICW4: 8086 mode */
    mov $1, %al
    out %al, $PIC1_DATA
    out %al, $PIC2_DATA

    /* Restore masks */
    pop %rax
    out %al, $PIC2_DATA
    pop %rax
    out %al, $PIC1_DATA

    pop %rax
    ret

/* Send End of Interrupt */
.section .text
.global pic_send_eoi
pic_send_eoi:
    push %rax
    mov $PIC_EOI, %al
    out %al, $PIC1_CMD
    pop %rax
    ret

/* Disable PIC */
.section .text
.global pic_disable
pic_disable:
    push %rax
    mov $0xff, %al
    out %al, $PIC1_DATA
    out %al, $PIC2_DATA
    pop %rax
    ret

/* Enable specific IRQ line */
.section .text
.global pic_enable_irq
pic_enable_irq:
    push %rax
    push %rcx
    push %rdx

    mov %rdi, %rdx
    cmp $8, %rdx
    jl pic_enable_irq_1
    sub $8, %rdx
    mov %rdx, %rcx
    mov $PIC2_DATA, %rdx
    in %dx, %al
    btrl %ecx, %eax
    out %al, %dx
    jmp pic_enable_irq_done
pic_enable_irq_1:
    mov %rdx, %rcx
    mov $PIC1_DATA, %rdx
    in %dx, %al
    btrl %ecx, %eax
    out %al, %dx
pic_enable_irq_done:
    pop %rdx
    pop %rcx
    pop %rax
    ret
