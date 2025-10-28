#include <stdio.h>
#define LOOPNUM 3

void main() {
    for (int i = 0; i < LOOPNUM; i++) printf("LoopCount:%d\n", i+1);
    // LoopCount:1
    // LoopCount:2
    // LoopCount:3
}
    