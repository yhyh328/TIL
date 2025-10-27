#include <stdio.h>

void main() {
    char s[] = "I love cat and dog.";
    char c = 'a';
    char *p = s;
    int n = 0;
    printf("\"%s\"の中から\'%c\'を探します。\n", s, c);
    while(*p != '\0') {
        if(*p == c) {
            printf("%d文字目で発見しました。\n", p - s + 1);
            n++;
        }
        p++;
    }
    if (n == 0) printf("一つも見つかりませんでした。\n");
    else printf("全部で%d個見つかりました。\n", n);
}