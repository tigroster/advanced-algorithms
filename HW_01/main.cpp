#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

vector<vector<pair<int, int>>> adj;
vector<vector<int>> adj_groups;

vector<vector<int>> depth_groups;
vector<vector<int>> component_groups;
vector<int> depth;
vector<int> node_group;

int next_group_id = 0;
long long components_mst = 0;
long long min_mst = LONG_LONG_MAX;

void dfs(int s, vector<bool>& visited) {
    stack<int> S;

    S.push(s);

    while (!S.empty()) {
        int n  = S.top();
        S.pop();
        if (!visited[n] && depth[s] == depth[n]) {
            visited[n] = true;
            node_group[n] = next_group_id;
            component_groups[next_group_id].push_back(n);
            for(auto neighbour : adj[n]) {
                S.push(neighbour.first);
            }
        }
    }
}

void add_group_id_to_nodes() {
    vector<bool> visited(adj.size(), false);
    node_group.resize(adj.size(), 0);
    component_groups.resize(adj.size());
    int i = 0;
    while(!depth_groups[i].empty()) {
        for (int j : depth_groups[i]) {
            if (!visited[j]) {
                dfs(j, visited);
                next_group_id++;
            }
        }
        i++;
    }
}

void add_depth_to_nodes(int start) {
    list<int> queue;
    depth.resize(adj.size(), 0);
    depth_groups.resize(adj.size());
    depth_groups[0].push_back(start);
    vector<bool> visited(adj.size(), false);
    visited[start] = true;

    queue.push_back(start);
    while (!queue.empty()) {

        int s = queue.front();
        queue.pop_front();

        for (auto adjacent: adj[s]) {
            if (!visited[adjacent.first]) {
                visited[adjacent.first] = true;
                depth[adjacent.first] = depth[s] + 1;
                depth_groups[depth[adjacent.first]].push_back(adjacent.first);

                queue.push_back(adjacent.first);
            }
        }
    }
}

void prim_mst(vector<int> group) {
    vector<int> mst(adj.size(), -1);
    int start = group[0];
    mst.push_back(start);
    vector<bool> visited(adj.size(), false);
    vector<int> value(adj.size(), INT_MAX);
    visited[start] = true;
    int current_group = node_group[start];
    vector<pair<int, int>> edges = adj[start];
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> queue;
    queue.emplace(0, start);
    value[start]=0;

    while (!queue.empty()) {
        int node = queue.top().second;
        visited[node] = true;
        queue.pop();
        for (auto neighbor : adj[node]) {
            int price = neighbor.second;
            int vertex = neighbor.first;
            if (!visited[vertex] && price < value[vertex] && node_group[vertex] == current_group) {
                value[vertex] = price;
                mst[vertex] = node;
                queue.emplace(value[vertex], vertex);
            }
            if (node_group[vertex] != current_group) {
                if (price < adj_groups[current_group][node_group[vertex]]) {
                    adj_groups[current_group][node_group[vertex]] = price;
                    adj_groups[node_group[vertex]][current_group] = price;

                }
            }
        }
    }

    for (int i : value) {
        if (i != INT_MAX)
            components_mst += i;
    }
}

void prim_mst_components() {
    vector<int> mst;
    int start = 0;
    mst.push_back(start);
    vector<bool> visited(adj_groups.size(), false);
    visited[start] = true;

    while (mst.size() < adj_groups.size()) {
        pair<int, int> min_v = make_pair(INT_MAX, INT_MAX);

        for (int i : mst) {
            vector<int> edges = adj_groups[i];

            for (int j = 0; j < edges.size(); ++j) {
                if (edges[j] != INT_MAX && !visited[j] && edges[j] < min_v.second && depth[component_groups[i][0]] < depth[component_groups[j][0]]) {
                    min_v = make_pair(j, edges[j]);
                }
            }
        }

        if (min_v.first != INT_MAX) {
            visited[min_v.first] = true;
            components_mst += min_v.second;
            mst.push_back(min_v.first);
        }
    }
}

int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int num_of_vertices, num_of_edges;
    cin >> num_of_vertices >> num_of_edges;
    adj.resize(num_of_vertices);

    for (int i = 0; i < num_of_edges; i++)
    {
        int v1_id, v2_id, price;
        cin >> v1_id >> v2_id >> price;

        adj[v1_id-1].emplace_back(v2_id-1, price);
        adj[v2_id-1].emplace_back(v1_id-1, price);
    }

    for (int i = 0; i < num_of_vertices; ++i) {
//        cout << "From node: " << i << "\n";
        add_depth_to_nodes(i);
        add_group_id_to_nodes();

        adj_groups.resize(next_group_id, vector<int>(next_group_id));
        for (auto &i : adj_groups) {
            fill(i.begin(), i.end(), INT_MAX);
        }

        for (const vector<int>& group : component_groups) {
            if (group.empty()) break;
            prim_mst(group);
        }
//        cout << "mst inside groups: " << components_mst << "\n" ;

        prim_mst_components();

//        cout << "mst with bridges: " << components_mst << "\n" ;

        if (components_mst < min_mst) {
            min_mst = components_mst;
        }

        adj_groups.clear();
        depth.clear();
        depth_groups.clear();
        node_group.clear();
        component_groups.clear();
        next_group_id = 0;
        components_mst = 0;
//        cout << "\n";
    }

    cout << min_mst << "\n" ;

    return 0;
}
