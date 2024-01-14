#ifndef TIMER_PIT_H
#define TIMER_PIT_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../arch/i386/io.h"

void timer_phase(int hz);
void show_elapsed_time(void);

#endif