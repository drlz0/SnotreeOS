#ifndef KTS_KTS_H
#define KTS_KTS_H

#include <stdio.h>
#include <stdbool.h>
#include <kernel/tty.h>

#include "../pfs/pfs.h"
#include "../ws/welcome_string.h"
#include "../timer/pit.h"

#define BUFFER_SIZE 32

void enqueue_scan_code(uint8_t scancode);
uint8_t dequeue_scan_code();
void process_scan_code (void);

#endif