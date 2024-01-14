#include "../../pic/pic.h"
#include "../../timer/time.h"

volatile int timer_ticks = 0;

void isr_timer_int(void)
{
    timer_ticks++;

    if (timer_ticks % 18 == 0)
    {
        system_time++;
    }

    PIC_sendEOI(0);
}