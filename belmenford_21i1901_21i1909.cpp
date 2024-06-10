#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <queue>
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
    vector<Edge> edges;
    int V; 
};

void Filereading(const string& filename,vector<Edge>& edges) 
{
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening in the file: " << filename << endl;
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
            continue;  
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

void bellmanFord(const Graph& graph, int source, vector<int>& distances, vector<std::queue<string>>& trace) 
{
    int V = graph.V;
    distances.assign(V, numeric_limits<int>::max());
    distances[source] = 0;

    for (int i = 0; i < V - 1; ++i) 
    {
        for (const auto& edge : graph.edges) 
        {
            int u = edge.source;
            int v = edge.destination;
            int weight = edge.weight;

            if (distances[u] != numeric_limits<int>::max() && distances[u] + weight < distances[v]) 
            {
                distances[v] = distances[u] + weight;

                stringstream traceString;
                traceString << "Node " << u << " -> Node " << v << " - Distance: " << distances[v];
                trace[v] = trace[u];
                trace[v].push(traceString.str());
            }
        }
    }
}

void WritingResults(const vector<int>& distances, const vector<queue<string>>& trace, const string& filename) 
{
    ofstream file(filename);
    if (!file.is_open()) 
    {
        cout << "Error opening in the file: " << filename << endl;
        return;
    }

    for (int distance : distances) 
    {
        file << distance <<endl;
    }

    file << "\nTrace of Bellman-Ford Algorithm:"<<endl;
    for (size_t i = 0; i < trace.size(); ++i)
     {
        file << "Node " << i << " Trace:"<<endl;
        queue<string> temp = trace[i];
        while (!temp.empty()) 
        {
            file << temp.front() << endl;
            temp.pop();
        }
    }

    file.close();
}

void displayShortestPaths(const vector<int>& distances, int source) 
{
    cout << "\nShortest paths from Source " << source << endl;
    for (size_t i = 0; i < distances.size(); ++i) {
        cout << "Node " << i << ": Distance = " << distances[i] <<endl;
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
        maxNode = std::max({maxNode, edge.source, edge.destination});
    }

    Graph graph;
    graph.V = maxNode + 1; 
    graph.edges = edges;

    vector<int> bellmanFordDistances;
    vector<queue<string>> trace(graph.V);

    // Measure time and apply Bellman-Ford algorith
    double bellmanFordTime = Timemeasurement([&]() 
    {
        bellmanFord(graph, 0, bellmanFordDistances, trace);
    });
    cout << "Bellman-Ford algorithm execution time: " << bellmanFordTime << " seconds"<<endl;

    // Display and store Bellman-Ford shortest path results
    displayShortestPaths(bellmanFordDistances, 0);
    WritingResults(bellmanFordDistances, trace, "bellmanford_shortest_distances_with_trace.csv");

    
}
