#include "../lib/hw.h"
#define MAX_BLOCKED_THREADS 50

struct _thread;
typedef struct _thread* thread_t;

extern void * mem_alloc_kernel(size_t size);
extern int mem_free_kernel(void * ptr);

typedef struct semaphore_list {
    thread_t nit;
    struct semaphore_list * next;
} semaphore_list;

struct _sem {
    semaphore_list * semaphore_list_array;
    semaphore_list *first_node, *last_node;
    unsigned init;
};
typedef struct _sem* sem_t;

semaphore_list * get_node_sem(sem_t sem) {
    for (int i = 0; i < MAX_BLOCKED_THREADS; ++i) {
        if(sem->semaphore_list_array[i].nit == 0) return (sem->semaphore_list_array + i);
        //&sceduler_list[i]
    }

    return 0;
}

void free_node_sem(semaphore_list * node){
    node->nit = 0;
    node->next = 0;
}

int sem_open_kernel (
        sem_t* handle,
        unsigned init
) {
    uint64 size = sizeof(struct _sem)/MEM_BLOCK_SIZE;
    size += (sizeof(struct _sem) % MEM_BLOCK_SIZE) == 0 ? 0 : 1;
    *handle = mem_alloc_kernel(size);

    if(*handle==0) {
        return -1;
    }
    (*handle)->init=init;
    size=MAX_BLOCKED_THREADS * sizeof(semaphore_list)/MEM_BLOCK_SIZE;
    size += ((sizeof(semaphore_list)*MAX_BLOCKED_THREADS) % MEM_BLOCK_SIZE) == 0 ? 0 : 1;
    (*handle)->semaphore_list_array=(semaphore_list *)mem_alloc_kernel(size);

    (*handle)->last_node=0;
    (*handle)->first_node=0;
    for (int i = 0; i < MAX_BLOCKED_THREADS; ++i) {
        (*handle)->semaphore_list_array[i].next=0;
        (*handle)->semaphore_list_array[i].nit=0;
    }
    return 0;
}

extern void put(thread_t);
extern void block_thread();
extern void unblock_thread(thread_t t);

int sem_close_kernel(sem_t id){

    while(id->first_node!=0){

        semaphore_list *old=id->first_node;
        id->first_node=id->first_node->next;
        unblock_thread(old->nit);
        if(id->first_node==0)id->last_node=0;

        put(old->nit);
        //mem_free_kernel(old);
        free_node_sem(old);

    }

    mem_free_kernel(id->semaphore_list_array);
    mem_free_kernel(id);
    return 0;
}

extern thread_t runningT;
int sem_wait_kernel(sem_t id){
    if(id->init==0){
        semaphore_list * new_node= get_node_sem(id);
        if(new_node==0)return -1;
        new_node->nit=runningT;
        block_thread();
        if(id->first_node==0)id->first_node=new_node;

        if(id->last_node!=0)id->last_node->next=new_node;

        id->last_node=new_node;

    }
    else id->init--;


    return 0;
}

int sem_signal_kernel(sem_t id){

    if(id->first_node!=0){

        semaphore_list *old=id->first_node;
        id->first_node=id->first_node->next;
        unblock_thread(old->nit);
        if(id->first_node==0)id->last_node=0;

        put(old->nit);
        //mem_free_kernel(old);
        free_node_sem(old);

    }
    else id->init++;

    return 0;
}

extern sem_t sem_for_get_c;
void get_c_from_buffer(){
    sem_signal_kernel(sem_for_get_c);
}

void init_sem_for_console(){
    sem_open_kernel(&sem_for_get_c,0);
}