#include <stdio.h>
#include "allocator.h"

int main() {
    int *x = my_malloc(sizeof(int));
    *x = 50;

    printf("Value: %d\n", *x);

    my_free(x);
    return 0;
}
