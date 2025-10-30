#include <stdio.h>

int solve(int a, int b, int c);

int main(void) {
    int a, b, c;
    scanf("%d %d %d", &a, &b, &c);
    printf("%d", solve(a, b, c));
    return 0;
}

int solve(int a, int b, int c) {
    int x;
    if (a == b && b == c) {
        x = a;
        return 10000 + x * 1000;
    } 
    if (a == b || a == c || b == c) {
        x = (b == c) ? b : a;
        return 1000 + x * 100;
    }
    x = (a > b && a > c) ? a : (b > c) ? b : c;
    return x * 100;
}