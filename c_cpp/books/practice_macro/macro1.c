#include <stdio.h>
#define HIKU(x, y) ((x)-(y))
#define HIKU1(x, y) (x - y)

void main()
{
    printf("マクロの実行結果：%d\n", HIKU(5, 3));

    // マクロの実行結果：3
    printf("マクロの実行結果：%d\n", HIKU(5 + 2, 1 + 3));

    // マクロ1の実行結果：9
    printf("マクロ1の実行結果：%d\n", HIKU1(5 + 2, 1 + 3));
}