# 1 "src/prekidna_rutina.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/riscv64-linux-gnu/include/stdc-predef.h" 1 3
# 32 "<command-line>" 2
# 1 "src/prekidna_rutina.S"
.global prekidna_rutina
.global prekid
.global mem_alloc_kernel
.global mem_free_kernel
.global runningT
.global thread_create_kernel
.global thread_dispatch_kernel
.global thread_exit_kernel
.global console_handler
.global sem_wait_kernel
.global sem_signal_kernel
.global sem_close_kernel
.global sem_open_kernel
.global putc_kernel
.global getc_kernel
.global timer

prekidna_rutina:
    #cuvanje t0 na stek
    addi sp, sp, -16
    sd t0, 0(sp)

    #cuvanje korsnickog sp
    ld t0, runningT
    sd sp, 16(t0)

    #dohvatanje novog ssp i prelazak na novi stek
    ld sp, 24(t0)


    #cuvanje registara
    addi sp, sp, -256
    sd zero, 0(sp)
    sd ra, 8(sp)
    sd sp, 16(sp)
    sd gp, 24(sp)
    sd tp, 32(sp)
    sd t0, 40(sp)
    sd t1, 48(sp)
    sd t2, 56(sp)
    sd s0, 64(sp)
    sd s1, 72(sp)
    sd a0, 80(sp)
    sd a1, 88(sp)
    sd a2, 96(sp)
    sd a3, 104(sp)
    sd a4, 112(sp)
    sd a5, 120(sp)
    sd a6, 128(sp)
    sd a7, 136(sp)
    sd s2, 144(sp)
    sd s3, 152(sp)
    sd s4, 160(sp)
    sd s5, 168(sp)
    sd s6, 176(sp)
    sd s7, 184(sp)
    sd s8, 192(sp)
    sd s9, 200(sp)
    sd s10, 208(sp)
    sd s11, 216(sp)
    sd t3, 224(sp)
    sd t4, 232(sp)
    sd t5, 240(sp)
    sd t6, 248(sp)

    csrr t0, scause
    li t1, 8
    li t2, 9
    beq t0,t1, softverski
    beq t0,t2, softverski
hardverski:

    li t0,0x8000000000000009
    csrr t1, scause
    beq t0, t1, preskok
    call timer
    ld a0, 80(sp)
    j kraj


preskok:
    call console_handler
    ld a0, 80(sp)
    j kraj

softverski:

    csrr t0, sepc
    addi t0, t0, 8 # ???
    csrw sepc, t0

    li t0, 1
    beq t0, a0, mem_alloc_labela
    li t0, 2
    beq t0, a0, mem_free_labela
    li t0, 17
    beq t0, a0, thread_create_labela
    li t0, 0x12
    beq t0, a0, thread_exit_labela
    li t0, 0x13
    beq t0, a0, thread_dispatch_labela
    li t0, 0x21
    beq t0, a0, sem_open_labela
    li t0, 0x22
    beq t0, a0, sem_close_labela
    li t0, 0x23
    beq t0, a0, sem_wait_labela
    li t0, 0x24
    beq t0, a0, sem_signal_labela
    li t0, 0x41
    beq t0, a0, getc_labela
    li t0, 0x42
    beq t0, a0, putc_labela

mem_alloc_labela:
    mv a0, a1
    addi sp, sp, -16
    sd ra, 0(sp)

    call mem_alloc_kernel

    ld ra, 0(sp)
    addi sp, sp, 16
    j kraj


mem_free_labela:
    mv a0, a1
    addi sp, sp, -16
    sd ra, 0(sp)

    call mem_free_kernel;

    ld ra, 0(sp)
    addi sp, sp, 16
    j kraj

thread_create_labela:
    mv a0, a1
    mv a1, a2
    mv a2, a3
    mv a3, a4
    addi sp, sp, -16
    sd ra, 0(sp)

    call thread_create_kernel

    ld ra, 0(sp)
    addi sp, sp, 16
    j kraj

thread_exit_labela:
    # mv a0, 0
    addi sp, sp, -16
    sd ra, 0(sp)

    call thread_exit_kernel

    ld ra, 0(sp)
    addi sp, sp, 16
    j kraj


thread_dispatch_labela:
    ld t0, runningT
    sd sp, 24(t0)

    addi sp, sp, -16
    sd ra, 0(sp)

    call thread_dispatch_kernel

    ld ra, 0(sp)
    addi sp, sp, 16

    ld t0, runningT
    ld sp, 24(t0)

    ld a0, 80(sp)
    j kraj


sem_signal_labela:
    mv a0, a1
    addi sp, sp, -16
    sd ra, 0(sp)

    call sem_signal_kernel

    ld ra, 0(sp)
    addi sp, sp, 16
    j kraj

sem_wait_labela:
    mv a0, a1
    addi sp, sp, -16
    sd ra, 0(sp)

    call sem_wait_kernel

    ld ra, 0(sp)
    addi sp, sp, 16
    j kraj
sem_close_labela:
    mv a0, a1
    addi sp, sp, -16
    sd ra, 0(sp)

    call sem_close_kernel

    ld ra, 0(sp)
    addi sp, sp, 16
    j kraj

sem_open_labela:
    mv a0, a1
    mv a1, a2
    addi sp, sp, -16
    sd ra, 0(sp)

    call sem_open_kernel

    ld ra, 0(sp)
    addi sp, sp, 16
    j kraj

putc_labela:
    mv a0, a1
    addi sp, sp, -16
    sd ra, 0(sp)

    call putc_kernel

    ld ra, 0(sp)
    addi sp, sp, 16
    j kraj

getc_labela:

    addi sp, sp, -16
    sd ra, 0(sp)

    call getc_kernel

    ld ra, 0(sp)
    addi sp, sp, 16
    j kraj

kraj:
    ld zero, 0(sp)
    ld ra, 8(sp)
  # ld sp, 16(sp)
    ld gp, 24(sp)
    ld tp, 32(sp)
    ld t0, 40(sp)
    ld t1, 48(sp)
    ld t2, 56(sp)
    ld s0, 64(sp)
    ld s1, 72(sp)
    ld a1, 88(sp)
    ld a2, 96(sp)
    ld a3, 104(sp)
    ld a4, 112(sp)
    ld a5, 120(sp)
    ld a6, 128(sp)
    ld a7, 136(sp)
    ld s2, 144(sp)
    ld s3, 152(sp)
    ld s4, 160(sp)
    ld s5, 168(sp)
    ld s6, 176(sp)
    ld s7, 184(sp)
    ld s8, 192(sp)
    ld s9, 200(sp)
    ld s10, 208(sp)
    ld s11, 216(sp)
    ld t3, 224(sp)
    ld t4, 232(sp)
    ld t5, 240(sp)
    ld t6, 248(sp)
# ne treba a0

    addi sp, sp, 256

    ld t0, runningT
    #cuvanje ssp
    sd sp, 24(t0)

    #vracanje na korisnicki
    ld sp, 16(t0)


    ld t0, 0(sp)

    addi sp,sp,16

    sret
