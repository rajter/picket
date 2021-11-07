#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "historymanager.h"

HistoryManager::HistoryManager(string path)
{
    filepath = path;
    if(!FileExists())
        CreateFile();
}

bool HistoryManager::FileExists()
{
    ifstream FILE(filepath);
    if(!FILE)
        return false;
    else
        return true;
}

bool HistoryManager::CreateFile()
{
    ofstream fout;
    fout.open(filepath);
    fout.close();

    return true;
}