#include <stdio.h>

void main()
{
    char x = 4, y;
    char *p = &x;
    y = *p;
    printf("変数ｘの値は%dです\n", y); // 変数ｘの値は4です
}