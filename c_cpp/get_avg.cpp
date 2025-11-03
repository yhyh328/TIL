#include <iostream>
#include <string>
#include <vector>
#include <numeric>

using namespace std;

double get_avg(const vector<int>& numbers) {
    int sum = accumulate(numbers.begin(), numbers.end(), 0);
    return static_cast<double>(sum) / numbers.size();
}

int get_sum(const vector<int>& numbers) {
    return accumulate(numbers.begin(), numbers.end(), 0);
}

int main() {

    int len;
    cout << "Enter the numbers: ";
    cin >> len;

    cout << "Insert numbers for " << len << " times." << endl;

    vector<int> numbers;
    numbers.reserve(len);

    for (int i = 0; i < len; i++) {
        
        int el;
        int mod = (i + 1) % 10;
        string suffix;

        if (11 <= (i + 1) % 100 && (i + 1) % 100 <= 13) {
            suffix = "th";
        } else if (mod == 1) {
            suffix = "st";
        } else if (mod == 2) {
            suffix = "nd";
        } else if (mod == 3) {
            suffix = "rd";
        } else {
            suffix = "th";
        }

        cout << "Enter the " << (i + 1) << suffix << " element: ";
        cin >> el;
        numbers.push_back(el);

    }

    int sum = get_sum(numbers);
    cout << "Sum: " << sum << endl;

    double avg = get_avg(numbers);
    cout << "Average: " << avg << endl;

    return 0;

}

