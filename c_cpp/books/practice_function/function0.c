#include <stdio.h>

int y;
int z;

void myfunc(int a)
{
    int z;
    int x;
    x = a;
    y = a;
    z = a;
}

void main()
{
    int x;
    x = 10;
    y = 10;
    z = 10;
    printf("x, y, zの値は %d, %d, %d\n", x, y, z); // 10, 10, 10
    myfunc(5);
    printf("x, y, zの値は %d, %d, %d\n", x, y, z); // 10, 5, 10
}