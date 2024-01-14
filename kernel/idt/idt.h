#ifndef IDT_IDT_H
#define IDT_IDT_H

#define IDT_MAX_DESCRIPTORS 256

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

// IDT entry structure
typedef struct {
    uint16_t    isr_low;      // The lower 16 bits of the ISR's address
    uint16_t    kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
    uint8_t     reserved;     // Set to zero
    uint8_t     attributes;   // Type and attributes; see the IDT page
    uint16_t    isr_high;     // The higher 16 bits of the ISR's address
} __attribute__((packed)) idt_entry_t;

// IDT Register structure
typedef struct {
    uint16_t    limit;
    uint32_t    base;
} __attribute__((packed)) idtr_t;

// IDT initialization function
void init_idt(void);

// IDT set descriptor function
void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);

// External array of ISR stubs
extern void* isr_stub_table[];

#endif
