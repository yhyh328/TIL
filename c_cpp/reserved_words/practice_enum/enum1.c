#include <stdio.h>

enum HttpStatus {
    OK = 200,
    NOT_FOUND = 404,
    SERVER_ERROR = 500
};

int main() {
    enum HttpStatus code = NOT_FOUND;
    printf("HTTP 상태 코드: %d\n", code);
    return 0;
}