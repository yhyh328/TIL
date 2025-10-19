#include <stdio.h>

enum Weekday { MON, TUE, WED, THU, FRI, SAT, SUN};

int main() {
    enum Weekday today = WED;
    printf("오늘은 %d번째 요일입니다.\n", today);  // 0부터 시작 → WED = 2
    return 0;
}