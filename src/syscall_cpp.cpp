//
// Created by os on 7/18/22.
//
#include "../h/syscall_cpp.hpp"
extern "C" void* mem_alloc_kernel(size_t size);
void* operator new(size_t size){
  //  return mem_alloc(size);
    return mem_alloc_kernel(size/MEM_BLOCK_SIZE+1);
}

void operator delete(void * ptr){
    mem_free(ptr);
}

Thread* Thread::niz_nit= nullptr;
uint8 Thread::zauzeto[MAX_NUM_OF_THREADS]={0};

uint8 Semaphore::zauzeto[MAX_NUM_OF_THREADS]={0};
Semaphore* Semaphore::semafori= nullptr;