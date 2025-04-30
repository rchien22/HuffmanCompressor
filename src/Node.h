#pragma once

#include <memory>

using namespace std;

// Node in the Huffman Tree
struct Node
{
    char ch;  // Character stored in node (only for leaf nodes)
    int freq; // Frequency of the character

    // Pointers to left and right child nodes in the tree
    shared_ptr<Node> left;
    shared_ptr<Node> right;

    // Leaf node constructor
    Node(char c, int f)
    {
        ch = c;
        freq = f;
        left = nullptr;
        right = nullptr;
    }

    // Internal node constructor (only frequency)
    Node(int f, shared_ptr<Node> leftNode, shared_ptr<Node> rightNode)
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
    bool operator()(const shared_ptr<Node> &a, const shared_ptr<Node> &b) const
    {
        return (a->freq > b->freq);
    }
};
