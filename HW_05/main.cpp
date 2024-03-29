#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <map>
#include <set>

using namespace std;

int N, F;
map<int, vector<string>> D;
string T;

set<int> D_lengths;
int hdd;

int hamming_distance(const string& substr, const string& d)
{
    int hd = 0;
    for (size_t i = 0; i < d.size(); ++i) {
        if (substr[i] != d[i]) ++hd;
    }
    return hd;
}

int min_D_hamming_distance(const string& substr, int D_length)
{
    int min_hd = INT_MAX;
    for (const string& d : D[D_length]) {
        min_hd = min(min_hd, hamming_distance(substr, d));
    }
    return min_hd;
}

vector<int> dp_short;
vector<int> dp_long;

void unbounded_knapsack()
{
    int T_size = T.size();

    dp_short.resize(T_size + 1, INT_MAX);
    dp_short[0] = 0;

    dp_long.resize(T_size + 1, INT_MAX);
    dp_long[0] = 0;

    for (int i = 1; i < T_size + 1; ++i) {
        for (int j = 1; j < F; ++j) {
            if (i < j || dp_long[i-j] == INT_MAX) continue;
            dp_short[i] = min(dp_short[i], dp_long[i-j] + 1);
        }

        for (int j : D_lengths) {
            if (i < j || (dp_long[i-j] == INT_MAX && dp_short[i-j] == INT_MAX)) continue;

            int prev_hdd = min(dp_long[i-j], dp_short[i-j]);
            int curr_hdd = min_D_hamming_distance(T.substr(i - j, j), j);

            dp_long[i] = min(dp_long[i], prev_hdd + curr_hdd);
        }
    }

    hdd = min(dp_long[T_size], dp_short[T_size]);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N >> F;

    for (int i = 0; i < N; ++i) {
        string d;
        cin >> d;
        int l = d.length();
        D[l].push_back(d);
        D_lengths.insert(l);
    }

    cin >> T;

    unbounded_knapsack();

    cout << hdd << endl;

    return 0;
}
