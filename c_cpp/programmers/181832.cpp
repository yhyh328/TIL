#include <string>
#include <vector>
#include <iostream>

using namespace std;

vector<vector<int>> solution(int n);

int main() {
    int n;
    cin >> n;
    vector<vector<int>> ans = solution(n);
    cout << "[";
    for (int i = 0; i < n; i++) {
        cout << "[";
        for (int j = 0; j < n; j++) {
            cout << ans[i][j];
            if (j != n - 1) cout << ", ";
        }
        cout << "]";
        if (i != n - 1) cout << ", ";
    }
    cout << "]" << endl;
    return 0;
}

vector<vector<int>> solution(int n) {

    vector<vector<int>> arr(n, vector<int>(n, 0));
    vector<vector<int>> visited(n, vector<int>(n, 0));

    int d = 0;
    vector<int> dr = {0, 1, 0, -1};
    vector<int> dc = {1, 0, -1, 0};

    int cr = 0, cc = 0;
    for (int val = 1; val < n * n + 1; val++) {
        arr[cr][cc] = val;
        visited[cr][cc] = 1;
        int nr = cr + dr[d], nc = cc + dc[d];
        if (0 <= nr && nr < n && 0 <= nc && nc < n && visited[nr][nc] == 0) {
            cr = nr;
            cc = nc;
        } else {
            d = (d + 1) % 4;
            cr += dr[d];
            cc += dc[d];
        }
    }

    return arr;

}