#include <stdio.h>
#include <string.h>

void main() 
{
    char a[] = "ABC";
    int l = strlen(a);
    printf("strlen(a) : %d\n", l);

    char b[6];
    strcpy(b, "Hello");
    printf("strcpy(b) : { ");
    for (int i = 0; i < strlen(b) + 1; i++) 
    { 
        if(b[i] != '\0') { printf("%c, ", b[i]); }
        else { printf("\\0"); }
    }
    printf(" }\n");

    char c[6] = "ABC";
    char d[] = "de";
    printf("strcat(c, d) : %s\n", strcat(c, d));

    char e[] = "ABC";
    char f[] = "AB";
    printf("strcmp(e, f) : %d\n", strcmp(e, f));
    // strcmp(s1, s2) == 0 -> s1とs2は等しい
    // strcmp(s1, s2) > 0 -> s1の方がs2よりも辞書的に後
    // strcmp(s1, s2) < 0 -> s1の方がs2よりも辞書的に前
}