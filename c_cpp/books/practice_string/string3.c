#include <stdio.h>
#include <string.h>

void main() {
    char s1[40] = {0};                 // 전체 0으로 초기화(권장)
    snprintf(s1, sizeof(s1), "%f", 143.5);

    printf("%s\n", s1);                // 143.500000
    printf("%zu\n", sizeof(s1));       // 20

    printf("{ ");
    // 널 문자까지 보고 싶으면 strlen(s1)까지(<=) 순회
    for (size_t i = 0; i <= strlen(s1); i++) {
        if (s1[i] != '\0') printf("%c, ", s1[i]);
        else                printf("\\0, ");
    }
    // 남은 패딩(초기화해두면 전부 \0)
    for (size_t i = strlen(s1) + 1; i < sizeof(s1); i++) {
        printf("\\0, ");
    }
    printf("}\n");

    char s2[] = "cat", s3[] = "dog";
    char s4[20];
    sprintf(s4, "I love %s and %s.", s2, s3);
    printf("「%s」の文字数は%d\n", s4, strlen(s4)); // 「I love cat and dog.」の文字数は19
}