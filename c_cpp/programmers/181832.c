#include <stdio.h>
#include <stdlib.h>

// Function signature for solution
int** solution(int n);

int main() {
    int n;
    printf("Enter the value of n: ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("Please enter a positive integer for n.\n");
        return 1;
    }

    int** ans = solution(n);

    printf("[");
    for (int i = 0; i < n; i++) {
        printf("[");
        for (int j = 0; j < n; j++) {
            printf("%d", ans[i][j]);
            if (j != n - 1) printf(", ");
        }
        printf("]");
        if (i != n - 1) printf(", ");
    }
    printf("]\n");

    // Free the dynamically allocated memory
    for (int i = 0; i < n; i++) {
        free(ans[i]);
    }
    free(ans);

    return 0;
}

int** solution(int n) {
    // Allocate memory for the 2D array
    int** arr = (int**)malloc(n * sizeof(int*));
    int** visited = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        arr[i] = (int*)malloc(n * sizeof(int));
        visited[i] = (int*)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            visited[i][j] = 0;
        }
    }

    int d = 0;
    int dr[] = {0, 1, 0, -1};
    int dc[] = {1, 0, -1, 0};

    int cr = 0, cc = 0;
    for (int val = 1; val <= n * n; val++) {
        arr[cr][cc] = val;
        visited[cr][cc] = 1;
        int nr = cr + dr[d];
        int nc = cc + dc[d];

        if (nr >= 0 && nr < n && nc >= 0 && nc < n && visited[nr][nc] == 0) {
            cr = nr;
            cc = nc;
        } else {
            d = (d + 1) % 4;
            cr += dr[d];
            cc += dc[d];
        }
    }

    // Free the visited array as it's no longer needed
    for (int i = 0; i < n; i++) {
        free(visited[i]);
    }
    free(visited);

    return arr;
}
