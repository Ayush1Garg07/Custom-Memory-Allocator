#include <stdio.h>
#include "../include/allocator.h"

int main()
{
    void* a = my_malloc(100);
    void* b = my_malloc(200);
    print_heap();

    my_free(a);
    print_heap();

    my_free(b);
    print_heap();

    void* c = my_malloc(150);  //We used void in c variable because we are telling compiler that intentionally 
    (void)c;                   //we didn't used this variable
    print_heap();
}

