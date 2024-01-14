#include "mm.h"


/*  Example usage:
    size_t heap_size = 4096;  // Adjust the heap size as needed
    init_heap(heap_size);

    void* mem1 = allocate(16);
    void* mem2 = allocate(32);

    // Use allocated memory as needed...

    deallocate(mem1);
    deallocate(mem2);
*/

/* Initialize the heap as a linked list with one large block */
struct Block* heap = (struct Block*)HEAP_START;

void init_heap(size_t heap_size)
{
    heap->size = heap_size - sizeof(struct Block);
    heap->next = NULL;
}

void* allocate(size_t size) 
{
    struct Block* current_block = heap;
    struct Block* previous_block = NULL;

    // Find the first free block that fits the requested size
    while (current_block && (current_block->size < size)) {
        previous_block = current_block;
        current_block = current_block->next;
    }

    if (!current_block) {
        // No free block found
        return NULL;
    }

    // Check if there's enough space to split the block
    if (current_block->size >= (size + sizeof(struct Block))) {
        struct Block* new_block = (struct Block*)((uint8_t*)current_block + size + sizeof(struct Block));
        new_block->size = current_block->size - size - sizeof(struct Block);
        new_block->next = current_block->next;
        current_block->size = size;
        current_block->next = new_block;
    }

    // Mark the block as allocated
    if (previous_block) {
        previous_block->next = current_block->next;
    } else {
        heap = current_block->next;
    }

    return (void*)((uint8_t*)current_block + sizeof(struct Block));
}

void deallocate(void* ptr) 
{
    struct Block* block = (struct Block*)((uint8_t*)ptr - sizeof(struct Block));
    block->next = heap;
    heap = block;
}