#include "../h/syscall_c.h"

#define MAX_NUMBER_OF_THREADS 500

extern void* mem_alloc(unsigned long);

typedef struct scheduler_list {
    thread_t nit;
    struct scheduler_list * next;
} scheduler_list;

scheduler_list * scheduler_list_array;
scheduler_list *first_node, *last_node;

void init_scheduler() {
    scheduler_list_array = (scheduler_list*)mem_alloc(MAX_NUMBER_OF_THREADS * sizeof(scheduler_list));
    first_node = 0;
    last_node = 0;
    for (int i = 0; i < MAX_NUMBER_OF_THREADS; ++i) {
        scheduler_list_array[i].next = 0;
        scheduler_list_array[i].nit = 0;
    }
}

scheduler_list * get_node() {
    for (int i = 0; i < MAX_NUMBER_OF_THREADS; ++i) {
        if(scheduler_list_array[i].nit == 0) return (scheduler_list_array + i);
        //&sceduler_list[i]
    }

    return 0;
}

void free_node(scheduler_list* node){
    node->nit = 0;
    node->next = 0;
}

void put(thread_t nit) {

    scheduler_list *new_node = get_node();
    //dodaj provjeru za memoriju
    new_node->nit = nit;

    if(first_node == 0) first_node = new_node;

    if(last_node != 0) last_node->next = new_node;

    last_node = new_node;

}

thread_t get() {

    if(first_node == 0) return 0;
    //idle thread
    thread_t nit = first_node->nit;
    scheduler_list *old = first_node;

    first_node = first_node->next;
    if(first_node == 0) last_node = 0;

    free_node(old);

    return nit;

}


