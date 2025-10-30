#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int *solve(int n, int m, char *x, char *y);
int *recursive_func(int *arr, int len);

int main() {

    int n, m;
    scanf("%d %d", &n, &m);

    char x[n + 1], y[m + 1];
    scanf("%s %s", x, y);

    int *res = solve(n, m, x, y);
    int percent = *(res + 0) * 10 + *(res + 1);

    printf("%d%%\n", percent);
    free(res);
    return 0;

}

int *solve(int n, int m, char *p, char *q) {

    int dict[26] = {
        3, 2, 1, 2, 4, 3, 1, 3, 1, 1, 3, 1, 3, 
        2, 1, 2, 2, 2, 1, 2, 1, 1, 1, 2, 2, 1
    };

    int arr[n + m + 1];

    int len = 0;

    int i = 0;
    int j, k;

    for (i = 0; ; i++) {
        if (!(*(p + i)) || !(*(q + i))) break;
        j = *(p + i) - 'A';
        k = *(q + i) - 'A';
        arr[len++] = dict[j];
        arr[len++] = dict[k];
    }

    while (i < n && *(p + i)) {
        j = *(p + i) - 'A';
        arr[len++] = dict[j];
        i++;
    }

    while (i < m && *(q + i)) {
        k = *(q + i) - 'A';
        arr[len++] = dict[k];
        i++;
    }

    return recursive_func(arr, len);
}

int *recursive_func(int *arr, int len) {
    if (len == 2) {
        int *ans = (int *)malloc(2 * sizeof(int));
        *(ans + 0) = *(arr + 0);
        *(ans + 1) = *(arr + 1);
        return ans;
    }
    int tmp[len - 1];
    int i;
    // printf("len%d\n", len);
    for (int j = 1; j < len; j++) {
        i = j - 1;
        *(tmp + i) = (*(arr + i) +  *(arr + j)) % 10;
        // printf("%d, ", *(tmp + i));
    }
    // printf("\n");
    return recursive_func(tmp, len - 1);
}
