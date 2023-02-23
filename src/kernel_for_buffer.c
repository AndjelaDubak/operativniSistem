#include "../lib/hw.h"
#define BUFFER_SIZE 10000

typedef struct buffer {
    char buff[BUFFER_SIZE];
    int insertPointer;
    int removePointer;

} buffer;

buffer inputBuffer;
buffer outputBuffer;



int insert_item(buffer* buffer, char item) {
    if(((buffer->removePointer-1+BUFFER_SIZE)%BUFFER_SIZE)==buffer->insertPointer)return 0;
    buffer->buff[buffer->insertPointer] = item;
    buffer->insertPointer = (buffer->insertPointer + 1) % BUFFER_SIZE;
    return 0;
}

char remove_item(buffer* buffer) {
    if(buffer->insertPointer==buffer->removePointer)return 0;
    char item = buffer->buff[buffer->removePointer];
    buffer->removePointer = (buffer->removePointer + 1) % BUFFER_SIZE;
    return item;
}

char getc_kernel () {
    return remove_item(&inputBuffer);
}
void transmit();
void putc_kernel(char item) {

    insert_item(&outputBuffer, item);
    transmit();
}

void initBuffer() {
    inputBuffer.removePointer=0;
    inputBuffer.insertPointer=0;

    outputBuffer.removePointer=0;
    outputBuffer.insertPointer=0;
}

void transmit() {
    uint8 status=(*(uint8*)CONSOLE_STATUS)&CONSOLE_TX_STATUS_BIT;

    while(status!=0)
    {
        char c= remove_item(&outputBuffer);

        if(c==0)break;

        *(uint8*)CONSOLE_TX_DATA=c;
        status=(*(uint8*)CONSOLE_STATUS)&CONSOLE_TX_STATUS_BIT;
    }

}

extern void get_c_from_buffer();

void console_transfer() {

    uint8 status=(*(uint8*)CONSOLE_STATUS)&CONSOLE_TX_STATUS_BIT;

    while(status!=0)
    {
        char c= remove_item(&outputBuffer);

        if(c==0)break;

        *(uint8*)CONSOLE_TX_DATA=c;
        status=(*(uint8*)CONSOLE_STATUS)&CONSOLE_TX_STATUS_BIT;
    }

    status=(*(uint8*)CONSOLE_STATUS)&CONSOLE_RX_STATUS_BIT;

    while(status!=0) {
        char c= *(uint8*)CONSOLE_RX_DATA;
        insert_item(&inputBuffer,c);
        get_c_from_buffer();
        //signalza semafor
        status=(*(uint8*)CONSOLE_STATUS)&CONSOLE_RX_STATUS_BIT;
    }
}


void console_handler(){

    if(plic_claim()==CONSOLE_IRQ){
        console_transfer();
        plic_complete(10);
    }

}