#include "gdt.h"


struct gdtdesc kgdt[GDTSIZE];  // Define kgdt
struct gdtr kgdtr;  // Define kgdtr

void init_gdt_desc(uint32_t base, uint32_t limit, uint8_t acces, uint8_t other, struct gdtdesc *desc)
{
    desc->lim0_15 = (limit & 0xffff);
    desc->base0_15 = (base & 0xffff);
    desc->base16_23 = (base & 0xff0000) >> 16;
    desc->acces = acces;
    desc->lim16_19 = (limit & 0xf0000) >> 16;
    desc->other = (other & 0xf);
    desc->base24_31 = (base & 0xff000000) >> 24;
    return;
}

void init_gdt(void)
{
    // Initialize gdt segments 
    init_gdt_desc(0x0, 0x0, 0x0, 0x0, &kgdt[0]);
    init_gdt_desc(0x0, 0xFFFFF, 0x9B, 0x0D, &kgdt[1]);    /* code */
    init_gdt_desc(0x0, 0xFFFFF, 0x93, 0x0D, &kgdt[2]);    /* data */
    init_gdt_desc(0x0, 0x0, 0x97, 0x0D, &kgdt[3]);        /* stack */

    init_gdt_desc(0x0, 0xFFFFF, 0xFF, 0x0D, &kgdt[4]);    /* ucode */
    init_gdt_desc(0x0, 0xFFFFF, 0xF3, 0x0D, &kgdt[5]);    /* udata */
    init_gdt_desc(0x0, 0x0, 0xF7, 0x0D, &kgdt[6]);        /* ustack */

    // Initialize the gdtr structure
    kgdtr.limit = GDTSIZE * 8;
    kgdtr.base = GDTBASE;

    // Copy the gdtr to its memory area
    memcpy((char *)kgdtr.base, (char *)kgdt, kgdtr.limit);

    // Load the gdtr registry 
    asm("lgdtl (kgdtr)");

    // initialize the segments
    asm("   movw $0x10, %ax    \n \
            movw %ax, %ds    \n \
            movw %ax, %es    \n \
            movw %ax, %fs    \n \
            movw %ax, %gs    \n \
            ljmp $0x08, $next    \n \
            next:        \n");
}
