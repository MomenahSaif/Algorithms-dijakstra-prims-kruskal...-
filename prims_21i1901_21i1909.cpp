#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <queue>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <functional>
using namespace std;


struct Edge
 {
    int source;
    int destination;
    int weight;
};

struct Graph
 {
    vector<vector<pair<int, int>>> adjList;
    int V; // Number of vertices
};

void Filereading(const string& filename, vector<Edge>& edges) 
{
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening in  the file: " << filename << endl;
        return;
    }

    string line;
    int lineNumber = 0;

    while (getline(file, line)) 
    {
        lineNumber++;

        istringstream ss(line);
        Edge edge;

        try 
        {
            string token;
            getline(ss, token, ',');
            edge.source = stoi(token);

            getline(ss, token, ',');
            edge.destination =stoi(token);

            getline(ss, token, ',');
            edge.weight = stoi(token);
        }
        
         catch (const invalid_argument& e) 
         {
            cout << "Error converting to integer in line " << lineNumber << ": " << e.what() << endl;
            continue;  
        } 
        catch (const std::out_of_range& e) 
        {
            cout << "Out of range in line " << lineNumber << ": " << e.what() << endl;
            continue;  
        }

        edges.push_back(edge);
    }

    file.close();
}

void AddingEdge(Graph& graph, const Edge& edge) 
{
    graph.adjList[edge.source].emplace_back(edge.destination, edge.weight);
    graph.adjList[edge.destination].emplace_back(edge.source, edge.weight);
}

void primMST(const Graph& graph, vector<int>& parent, vector<string>& trace) 
{
    int V = graph.V;
    vector<int> key(V, numeric_limits<int>::max());
    vector<bool> mstSet(V, false);

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    key[0] = 0;
    parent[0] = -1;
    pq.push(make_pair(0, 0));

    while (!pq.empty()) 
    {
        int u = pq.top().second;
        pq.pop();

        mstSet[u] = true;

        stringstream ss;
        ss << "Node insertion: " << u << " (Key Value: " << key[u] << ")"<<endl;
        trace.push_back(ss.str());

        for (const auto& neighbor : graph.adjList[u]) 
        {
            int v = neighbor.first;
            int weight = neighbor.second;

            if (!mstSet[v] && weight < key[v]) {
                parent[v] = u;
                key[v] = weight;
                pq.push(make_pair(key[v], v));

                stringstream ss;
                ss << "Edge insertion: " << u << " - " << v << " (Weight: " << weight << ", Updated Key Value: " << key[v] << ")"<<endl;
                trace.push_back(ss.str());
            }
        }
    }
}

void WritingResults(const vector<int>& parent, const vector<string>& trace, const string& filename) 
{
    ofstream file(filename);
    if (!file.is_open())
     {
        std::cerr << "Error opening  in the file: " << filename << endl;
        return;
    }

    file << "Minimum Spanning Tree:"<<endl;
    for (size_t i = 1; i < parent.size(); ++i) 
    {
        if (parent[i] != -1) {
            file << "Edge: " << parent[i] << " - " << i << endl;
        }
    }

    file << "\nTrace of Prim's Algorithm:"<<endl;
    for (const auto& line : trace) {
        file << line;
    }

    file.close();
}

void displayMST(const std::vector<int>& parent) 
{
    cout << "\nMinimum Spanning Tree:"<<endl;
    for (size_t i = 1; i < parent.size(); ++i) 
    {
        if (parent[i] != -1) 
        {
            cout << "Edge: " << parent[i] << " - " << i << endl;
        }
    }
}

double Timemeasurement(function<void()> function) 
{
    auto start = chrono::high_resolution_clock::now();
    function();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    return duration.count();
}

int main() 
{
    vector<Edge> edges;

    // Read data from file
    Filereading("data.csv", edges);

    int maxNode = 0;
    for (const auto& edge : edges)
     {
        maxNode = max({maxNode, edge.source, edge.destination});
    }

    Graph graph;
    //  nodes are labeled from 0 to maxNode
    graph.V = maxNode + 1; 
    graph.adjList.resize(graph.V);

    for (const auto& edge : edges) 
    {
        AddingEdge(graph, edge);
    }

    vector<int> parent(graph.V, -1);
    vector<string> trace;

    // Measure time and apply Prim's algorithm
    double primTime = Timemeasurement([&]() 
    {
        primMST(graph, parent, trace);
    });
    cout << "Prim's algorithm execution time: " << primTime << " seconds"<<endl;

    // Display and store Prim's MST results
    displayMST(parent);
    WritingResults(parent, trace, "prim_minimum_spanning_tree_with_trace.csv");

   
}
