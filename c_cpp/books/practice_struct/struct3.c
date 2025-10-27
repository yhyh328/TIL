#include <stdio.h>
#include "my_header.h"

// &의 사용법 정리:
//
// C언어에는 "객체"라는 개념이 없음 (C++에만 있음)
// C언어는 원시형과 구조체만 존재
//
// 1. 일반 변수 (원시형/구조체 모두): & 필요
//    int num = 5;             // 원시형
//    int *p1 = &num;          // O
//    
//    struct Point pt;         // 구조체
//    struct Point *p2 = &pt;  // O (구조체도 & 필요!)
//
// 2. 배열만 특별함: & 불필요! (이미 주소가 됨)
//    int arr[10];             // 배열
//    int *p3 = arr;           // O: arr == &arr[0]
//    int *p3 = &arr[0];       // O: 명시적으로 첫 번째 요소
//    int *p3 = &arr;          // X: 에러! (배열 전체의 주소, 타입이 다름)
//
// 핵심: 배열이냐 아니냐만 중요! (원시형/구조체는 구분 없음)
struct data *sp = arr;  // O (list1은 배열이므로 이미 주소)
// struct data *sp = &arr;  // X (잘못됨 - 배열 전체의 주소, 타입 불일치!)

void main() 
{
    for (int i = 0; i < 10; i++)
    {
        printf("No: %d, Name: %s, Age: %d\n", 
               (*(sp+i)).no, (*(sp+i)).name, (*(sp+i)).age);
    }
}

