#include <iostream>
#include <vector>
#include <set>
#include <iterator>
#include <algorithm>
#include <map>

using namespace std;

int N, C, F;

struct network{
    vector<bool> is_fast;
    vector<pair<int, int>> degrees;
    vector<vector<int>> connections;
    map<tuple<bool, int, int>, vector<int>> degree_mapping;
};

network network_A;
network network_B;
bool is_isomorphism_flag = false;
vector<int> A_degree_mapping;

bool is_isomorphism(const std::vector<int>& permutation) {
    for (int i = 0; i < N; ++i) {
        for (int j = i+1; j < N; ++j) {
            if (network_A.connections[A_degree_mapping[i]][A_degree_mapping[j]] != network_B.connections[permutation[i]][permutation[j]]) {
                return false;
            }
        }
    }
    is_isomorphism_flag = true;
    return true;
}

std::vector<int> flatten(std::vector<std::vector<int>> const &vec)
{
    int size = 0;
    for (auto &v: vec) {
        size += v.size();
    }

    std::vector<int> flattened;
    flattened.reserve(size);

    for (auto &v: vec) {
        for (auto &e: v) {
            flattened.push_back(e);
        }
    }

    return flattened;
}

void calculate(vector<int>& p, size_t n, vector<vector<int>>& va)
{
    if (is_isomorphism_flag) return;
    if (va.size() > n)
    {
        do {
            if (is_isomorphism_flag) return;

            for (auto x : va[n])
            {
                p.push_back(x);
            }

            calculate(p, n+1, va);
        } while (next_permutation(va[n].begin(), va[n].end()));

    }
    else
    {
        is_isomorphism(flatten(va));
    }
}


bool check_networks_isomorphism() {
    vector<vector<int>> groups;
    vector<vector<int>> groups_A;
    vector<int> permutation;

    auto it = network_B.degree_mapping.begin();

    while (it != network_B.degree_mapping.end()) {
        groups.push_back(it->second);
        ++it;
    }

    auto itA = network_A.degree_mapping.begin();

    while (itA != network_A.degree_mapping.end()) {
        groups_A.push_back(itA->second);
        ++itA;
    }

    vector<pair<int, int>> p;

    for (int i = 0; i < groups.size(); ++i) {
        if (groups[i].size() > 1) {
            p.emplace_back(i, groups[i].size());
        }
    }

    A_degree_mapping = flatten(groups_A);
    calculate(permutation, 0, groups);

    return is_isomorphism_flag;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N >> C >> F;

    set<int> tmp_set;
    for (int i = 0; i < N; ++i) {
        tmp_set.insert(i);
    }

    network_A.connections.resize(N, vector<int>(N));
    network_B.connections.resize(N, vector<int>(N));

    network_A.degrees.resize(N);
    network_B.degrees.resize(N);

    network_A.is_fast.resize(N);
    network_B.is_fast.resize(N);

    int fast_server_id;
    for (int i = 0; i < F; ++i) {
        cin >> fast_server_id;
        network_A.is_fast[fast_server_id] = true;
    }

    int from, to;
    for (int i = 0; i < C-1; ++i) {
        cin >> from >> to;
        network_A.connections[from][to] = 1;
        network_A.connections[to][from] = 1;

        if (network_A.is_fast[from]) {
            network_A.degrees[to].second++;
        } else {
            network_A.degrees[to].first++;
        }

        if (network_A.is_fast[to]) {
            network_A.degrees[from].second++;
        } else {
            network_A.degrees[from].first++;
        }
    }

    for (int i = 0; i < N; ++i) {
        if (network_A.is_fast[i]) {
            network_A.degree_mapping[make_tuple(true, network_A.degrees[i].first, network_A.degrees[i].second)].push_back(i);
        } else {
            network_A.degree_mapping[make_tuple(false, network_A.degrees[i].first, network_A.degrees[i].second)].push_back(i);
        }
    }

    for (int i = 0; i < F; ++i) {
        cin >> fast_server_id;
        network_B.is_fast[fast_server_id] = true;
    }


    for (int i = 0; i < C+1; ++i) {
        cin >> from >> to;
        network_B.connections[from][to] = 1;
        network_B.connections[to][from] = 1;

        if (network_B.is_fast[from]) {
            network_B.degrees[to].second++;
        } else {
            network_B.degrees[to].first++;
        }

        if (network_B.is_fast[to]) {
            network_B.degrees[from].second++;
        } else {
            network_B.degrees[from].first++;
        }
    }

    for (int i = 0; i < N; ++i) {
        if (network_B.is_fast[i]) {
            network_B.degree_mapping[make_tuple(true, network_B.degrees[i].first, network_B.degrees[i].second)].push_back(i);
        } else {
            network_B.degree_mapping[make_tuple(false, network_B.degrees[i].first, network_B.degrees[i].second)].push_back(i);
        }
    }

    for (int i_A = 0; i_A < N; ++i_A) {
        for (int j_A = i_A+1; j_A < N; ++j_A) {

            if (network_A.connections[i_A][j_A] == 0 ) {
                for (int i_B = 0; i_B < N; ++i_B) {
                    for (int j_B = i_B+1; j_B < N; ++j_B) {

                        if (network_B.connections[i_B][j_B] == 1 ) {
                            network_B.connections[i_B][j_B]--;
                            network_B.connections[j_B][i_B]--;
                            network_A.connections[i_A][j_A]++;
                            network_A.connections[j_A][i_A]++;

                            vector<pair<int, int>> degrees_B = network_B.degrees;
                            if (network_B.is_fast[i_B]) {
                                network_B.degrees[j_B].second--;
                            } else {
                                network_B.degrees[j_B].first--;
                            }

                            if (network_B.is_fast[j_B]) {
                                network_B.degrees[i_B].second--;
                            } else {
                                network_B.degrees[i_B].first--;
                            }

                            vector<pair<int, int>> degrees_A = network_A.degrees;
                            if (network_A.is_fast[i_A]) {
                                network_A.degrees[j_A].second++;
                            } else {
                                network_A.degrees[j_A].first++;
                            }

                            if (network_A.is_fast[j_A]) {
                                network_A.degrees[i_A].second++;
                            } else {
                                network_A.degrees[i_A].first++;
                            }

                            network_A.degree_mapping.clear();
                            network_B.degree_mapping.clear();
                            for (int i = 0; i < N; ++i) {
                                if (network_A.is_fast[i]) {
                                    network_A.degree_mapping[make_tuple(true, network_A.degrees[i].first, network_A.degrees[i].second)].push_back(i);
                                } else {
                                    network_A.degree_mapping[make_tuple(false, network_A.degrees[i].first, network_A.degrees[i].second)].push_back(i);
                                }
                            }

                            for (int i = 0; i < N; ++i) {
                                if (network_B.is_fast[i]) {
                                    network_B.degree_mapping[make_tuple(true, network_B.degrees[i].first, network_B.degrees[i].second)].push_back(i);
                                } else {
                                    network_B.degree_mapping[make_tuple(false, network_B.degrees[i].first, network_B.degrees[i].second)].push_back(i);
                                }
                            }

                            sort(network_A.degrees.begin(), network_A.degrees.end());
                            sort(network_B.degrees.begin(), network_B.degrees.end());



                            if (network_A.degrees == network_B.degrees) {

                                if (check_networks_isomorphism()) {
                                    printf("%d %d %d %d\n", i_A, j_A, i_B, j_B);
                                    is_isomorphism_flag = false;
                                }
                            }

                            network_B.connections[i_B][j_B]++;
                            network_B.connections[j_B][i_B]++;
                            network_A.connections[i_A][j_A]--;
                            network_A.connections[j_A][i_A]--;

                            network_B.degrees = degrees_B;
                            network_A.degrees = degrees_A;
                        }

                    }
                }
            }

        }
    }

    return 0;
}
