#include "HuffmanTree.h"
#include <queue>
#include <fstream>

// This builds the Huffman Tree using a min-heap (priority queue)
void HuffmanTree::build(const unordered_map<char, int> &freqMap)
{
    // Priority Queue ordered by frequency (smallest at root)
    priority_queue<shared_ptr<Node>, vector<shared_ptr<Node>>, CompareNodes> minHeap;

    // Creates a leaf node for each char and pushes into heap
    for (const auto &pair : freqMap)
    {
        minHeap.push(make_shared<Node>(pair.first, pair.second));
    }

    // Combine nodes until a single root node remains
    while (minHeap.size() > 1)
    {
        auto left = minHeap.top();
        minHeap.pop();
        auto right = minHeap.top();
        minHeap.pop();

        // New parent node with their combined frequencies
        int combinedFreq = left->freq + right->freq;
        auto parent = make_shared<Node>(combinedFreq, left, right);

        minHeap.push(parent);
    }

    // Set the root of the tree
    root = minHeap.top();

    // Generate the Huffman codes for each char
    codeTable.clear();
    buildCodeTable(root, "");
}

// Traverse the Huffman tree recursively to assign codes to each char
// Left = '0', Right = '1'
void HuffmanTree::buildCodeTable(const shared_ptr<Node> &node, const string &path)
{
    if (!node)
        return;

    // If leaf node, store the path as code for this char
    if (!node->left && !node->right)
    {
        codeTable[node->ch] = path;
        return;
    }

    // Traverse left- adds '0' to path
    buildCodeTable(node->left, path + "0");

    // Traverse right- adds '1' to path
    buildCodeTable(node->right, path + "1");
}

// Returns the root node of the current tree
shared_ptr<Node> HuffmanTree::getRoot() const
{
    return root;
}

// Clears the tree and code table
void HuffmanTree::clear()
{
    root = nullptr;
    codeTable.clear();
}

// Serializes the tree to an out stream
void HuffmanTree::serialize(ostream &outFile) const
{
    serializeHelper(root, outFile);
}

// Helper function: reconstructs tree using preorder traversal from stream
void HuffmanTree::serializeHelper(const shared_ptr<Node> &node, ostream &outFile) const
{
    if (!node)
        return;

    // If leaf: write marker + char
    if (!node->left && !node->right)
    {
        outFile.put('1');
        outFile.put(node->ch);
    }
    else
    // If internal node: write marker
    {
        outFile.put('0');
        serializeHelper(node->left, outFile);
        serializeHelper(node->right, outFile);
    }
}

// Loads a tree from a stream
void HuffmanTree::deserialize(istream &in)
{
    root = deserializeHelper(in);
    codeTable.clear();
    buildCodeTable(root, "");
}

// Reconstructs tree from stream using marker format
shared_ptr<Node> HuffmanTree::deserializeHelper(istream &in)
{
    char marker;
    if (!in.get(marker))
        return nullptr;

    if (marker == '1')
    {
        char ch;
        in.get(ch);
        return make_shared<Node>(ch, 0);
    }
    else
    {
        auto left = deserializeHelper(in);
        auto right = deserializeHelper(in);
        return make_shared<Node>(0, left, right);
    }
}
