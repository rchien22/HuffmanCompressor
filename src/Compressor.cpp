#include "Compressor.h"
#include <fstream>
#include <iostream>

using namespace std;

// Builds a frequency map of all characters in a given txt file.
unordered_map<char, int> analyzeFreq(const string &fileName)
{
    unordered_map<char, int> freqMap;

    // Opens in binary mode to treat all byte values uniformly
    ifstream file(fileName, ios::binary);

    // File opening error check
    if (!file)
    {
        cerr << "Error opening file " << fileName << endl;
        return freqMap;
    }

    char c;
    while (file.get(c))
    {
        freqMap[c]++; // Increment frequency count for each character
    }

    file.close();
    return freqMap;
}
