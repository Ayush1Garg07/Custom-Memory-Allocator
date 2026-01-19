#define _DEFAULT_SOURCE

#include "../include/allocator.h"
#include <stdlib.h>

#ifdef __linux__
#include <unistd.h>   // for sbrk
#include <sys/types.h>
#endif

extern BlockHeader* head;
extern alloc_strategy_t current_strategy;

BlockHeader* last_alloc = NULL;

BlockHeader* findFreeBlock(size_t size)
{
    BlockHeader* curr = head;
    BlockHeader* best = NULL;

    switch (current_strategy)
    {
    case FIRST_FIT:

        while(curr)
        {
            if(curr->free && curr->size >= size){
                return curr;
            }
            curr = curr->next;
        }
        break;

    case BEST_FIT:
        curr = head;
        while(curr)
        {
            if(curr->free && curr->size >= size)
            {
                if(!best || curr->size < best->size){
                    best = curr;
                }

                curr=curr->next;
            }
        }
        return best;
        
    
    case WORST_FIT:
        curr = head;
        while(curr)
        {
            if(curr->free && curr->size >= size)
            {
                if(!best || curr->size > best->size){
                    best = curr;
                }

                curr=curr->next;
            }
        }
        return best; 
        
        
    case NEXT_FIT:
        if(!last_alloc) last_alloc = head;
        curr = last_alloc;
        
        BlockHeader* start = curr;
        do{
            if(curr->free && curr->size>=size)
            {
                last_alloc = curr;
                return curr;
            }
            curr = curr->next ? curr->next : head;
        }while(curr != start);
        break;
    
    default:
        return NULL;
    }

    return NULL;     //no block found
}


void* requestSpace(size_t size)
{
    size_t totalSize = ALIGN16(size + BLOCK_SIZE);

#ifdef __linux__
    // -------- Linux version (real heap growth) --------
    void* request = sbrk(totalSize);
    if (request == (void*) -1)
        return NULL;
#else
    void* request = malloc(totalSize);
    if (request == NULL)
        return NULL;
#endif

    BlockHeader* newblock = (BlockHeader*) request;
    newblock->size = totalSize - BLOCK_SIZE;
    newblock->free = 0;
    newblock->next = NULL;

    return newblock;
}



void coalesce()
{
    BlockHeader* curr = head;

    while(curr && curr->next)
    {
        if(curr->free && curr->next->free)
        {
            curr->size += curr->next->size + BLOCK_SIZE;
            curr->next = curr->next->next;
        }
        else{
            curr = curr->next;
        }
    }
}

void splitBlock(BlockHeader* block, size_t size)
{
    if(block->size <= BLOCK_SIZE+size) return;     //Not Enough Space to split 

    BlockHeader* newBlock = (BlockHeader*)((char*) (block+1) + size);
    newBlock->size= block->size - size - BLOCK_SIZE;
    newBlock->free = 1;
    newBlock->next = block->next;

    block->size = size;
    block->next = newBlock;
}
