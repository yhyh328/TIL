#include <stdio.h>

void main() {
    printf("%d\n", 3/2); // 1
    printf("%d\n", 3.0/2.0); // 185594528 (garbage value)
    printf("%d\n", 3.0/2); // 185594528 (garbage value)
    printf("%d\n", 3/2.0); // 185594528 (garbage value)
    printf("\n");
    printf("%f\n", 3.0/2.0); // 1.500000
    printf("%f\n", 3.0/2); // 1.500000
    printf("%f\n", 3/2.0); // 1.500000
    printf("\n");
    printf("%f\n", 3/2); // 0.000000
    printf("%f\n", 3/(float)2); // 1.500000
    printf("%f\n", (float)3/2); // 1.500000
}