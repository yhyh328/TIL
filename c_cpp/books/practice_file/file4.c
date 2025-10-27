#include <stdio.h>

void main()
{
    int a, b = 7;
    char s[40];
    printf("名前を入力してください\n");
    fgets(s, sizeof(s), stdin);
    printf("数あてクイズ！０から９の数字を入力してくださいしてね\n");
    while(a != b) {
        scanf("%d", &a);
        if((a == b - 1) || (a == b + 1)) { printf("おしい!\n"); }
        else if(a > b + 1) { printf("もっと小さい数です\n"); }
        else if(a < b - 1) { printf("もっと大きい数です\n"); }
    }
    printf("正解！ %s さん、おめでとう！！\n", s);
}