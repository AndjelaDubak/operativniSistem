//
// Created by os on 7/14/22.
//

#include "../h/syscall_c.h"
#include "../lib/console.h"

extern void* mem_alloc_abi(uint64, size_t);
extern int mem_free_abi(uint64, void*);
extern int thread_create_abi(uint64 kod, thread_t* handle,
                             void(*start_routine)(void*),
                             void* arg,
                             void* stack_space);
extern int thread_exit_abi(uint64 kod);

void* mem_alloc(size_t size) {

    size_t brojBlokova= size / MEM_BLOCK_SIZE;
    brojBlokova += (size % MEM_BLOCK_SIZE == 0) ? 0 : 1;

    return mem_alloc_abi(0x01, size);
}

int mem_free(void* ptr) {
    return mem_free_abi(0x02, ptr);
}

int thread_create (thread_t* handle,
                   void(*start_routine)(void*),
                   void* arg) {
    void *stack_space = mem_alloc(DEFAULT_STACK_SIZE);
    if (stack_space == 0) return -1;
    int a = thread_create_abi(0x11, handle, start_routine, arg, stack_space);

    return a;
}

int thread_exit() {
    int a= thread_exit_abi(0x12);
    if(a==0)
        thread_dispatch();

    return a;
}
extern void thread_dispatch_abi(uint64);
void thread_dispatch(){
    thread_dispatch_abi(0x13);
}

uint64 old_interrupt;
extern void prekidna_rutina();
sem_t sem_for_get_c;

extern void putc_abi(uint64,char);
extern char getc_abi(uint64);
char getc(){

  //  char c= __getc();
  //  return c;
    sem_wait(sem_for_get_c);
    return getc_abi(0x41);
    }

void putc(char c){
    //__putc(c);
    putc_abi(0x42,c);
}

extern int sem_open_abi(uint64,sem_t*,unsigned );
extern int sem_close_abi(uint64,sem_t);
extern int sem_wait_abi(uint64,sem_t);
extern int sem_signal_abi(uint64,sem_t);
int sem_open (sem_t* handle, unsigned init){
    return sem_open_abi(0x21,handle,init);
}

int sem_close (sem_t handle)
{
    return sem_close_abi(0x22,handle);
}
int sem_wait (sem_t id){
    int a= sem_wait_abi(0x23,id);
   // if(a==0)
   thread_dispatch();

    return a;
}

int sem_signal (sem_t id){
    return sem_signal_abi(0x24,id);
}

