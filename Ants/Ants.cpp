#include <bits/stdc++.h>
using namespace std;

class Graph {
private:
    int V;
    vector<vector<int>> adj;
    vector<vector<int>> capacity;

    // Recursive function to find all paths from source to sink
    bool findPathUtil(int u, int t, vector<bool>& visited, vector<int>& path) {
        visited[u] = true;
        path.push_back(u);

        // If we reached the sink
        if (u == t) {
            return true;
        }

        // Explore neighbors
        for (int v : adj[u]) {
            if (!visited[v] && capacity[u][v] > 0) { // Only consider available edges
                if (findPathUtil(v, t, visited, path)) {
                    return true;
                }
            }
        }

        // Backtrack
        visited[u] = false;
        path.pop_back();
        return false;
    }

    // Function to remove edges after a path is found
    void removeEdges(const vector<int>& path) {
        for (size_t i = 0; i < path.size() - 1; ++i) {
            int u = path[i];
            int v = path[i + 1];
            capacity[u][v] = 0; // Remove capacity
            capacity[v][u] = 0; // For undirected graph
        }
    }

public:
    Graph(int vertices) : V(vertices) {
        adj.resize(V);
        capacity.resize(V, vector<int>(V, 0));
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
        capacity[u][v] = 1;
        capacity[v][u] = 1;
    }

    // Find all paths from source to sink, remove edges after each found path
    void findAndRemovePaths(int s, int t, int ants) {
        vector<vector<int>> allPaths; // To store all found paths

        while (true) {
            vector<bool> visited(V, false);
            vector<int> path;

            // Find a path
            if (!findPathUtil(s, t, visited, path)) {
                break; // No more paths available
            }

            allPaths.push_back(path);

            removeEdges(path);
        }

        sort(allPaths.begin(), allPaths.end(), [](const vector<int>& a, const vector<int>& b) {
            return a.size() < b.size();
        });
        int numSteps = 0, sumSteps = 0;
        cout << "Found paths (from shortest to longest):\n";
        for (const auto& p : allPaths) {
            for (int node : p) {
                cout << node << " ";
                numSteps += 1;
            }
            cout << "\nNumber of steps from source to sink: " << numSteps-1 << endl;
            sumSteps += numSteps-2;
            numSteps = 0;
        }
/*
        for (const auto i : allPaths.size()) {
            k = 
        }
*/
        float k = sumSteps+ants;
        k = k / allPaths.size();
        int n = (sumSteps+ants) / allPaths.size();
        if (k != n) {
            cout << "Number of iterations: " << n+1 <<endl;
        }

    }
};

int main() {
    
    int ants; cin>>ants;

    Graph g(21);

    g.addEdge(0, 21);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 5);
    g.addEdge(5, 6);
    g.addEdge(6, 7);
    g.addEdge(7, 8);
    g.addEdge(8, 9);
    g.addEdge(9, 10);
    g.addEdge(10, 11);
    g.addEdge(11, 12);
    g.addEdge(12, 13);
    g.addEdge(13, 14);
    g.addEdge(14, 15);
    g.addEdge(15, 16);
    g.addEdge(16, 17);
    g.addEdge(17, 18);
    g.addEdge(18, 19);
    g.addEdge(19, 20);
    g.addEdge(20, 21);

    g.findAndRemovePaths(0, 5, ants);

    return 0;
}
