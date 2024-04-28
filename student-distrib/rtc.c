#include "rtc.h"
#include "i8259.h"
#include "lib.h"
#include "keyboard.h"
#include "terminal.h"
#include "x86_desc.h"
int rtc_int[3] = {0,0,0};
/* void: rtc_init(void)
 * Inputs: void
 * Return Value: void
 * Function: Initialize rtc, used osDev for documentation */
void rtc_init()
{
    // int i;
    outb(0x8A, 0x70);
    // outb(0x20,0x71);
    // sti();

    // cli();
    outb(0x8B, 0x70); // 0x70 is the rtc port index and 0x8B is rtc status register B. 
    char prev = inb(0x71); // read from the CMOS port (0x71)

    outb(0x8B,0x70); // set the index again (a read will reset the index to register D)
    outb(prev | 0x40, 0x71);	// write the previous value ORed with 0x40. This turns on bit 6 of register B
    // sti();
    enable_irq(0x08); // this command enables the irq_8 that was just initialized
    // rtc_rate = 1; // default frequency 1024/1024;
    // rtc_ticks = rtc_rate;
}
/* void: rtc_handler(void)
 * Inputs: void
 * Return Value: void
 * Function: Calls handler for rtc to generate interrupt */
void rtc_handler()
{
    int i;
    // cli();
    outb(0x8C, 0x70);	// select register C
    inb(0x71);		
    for(i = 0; i < 3; i++)
    {
        Terminals[i].rtc_ticks--;   
        if(Terminals[i].rtc_ticks == 0)
        {
        rtc_int[i] = 1;
        Terminals[i].rtc_ticks = Terminals[i].rtc_rate;
        if(Terminals[i].rtc_mode == 1)
        {
            gotNewLine[i] = 1;
        }
        }
    }

    //    Terminals[ActiveTerminal].rtc_ticks--;
    // if(Terminals[ActiveTerminal].rtc_ticks == 0)
    // {
    //     rtc_int = 1;
    //     Terminals[ActiveTerminal].rtc_ticks = Terminals[ActiveTerminal].rtc_rate;
    //     if(Terminals[ActiveTerminal].rtc_mode == 1)
    //     {
    //         gotNewLine[ActiveTerminal] = 1;
    //     }
    // }

        
    // if(Terminals[ActiveTerminal].rtc_ticks == 0)
    // {
    //     rtc_int = 1;
    //     Terminals[ActiveTerminal].rtc_ticks = Terminals[ActiveTerminal].rtc_rate;
    //     if(Terminals[ActiveTerminal].rtc_mode == 1)
    //     {
    //         gotNewLine[ActiveTerminal] = 1;
    //     }
    // }// just throw away contents
         
   
    // sti();
    // test_interrupts();

    send_eoi(0x08); // end of instruction for the irq_8. 
}
/* void rtc_close (int32_t fd) 
 * Inputs: filename - unused
 * Return Value: 0
 * Function: Initializes the default freq to 2HZ. */
int32_t rtc_open(const uint8_t* filename) {
    // rtc_change_rate(2); // 2 represents the minimum frequency
    Terminals[ScheduledTerminal].rtc_rate = 512; // 512 represents a frequency of 2 hertz. 
    Terminals[ScheduledTerminal].rtc_ticks = Terminals[ScheduledTerminal].rtc_rate; 
    return 0;
}
/* void rtc_close (int32_t fd) 
 * Inputs: fd - unused
 * Return Value: 0
 * Function: Does nothing. */
int32_t rtc_close(int32_t fd) {
    return 0;
}
/* void rtc_read (int32_t fd, const int32_t* buf, int32_t nbytes) 
 * Inputs: fd, nbytes,buf - unused arguments
 * Return Value: 0
 * Function: waits for rtc int to be held high so that it can update the screen.  */
int32_t rtc_read(int32_t fd, void* buf, int32_t nbytes) {
    rtc_int[ScheduledTerminal] = 0;
    gotNewLine[ScheduledTerminal] = 0;
    while(rtc_int[ScheduledTerminal] == 0) {
        //wait for rtc_int to be 1
    }
    
    return 0;
}
/* void rtc_write (int32_t fd, const int32_t* buf, int32_t nbytes) 
 * Inputs: fd, nbytes - unused arguments
            buf - species the frequency that you want to write to
 * Return Value: void
 * Function: sets a different amount of ticks for the rtc. */
int32_t rtc_write (int32_t fd, const void* buf, int32_t nbytes) {

    if(((uint32_t*)buf)[0] > 1024)
    {
        return -1;
    }
    Terminals[ScheduledTerminal].rtc_rate = 1024 / ((uint32_t*)buf)[0]; // 1024 / frequency;
    Terminals[ScheduledTerminal].rtc_ticks = Terminals[ScheduledTerminal].rtc_rate;
    
    return 0;
}
/* void rtc_change rate(int32_t frequency)
 * Inputs: frequency - frequency you want to change to. 
 * Return Value: void
 * Function: change frequency of the rtc*/
int32_t rtc_change_rate(int32_t frequency)
{
      
//     int log2 = 0;
//     int temp = frequency;
//     while(temp > 1)
//     {
//         temp /= 2;
//         log2++;
//     } // used to calculate the log2(frequency).
    
//     int rate =  (MAX_RATE - log2) + 1; // rearrangement to find rate of equation frequency = 32768 >> (rate - 1)
//     rate &= MAX_RATE; // rate cannot exceed 15
//    //controls the rate at which an interrupt arises.
//     //disable interrupts to perform operation
//     outb(0x8A,0x70);		// set index to register A, disable NMI. 0x8A represents the RTC status register A. 
//     char prev = inb(0x71);	// get initial value of register A
//     outb(0x8A,0x70);		// reset index to A
//     outb((prev & 0xF0) | rate, 0x71); //write only our rate to A. Note, rate is the bottom 4 bits. 0xF0 masks the top 4 bits.
//     //enable interrupts once again
    return 0;  // indicates the program succesfully ran
}
