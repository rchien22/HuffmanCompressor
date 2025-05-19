#include "Compressor.h"
#include "HuffmanTree.h"
#include <fstream>
#include <iostream>
#include <bitset>
#include <vector>
#include <sstream>

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

// Encodes input using Huffman codes and writes a .huff file
// Format: [4-byte treeSize][tree][1-byte padding][bitstream]
void encodeFile(const string &inputPath,
                const string &outputPath, const HuffmanTree &tree)
{
    ifstream inFile(inputPath, ios::binary);
    ofstream outFile(outputPath, ios::binary);

    if (!inFile || !outFile)
    {
        cerr << "Error opening input/output file.\n";
        return;
    }

    // Serialize the tree into memory
    stringstream treeBuffer;
    tree.serialize(treeBuffer);
    string treeData = treeBuffer.str();
    uint32_t treeSize = treeData.size();

    // Write 4-byte TREE_SIZE
    outFile.write(reinterpret_cast<const char *>(&treeSize), sizeof(treeSize));

    // Write TREE bytes
    outFile.write(treeData.data(), treeSize);

    // Encode actual data to bitstring
    string bitBuffer;
    char c;
    while (inFile.get(c))
    {
        bitBuffer += tree.codeTable.at(c);
    }

    // Calculate how many padding bits we need
    int paddingBits = (8 - (bitBuffer.size() % 8)) % 8;
    // Write number of padding bits as a single byte
    outFile.put(static_cast<char>(paddingBits));
    bitBuffer.append(paddingBits, '0');

    // Write compressed data (byte by byte)
    for (size_t i = 0; i < bitBuffer.size(); i += 8)
    {
        bitset<8> byte(bitBuffer.substr(i, 8));
        outFile.put(static_cast<unsigned char>(byte.to_ulong()));
    }

    inFile.close();
    outFile.close();
}

// Decodes a .huff file back into original text file
// Format: [treeSize][tree][paddingByte][bitstream]
void decodeFile(const string &inPath, const string &outPath)
{
    ifstream in(inPath, ios::binary);
    ofstream out(outPath, ios::binary);

    if (!in || !out)
    {
        cerr << "Error opening input / output file.\n";
        return;
    }

    // Read 4-byte tree size
    uint32_t treeSize;
    in.read(reinterpret_cast<char *>(&treeSize), sizeof(treeSize));

    // Read serialized tree
    string treeData(treeSize, '\0');
    in.read(&treeData[0], treeSize);
    stringstream treeStream(treeData);

    HuffmanTree tree;
    tree.deserialize(treeStream);

    // Read 1-byte padding info
    int paddingBits = in.get();

    // Read remaining data into a bitstring
    string bitBuffer;
    char byte;
    while (in.get(byte))
    {
        bitset<8> bits(static_cast<unsigned char>(byte));
        bitBuffer += bits.to_string();
    }

    // Remove padding
    if (paddingBits > 0)
    {
        bitBuffer.erase(bitBuffer.end() - paddingBits, bitBuffer.end());
    }

    // Decode bits using the Huffman tree
    shared_ptr<Node> current = tree.getRoot();
    for (char bit : bitBuffer)
    {
        current = (bit == '0') ? current->left : current->right;

        if (!current->left && !current->right)
        {
            out.put(current->ch);
            current = tree.getRoot();
        }
    }

    in.close();
    out.close();
}
