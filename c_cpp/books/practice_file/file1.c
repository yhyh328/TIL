#include <stdio.h>

void main()
{
    FILE *fp;
    int a = 100, b = 5, c = 40;
    int x = 1, y = 10, z = 100;
    char delm[] = "----====----\n";

    fp = fopen("created_by_file1.txt", "w");
    if(fp == NULL) return;
    fputs(delm, fp);
    fprintf(fp, "%4d%4d%4d\n%4d%4d%4d\n", a, b, c, x, y, z);
    fputs(delm, fp);
    fclose(fp);
}