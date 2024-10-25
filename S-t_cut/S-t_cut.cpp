#include <bits/stdc++.h>
using namespace std;

class MinCut {
private:
    int V;
    vector<vector<int>> graph;

    // Bfs za da vrati true ako ima pat od s do t vo povratniot graf
    bool bfs(vector<vector<int>>& rGraph, int s, int t, vector<int>& parent) {
        vector<bool> visited(V, false);
        queue<int> q;
        q.push(s);
        visited[s] = true;
        parent[s] = -1;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v = 0; v < V; v++) {
                if (!visited[v] && rGraph[u][v] > 0) {
                    q.push(v);
                    parent[v] = u;
                    visited[v] = true;
                }
            }
        }
        return visited[t]; // Return true ako stiga do t
    }

    // Dfs za da gi najde site nodovi do koi moze da se stigne od s
    void dfs(vector<vector<int>>& rGraph, int s, vector<bool>& visited) {
        visited[s] = true;
        for (int i = 0; i < V; i++) {
            if (rGraph[s][i] > 0 && !visited[i]) {
                dfs(rGraph, i, visited);
            }
        }
    }

public:
    MinCut(int vertices) : V(vertices), graph(vertices, vector<int>(vertices, 0)) {}


    void addEdge(int u, int v, int capacity) {
        graph[u][v] += capacity;
    }

    int findMinCut(int s, int t) {
        vector<vector<int>> rGraph = graph; // Povraten graf (residual graph)
        vector<int> parent(V); // Niza za da go cuva patot
        int maxFlow = 0;

        // Zgolemuvanje na flow dodeka ima pateka od s do t
        while (bfs(rGraph, s, t, parent)) {
            int path_flow = INT_MAX;
            for (int v = t; v != s; v = parent[v]) {
                int u = parent[v];
                path_flow = min(path_flow, rGraph[u][v]);
            }

            // Update na residual capacities na nodovite i na reverse edges
            for (int v = t; v != s; v = parent[v]) {
                int u = parent[v];
                rGraph[u][v] -= path_flow;
                rGraph[v][u] += path_flow;
            }

            maxFlow += path_flow;
        }

        // Bara nodovi koi moze da se stignat od s
        vector<bool> visited(V, false);
        dfs(rGraph, s, visited);

        // Rabovite koi go formiraat min-cut
        cout << "The following edges are in the minimum cut:\n";
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (visited[i] && !visited[j] && graph[i][j]) {
                    cout << i << " - " << j << endl;
                }
            }
        }

        return maxFlow;
    }
};

int main() {
    MinCut graph(6);
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

    int maxFlow = graph.findMinCut(0, 5);

    cout << "The maximum flow is: " << maxFlow << endl;

    return 0;
}
