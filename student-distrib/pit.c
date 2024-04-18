#include "pit.h"
#include "types.h"
#include "idt.h"
#include "lib.h"
#include "i8259.h"
#include "syscall.h"

int pit_ticks;
int num_shells = 0;

void pit_init()
{
    int divisor = RATE / HZ;       /* Calculate our divisor */
    outb(0x36, 0x43);             /* Set our command byte 0x36 */
    outb(divisor & 0xFF,0x40);   /* Set low byte of divisor */
    outb(divisor >> 8, 0x40); /* Set high byte of divisor */
    pit_ticks = 18; // 18 ticks for a second
    enable_irq(0);    // 0 is the irq number for pit
}

void pit_handler()
{
    // if(num_shells == 0){
    //     num_shells++;

    //     clear();
    //     execute((uint8_t*)("shell"));
    //     send_eoi(0);
    // }
    // if(num_shells < 3){
    //     num_shells++;
    //     //Check what terminal and execute in the right place
    //     execute((uint8_t*)"shell");
        
    //     send_eoi(0);
    //     return;
    // }
    pit_ticks--;
    if(pit_ticks == 0)
    {
        pit_ticks = DEFAULT_TICKS;
    }
    send_eoi(0);
}
