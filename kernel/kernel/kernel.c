#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/read_scan_code.h>
#include <../serial/serial.h>
#include "../pic/pic.h"
#include "../gdt/gdt.h"
#include "../idt/idt.h"
#include "../ws/welcome_string.h"
#include "../mm/mm.h"
#include "../timer/pit.h"
#include "../kts/kts.h"

#define PROCESS_SCAN_INTERVAL 2 // Check the queue 9 times per second
extern volatile int timer_ticks;

void kernel_main(void) {
	terminal_initialize();
	init_serial();
	init_gdt();
	init_idt();
	PIC_remap(32,40);
	IRQ_clear_mask(0);
	IRQ_clear_mask(1);
	print_welcome_string();
	while (1)
	{
		if (timer_ticks % PROCESS_SCAN_INTERVAL == 0) {
			process_scan_code();
		}
	}
}