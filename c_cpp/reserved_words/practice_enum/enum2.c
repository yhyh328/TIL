#include <stdio.h>

enum Direction { UP, DOWN, LEFT, RIGHT };

int main() {
    enum Direction d = LEFT;

    switch(d) {
        case UP:    printf("위로 이동\n"); break;
        case DOWN:  printf("아래로 이동\n"); break;
        case LEFT:  printf("왼쪽으로 이동\n"); break;
        case RIGHT: printf("오른쪽으로 이동\n"); break;
    }

    return 0;
}