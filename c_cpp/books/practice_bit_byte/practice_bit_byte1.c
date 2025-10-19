#include <stdio.h>

void main() {
    char c = 1;
    char s[10] = "Hello";

    printf("size of long = %d byte(s)\n", sizeof(long)); // 8 byte(s)
    printf("size of character = %d byte(s)\n", sizeof(c)); // 1 byte(s)
    printf("size of string = %d byte(s)\n", sizeof(s)); // 10 byte(s)
}