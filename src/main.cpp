#include "Compressor.h"
#include "HuffmanTree.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[])
{
    // Checks for correct CLI argument count
    if (argc < 4)
    {
        cerr << "Usage:\n"
             << argv[0] << " compress input.txt output.huff\n"
             << argv[0] << " decompress input.huff output.txt\n";
        return 1;
    }

    string mode = argv[1];
    string inputPath = argv[2];
    string outputPath = argv[3];

    // COMPRESSION MODE
    if (mode == "compress")
    {
        // Analyze our input frequencies
        auto freqMap = analyzeFreq(inputPath);

        // Build the tree
        HuffmanTree tree;
        tree.build(freqMap);

        // Write tree and encoded data to output.huff
        encodeFile(inputPath, outputPath, tree);
        cout << "Compression complete! Result: " << outputPath << endl;
    }
    // DECOMPRESSION MODE
    else if (mode == "decompress")
    {
        decodeFile(inputPath, outputPath);
        cout << "Decompression complete! Result: " << outputPath << endl;
    }
    else
    {
        cerr << "Error: unknown mode. Use 'compress' or 'decompress'." << endl;
        return 1;
    }

    return 0;
}
