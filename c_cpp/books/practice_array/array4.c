#include <stdio.h>
#include <string.h>

// strchr()
//　指定した文字が文字列内に存在するか検索します。
//　○　存在する場合は、最初にその文字が現れた位置のポインタを返します。
//　○　存在するしない場合は、NULLを返します。

void main()
{
    char s[] = "I love cat.";
    char c = 'c';
    char *p = NULL;

    printf("文字列「%s」の中に文字「%c」", s, c);
    p = strchr(s, c);

    if (!p) printf("はありません。\n");
    else printf("を発見しました -> %d。\n", p);
}