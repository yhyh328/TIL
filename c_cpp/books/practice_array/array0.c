#include <stdio.h>

void main()
{
    int i = 0;
    char a[] = "NET";
    char b[4];

    while(a[i] != '\0')
    {
        b[i] = a[2 - i];
        i++;
    }
    b[3] = '\0';
    printf("%sは逆から読むと%s\n", a, b);
    // NETは逆から読むとTEN
}