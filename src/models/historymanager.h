#pragma once

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <iterator>
#include "color.h"

using namespace std;

struct ColorHistory
{
    Color color;
    string datetime;
};

class HistoryManager
{
private:
    vector<ColorHistory> historyList;
    string filepath = "";
public:
    HistoryManager(string path);
    bool FileExists();
    bool CreateFile();
    bool SaveColor(const Color &color);
    void LoadHistory();
    vector<ColorHistory> GetColorHistoryList();
    ColorHistory GetColorHistoryList(int index);
};