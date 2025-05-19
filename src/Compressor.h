#pragma once
#include <unordered_map>
#include <string>

using namespace std;

unordered_map<char, int> analyzeFreq(const string &file);

void encodeFile(const string& inputPath, const string& outputPath, 
    const unordered_map<char, string>& codeTable);


