#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;


struct Edge 
{
    int source;
    int destination;
    int weight;
};

void Filereading(const string& filename, vector<Edge>& edges)
 {
    ifstream file(filename);
    if (!file.is_open())
     {
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
            edge.destination =stoi(token);

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


bool ComparingEdges(const Edge& m, const Edge& n) 
{
    return m.weight < n.weight;
}

int finding(vector<int>& parent, int i) 
{
    if (parent[i] == -1) {
        return i;
    }
    return finding(parent, parent[i]);
}

void Union(vector<int>& parent, int x, int y)
 {
    parent[x] = y;
}

void detectCycles(const vector<Edge>& edges, int V) 
{
    vector<Edge> cycleEdges;
    vector<int> parent(V, -1);

    vector<Edge> sortedEdges = edges;
    sort(sortedEdges.begin(), sortedEdges.end(), ComparingEdges);

    cout << "Sorted Edges:"<<endl;
    for (const auto& edge : sortedEdges) 
   {
        int x = finding(parent, edge.source);
        int y = finding(parent, edge.destination);

        if (x != y) 
        {
            Union(parent, x, y);
        } else {
            cycleEdges.push_back(edge);
        }

        cout << "Edge: " << edge.source << " - " << edge.destination << " Weight: " << edge.weight << endl;
    }

    cout << "\nEdges forming cycles:"<<endl;
    for (const auto& edge : cycleEdges) 
    {
        cout << "Edge: " << edge.source << " - " << edge.destination << " Weight: " << edge.weight << endl;
    }
}

int main()
 {
    vector<Edge> edges;

    // Read data from file
    Filereading("data.csv", edges);

    int maxNode = 0;
    for (const auto& edge : edges) 
    {
        maxNode =max({maxNode, edge.source, edge.destination});
    }

    int V = maxNode + 1; 

    // Detect cycles without applying any algorithm
    detectCycles(edges, V);

    
}
