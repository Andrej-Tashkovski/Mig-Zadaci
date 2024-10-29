#include <bits/stdc++.h>
using namespace std;

class PushRelabel {
private:
    int V;
    vector<vector<int>> capacity;
    vector<vector<int>> flow;
    vector<int> height;
    vector<int> excess;

public:
    PushRelabel(int vertices) : V(vertices) {
        capacity.assign(V, vector<int>(V, 0));
        flow.assign(V, vector<int>(V, 0));
        height.assign(V, 0);
        excess.assign(V, 0);
    }

    void addEdge(int u, int v, int cap) {
        capacity[u][v] = cap;
    }

    void push(int u, int v) {
        int send = min(excess[u], capacity[u][v] - flow[u][v]);
        flow[u][v] += send;
        flow[v][u] -= send;
        excess[u] -= send;
        excess[v] += send;
    }

    void relabel(int u) {
        int minHeight = INT_MAX;
        for (int v = 0; v < V; ++v) {
            if (capacity[u][v] - flow[u][v] > 0) {
                minHeight = min(minHeight, height[v]);
            }
        }
        if (minHeight < INT_MAX) {
            height[u] = minHeight + 1;
        }
    }

    void discharge(int u) { // Vrakame excess flow
        while (excess[u] > 0) {
            bool pushed = false;
            for (int v = 0; v < V; ++v) {
                if (capacity[u][v] - flow[u][v] > 0 && height[u] == height[v] + 1) {
                    push(u, v);
                    pushed = true;
                    if (excess[u] == 0) break;
                }
            }
            if (!pushed) {
                relabel(u);
            }
        }
    }

    int PR(int source, int sink) {
        height[source] = V;
        excess[source] = INT_MAX;
        for (int v = 0; v < V; ++v) {
            if (capacity[source][v] > 0) {
                push(source, v);
            }
        }

        vector<int> vertices;
        for (int i = 0; i < V; ++i) {
            if (i != source && i != sink) vertices.push_back(i);
        }

        int idx = 0;
        while (idx < vertices.size()) {
            int u = vertices[idx];  // Pocnuva od prvoto teme bez sink-ot i source-ot
            int oldHeight = height[u];  // Stara visina
            discharge(u);   // Turkame excess flow ako ima
            if (height[u] > oldHeight) {                    // Ako se smeni visinata go stavame 
                vertices.erase(vertices.begin() + idx);     // temeto prvo za da proverime dali
                vertices.insert(vertices.begin(), u);       // moze uste flow da se push-ne (idx = 0)
                idx = 0;
            } else {
                ++idx;
            }
        }

        int maxFlow = 0;
        for (int v = 0; v < V; ++v) {
            maxFlow += flow[source][v];
        }

        return maxFlow;
    }
};

int main() {
    PushRelabel graph(6);
    graph.addEdge(0, 1, 10);
    graph.addEdge(0, 2, 5);
    graph.addEdge(1, 3, 5);
    graph.addEdge(2, 4, 10);
    graph.addEdge(3, 4, 10);
    graph.addEdge(3, 5, 10);
    graph.addEdge(4, 5, 10);

    int maxFlow = graph.PR(0, 5);
    cout << "Maximum Flow: " << maxFlow << endl;

    return 0;
}
