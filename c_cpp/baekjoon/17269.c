#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int *solve(int n, int m, char *x, char *y);
int *recursive_func(int *t);

int main(void) {

    int n, m;
    scanf("%d %d", &n, &m);

    char x[n + 1], y[m + 1];
    scanf("%s %s", x, y);

    printf("%ld%", solve(x, y));
    return 0;

}

int *solve(int n, int m, char *p, char *q) {

    int dict[26] = {
        3, 2, 1, 2, 4, 3, 1, 3, 1, 1, 3, 1, 3, 
        2, 1, 2, 2, 2, 1, 2, 1, 1, 1, 2, 2, 1
    };

    int arr[n + m + 1];

    for (int i = 0; i < (n + m) / 2; i++) {

        if (!(*(p + i)) || !(*(q + i))) break;

        int j = *(p + i) - 'A';
        int k = *(q + i) - 'A';

        arr[i * 2] = dict[j];
        arr[i * 2 + 1] = dict[k];
    }

    return recursive_func(arr);
}

int *recursive_func(int *t) {
    int len = (sizeof(t) / sizeof(t[0]));
    if (len == 2) {
        int ans[2] = {*(t + 0), *(t + 1)};
        return ans;
    }
    int tmp[len - 1];
    int i, j;
    for (int  = 1; j < len; j++) {
        i = j - 1;
        *(tmp + i) = (*(t + i) +  *(t + j)) % 10;
    }
    return *recursive_func(int *tmp);
}