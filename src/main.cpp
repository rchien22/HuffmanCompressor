#include "Compressor.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "Missing file name!" << endl;
        return 1;
    }

    auto freqMap = analyzeFreq(argv[1]);

    for (const auto &pair : freqMap)
    {
        if (pair.first == '\n')
            cout << "'\\n': " << pair.second << endl;
        else if (pair.first == '\t')
            cout << "'\\t': " << pair.second << endl;
        else
            cout << "'" << pair.first << "': " << pair.second << endl;
    }
}
