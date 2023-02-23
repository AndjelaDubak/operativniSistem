#include "../lib/hw.h"

struct _thread;
typedef struct _thread* thread_t;
struct _sem;
typedef struct _sem* sem_t;

void* mem_alloc_abi(uint64 kod, size_t blokovi) {
    asm volatile("ecall");
    void *ret_adr;
    asm volatile("mv %0 , a0" : "=r"(ret_adr));
    return ret_adr;
}

int mem_free_abi(uint64 kod, void *ptr) {
    asm volatile("ecall");
    int ret_adr;
    asm volatile("mv %0 , a0" : "=r"(ret_adr));
    return ret_adr;
}

int thread_create_abi(uint64 kod, thread_t* handle,
                      void(*start_routine)(void*),
                      void* arg,
                      void* stack_space) {
    asm volatile("ecall");
    int ret_adr;
    asm volatile("mv %0 , a0" : "=r"(ret_adr));
    return ret_adr;
}

int thread_exit_abi(uint64 kod) {
    asm volatile("ecall");
    int ret_adr;
    asm volatile("mv %0 , a0" : "=r"(ret_adr));
    return ret_adr;
}
void thread_dispatch_abi(uint64 kod) {
    asm volatile("ecall");
}

int sem_open_abi(uint64 kod,sem_t * handle,unsigned init) {
    asm volatile("ecall");
    int ret_adr;
    asm volatile("mv %0 , a0" : "=r"(ret_adr));
    return ret_adr;
}

int sem_close_abi(uint64 kod,sem_t id) {
    asm volatile("ecall");
    int ret_adr;
    asm volatile("mv %0 , a0" : "=r"(ret_adr));
    return ret_adr;
}


int sem_wait_abi(uint64 kod,sem_t id) {
    asm volatile("ecall");
    int ret_adr;
    asm volatile("mv %0 , a0" : "=r"(ret_adr));
    return ret_adr;
}


int sem_signal_abi(uint64 kod,sem_t id) {
    asm volatile("ecall");
    int ret_adr;
    asm volatile("mv %0 , a0" : "=r"(ret_adr));
    return ret_adr;
}

void putc_abi(uint64 kod,char c){
    asm volatile("ecall");
}
char getc_abi(uint64 kod){
    asm volatile("ecall");
    char ret_adr;
    asm volatile("mv %0 , a0" : "=r"(ret_adr));
    return ret_adr;
}