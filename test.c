#include <stdio.h>
#include "allocator.h"

int main()
{
    void* a = my_malloc(100);
    void* b = my_malloc(200);
    print_heap();

    my_free(a);
    print_heap();

    my_free(b);
    print_heap();

    void* c = my_malloc(150);
    print_heap();
}

