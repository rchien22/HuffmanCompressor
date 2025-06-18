#include "Compressor.h"
#include "HuffmanTree.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <iomanip>

int main(int argc, char *argv[])
{
    // Checks for correct CLI argument count
    if (argc < 4)
    {
        std::cerr << "Usage:\n"
                  << argv[0] << " compress input.txt output.huff\n"
                  << argv[0] << " decompress input.huff output.txt\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string inputPath = argv[2];
    std::string outputPath = argv[3];

    namespace fs = std::filesystem;

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
        std::cout << "Compression complete! Result: " << outputPath << std::endl;
    }
    // DECOMPRESSION MODE
    else if (mode == "decompress")
    {
        decodeFile(inputPath, outputPath);
        std::cout << "Decompression complete! Result: " << outputPath << std::endl;
    }
    // INVALID ARGUMENT
    else
    {
        std::cerr << "Error: unknown mode. Use 'compress' or 'decompress'." << std::endl;
        return 1;
    }

    return 0;
}
