#include <stdio.h>
#include <string.h>

void main() {
    printf("      %8s  %8s\n", "商品A", "商品B");
    printf(" 数量 %08d %08d\n", 16, 246);
    printf(" 重量 %8.4f %8.4f\n", 76.3, 556.1);
    printf(" %d%c", 20, 10);  // 文字コード１０は改行になります。
    printf(" %d\bA\n", 20);
    printf(" %d\t%d\n", 20, 30);
}