#include <stdio.h>

void main() {
    int x, y;
    int arr[2][3] = {
        {10, 20, 30},
        {40, 50, 60}
    };

    // Size of the entire array in bytes: 4 bytes * 2 rows * 3 columns = 24
    int arr_size = sizeof(arr);

    // Size of int: 4
    int int_size = sizeof(int);

    // Total number of elements: 6
    int total_els = arr_size / int_size;

    // Size of row: 12
    int row_size = sizeof(arr[0]);

    // Number of rows: 3
    int total_rows = arr_size / row_size;

    // Number of cols: 2
    int total_cols = row_size / int_size;

    printf("Array size (bytes): %d\n", arr_size);
    printf("Size of int: %d\n", int_size);
    printf("Total elements: %d\n", total_els);
    printf("Row size (bytes): %d\n", row_size);
    printf("Number of rows: %d\n", total_rows);
    printf("Number of columns: %d\n", total_cols);
} 