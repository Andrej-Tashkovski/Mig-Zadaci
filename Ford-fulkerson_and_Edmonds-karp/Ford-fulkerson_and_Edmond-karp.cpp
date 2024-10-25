#include <bits/stdc++.h>
using namespace std;

// So adjacency matrix
class GraphMatrix {
    int V; // Broj na teminja
    vector<vector<int>> capacity; // Matrica od kapaciteti

public:
    GraphMatrix(int V) : V(V), capacity(V, vector<int>(V, 0)) {}

    void addEdge(int u, int v, int cap) {
        capacity[u][v] = cap;               // Ova gi rasporeduva kapacitetite megju site teminja
    }

    // Izminuvanje so bfs
    bool bfs(vector<vector<int>>& residual, vector<int>& parent, int s, int t) {
        vector<bool> visited(V, false);
        queue<int> q;
        q.push(s);
        visited[s] = true;
        parent[s] = -1;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v = 0; v < V; v++) {
                if (!visited[v] && residual[u][v] > 0) {
                    q.push(v);
                    parent[v] = u;
                    visited[v] = true;
                    if (v == t)
                        return true;
                }
            }
        }
        return false;
    }

    // Edmonds-Karp algoritam za maximum flow
    int edmondsKarp(int source, int sink) {
        vector<vector<int>> residual = capacity; // Residual capacity matrix
        vector<int> parent(V);
        int maxFlow = 0;

        while (bfs(residual, parent, source, sink)) {
            int pathFlow = INT_MAX;

            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                pathFlow = min(pathFlow, residual[u][v]);
            }

            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                residual[u][v] -= pathFlow;
                residual[v][u] += pathFlow;
            }

            maxFlow += pathFlow;
        }

        return maxFlow;
    }
};

// So adjacency list
class GraphList {
    int V; // Broj na teminja
    vector<vector<pair<int, int>>> adj; // Lista so kapaciteti

public:
    GraphList(int V) : V(V), adj(V) {}

    void addEdge(int u, int v, int cap) {
        adj[u].push_back({v, cap});
        adj[v].push_back({u, 0}); // Reverse edge for residual graph
    }

    // Izminuvanje so dfs
    bool dfs(vector<vector<int>>& residual, vector<int>& parent, int u, int t) {
        if (u == t)
            return true;
        for (auto& edge : adj[u]) {
            int v = edge.first;
            int cap = residual[u][v];
            if (parent[v] == -1 && cap > 0) {
                parent[v] = u;
                if (dfs(residual, parent, v, t))
                    return true;
            }
        }
        return false;
    }

    // Ford-Fulkerson algoritam za maximum flow
    int fordFulkerson(int source, int sink) {
        vector<vector<int>> residual(V, vector<int>(V, 0));

        // Initialize residual capacities
        for (int u = 0; u < V; u++) {
            for (auto& edge : adj[u]) {
                int v = edge.first, cap = edge.second;
                residual[u][v] = cap;
            }
        }

        vector<int> parent(V);
        int maxFlow = 0;

        while (true) {
            fill(parent.begin(), parent.end(), -1);
            if (!dfs(residual, parent, source, sink))
                break;

            int pathFlow = INT_MAX;

            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                pathFlow = min(pathFlow, residual[u][v]);
            }

            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                residual[u][v] -= pathFlow;
                residual[v][u] += pathFlow;
            }

            maxFlow += pathFlow;
        }

        return maxFlow;
    }
};

int main() {
    cout << "Adjacency Matrix: Edmonds-Karp Algorithm\n";
    GraphMatrix graphMatrix(6);
    graphMatrix.addEdge(0, 1, 16);
    graphMatrix.addEdge(0, 2, 13);
    graphMatrix.addEdge(1, 2, 10);
    graphMatrix.addEdge(1, 3, 12);
    graphMatrix.addEdge(2, 1, 4);
    graphMatrix.addEdge(2, 4, 14);
    graphMatrix.addEdge(3, 2, 9);
    graphMatrix.addEdge(3, 5, 20);
    graphMatrix.addEdge(4, 3, 7);
    graphMatrix.addEdge(4, 5, 4);

    int maxFlowMatrix = graphMatrix.edmondsKarp(0, 5);
    cout << "Max Flow (Matrix): " << maxFlowMatrix << endl;

    cout << "\nAdjacency List: Ford-Fulkerson Algorithm\n";
    GraphList graphList(6);
    graphList.addEdge(0, 1, 16);
    graphList.addEdge(0, 2, 13);
    graphList.addEdge(1, 2, 10);
    graphList.addEdge(1, 3, 12);
    graphList.addEdge(2, 1, 4);
    graphList.addEdge(2, 4, 14);
    graphList.addEdge(3, 2, 9);
    graphList.addEdge(3, 5, 20);
    graphList.addEdge(4, 3, 7);
    graphList.addEdge(4, 5, 4);

    int maxFlowList = graphList.fordFulkerson(0, 5);
    cout << "Max Flow (List): " << maxFlowList << endl;

    return 0;
}
