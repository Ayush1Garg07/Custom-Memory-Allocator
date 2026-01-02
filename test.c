#include <stdio.h>
#include "allocator.h"

int main()
{
    void* a = my_malloc(100);
    void* b = my_malloc(200);
    my_free(a);
    my_free(b);
    void* c = my_malloc(150);
    my_free(c);
    return 0;
}

