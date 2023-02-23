#include "../lib/hw.h"

uint64 headerSize;
uint64 boundary;

void init() {
    headerSize = sizeof(int);
    uint64 noOfBlocks = ((uint64)(HEAP_END_ADDR) - (uint64)HEAP_START_ADDR) / MEM_BLOCK_SIZE;
    *((int *)HEAP_START_ADDR) = noOfBlocks;
    boundary = (uint64)HEAP_START_ADDR + noOfBlocks * (MEM_BLOCK_SIZE + headerSize);
}

void* mem_alloc_kernel(size_t numOfBlk) {
    uint64 tmp = (uint64)HEAP_START_ADDR;
    int a = *((int *)tmp);
    while (tmp < boundary && (a<0 || a < numOfBlk)) {
        tmp += a > 0 ? a * (MEM_BLOCK_SIZE + headerSize) : -a * (MEM_BLOCK_SIZE + headerSize);
        a = *((int *) tmp);
    }

    if (tmp >= boundary) return 0;

    void *return_adr = (void*)(tmp + headerSize);

    *((int*)tmp) = -(int)numOfBlk;
    *((int*)(tmp + numOfBlk * (MEM_BLOCK_SIZE + headerSize))) = a - numOfBlk;

    return return_adr;
}

int mem_free_kernel(void* ptr) {

    // adresa van opsega
    if((uint64)ptr < (uint64)HEAP_START_ADDR || (uint64)ptr > (uint64)HEAP_END_ADDR) return -1;

    // adresa nije poravnata



    int a = *((int*)((uint64)ptr - headerSize));

    //sa sledbenikom
    if (((uint64)ptr + (-a) * (headerSize + MEM_BLOCK_SIZE)) < boundary) {
        int vr_sledbenika = *((int*)((uint64)ptr - headerSize + ((-a) * (headerSize + MEM_BLOCK_SIZE))));
        if (vr_sledbenika > 0) {
            *((int*)((uint64)ptr - headerSize)) = vr_sledbenika - a;
            a -= vr_sledbenika;
        }
    }

    *((int*)((uint64)ptr - headerSize)) = -a;
    //sa prethodnikom
    if (((uint64)ptr - headerSize) != (uint64)HEAP_START_ADDR) {
        uint64 tmp = (uint64)HEAP_START_ADDR;
        int pom = *(int*)(tmp);
        pom *= pom > 0 ? 1 : -1;
        while ((tmp + pom*(MEM_BLOCK_SIZE+headerSize)) != ((uint64)ptr-headerSize)) {
            tmp = tmp + pom * (MEM_BLOCK_SIZE + headerSize);
            pom = *(int*)(tmp);
            pom *= pom > 0 ? 1 : -1;
        }

        pom = *(int*)(tmp);
        if (pom > 0) *(int*)tmp = pom - a;
    }

    return 0;
}
