// Bondar Ihor K-15 Variant 8
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

class Graph {
private:
    map<string, vector<pair<string, int>>> adj;
    set<string> vertices;
    vector<pair<string, string>> edges;
    map<string, bool> marked;
    map<string, int> component;

public:
    void add_edge(const string& u, const string& v) {
        edges.push_back({u, v});
        int edge_i = edges.size() - 1;

        adj[u].push_back({v, edge_i});
        if (u != v) {
            adj[v].push_back({u, edge_i});
        }

        vertices.insert(u);
        vertices.insert(v);
    }

    void dfs(const string& start, int comp_num, set<int>& comp_edges) {
        vector<string> stack;
        stack.push_back(start);
        marked[start] = true;
        component[start] = comp_num;

        while (!stack.empty()) {
            string v = stack.back();
            stack.pop_back();

            for (const auto& [neighbor, edge_i] : adj[v]) {
                comp_edges.insert(edge_i);
                if (!marked[neighbor]) {
                    marked[neighbor] = true;
                    component[neighbor] = comp_num;
                    stack.push_back(neighbor);
                }
            }
        }
    }

    double avg_edges_non_acyclic() {

        for (const auto& v : vertices) {
            marked[v] = false;
            component[v] = -1;
        }

        int comp_num = 0;
        vector<int> comp_vertices;
        vector<int> comp_edges;

        for (const auto& v : vertices) {
            if (!marked[v]) {
                set<int> edges_comp;
                dfs(v, comp_num, edges_comp);
                int vertices_count = 0;
                for (const auto& u : vertices) {
                    if (component[u] == comp_num) {
                        vertices_count++;
                    }
                }
                comp_vertices.push_back(vertices_count);
                comp_edges.push_back(edges_comp.size());
                comp_num++;
            }
        }

        int non_acyclic_count = 0;
        long long edges_count = 0;

        for (int i = 0; i < comp_num; i++) {

            if (comp_edges[i] >= comp_vertices[i]) {
                non_acyclic_count++;
                edges_count += comp_edges[i];
            }
        }

        if (non_acyclic_count == 0) {
            return 0.0;
        }

        return static_cast<double>(edges_count)/non_acyclic_count;
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Помилка: не вказано аргумент." << endl;
        return 1;
    }

    ifstream file(argv[1]);
    if (!file.is_open()) {
        cerr << "Помилка: не вдалося відкрити файл " << argv[1] << endl;
        return 1;
    }

    Graph g;
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;

        istringstream iss(line);
        string u, v;

        if (!(iss >> u >> v)) {
            cerr << "Помилка: некоректний рядок: " << line << endl;
            return 1;
        }

        string extra;
        if (iss >> extra) {
            cerr << "Помилка: зайві символи в рядку: " << line << endl;
            return 1;
        }
        g.add_edge(u, v);
    }

    file.close();

    double result = g.avg_edges_non_acyclic();
    cout << endl;
    cout << fixed << setprecision(2) << result;

    return 0;
}