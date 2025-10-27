#include <stdio.h>

void dispnum(int a); //　プロトタイプ宣言

void main()
{
    int x = 10;
    dispnum(5); // 引数の値は : 5
    dispnum(x); // 引数の値は : 10
}

void dispnum(int a) // 定義
{ printf("引数の値は : %d\n", a); }