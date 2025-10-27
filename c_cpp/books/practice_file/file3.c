#include <stdio.h>

void main()
{
    char s[30];
    fgets(s, 29, stdin); // Store input from keyboard into s
    fputs(s, stdout); // Display s on screen
    fputs("error!\n", stderr); // Display error! on screen
}