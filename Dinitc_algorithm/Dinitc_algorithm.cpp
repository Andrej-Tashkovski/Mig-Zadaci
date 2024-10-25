#include <bits/stdc++.h>
using namespace std;

class Dinic {
    int V;
    vector<vector<int>> capacity;
    vector<vector<int>> adj;

public:
    Dinic(int V) : V(V), capacity(V, vector<int>(V, 0)), adj(V) {}

    void addEdge(int u, int v, int cap) {
        capacity[u][v] += cap; // Handle multiple edges
        adj[u].push_back(v);
        adj[v].push_back(u); // Reverse edge for the residual graph
    }

    // BFS za leveli vo grafot
    bool bfs(int source, int sink, vector<int>& level) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        q.push(source);
        level[source] = 0;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v : adj[u]) {
                if (level[v] < 0 && capacity[u][v] > 0) { // Not visited and positive capacity
                    level[v] = level[u] + 1;
                    q.push(v);
                }
            }
        }
        return level[sink] >= 0; // Return true if sink is reachable
    }

    // DFS za flow
    int dfs(int u, int flow, int sink, vector<int>& level, vector<int>& start) {
        if (u == sink) return flow; // If we reached the sink, return flow

        for (; start[u] < adj[u].size(); ++start[u]) {
            int v = adj[u][start[u]];

            if (level[v] == level[u] + 1 && capacity[u][v] > 0) { // Valid path
                int currFlow = min(flow, capacity[u][v]);
                int tempFlow = dfs(v, currFlow, sink, level, start);
                
                if (tempFlow > 0) {
                    capacity[u][v] -= tempFlow; // Update forward capacity
                    capacity[v][u] += tempFlow; // Update reverse capacity
                    return tempFlow; // Return flow sent
                }
            }
        }
        return 0; // No flow sent
    }

    int dinic(int source, int sink) {
        int maxFlow = 0;
        vector<int> level(V);
        vector<int> start(V);

        while (bfs(source, sink, level)) {
            fill(start.begin(), start.end(), 0); // Reset start index for DFS

            while (int flow = dfs(source, INT_MAX, sink, level, start)) {
                maxFlow += flow; // Add flow to total
            }
        }

        return maxFlow;
    }
};

int main() {
    Dinic graph(6);
    graph.addEdge(0, 1, 16);
    graph.addEdge(0, 2, 13);
    graph.addEdge(1, 2, 10);
    graph.addEdge(1, 3, 12);
    graph.addEdge(2, 1, 4);
    graph.addEdge(2, 4, 14);
    graph.addEdge(3, 2, 9);
    graph.addEdge(3, 5, 20);
    graph.addEdge(4, 3, 7);
    graph.addEdge(4, 5, 4);

    int maxFlow = graph.dinic(0, 5);
    cout << "Max Flow: " << maxFlow << endl;

    return 0;
}
