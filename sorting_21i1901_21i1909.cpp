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
        cout << "Error  in opening the file: " << filename << endl;
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
        try 
        {
            if (getline(ss, token, ','))
                data.source = stoi(token);

            if (getline(ss, token, ','))
                data.destination = stoi(token);

            if (getline(ss, token, ','))
                data.weight = stoi(token);

            if (getline(ss, token, ','))
                data.timestamp = stoll(token);

            dataVector.push_back(data);
        } 
        
        catch (const exception& e) 
        {
            cerr << "Error line " << Counter << ": " << e.what() << endl;
        }
    }

    file.close();
}



//This function is a comparator used for sorting ,it compares two Data structures based on their timestamp field.
bool ComparingTimestamp(const Data& m , const Data& n) 
{
    return m.timestamp < n.timestamp;
}

void QuickSorting(vector<Data>& dataVector) 
{
    sort(dataVector.begin(), dataVector.end(), ComparingTimestamp);
}

void MergeSorting(vector<Data>& dataVector) 
{
    stable_sort(dataVector.begin(), dataVector.end(), ComparingTimestamp);
}

void heapify(vector<Data>& dataVector, int n, int i) 
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && ComparingTimestamp(dataVector[left], dataVector[largest]))
        largest = left;

    if (right < n && ComparingTimestamp(dataVector[right], dataVector[largest]))
        largest = right;

    if (largest != i) 
    {
        swap(dataVector[i], dataVector[largest]);
        heapify(dataVector, n, largest);
    }
}

void HeapSorting(vector<Data>& dataVector)
 {
    int n = dataVector.size();

    for (int i = n / 2 - 1; i >= 0; --i)
        heapify(dataVector, n, i);

    for (int i = n - 1; i > 0; --i) {
        swap(dataVector[0], dataVector[i]);
        heapify(dataVector, i, 0);
    }
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

void displayGraph(const vector<Data>& dataVector) 
{
    cout << "Source | Destination | Weight | Timestamp\n";
    for (const auto& data : dataVector) 
    {
        cout << setw(7) << data.source << " | "
                  << setw(11) << data.destination << " | "
                  << setw(6) << data.weight << " | "
                  << setw(10) << data.timestamp << "\n";
    }
}

double Timemeasurement(function<void()> function)
 {
    auto start =chrono::high_resolution_clock::now();
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

    // Sort using Quick Sort
    vector<Data> quickSorted = dataVector;
    double quickSortTime = Timemeasurement([&quickSorted]() 
    {
        QuickSorting(quickSorted);
    });
    cout << "Quick Sort Time: " << quickSortTime<< " seconds\n";

    // Display graph for Quick Sort
    cout << "\nSorted Data using Quick Sort:"<<endl;
    displayGraph(quickSorted);

    // Store Quick Sort result to file
    Filewriting(quickSorted, "quick_sorted.csv");

    // Sort using Merge Sort
    vector<Data> mergeSorted = dataVector;
    double mergeSortTime = Timemeasurement([&mergeSorted]() 
    {
        MergeSorting(mergeSorted);
    });
    cout << "\nMerge Sort Time: " << mergeSortTime << " seconds"<<endl;

    // Display graph for Merge Sort
    cout << "\nSorted Data using Merge Sort:"<<endl;;
    displayGraph(mergeSorted);

    // Store Merge Sort result to file
    Filewriting(mergeSorted, "merge_sorted.csv");

    // Sort using Heap Sort
    vector<Data> heapSorted = dataVector;
    double heapSortTime = Timemeasurement([&heapSorted]() 
    {
        HeapSorting(heapSorted);
    });
   cout << "\nHeap Sort Time: " << heapSortTime << " seconds"<<endl;

    // Display graph for Heap Sort
    cout << "\nSorted Data using Heap Sort:"<<endl;
    displayGraph(heapSorted);

    // Store Heap Sort result to file
    Filewriting(heapSorted, "heap_sorted.csv");

    
}
