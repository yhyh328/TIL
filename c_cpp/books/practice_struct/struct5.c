#include <stdio.h>

// typedef는 타입의 별명(별칭)을 만드는 키워드
// 긴 타입 이름을 짧게 바꾸는 용도

// 예시 1: unsigned char를 u_char로 축약
typedef unsigned char u_char;
u_char c;  // 이제 unsigned char c; 와 동일
unsigned char c2;  // 둘은 완전히 같은 타입

// 예시 2: unsigned int*를 pt_int로 축약
typedef unsigned int *pt_int;
pt_int a;  // 이제 unsigned int *a; 와 동일
unsigned int *a2;  // 둘은 완전히 같은 타입

// 예시 3: 구조체도 가능
typedef struct {
    int x;
    int y;
} Point;
Point p;  // struct { int x; int y; } p; 와 동일

// 왜 사용하나?
// 1. 코드가 간결해짐
// 2. 가독성 향상
// 3. 유지보수 용이 (한 곳에서 타입 변경 시 모든 곳에 반영)

typedef struct _PROFILE {
    char name[40];
    int age;
} PROFILE;

void main() {
    PROFILE prof[2] = { {"Maiko", 20}, {"Naoki", 31} };
    for (int i = 0; i < 2; i++) {
        printf("%sさんは%d歳\n", prof[i].name, prof[i].age);
    }
}