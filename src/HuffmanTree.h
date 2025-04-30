#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include "Node.h"

using namespace std;

// Class for building and managing our Huffman Tree
class HuffmanTree
{
private:
    shared_ptr<Node> root; // Root node of our tree

    // Builds our character-to-code table by traversing the tree
    // 'path' acculumates the bitstring ("010")
    void buildCodeTable(const shared_ptr<Node> &node, const string &path);

public:
    // Maps characters to its corresponding bitstring ('f' -> "010")
    unordered_map<char, string> codeTable;

    // Builds the Huffman tree from a frequency map
    void build(const unordered_map<char, int> &freqMap);

    // Returns the root node of the current tree
    shared_ptr<Node> getRoot() const;

    // Clears the tree and code table
    void clear();
};
