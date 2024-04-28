#include "pit.h"
#include "types.h"
#include "idt.h"
#include "lib.h"
#include "i8259.h"
#include "syscall.h"
#include "keyboard.h"
#include "terminal.h"

int pit_ticks;
int num_shells = 0;

/* void pit_init(void)
 * Inputs: void
 * Return Value: void
 * Function: initializes PIT by filling in needed divisor vars
             and enables interupts for pit*/

void pit_init()
{
    int divisor = RATE / HZ;       /* Calculate our divisor */
    outb(0x36, 0x43);             /* Set our command byte 0x36 */
    outb(divisor & 0xFF,0x40);   /* Set low byte of divisor */
    outb(divisor >> 8, 0x40); /* Set high byte of divisor */
    pit_ticks = 18; // 18 ticks for a second
    enable_irq(0);    // 0 is the irq number for pit
    num_shells = 0; //1 shell is created in kernel.c
}

/* void pit_handler(void)
 * Inputs: void
 * Return Value: void
 * Function: contains logic needed for PIT ticks, and schedules terminals by calling switchterminal pased on ticks 
 */

void pit_handler()
{
    // if(num_shells == 0){
    //     num_shells++;

    //     clear();
    //     execute((uint8_t*)("shell"));
    //     send_eoi(0);
    // }
    
    pit_ticks--;
    if(pit_ticks == 0)
    {
        pit_ticks = DEFAULT_TICKS;
        //Switch Scheduled Terminal to the next Terminal that's been started
        //int OldTerminal = ScheduledTerminal;  
        ScheduledTerminal = (ScheduledTerminal + 1) % 3;
        while(!Terminals[ScheduledTerminal].Started){
            ScheduledTerminal = (ScheduledTerminal + 1) % 3;
        }

        if(ActiveTerminal != ScheduledTerminal){
            SwitchTerminal(ScheduledTerminal);
            send_eoi(0); 
            return;
        }
        //else{ 
        send_eoi(0); 
        //}
    }
    else{ send_eoi(0); }
}
