#include <stdio.h>

void main()
{
    int mat[3][4] = {
        {20, 42, 70, 34},
        {67, 98, 37, 41},
        {76, 99, 43, 65}
    };
    int i, j;
    int sum_r; 
    int sum_c[4] = {0, 0, 0, 0};
    int total = 0;

    // printf("%4d\n", 7);
    // printf("%4d\n", 42);
    // printf("%4d\n", 1234);
    // printf("%4d\n", 56789);

    //       7
    //      42
    //    1234
    //   56789

    for (j = 0; j < 3; j++) {
        sum_r = 0;
        for (i = 0; i < 4; i++) {
            printf("%4d", mat[j][i]); 
            sum_r += mat[j][i];
            sum_c[i] += mat[j][i];
        }
        printf(" | %4d\n", sum_r);
    }

    printf("-----------------+------\n");
    for (i = 0; i < 4; i++) {
        printf("%4d", sum_c[i]);
        total += sum_c[i];
    }
    printf(" | %4d\n", total);
}