#include <stdio.h>

// _point2d는 구조체 타입의 이름 (구조체 정의)
// pt는 이 구조체 타입으로 선언된 변수의 이름 (구조체 인스턴스)
struct _point2d 
{
    double x;
    double y;
} pt;  // 구조체 정의와 동시에 pt라는 변수를 선언

void main() 
{
    // 첫 번째 점
    pt.x = 30.0;
    pt.y = 23.6;
    printf("pt = (%4.1f, %4.1f)\n", pt.x, pt.y);
    
    // _point2d 타입으로 새로운 변수들 생성 가능!
    struct _point2d pt2;  // 두 번째 점
    struct _point2d pt3;  // 세 번째 점
    
    pt2.x = 10.5;
    pt2.y = 20.0;
    printf("pt2 = (%4.1f, %4.1f)\n", pt2.x, pt2.y);
    
    pt3.x = 0.0;
    pt3.y = 0.0;
    printf("pt3 = (%4.1f, %4.1f)\n", pt3.x, pt3.y);
}