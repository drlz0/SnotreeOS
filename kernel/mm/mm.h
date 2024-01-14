#ifndef _MM_MM_H
#define _MM_MM_H

#define HEAP_START 0x100000

#include <stddef.h>
#include <stdint.h>

/* Define a structure for a block in the linked list */
struct Block {
    size_t size;
    struct Block* next;
};

void init_heap(size_t heap_size);
void deallocate(void* ptr);
void* allocate(size_t size);

#endif