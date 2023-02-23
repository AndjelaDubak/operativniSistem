

void timer(){
    unsigned long sip;
    asm volatile("csrr %0, sip" : "=r" (sip));
    sip &= ~2;
    asm volatile("csrw sip, %0" : : "r" (sip));
}