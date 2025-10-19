#include <stdio.h>
#include <string.h>

void print(char s[], int size) {
    printf("%s\n", s);
    printf("strlen(s) : %d\n", strlen(s));
    // rintf("sizeof(s) : %d\n", sizeof(s));
    printf("sizeof(s) : %d\n", size);
}

void main() {
    char s[10] = "Hello";
    print(s, sizeof(s));
    strcpy(s, "Good bye");
    print(s, sizeof(s));
}