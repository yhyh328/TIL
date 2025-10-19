#include <stdio.h>
#include <stdlib.h>
#include <limits.h>  

void main() {

    int n;
    scanf("%d", &n);
    int *arr = (int*)malloc(sizeof(int) * n);

    int min = INT_MAX;
    for (int i = 0; i < n; i++) {
        int j;
        scanf("%d", &j);
        arr[i] = j;
        if (min > j) { min = j; }
    }

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[i] > arr[j]) {
                int t = arr[i];
                arr[i] = arr[j];
                arr[j] = t;
            }
        }
    }

    printf("[ ");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i != n - 1) { printf("%s", ", "); }
    }
    printf(" ]");

}