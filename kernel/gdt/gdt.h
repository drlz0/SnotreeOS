#ifndef GDT_GDT_H
#define GDT_GDT_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define GDTBASE    0x00000800
#define GDTSIZE    7  // Number of entries in the GDT

/* GDT Register */
struct gdtr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

/* GDT entry */
struct gdtdesc {
    uint16_t lim0_15;
    uint16_t base0_15;
    uint8_t base16_23;
    uint8_t acces;
    uint8_t lim16_19:4;
    uint8_t other:4;
    uint8_t base24_31;
} __attribute__((packed));

extern struct gdtdesc kgdt[GDTSIZE];  // Declare kgdt as an array of gdtdesc
extern struct gdtr kgdtr;  // Declare kgdtr

void init_gdt_desc(uint32_t base, uint32_t limit, uint8_t acces, uint8_t other, struct gdtdesc *desc);
void init_gdt(void);
#endif