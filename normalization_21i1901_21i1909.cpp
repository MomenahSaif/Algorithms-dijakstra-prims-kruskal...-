#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <functional>
using namespace std;

//This representing different columns of data
struct Data
 {
    int col1;
    int col2;
    int col3;
    long long col4;
};


//This function reads a file and processes its contents it takes a filename and a reference to a vector of Data as parameters.
void Filereading(const string& filename, vector<Data>& dataVector) 
{
      //Attempts to open the file using an input filestream 
      ifstream file(filename);
    if (!file.is_open())
     {
         cout << "Error opening the file: " << filename << endl;
        return;
    }
      //Reads each line from the file, extracts four values using sscanf, creates a Data structure, and adds it to the vector.
   string line;
    while (getline(file, line))
     {
        Data data;
        sscanf(line.c_str(), "%d\t%d\t%d\t%lld", &data.col1, &data.col2, &data.col3, &data.col4);
        dataVector.push_back(data);
    }

    file.close();
}

//This function measures the execution time of a given function using the <chrono> library
void print(const vector<Data>& dataVector) 
{
    for (const auto& data : dataVector) 
    {
        cout << data.col1 << "\t" << data.col2 << "\t" << data.col3 << "\t" << data.col4 << endl;
    }
}

double ExecutionTimeCalculation(function<void()> function) 
{
    auto start =chrono::high_resolution_clock::now();
    function();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    return duration.count();
}

int main() 
{
    //vector of Data named dataVector is declared.
    vector<Data> dataVector;

    // Measure the execution time of readFileAndProcess function
    double executionTime = ExecutionTimeCalculation([&dataVector]() 
    {
        Filereading("data.csv", dataVector);
    });

   cout << "Time taken to read and process data: " << executionTime << " seconds" << endl;

    // Print the read data
    print(dataVector);

   
}
