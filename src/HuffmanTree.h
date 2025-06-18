#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include "Node.h"

constexpr char LEAF_MARKER = '1';
constexpr char NODE_MARKER = '0';

// Class for building and managing our Huffman Tree
class HuffmanTree
{
private:
    std::shared_ptr<Node> root; // Root node of our tree

    // Builds our character-to-code table by traversing the tree
    // 'path' acculumates the bitstring ("010")
    void buildCodeTable(const std::shared_ptr<Node> &node, const std::string &path);

    // Helper function: reconstructs tree using preorder traversal from stream
    void serializeHelper(const std::shared_ptr<Node> &node, std::ostream &outFile) const;

    // Reconstructs tree from stream using marker format
    std::shared_ptr<Node> deserializeHelper(std::istream &in);

public:
    // Maps characters to its corresponding bitstring ('f' -> "010")
    std::unordered_map<char, std::string> codeTable;

    // Builds the Huffman tree from a frequency map
    void build(const std::unordered_map<char, int> &freqMap);

    // Returns the root node of the current tree
    std::shared_ptr<Node> getRoot() const;

    // Clears the tree and code table
    void clear();

    // Serializes the tree to an out stream
    void serialize(std::ostream &outFile) const;

    // Loads a tree from a stream
    void deserialize(std::istream &in);
};
