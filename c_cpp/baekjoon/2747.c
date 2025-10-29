#include <stdio.h>

int memo[45 + 1];

int dp(int n, int *p);

int main() {
    *(memo + 0) = 0;
    *(memo + 1) = 1;
    for (int i = 2; i <= 45; i++) *(memo + i) = -1;
    int n;
    scanf("%d", &n);
     printf("%d\n", dp(n, memo));
}

int dp(int n, int *p) {
    if (n <= 0) return *(p + 0);
    if (*(p + n) != -1) return *(p + n);
    *(p + n) = dp(n - 1, p) + dp(n - 2, p);
    return *(p + n);
}