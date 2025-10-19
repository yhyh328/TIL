#include <stdio.h>

void main() {
    char a;
    do {
        a = getchar(); // キーボードから入力された半角文字一つを得ます。
        printf("%c\n", a);
    } while(a != 'e');
}