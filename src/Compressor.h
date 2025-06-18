#pragma once

#include "HuffmanTree.h"
#include <unordered_map>
#include <string>

constexpr size_t TREE_HEADER_SIZE = 4;

// Builds a frequency map [char, count] from the input file
std::unordered_map<char, int> analyzeFreq(const std::string &file);

// Compresses input to a single .huff file that includes tree
void encodeFile(const std::string &inputPath,
                const std::string &outputPath, const HuffmanTree &tree);

// Decompresses a .huff file back to a single txt.file
void decodeFile(const std::string &inPath, const std::string &outPath);
