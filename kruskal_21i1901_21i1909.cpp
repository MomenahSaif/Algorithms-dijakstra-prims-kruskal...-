#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <functional>
#include <stack>
using namespace std;

struct Edge 
{
    int source;
    int destination;
    int weight;
};

struct DisjointSets 
{
    vector<int> parent, rank;
};

void Filereading(const string& filename, vector<Edge>& edges) 
{
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening  in the file: " << filename << endl;
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
            edge.destination = stoi(token);

            getline(ss, token, ',');
            edge.weight = stoi(token);
        } 
        catch (const invalid_argument& e)
         {
            cout << "Error converting to integer in line " << lineNumber << ": " << e.what() << endl;
            continue;  // Skip this line and proceed to the next one
        } 
        catch (const out_of_range& e)
         {
            cout << "Out of range in line " << lineNumber << ": " << e.what() << endl;
            continue;  
        }

        edges.push_back(edge);
    }

    file.close();
}

bool ComparingEdges(const Edge& m, const Edge& n) 
{
    return m.weight < n.weight;
}

int finding(DisjointSets& ds, int i)
 {
    if (ds.parent[i] != i) {
        ds.parent[i] = finding(ds, ds.parent[i]);
    }
    return ds.parent[i];
}

void Union(DisjointSets& ds, int x, int y) 
{
    int xRoot = finding(ds, x);
    int yRoot = finding(ds, y);

    if (ds.rank[xRoot] < ds.rank[yRoot]) 
    {
        ds.parent[xRoot] = yRoot;
    } 
    else if (ds.rank[xRoot] > ds.rank[yRoot]) 
    {
        ds.parent[yRoot] = xRoot;
    } 
    else 
    {
        ds.parent[yRoot] = xRoot;
        ds.rank[xRoot]++;
    }
}

void kruskalMST(const vector<Edge>& edges, int V) 
{
    vector<Edge> result;
    DisjointSets ds;
    ds.parent.resize(V);
    ds.rank.resize(V, 0);

    for (int i = 0; i < V; ++i) 
    {
        ds.parent[i] = i;
    }

    vector<Edge> sortedEdges = edges;
    sort(sortedEdges.begin(), sortedEdges.end(), ComparingEdges);

    stack<string> traceStack;

    for (const auto& edge : sortedEdges) 
    {
        int x = finding(ds, edge.source);
        int y = finding(ds, edge.destination);

       stringstream trace;
        trace << "Edge: " << edge.source << " - " << edge.destination << " Weight: " << edge.weight;

        if (x != y) 
        {
            result.push_back(edge);
            Union(ds, x, y);
            traceStack.push(trace.str() + " - Accepted");
        }
         else
          {
            traceStack.push(trace.str() + " - Rejected");
        }
    }

    ofstream outputFile("kruskal_output.csv");
    if (!outputFile.is_open())
     {
        cout << "Error opening  in the output file!" << endl;
        return;
    }

    outputFile << "Minimum Spanning Tree:"<<endl;
    for (const auto& edge : result) 
    {
        outputFile << "Edge: " << edge.source << " - " << edge.destination << " Weight: " << edge.weight << endl;
    }

    outputFile << "\nTrace of Kruskal's Algorithm:"<<endl;
    while (!traceStack.empty()) 
    {
        outputFile << traceStack.top() << endl;
        traceStack.pop();
    }

    outputFile.close();

    cout << "\nMinimum Spanning Tree:"<<endl;
    for (const auto& edge : result) 
    {
        cout << "Edge: " << edge.source << " - " << edge.destination << " Weight: " << edge.weight << endl;
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
    for (const auto& edge : edges) {
        maxNode = max({maxNode, edge.source, edge.destination});
    }

    int V = maxNode + 1; 

    // Measure time and apply Kruskal's algorithm
    double kruskalTime = Timemeasurement([&]() 
    {
        kruskalMST(edges, V);
    });
    cout << "Kruskal's algorithm execution time: " << kruskalTime << " seconds"<<endl;

   
}
