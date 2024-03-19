/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7  */
uint8_t slave_mask;  /* IRQs 8-15 */

/* Initialize the 8259 PIC */
void i8259_init(void) {
    master_mask = 0xFF; // save masks 
    slave_mask = 0xFF; 
    // did not include io_wait()

    outb(ICW1, MASTER_8259_PORT); // send ICW1 to master and slave ports
    outb(ICW1, SLAVE_8259_PORT); // starts initilization sequence (in cascade mode)

    outb(ICW2_MASTER, MASTER_8259_DATA);// ICW2_MASTER and ICW2_SLAVE shows The offset to the master and slave ports, respectively 
    outb(ICW2_SLAVE, SLAVE_8259_DATA);

    outb(ICW3_MASTER, MASTER_8259_DATA); // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)                                                          
    outb(ICW3_SLAVE, SLAVE_8259_DATA); // ICW3: tell Slave PIC its cascade identity (0000 0010) 

    outb(ICW4, MASTER_8259_DATA); // ICW4 tells the pic to use 8086 mode on both master and slave
    outb(ICW4, SLAVE_8259_DATA);

    outb(master_mask, MASTER_8259_DATA); // restore masks
    outb(slave_mask, SLAVE_8259_DATA);
    enable_irq(0x02); // need to enable irq_2 for the RTC int
}

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
    uint16_t port;
    uint8_t value;
 
    if(irq_num < 8) { // if irq is less than 8 we know to enable master 
        port = MASTER_8259_DATA;
    } else {
        port = SLAVE_8259_DATA;
        irq_num -= 8;
    }
    value = inb(port) & ~(1 << irq_num); // we went to clear the mask 
    outb(value, port);   
}

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
    uint16_t port;
    uint8_t value;
 
    if(irq_num < 8) { // if irq is less than 8 we know to enable master 
        port = MASTER_8259_DATA;
    } else {
        port = SLAVE_8259_DATA;
        irq_num -= 8;
    }
    value = inb(port) | (1 << irq_num); // we went to set the mask 
    outb(value, port);     
}

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {
    if(irq_num >= 8){
	outb(EOI | (irq_num - 8), SLAVE_8259_PORT); // if irq_num > 8 write to slave too.
    outb(EOI | ICW3_SLAVE, MASTER_8259_PORT); // tells the master of the cascade identity. 
    return; 
    }
	outb(EOI | irq_num , MASTER_8259_PORT);
}
