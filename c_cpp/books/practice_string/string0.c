#include <stdio.h>
#include <string.h>

void main() {
    char str[10] = "Hello";  // H, e, l, l, o, \0, \0, \0, \0, \0, 
    // char str[] = "Hello";
    for (int i = 0; i < sizeof(str); i++) {
        if (str[i]) { printf("%c, ", str[i]); }
        else { printf("\\0, "); }
    }
    printf("\n");
    printf("strlen() : %d\n", strlen(str)); 
    printf("sizeof() : %d\n", sizeof(str));
}