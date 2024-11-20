/*
1 - Ford-Fulkerson and Edmonds-Karp
2 - Dinic
3 - S-T cut / Min-Cut
4 - Push-Relabel
 */
#include <bits/stdc++.h>
using namespace std;

class Graph {
protected:
    int vertices;
    bool directional;
    bool weighted;
    int defEdgeWeight;
public:
    int BFS(int source, int sink, vector<int> &parent, vector<vector<int>> adj, vector<vector<int>> capacity) {
        fill(parent.begin(), parent.end(), -1);
        parent[source] = -2;
        queue<pair<int, int>> q;
        q.push({source, __INT_MAX__});

        while (!q.empty()) {
            int cur = q.front().first;
            int flow = q.front().second;
            q.pop();

            for (int next : adj[cur]) {
                if (parent[next] == -1 && capacity[cur][next] != 0) {
                    parent[next] = cur;
                    int newFlow = min(flow, capacity[cur][next]);
                    if (next == sink) {
                        return newFlow;
                    }
                    q.push({next, newFlow});
                }
            }
        }
        return 0;
    }

    int EdmondsKarpAlgorithm(int source, int sink, vector<vector<int>> &adj, vector<vector<int>> &capacity) {
        int maxFlow = 0, flow;
        vector<int> parent(vertices, -1);

        while (flow = BFS(source, sink, parent, adj, capacity)) {
            maxFlow += flow;
            int cur = sink;
            while (cur != source) {
                int prev = parent[cur];
                capacity[prev][cur] -= flow;
                capacity[cur][prev] += flow;
                cur = prev;
            }
        }
        return maxFlow;
    }

    int DFS(int source, int sink, vector<int> &parent, vector<vector<int>> adj, vector<vector<int>> capacity) {
        fill(parent.begin(), parent.end(), -1);
        parent[source] = -2;
        stack<pair<int, int>> q;
        q.push({source, INT_MAX});

        while (!q.empty()) {
            int cur = q.top().first;
            int flow = q.top().second;
            q.pop();

            for (int next : adj[cur]) {
                if (parent[next] == -1 && capacity[cur][next] != 0) {
                    parent[next] = cur;
                    int newFlow = min(flow, capacity[cur][next]);
                    if (next == sink) {
                        return newFlow;
                    }
                    q.push({next, newFlow});
                }
            }
        }
        return 0;
    }

    int FordFulkersonAlgorithm(int source, int sink, vector<vector<int>> &adj, vector<vector<int>> &capacity) {
        int maxFlow = 0, flow;
        vector<int> parent(vertices, -1);

        while (flow = DFS(source, sink, parent, adj, capacity)) {
            maxFlow += flow;
            int cur = sink;
            while (cur != source)
            {
                int prev = parent[cur];
                capacity[prev][cur] -= flow;
                capacity[cur][prev] += flow;
                cur = prev;
            }
        }
        return maxFlow;
    }

    bool Levels(int source, int sink, vector<int> &level, vector<vector<int>> adj, vector<vector<int>> capacity) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        q.push(source);
        level[source] = 0;

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            for (int next : adj[current]) {
                if (level[next] < 0 && capacity[current][next] > 0) {
                    level[next] = level[current] + 1;
                    q.push(next);
                }
            }
        }
        return level[sink] >= 0;
    }

    int DFSDinic(int sink, int cur, int flow, vector<int> &level, vector<int> &start, vector<vector<int>> adj, vector<vector<int>> &capacity) {
        if (cur == sink) {
            return flow;
        }
        for (; start[cur] < adj[cur].size(); start[cur]++) {
            int next = adj[cur][start[cur]];
            if (level[next] == level[cur] + 1 && capacity[cur][next] > 0) {
                int curFlow = min(flow, capacity[cur][next]);
                int tempFlow = DFSDinic(sink, next, curFlow, level, start, adj, capacity);
                if (tempFlow > 0) {
                    capacity[cur][next] -= tempFlow;
                    capacity[next][cur] += tempFlow;
                    return tempFlow;
                }
            }
        }
        return 0;
    }

    int DinicAlgorithm(int source, int sink, vector<vector<int>> adj, vector<vector<int>> &capacity) {
        int maxFlow = 0;
        vector<int> level(vertices);
        vector<int> start(vertices);

        while (Levels(source, sink, level, adj, capacity)) {
            fill(start.begin(), start.end(), 0);

            while (int flow = DFSDinic(sink, source, INT_MAX, level, start, adj, capacity)) {
                maxFlow += flow;
            }
        }
        return maxFlow;
    }

    vector<pair<int, int>> MinCutAlgorithm(int source, int sink, vector<vector<int>> &adj, vector<vector<int>> &capacity) {
        DinicAlgorithm(source, sink, adj, capacity);
        vector<int> level(vertices, -1);
        Levels(source, sink, level, adj, capacity);
        vector<pair<int, int>> cutEdges;
        for (int v = 0; v < vertices; v++) {
            if (level[v] != -1) {
                for (int u : adj[v]) {
                    if (level[u] == -1) {
                        cutEdges.push_back({ v, u });
                        cout << v << " -> " << u << ", ";
                    }
                }
            }
        }
        return cutEdges;
    }

    void Push(int u, int v, vector<int> &excess, queue<int> &excess_vertices, vector<vector<int>> &capacity, vector<vector<int>> &flow) {
    int d = min(excess[u], capacity[u][v] - flow[u][v]);
    flow[u][v] += d;
    flow[v][u] -= d;
    excess[u] -= d;
    excess[v] += d;
    if (d && excess[v] == d)
        excess_vertices.push(v);
    }

    void Relabel(int u, vector<int> &height, vector<vector<int>> &capacity, vector<vector<int>> &flow) {
        int d = INT_MAX;
        for (int i = 0; i < vertices; i++) {
            if (capacity[u][i] - flow[u][i] > 0) {
                d = min(d, height[i]);
            }
        }
        if (d < INT_MAX) {
            height[u] = d + 1;
        }
    }

    void Discharge(int u, vector<int> &excess, vector<int> &seen, vector<int> &height, queue<int> &excess_vertices, vector<vector<int>> &capacity, vector<vector<int>> &flow) {
        while (excess[u] > 0) {
            if (seen[u] < vertices) {
                int v = seen[u];
                if (capacity[u][v] - flow[u][v] > 0 && height[u] > height[v]) {
                    Push(u, v, excess, excess_vertices, capacity, flow);
                } else {
                    seen[u]++;
                }
            } else {
                Relabel(u, height, capacity, flow);
                seen[u] = 0;
            }
        }
    }

    int PushRelabelAlgorithm(int source, int sink, vector<vector<int>> &capacity) {
        vector<vector<int>> flow;
        vector<int> height, excess, seen;
        queue<int> excess_vertices;

        height.assign(vertices, 0);
        height[source] = vertices;
        flow.assign(vertices, vector<int>(vertices, 0));
        excess.assign(vertices, 0);
        excess[source] = INT_MAX;
        for (int i = 0; i < vertices; i++) {
            if (i != source) {
                Push(source, i, excess, excess_vertices, capacity, flow);
            }
        }
        seen.assign(vertices, 0);

        while (!excess_vertices.empty()) {
            int u = excess_vertices.front();
            excess_vertices.pop();
            if (u != source && u != sink) {
                Discharge(u, excess, seen, height, excess_vertices, capacity, flow);
            }
        }

        int maxFlow = 0;
        for (int i = 0; i < vertices; i++) {
            maxFlow += flow[i][sink];
        }
        return maxFlow;
    }

public:

    virtual int EdmondsKarp(int, int) = 0;
    virtual int FordFulkerson(int, int) = 0;
    virtual int Dinic(int, int) = 0;
    virtual vector<pair<int, int>> MinCut(int, int) = 0;
    virtual int PushRelabel(int, int) = 0;

    virtual void addEdge(int From, int To) = 0;
    virtual void addEdgeWeight(int From, int To, int weight) = 0;
    virtual void add_vertex() = 0;

    virtual void print() = 0;
    Graph(int vertices = 0, bool directional = true, bool weighted = false, int defEdgeWeight = 1) :vertices(vertices), directional(directional), weighted(weighted), defEdgeWeight(defEdgeWeight) {};

};

class MatrixGraph : public Graph {
public:

    vector<vector<int>> matrix;
    vector<vector<int>> adjList;

    MatrixGraph(int vertices = 0, bool directional = true, bool weighted = false, int defEdgeWeight = 1) : Graph(vertices, directional, weighted, defEdgeWeight), matrix(vertices, vector<int>(vertices, 0)), adjList(vertices) {}

    void addEdgeWeight(int From, int To, int weight) {
        matrix[From][To] = weight;
        adjList[From].push_back(To);

        if (!directional) {
            matrix[To][From] = weight;
            adjList[To].push_back(From);
        }
    }
    void addEdge(int From, int To) { addEdgeWeight(From, To, defEdgeWeight); }
    void add_vertex() {
        vertices++;
        for (vector<int> & vec : matrix) {
            vec.push_back(0);
        }
        matrix.push_back(vector<int>(vertices, 0));
        adjList.push_back(vector<int>());
    }
    void print() {
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                cout << matrix[i][j] << " ";
            }
            cout << endl;
        }
        for (int i = 0; i < vertices; i++) {
            for (int el : adjList[i]) {
                cout << el << " ";
            }
            cout << endl;
        }
    }
public:
    int EdmondsKarp(int s = 0, int t = -1) {
        if (t == -1) { t = vertices - 1; }
        vector<vector<int>> capacity(matrix);
        return EdmondsKarpAlgorithm(s, t, adjList, capacity);
    }

    int FordFulkerson(int s = 0, int t = -1) {
        if (t == -1) { t = vertices - 1; }
        vector<vector<int>> capacity(matrix);
        return FordFulkersonAlgorithm(s, t, adjList, capacity);
    }

    int Dinic(int s = 0, int t = -1) {
        if (t == -1) { t = vertices - 1; }
        vector<vector<int>> capacity(matrix);
        return DinicAlgorithm(s, t, adjList, capacity);
    }

    vector<pair<int, int>> MinCut(int s = 0, int t = -1) {
        if (t == -1) { t = vertices - 1; }
        vector<vector<int>> capacity(matrix);
        return MinCutAlgorithm(s, t, adjList, capacity);
    }
    int PushRelabel(int s = 0, int t = -1) {
        if (t == -1) { t = vertices - 1; }
        vector<vector<int>> capacity(matrix);
        return PushRelabelAlgorithm(s, t, capacity);
    }
};

class ListGraph : public Graph {
public:

    vector<vector<pair<int, int>>> adjList;

    ListGraph(int vertices = 0, bool directional = true, bool weighted = false, int default_edge_weight = 1) : Graph(vertices, directional, weighted, default_edge_weight), adjList(vertices) {}

    void addEdgeWeight(int From, int To, int weight) {
        adjList[From].push_back({ To, weight });
        if (!directional) {
            adjList[To].push_back({ From,weight });
        }
    }
    void addEdge(int From, int To) { addEdgeWeight(From, To, defEdgeWeight); }
    void add_vertex() {
        vertices++;
        adjList.push_back(vector<pair<int, int>>());
    }
    void print() {
        for (int i = 0; i < vertices; i++) {
            for (pair<int, int> el : adjList[i]) {
                cout << el.first << " " << el.second;
            }
            cout << endl;
        }
    }
private:
    vector<vector<int>> capacity;
    vector<vector<int>> temp_adj;
    void prepare_capacity() {
        capacity = vector<vector<int>>(vertices, vector<int>(vertices, 0));
        for (int i = 0; i < vertices; i++)
            for (pair<int, int> el : adjList[i])
                capacity[i][el.first] = el.second;
    }
    void prepare_temp_adj() {
        temp_adj = vector<vector<int>>(vertices);
        for (int i = 0; i < vertices; i++) {
            for (auto pr : adjList[i]) {
                temp_adj[i].push_back(pr.first);
            }
        }
    }
public:
    int EdmondsKarp(int s = 0, int t = -1) {
        if (t == -1) { t = vertices - 1; }
        prepare_capacity();
        prepare_temp_adj();
        return EdmondsKarpAlgorithm(s, t, temp_adj, capacity);
    }
    int FordFulkerson(int s = 0, int t = -1) {
        if (t == -1) { t = vertices - 1; }
        prepare_capacity();
        prepare_temp_adj();
        return FordFulkersonAlgorithm(s, t, temp_adj, capacity);
    }
    int Dinic(int s = 0, int t = -1) {
        if (t == -1) { t = vertices - 1; }
        prepare_capacity();
        prepare_temp_adj();
        return DinicAlgorithm(s, t, temp_adj, capacity);
    }
    vector<pair<int, int>> MinCut(int s = 0, int t = -1) {
        if (t == -1) { t = vertices - 1; }

        prepare_capacity();
        prepare_temp_adj();

        return MinCutAlgorithm(s, t, temp_adj, capacity);
    }
    int PushRelabel(int s = 0, int t = -1) {
        if (t == -1) { t = vertices - 1; }
        prepare_capacity();
        return PushRelabelAlgorithm(s, t, capacity);
    }
};

int main() {

    int n, m; cin >> n >> m;

    MatrixGraph g1(n);
    ListGraph g2(n);
    for (int i = 0; i < m; i++) {
        int f, t, w;
        cin >> f >> t >> w;
        g1.addEdgeWeight(f, t, w);
        g2.addEdgeWeight(f, t, w);
    }

    cout << "Using Matrix:" << endl;
    cout << "Max flow with Edmonds-Karp Algorithm: " << g1.EdmondsKarp() << endl;
    cout << "Max flow with Ford-Fulkerson Algorithm: " << g1.FordFulkerson() << endl;
    cout << "Max flow with Dinic Algorithm: " << g1.Dinic() << endl;
    cout << "Max flow with Push-Relabel Algorithm: " << g1.PushRelabel() << endl;
    cout << "The cut edges using Min-Cut Algorithm: " ; g1.MinCut(); cout << endl;

    cout << "\nUsing List:" << endl;
    cout << "Max flow with Edmonds-Karp Algorithm: " << g2.EdmondsKarp() << endl;
    cout << "Max flow with Ford-Fulkerson Algorithm: " << g2.FordFulkerson() << endl;
    cout << "Max flow with Dinic Algorithm: " << g2.Dinic() << endl;
    cout << "Max flow with Push-Relabel Algorithm: " << g2.PushRelabel() << endl;
    cout << "The cut edges using Min-Cut Algorithm: " ; g2.MinCut(); cout << endl;

    return 0;
}