#pragma once

#include <memory>

using namespace std;

struct Node
{
    char ch;
    int freq;

    shared_ptr<Node> left;
    shared_ptr<Node> right;

    Node(char c, int f)
    {
        ch = c;
        freq = f;
        left = nullptr;
        right = nullptr;
    }

    Node(int f, shared_ptr<Node> leftNode, shared_ptr<Node> rightNode)
    {
        ch = '\0';
        freq = f;
        left = leftNode;
        right = rightNode;
    }
};

struct CompareNodes
{
    bool operator()(const shared_ptr<Node> &a, const shared_ptr<Node> &b) const
    {
        return (a->freq > b->freq);
    }
};
