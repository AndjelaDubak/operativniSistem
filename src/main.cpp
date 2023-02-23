//
// Created by os on 5/24/22.
//
#include "../lib/console.h"
#include "../lib/hw.h"

#include "../h/syscall_cpp.hpp"

extern "C" void prekidna_rutina();
extern "C" void init();
extern "C" void* mem_alloc_kernel(size_t size);
extern "C" int mem_free_kernel(void *ptr);
extern "C" int main_thread_init();
extern "C" void init_scheduler();
extern "C" void idle_init();
sem_t sem;
int kraj=0;
void t1(void*a){
    for (int i = 0; i < 10; ++i) {
 //       __putc('A'+(uint64)a);
        thread_dispatch();
    }
    getc();
    sem_signal(sem);
}
extern void userMain();
extern "C" uint64 old_interrupt;
extern "C" void initBuffer();
extern "C" void init_sem_for_console();
int main() {

    asm volatile ("csrr %0, stvec" : "=r"(old_interrupt));
    initBuffer();
    init();
    main_thread_init();
    init_sem_for_console();
    asm volatile ("csrw stvec, %0" : : "r"(prekidna_rutina));
    init_scheduler();
    idle_init();
    Thread::init_Thread();
    Semaphore::init_Semaphore();
    uint64 sstatus;
    asm volatile("csrr %0, sstatus" : "=r" (sstatus));
    sstatus |= 1 << 1;
    asm volatile("csrw sstatus, %0" : : "r" (sstatus));

    putc('a');
    userMain();



    return 0;
}
