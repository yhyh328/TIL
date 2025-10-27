#include <stdio.h>
#include "my_header.h"

struct data *sp;

void main()
{
    size_t len = sizeof(arr) / sizeof(arr[0]);
    for (sp = arr; sp != arr + len; sp++) {
        printf("No: %d, Name: %s, Age: %d\n", 
            sp->no, sp->name, sp->age);
    }
}