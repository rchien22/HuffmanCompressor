#include "Compressor.h"
#include <fstream>
#include <iostream>
#include <bitset>
#include <vector>

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

void encodeFile(const string &inputPath, const string &outputPath,
                const unordered_map<char, string> &codeTable)
{
    ifstream inFile(inputPath, ios::binary);
    ofstream outFile(outputPath, ios::binary);

    if (!inFile || !outFile)
    {
        cerr << "Error opening input / output file!\n";
        return;
    }

    // Holds the combined bits as a string
    string bitBuffer;
    char c;

    // Convert each char into Huffman code
    while (inFile.get(c))
    {
        // Append corresponding bitstring
        bitBuffer += codeTable.at(c);
    }

    // Convert the string of bits into actual bytes and write to output
    while (bitBuffer.size() >= 8)
    {
        bitset<8> byte(bitBuffer.substr(0, 8));
        outFile.put(static_cast<unsigned char>(byte.to_ulong()));
        bitBuffer.erase(0, 8);
    }

    // Handle leftover bits (add zeros to make a full byte)
    if (!bitBuffer.empty())
    {
        bitBuffer.append(8 - bitBuffer.size(), '0');
        bitset<8> finalByte(bitBuffer);
        outFile.put(static_cast<unsigned char>(finalByte.to_ulong()));
    }

    inFile.close();
    outFile.close();
}
