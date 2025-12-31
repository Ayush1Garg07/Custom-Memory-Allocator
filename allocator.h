#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

/* ===== Block metadata ===== */
typedef struct BlockHeader {
    size_t size;
    int free;
    struct BlockHeader* next;
} BlockHeader;

typedef enum{
    FIRST_FIT,
    BEST_FIT,
    WORST_FIT,
    NEXT_FIT,

}alloc_strategy_t;

/* ===== Alignment helpers ===== */
#define ALIGN16(x) (((x) + 15) & ~15)
#define BLOCK_SIZE sizeof(BlockHeader)


/* ===== Public APIs ===== */
void* my_malloc(size_t size);
void  my_free(void* ptr);
void* my_calloc(size_t n, size_t size);
void* my_realloc(void* ptr, size_t size);

#endif
