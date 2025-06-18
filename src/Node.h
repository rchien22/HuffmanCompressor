#pragma once

#include <memory>

// Node in the Huffman Tree
struct Node
{
    char ch;  // Character stored in node (only for leaf nodes)
    int freq; // Frequency of the character

    // Pointers to left and right child nodes in the tree
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;

    // Leaf node constructor
    Node(char c, int f)
    {
        ch = c;
        freq = f;
        left = nullptr;
        right = nullptr;
    }

    // Internal node constructor (only frequency)
    Node(int f, std::shared_ptr<Node> leftNode, std::shared_ptr<Node> rightNode)
    {
        ch = '\0';
        freq = f;
        left = leftNode;
        right = rightNode;
    }
};

// Compare two nodes based on their frequency
// Enable use in a min-heap
struct CompareNodes
{
    bool operator()(const std::shared_ptr<Node> &a, const std::shared_ptr<Node> &b) const
    {
        return (a->freq > b->freq);
    }
};
