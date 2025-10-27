#include <stdio.h>

void main()
{
    int a[4] = {10, 20, 30, 40};
    printf("配列a[3]の値は%d\n", *(a+3)); // 40
    printf("配列a[0]の値に3を足すと%d\n", *a+3); // 13
}