#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void main() 
{
    char s1[] = "340";
    printf("%d\n", atoi(s1)); // 340

    char s2[] = "340.0";
    printf("%f\n", atof(s2)); // 340.000000
}