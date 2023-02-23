
#ifndef _syscall_cpp
#define _syscall_cpp
extern "C"{
#include "syscall_c.h"
}
#define MAX_NUM_OF_THREADS 128

void* operator new(size_t size);

void operator delete(void * ptr);
class Thread {
public:
   // lista niti koje cekaju na mene
   //semafor
    static Thread* allThreads[128];//pocetno su nula
 //   static int ID_Generator;//0
 //   int id;
    static Thread* niz_nit;
    static uint8 zauzeto[MAX_NUM_OF_THREADS];
    static void init_Thread(){
        niz_nit=(Thread*)mem_alloc(MAX_NUM_OF_THREADS*(sizeof(Thread)+8));
        for (int i = 0; i <MAX_NUM_OF_THREADS ; ++i) {
            zauzeto[i]=0;
        }
    }
    Thread (void (*body)(void*), void* arg){
        this->arg=arg;
        this->body=body;
        //prodjes kroz allThreads allThreads[i]=this
     //   id=ID_Generator++;
    }
    virtual ~Thread (){
      //  thread_exit();
    }
    int start (){
        return thread_create(&myHandle,body,arg);

        //myHandle->id=id;
    }
    static void dispatch (){
        thread_dispatch();
    }
 /*   void* operator new(size_t size){
        for (int i = 0; i < MAX_NUM_OF_THREADS; ++i) {
            if(zauzeto[i]==0){
                zauzeto[i]=1;
                return (void*)((uint64)niz_nit+i*size);
            }
        }
        return niz_nit;
    }
    void operator delete(void* ptr){
        uint64 index=((uint64)ptr - (uint64)niz_nit)/sizeof(Thread);
        zauzeto[index]=0;
    }*/
    //static int sleep (time_t);
protected:
    Thread (){
        body=wrapper_cpp;
        arg= this;
        //   id=ID_Generator++;
    }
    virtual void run () {}
private:
    thread_t myHandle;
    void (*body)(void*);
    void* arg;
    static void wrapper_cpp(void* arg){
        Thread * t=(Thread*)arg;
        t->run();
    }
};
class Semaphore {
public:
    static Semaphore * semafori;
    static uint8 zauzeto[MAX_NUM_OF_THREADS];
    static void init_Semaphore(){
        semafori= (Semaphore*)mem_alloc(MAX_NUM_OF_THREADS*sizeof(Semaphore));
        for (int i = 0; i <MAX_NUM_OF_THREADS ; ++i) {
            zauzeto[i]=0;
        }
    }
    Semaphore (unsigned init = 1){
        sem_open(&(this->myHandle),init);
    }
    ~Semaphore (){
        sem_close(myHandle);
    }
    int wait (){
        return sem_wait(myHandle);
    }
    int signal (){
        return sem_signal(myHandle);
    }
    void* operator new(size_t size){
        for (int i = 0; i < MAX_NUM_OF_THREADS; ++i) {
            if(zauzeto[i]==0){
                zauzeto[i]=1;
                return semafori+i;
            }
        }
        return semafori;
    }
    void operator delete(void* ptr){
        uint64 index=((uint64)ptr - (uint64)semafori)/sizeof(Semaphore);
        zauzeto[index]=0;
    }
private:
    sem_t myHandle;
};
/*
class PeriodicThread : public Thread {
public:
    static void wrapperPeriodic(void* arg){
        PeriodicThread* p=(PeriodicThread*) arg;
        p->aktivna=1;
        while(p->aktivna==1){

            p->periodicActivation();

        }
    }
protected:
    PeriodicThread (time_t period):Thread(wrapperPeriodic,this){
        this->period=period;
    }
    virtual void periodicActivation () {}
private:
    time_t period;
    uint8 aktivna;
};*/
class Console {
public:
    static char getc (){
        return ::getc();
    }
    static void putc (char a){
        ::putc(a);
    }
};
#endif