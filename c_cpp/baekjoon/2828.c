#include <stdio.h>

int traversal(int m, int *traveler, int target);

int main() {
    
    int n, m;
    scanf("%d %d", &n, &m);
    
    int traveler[m];
    for (int i = 0; i < m; i++) *(traveler + i) = i;

    int l;
    scanf("%d", &l);

    int vals[l];
    for (int i = 0; i < l; i++) {
        int start = m - 1;
        int target;
        scanf("%d", &target);
        target -= 1;
        int val = traversal(m, traveler, target);
        *(vals + i) = val;
    }

    int ans = 0;
    for (int i = 0; i < l; i++) ans += *(vals + i);

    printf("%d", ans);

    return 0;

}

int traversal(int m, int *traveler, int target) {

    int l = *(traveler + 0);
    int r = *(traveler + m - 1);

    int steps = 0;

    if (l <= target && target <= r) return steps;

    if (target < l) {
        steps += (l - target);
        for (int i = 0; i < m; i ++) *(traveler + i) -= steps;
    } else {
        steps += (target - r);
        for (int i = 0; i < m; i ++) *(traveler + i) += steps;
    }
    
    // if (l < target) {
    //     steps += target - l;
    //     for (int i = 0; i < m; i ++) *(tmp + i) = *(traveler + i) + steps;
    // } else {
    //     steps += r - target;
    //     for (int i = 0; i < m; i ++) *(tmp + i) = *(traveler + i) - steps;
    // }

    return steps;

}