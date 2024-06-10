#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <queue>
#include <sstream>
#include <chrono>
#include <functional>
#include <unordered_map>
#include <unordered_set>
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
    int V; // No of vertices
};

void readSelectedNodes(const string& filename, vector<Edge>& edges, unordered_set<int>& nodes) 
{
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening in the file: " << filename <<endl;
        return;
    }

    string line;
    while (std::getline(file, line)) 
    {
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

            edges.push_back(edge);
            nodes.insert(edge.source);
            nodes.insert(edge.destination);
        } 
        catch (const invalid_argument& e)
         {
            cout << "Error converting to integer: " << e.what() << std::endl;
            // Skip this line and proceed to the next one
            continue;  
        } 
        catch (const out_of_range& e) 
        {
            cout << "Out of range: " << e.what() << endl;
            continue;  
        }
    }

    file.close();
}

void AddingEdge(Graph& graph, const Edge& edge) 
{
    graph.adjList[edge.source].emplace_back(edge.destination, edge.weight);
}

void Dijkstra(const Graph& graph, int source, vector<int>& distances, vector<std::vector<string>>& trace, const unordered_set<int>& nodes) 
{
    int V = graph.V;
    distances.assign(V, numeric_limits<int>::max());
    distances[source] = 0;

    priority_queue<pair<int, int>, vector<std::pair<int, int>>, greater<std::pair<int, int>>> pq;
    pq.push(make_pair(0, source));
     // Track trace for nodes
    unordered_map<int, vector<string>> nodeTrace; 

    while (!pq.empty()) 
    {
        int u = pq.top().second;
        int dist_u = pq.top().first;
        pq.pop();

        if (distances[u] < dist_u) 
        {
            continue;
        }

        for (const auto& neighbor : graph.adjList[u]) 
        {
            int v = neighbor.first;
            int weight = neighbor.second;

            if (distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
                pq.push(make_pair(distances[v], v));

                stringstream traceString;
                traceString << "Node " << u << " -> Node " << v << " - Distance: " << distances[v];

                // Update trace for node v
                nodeTrace[v] = nodeTrace[u];
                nodeTrace[v].push_back(traceString.str());
            }
        }
    }

    // Copy traces only for nodes present in the given set of nodes
    for (int node : nodes) 
    {
        if (nodeTrace.find(node) != nodeTrace.end()) 
        {
            trace[node] = nodeTrace[node];
        }
    }
}

void WritingResults(const vector<int>& distances, const vector<vector<string>>& trace, const string& filename, const unordered_set<int>& nodes) 
{
    ofstream file(filename);
    if (!file.is_open()) 
    {
        cout << "Error opening the file: " << filename <<endl;
        return;
    }

    for (int distance : distances) 
    {
        file << distance << endl;
    }

    file << "\nTrace of Dijkstra's Algorithm:"<<endl;
    for (int node : nodes) 
    {
        if (!trace[node].empty()) {
            file << "Node " << node << " Trace:"<<endl;
            for (const std::string& traceInfo : trace[node]) 
            {
                file << traceInfo << endl;
            }
        }
    }

    file.close();
}

void DisplayShortestPaths(const vector<int>& distances, int source, const unordered_set<int>& nodes) 
{
    cout << "\nShortest paths from Source " << source << endl;
    for (int node : nodes) 
    {
        if (distances[node] != numeric_limits<int>::max()) 
        {
            cout << "Node " << node << ": Distance = " << distances[node] <<endl;
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
    unordered_set<int> nodes;

    // Read and process only the given 8 nodes and their edges
    readSelectedNodes("data.csv", edges, nodes);

    int maxNode = 0;
    for (const auto& edge : edges) 
    {
        maxNode = max({maxNode, edge.source, edge.destination});
    }

    Graph graph;
    // Assuming nodes are labeled from 0 to maxNode
    graph.V = maxNode + 1; 
    graph.adjList.resize(graph.V);

    for (const auto& edge : edges) 
    {
        AddingEdge(graph, edge);
    }

    vector<int> dijkstraDistances(graph.V, numeric_limits<int>::max());
    vector<vector<string>> trace(graph.V);

    // Measure time and apply Dijkstra's algorithm for the given nodes
    double dijkstraTime = Timemeasurement([&]() 
    {
        Dijkstra(graph, 0, dijkstraDistances, trace, nodes);
    });
    cout << "Dijkstra's algorithm execution time: " << dijkstraTime << " seconds"<<endl;

    // Display and store Dijkstra's shortest path results 
    DisplayShortestPaths(dijkstraDistances, 0, nodes);
    WritingResults(dijkstraDistances, trace, "dijkstra_shortest_distances_with_trace.csv", nodes);

    
}
