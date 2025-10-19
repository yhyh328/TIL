#include <stdio.h>
#include <string.h>

void main() {
    int n, m;
    n = sizeof(short);
    m = sizeof(n);
    char str[] = "abcdefghijklmn";
    int num_arr[] = {1, 2, 3, 4, 5, 6};
    printf("%d\n", n); // 2
    printf("%d\n", m); // 4
    // %zu is format specifier for size_t (what sizeof returns)
    printf("%zu\n", sizeof(str)); // 15 (15 * 1 bytes per char)
    printf("%zu\n", sizeof(num_arr)); // 24 (6 * 4 bytes per int)
}