#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "historymanager.h"
#include "color.h"

HistoryManager::HistoryManager(string path)
{
    filepath = path;
    if(!FileExists())
    {
        CreateFile();
    }

    LoadHistory();
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

bool HistoryManager::SaveColor(const Color &color)
{
    ofstream fout;
    fout.open(filepath, std::ios_base::app);
    if(fout.is_open())
    {
        fout << color.ToString() << endl;
    }
    fout.close();

    return true;
}

void HistoryManager::LoadHistory()
{
    ifstream input(filepath);
    for( std::string line; getline( input, line ); )
    {
        if(line.size() == 0)
        {
            continue;
        }
        else
        {
            size_t endOfKey = line.find_first_of("|");

            if(endOfKey != string::npos)
            {
                string colorString = line.substr(endOfKey + 1);
                std::stringstream ss(colorString);
                string colorValue;
                vector<int> rgbvalues;
                while(std::getline(ss, colorValue, ';'))
                {
                    rgbvalues.push_back(std::stoi(colorValue));
                }

                ColorHistory colorH = ColorHistory();
                colorH.color = Color(rgbvalues[0], rgbvalues[1], rgbvalues[2], rgbvalues[3]);
                colorH.datetime = line.substr(0, endOfKey);
                colorHistory.push_back(colorH);
            }
        }
    }
}