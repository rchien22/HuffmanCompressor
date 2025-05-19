#pragma once
#include <unordered_map>
#include <string>

using namespace std;

// Builds a frequency map [char, count] from the input file
unordered_map<char, int> analyzeFreq(const string &file);

// Compresses input to a single .huff file that includes tree
void encodeFile(const string &inputPath,
                const string &outputPath, const HuffmanTree &tree);

// Decompresses a .huff file back to a single txt.file
void decodeFile(const string &inPath, const string &outPath);
