#include <stdio.h>

int memo[45 + 1];

int f(int n, int *p);

int main(void) {

    int n;
    scanf("%d", &n);

    *(memo + 0) = 0;
    *(memo + 1) = 1;
    for (int i = 2; i < 45 + 1; i++) *(memo + i) = -1;
    printf("%d", f(n, memo)); 
    return 0;
}

int f(int n, int *p) {
    if (*(p + n) != -1) return *(p + n);
    *(p + n) = f(n - 1, p) + f(n - 2, p);
    return *(p + n);
}