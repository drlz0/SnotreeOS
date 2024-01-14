#include <kernel/read_scan_code.h>
#include "../../pic/pic.h"
#include "../../kts/kts.h"


void isr_keyboard_int (void)
{   
    uint8_t scancode = read_scan_code();
    enqueue_scan_code(scancode);
    PIC_sendEOI(1);
}
