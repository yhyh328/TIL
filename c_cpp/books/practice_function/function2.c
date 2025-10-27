#include <stdio.h>

void swapbyval(int, int);
void swapbyref(int *, int *);

void main()
{
    int a = 2, b = 7;
    printf("a=%d, b=%d\n", a, b); // a=2, b=7
    swapbyval(a, b);
    printf("a=%d, b=%d\n", a, b); // a=2, b=7
    swapbyref(&a, &b);
    printf("a=%d, b=%d\n", a, b); // a=7, b=2
}

void swapbyval(int x, int y)
{
    int temp;
    temp = x;
    x = y;
    y = temp;
}

void swapbyref(int *x, int *y)
{
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}