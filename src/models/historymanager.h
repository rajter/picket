#pragma once

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <iterator>

using namespace std;

class HistoryManager
{
private:
    string filepath = "";
public:
    HistoryManager(string path);
    bool FileExists();
    bool CreateFile();
};