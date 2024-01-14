#include "kernel/read_scan_code.h"
#include "io.h"

#define KBD_DATA_PORT   0x60

unsigned char read_scan_code(void)
{
    return inb(KBD_DATA_PORT);
}