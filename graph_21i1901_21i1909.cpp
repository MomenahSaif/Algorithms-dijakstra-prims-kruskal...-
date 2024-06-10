#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <functional>
using namespace std;

struct Data 
{
    int source;
    int destination;
    int weight;
    long long timestamp;
};

void Filereading(const string& filename, vector<Data>& dataVector) 
{
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening  in the file: " << filename << endl;
        return;
    }

    string line;
    int Counter = 0;
    while (getline(file, line)) 
    {
        Counter++;
        istringstream ss(line);
        Data data;

        string token;
        //By using try and catch, the program can continue processing the remaining lines of the file even if an error occurs during the parsing of a specific line
        try 
        {
            if (getline(ss, token, ','))
                data.source = std::stoi(token);

            if (getline(ss, token, ','))
                data.destination = std::stoi(token);

            if (getline(ss, token, ','))
                data.weight = std::stoi(token);

            if (getline(ss, token, ','))
                data.timestamp = std::stoll(token);

            dataVector.push_back(data);
        } 
        catch (const exception& e) 
        {
            cout << "Error parsing line " << Counter << ": " << e.what() << endl;
        }
    }

    file.close();
}


bool ComparingTimestamp(const Data& m, const Data& n)
 {
    return m.timestamp < n.timestamp;
}

void MergeSorting(vector<Data>& dataVector) 
{
    stable_sort(dataVector.begin(), dataVector.end(), ComparingTimestamp);
}

void Filewriting(const vector<Data>& dataVector, const string& filename) 
{
    ofstream file(filename);
    if (!file.is_open()) {
       cout << "Error opening in the file: " << filename << endl;
        return;
    }

    for (const auto& data : dataVector) 
    {
        file << data.source << "," << data.destination << "," << data.weight << "," << data.timestamp << endl;
    }

    file.close();
}

void displayGraph(const std::vector<Data>& dataVector)
 {
    // Displaying graph connections
   cout << "\nGraph Representation:"<<endl;
    for (const auto& data : dataVector) 
    {
        cout << "Source: " << data.source << " -> Destination: " << data.destination << endl;
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
    vector<Data> dataVector;

    // Read data from file
    Filereading("data.csv", dataVector);

    // Sort using Merge Sort
    MergeSorting(dataVector);
    displayGraph(dataVector);

   
}
