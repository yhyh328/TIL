#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {

    int dict[26] = {
        3, 2, 1, 2, 4, 3, 1, 3, 1, 1, 3, 1, 3, 
        2, 1, 2, 2, 2, 1, 2, 1, 1, 1, 2, 2, 1
    };

    int n, m;
    scanf("%d %d", &n, &m);

    char x[n + 1], y[m + 1];
    scanf("%s %s", x, y);

    int z[n + m];

    int idx = 0, len = 0;

    while (idx < n && idx < m) {
        *(z + len++) = *(dict + (int)(*(x + idx) - 'A'));
        *(z + len++) = *(dict + (int)(*(y + idx) - 'A'));
        idx++;
    } 
    while (idx < n) {
        *(z + len++) = *(dict + (int)(*(x + idx++) - 'A'));
    }
    while (idx < m) {
        *(z + len++) = *(dict + (int)(*(y + idx++) - 'A'));
    }

    while (len != 2) {
        for (int i = 0; i < len - 1; i++) {
            *(z + i) = (*(z + i) + *(z + i + 1)) % 10;
        }
        len--;
    }

    if (*(z + 0)) printf("%d%d%%", *(z + 0), *(z + 1));
    else printf("%d%%", *(z + 1));

    return 0;
}