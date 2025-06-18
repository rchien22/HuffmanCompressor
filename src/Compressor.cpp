#include "Compressor.h"
#include "HuffmanTree.h"
#include <fstream>
#include <iostream>
#include <bitset>
#include <vector>
#include <sstream>

// Builds a frequency map of all characters in a given txt file.
std::unordered_map<char, int> analyzeFreq(const std::string &fileName)
{
    std::unordered_map<char, int> freqMap;

    // Opens in binary mode to treat all byte values uniformly
    std::ifstream file(fileName, std::ios::binary);

    // File opening error check
    if (!file)
    {
        std::cerr << "Error opening file " << fileName << std::endl;
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
void encodeFile(const std::string &inputPath,
                const std::string &outputPath, const HuffmanTree &tree)
{
    std::ifstream inFile(inputPath, std::ios::binary);
    std::ofstream outFile(outputPath, std::ios::binary);

    if (!inFile || !outFile)
    {
        std::cerr << "Error opening input/output file.\n";
        return;
    }

    // Serialize the tree into memory
    std::stringstream treeBuffer;
    tree.serialize(treeBuffer);
    std::string treeData = treeBuffer.str();
    uint32_t treeSize = treeData.size();

    // Write 4-byte treeSize
    outFile.write(reinterpret_cast<const char *>(&treeSize), TREE_HEADER_SIZE);

    // Write TREE bytes
    outFile.write(treeData.data(), treeSize);

    // Encode actual data to bitstring
    std::string bitBuffer;
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
        std::bitset<8> byte(bitBuffer.substr(i, 8));
        outFile.put(static_cast<unsigned char>(byte.to_ulong()));
    }

    inFile.close();
    outFile.close();
}

// Decodes a .huff file back into original text file
// Format: [treeSize][tree][paddingByte][bitstream]
void decodeFile(const std::string &inPath, const std::string &outPath)
{
    std::ifstream in(inPath, std::ios::binary);
    std::ofstream out(outPath, std::ios::binary);

    if (!in || !out)
    {
        std::cerr << "Error opening input / output file.\n";
        return;
    }

    // Read 4-byte tree size
    uint32_t treeSize;
    in.read(reinterpret_cast<char *>(&treeSize), sizeof(treeSize));

    // Read serialized tree
    std::string treeData(treeSize, '\0');
    in.read(&treeData[0], treeSize);
    std::stringstream treeStream(treeData);

    HuffmanTree tree;
    tree.deserialize(treeStream);

    // Read 1-byte padding info
    int paddingBits = in.get();

    // Read remaining data into a bitstring
    std::string bitBuffer;
    char byte;
    while (in.get(byte))
    {
        std::bitset<8> bits(static_cast<unsigned char>(byte));
        bitBuffer += bits.to_string();
    }

    // Remove padding
    if (paddingBits > 0)
    {
        bitBuffer.erase(bitBuffer.end() - paddingBits, bitBuffer.end());
    }

    // Decode bits using the Huffman tree
    std::shared_ptr<Node> current = tree.getRoot();
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
