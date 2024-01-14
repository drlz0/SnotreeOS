#include "pit.h"
#include "time.h"
#include <kernel/tty.h>

/* 1193180 Hz - Frequency of PIT's oscillator, 1193180/65480.18878279 = 18.222 - IRQ 0 frequency */
/* Use to set the rate at which IRQ 0 calls, i will stick to the default 18.222 times per second */
void timer_phase(int hz)
{
    uint16_t divisor = 1193180 / hz;    /* Calculate our divisor */
    outb(0x43, 0x36);                   /* Set our command byte 0x36 */
    outb(0x40, divisor & 0xFF);         /* Set low byte of divisor */
    outb(0x40, divisor >> 8);           /* Set high byte of divisor */
}

void show_elapsed_time(void)
{
    char* time_string = uint64_to_string(system_time);
    terminal_writestring("Time elapsed from the start: ");
    terminal_writestring(time_string);
    terminal_writestring(" seconds\n");
}