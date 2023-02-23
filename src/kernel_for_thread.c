#include "../lib/hw.h"
#include "../lib/console.h"
extern void * mem_alloc_kernel(size_t size);
extern int mem_free_kernel(void * ptr);
int thread_exit_kernel ();
struct _thread {
    void *stack;
    void *sstack;
    uint64 sp;
    uint64 ssp;
    uint64 blokirana;
    uint64 gotova;
    uint64 pc;
    void(*start_routine)(void*);
    void* arg;
};

typedef struct _thread* thread_t;
thread_t runningT;
extern void put(thread_t);
extern int thread_exit();
void wrapper_function() {

    runningT->start_routine(runningT->arg);

    //kroz listu allThreads i po id

    thread_exit();

}

int thread_create_kernel(
                        thread_t* handle,
                        void(*start_routine)(void*),
                        void* arg,
                        void* stack_space
) {
    uint64 size = sizeof(struct _thread)/MEM_BLOCK_SIZE;
    size += (sizeof(struct _thread) % MEM_BLOCK_SIZE) == 0 ? 0 : 1;
    *handle = mem_alloc_kernel(size);


    void * sstack_space = mem_alloc_kernel(512 / MEM_BLOCK_SIZE);

    if(sstack_space == 0) {
        mem_free_kernel(stack_space);
        mem_free_kernel(*handle);
        return -1;
    }

    thread_t tmp = *handle;

    tmp->blokirana = 0;
    tmp->gotova = 0;
    tmp->stack = stack_space;
    tmp->sstack = sstack_space;
    tmp->sp = (uint64)stack_space + DEFAULT_STACK_SIZE - (uint64)stack_space % 16;
    tmp->sp -= 32;
    tmp->ssp = (uint64)sstack_space + 512 - (uint64)sstack_space % 16;
    tmp->ssp = tmp->ssp - 256;
    for (int i = 0; i < 32 ; ++i) {
        ((uint64 *) tmp->ssp)[i] = 0;
    }
 //   tmp->ssp=tmp->ssp-32;
    ((uint64 *) tmp->ssp)[1] = tmp->pc;
    tmp->arg = arg;
    tmp->start_routine = start_routine;
    tmp->pc = (uint64)wrapper_function;

    //UBACITI U SCHEDULER
    put(tmp);

    return 0;

}

thread_t main_thread;
int main_thread_init() {
    uint64 size = sizeof(struct _thread)/MEM_BLOCK_SIZE;
    size += sizeof(struct _thread) % MEM_BLOCK_SIZE == 0 ? 0 : 1;
    main_thread = mem_alloc_kernel(size);

    if(main_thread == 0){
        return -1;
    }

    void * sstack_space = mem_alloc_kernel(512/MEM_BLOCK_SIZE);

    if(sstack_space == 0){
        mem_free_kernel(main_thread);
        return -1;
    }

    thread_t tmp = main_thread;

    tmp->blokirana = 0;
    tmp->gotova = 0;
    tmp->sstack = sstack_space;
    tmp->ssp = (uint64)sstack_space + 512 - (uint64)sstack_space % 16;
    runningT = tmp;

    return 0;

}

int thread_exit_kernel () {
    int p1 = mem_free_kernel(runningT->stack);
    int p2 = mem_free_kernel(runningT->sstack);
    int p3 = mem_free_kernel(runningT);
    runningT->gotova=1;

    if(p1 !=0 || p2 != 0 || p3 != 0) return -1;

    return 0;
}
extern thread_t get();
thread_t idle;
void thread_dispatch_kernel() {
//    asm volatile("mv t0 , %0" : : "r"(runningT));
//    asm volatile ("sd sp, 24(t0)");

    if(runningT->gotova==0 && runningT->blokirana==0 )
        put(runningT);

    thread_t new_thread=get();
    if(new_thread==0)new_thread=idle;

    if(new_thread==runningT)return;
    asm volatile("csrr %0, sepc":"=r"(runningT->pc));

    asm volatile ("csrw sepc, %0" : : "r"(new_thread->pc));
    runningT=new_thread;
//    asm volatile("mv t0 , %0" : : "r"(runningT));
//    asm volatile ("ld sp, 24(t0)");

}
extern void thread_dispatch();
void idle_function(void *args){
    while(1){

        thread_dispatch();
    }

}
extern int thread_create(thread_t* handle,
                         void(*start_routine)(void*),
                         void* arg);
void idle_init(){
    thread_create(&idle,idle_function,0);
    idle->blokirana=1;

}

void block_thread(){
    runningT->blokirana=1;
}
void unblock_thread(thread_t t){
    t->blokirana=0;
}