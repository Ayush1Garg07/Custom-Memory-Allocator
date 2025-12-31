#include "allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void* requestSpace(size_t size);
void splitBlock(BlockHeader* block, size_t size);
BlockHeader* findFreeBlock(size_t size);
void coalesce();

alloc_strategy_t current_strategy = FIRST_FIT;

#define NUM_BINS 6

BlockHeader* head = NULL;
BlockHeader* BINS[NUM_BINS]={NULL};

int get_bin_index(size_t size)
{
    if(size<=64) return 0;
    else if(size <= 128) return 1;
    else if(size <= 256) return 2;
    else if(size <= 512) return 3;
    else if(size <= 1024) return 4;
    else return 5;
}


void insert_into_bin(BlockHeader* block)
{ 
    int index = get_bin_index(block->size);
    block->next = BINS[index];
    BINS[index] = block;
}

BlockHeader* get_from_bin(size_t size)
{
    int index = get_bin_index(size);

    for(int i=0; i<NUM_BINS; i++)
    {
        BlockHeader* curr = BINS[i];
        BlockHeader* prev = NULL;

        while(curr)
        {
            if(curr->free && curr->size >= size)
            {
                if(prev) prev->next = curr->next;
                else BINS[i] = curr->next;

                curr->next = NULL;
                return curr;
            }

            prev = curr;
            curr = curr->next; 
        }
    }

    return NULL;
}

void* my_malloc(size_t size)
{
    if(size == 0) return 0;

    size = ALIGN16(size);
    BlockHeader* block;

    if(head == NULL){
        block = requestSpace(size);
        if(!block) return NULL;
        head = block;
    }
    block = get_from_bin(size);

    if(!block){
        block = findFreeBlock(size);

        if(block)
        {
            splitBlock(block, size);
            block->free = 0;
            return (block+1);
        }

        BlockHeader* last = head;
        while(last->next)
        {
            last = last->next;
        }
        
        block = requestSpace(size);
        if(!block) return NULL;

        last->next = block;
        return (block+1);

    }

    return (block+1);
}


void my_free(void* ptr)
{
    if(!ptr) return;

    BlockHeader* header = (BlockHeader*)ptr-1;

    if (header->free) {
    printf("Warning: Double free detected!\n");
    return;
}

    header->free = 1;
    printf("Freed memory at address %p (block size = %zu bytes)\n", ptr, header->size);
    coalesce();
}

void* my_realloc(void* ptr, size_t new_size)
{
    if(!ptr){
        return my_malloc(new_size);
    }

    if(new_size == 0)
    {
        my_free(ptr);
        return NULL;
    }

    BlockHeader* block = (BlockHeader*)ptr-1;

    if(block->size >= new_size)
    {
        splitBlock(block, new_size);
        return ptr;
    }

    if(block->next && block->next->free && block->size + BLOCK_SIZE + block->next->size >= new_size)
    {
        block->size += BLOCK_SIZE + block->next->size;
        block->next = block->next->next;
        splitBlock(block, new_size);
        return ptr;
    }

    void* new_ptr = my_malloc(new_size);
    if(!new_ptr) return NULL;

    memcpy(new_ptr, ptr, block->size);
    my_free(ptr);

    return new_ptr; 
    
}

