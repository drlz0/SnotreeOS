#ifndef _KERNEL_READ_SCAN_CODE_H
#define _KERNEL_READ_SCAN_CODE_H

#include <stdio.h>

#define KBD_DATA_PORT   0x60

unsigned char read_scan_code(void);


#endif