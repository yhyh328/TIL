#ifndef MY_HEADER_H
#define MY_HEADER_H

// // 함수 선언 (선언만)
// int add(int a, int b);
// void printHello();

// // 구조체 선언
// struct Point {
//     int x;
//     int y;
// };

struct data 
{
    int no;
    char name[10];
    int age;
};

struct data arr[10] = {
    {1, "nagashima", 39},
    {2, "tanaka", 25},
    {3, "suzuki", 32},
    {4, "yamada", 28},
    {5, "sato", 45},
    {6, "watanabe", 30},
    {7, "ito", 35},
    {8, "kobayashi", 27},
    {9, "kato", 40},
    {10, "yoshida", 22}
};

#endif