#include <stdio.h>

void main()
{
    FILE *fp;
    char s[20];
    int i = 1;
    fp = fopen("abc.txt", "r");
    if(fp == NULL) return;
    while(1)
    {
        fgets(s, 20, fp);
        if(feof(fp)) break; // end of file
        printf("%04d:%s", i, s);
        i++;
    }
    fclose(fp);
}


// 0001:abcdefg
// 0002:hijklmn
// 0003:opqrstu
// 0004:vwxyz