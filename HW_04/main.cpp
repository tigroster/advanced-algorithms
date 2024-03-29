#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

struct trie_node {
    int visits_count;
    vector<trie_node*> nodes;

    trie_node() : visits_count(0) {
        nodes.resize(2, nullptr);
    }
};

int LS, LT, L1, L2;
string S, T;
int D = 0, L = 1, P = 0;

trie_node* trie_root = new trie_node;

void remove_leaf(int P_decrement)
{
    L--;
    P -= P_decrement;
}

void add_leaf(int P_increment)
{
    L++;
    P += P_increment;
}

void build_trie()
{
    for (int substr_beginning = 0; substr_beginning <= LS; substr_beginning++) {

        int substr_length = L2;
        while (substr_beginning + substr_length > LS) substr_length--;
        if (substr_length < L1) break;

        trie_node* current = trie_root;
        string w = S.substr(substr_beginning, substr_length);

        for (int ch_id = 0; ch_id < w.length(); ch_id++) {
            char ch = w[ch_id];

            if (!current->nodes[0] && !current->nodes[1])
                remove_leaf(ch_id);

            if (!current->nodes[ch-'0']) {
                current->nodes[ch-'0'] = new trie_node;
                add_leaf(ch_id + 1);
            }

            current = current->nodes[ch-'0'];
            current->visits_count++;
        }
    }
}

int elementary_similarity(trie_node*& current, const string& substr, int substr_length)
{
    for (const char& ch : substr) {
        if (!current->nodes[ch-'0']) return 0;

        current = current->nodes[ch-'0'];
    }
    return current->visits_count * substr_length;
}

void restricted_composite_distance()
{
    for (int substr_beginning = 0; substr_beginning <= LT; substr_beginning++) {
        trie_node* current = trie_root;

        int substr_length = L1;
        while (substr_beginning + substr_length <= LT) {

            string w;
            if (substr_length == L1) w = T.substr(substr_beginning, substr_length);
            else w = T[substr_beginning + substr_length - 1];

            int similarity_value = elementary_similarity(current, w, substr_length);
            if (!similarity_value) break;
            D += similarity_value;

            substr_length++;
            if (substr_length > L2) break;
        }
    }
}

int main() {
    cin >> LS >> LT >> L1 >> L2;
    cin >> S >> T;

    build_trie();
    restricted_composite_distance();

    cout << D << " " << L << " " << P << '\n';

    return 0;
}
