#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "colorformatmanager.h"
#include "../utils/stringutils.cpp"

using namespace std;

ColorFormatManager::ColorFormatManager(string path)
{
    filepath = path;
    if(!FormatFileExists())
        CreateFormatFile();
}

string ColorFormatManager::FormatFilePath()
{
    return filepath;
}

string ColorFormatManager::FormatsString()
{
    return formatsString;
}

bool ColorFormatManager::FormatFileExists()
{
    ifstream FILE(filepath);
    if(!FILE)
        return false;
    else
        return true;
}

bool ColorFormatManager::CreateFormatFile()
{
    ofstream fout;
    fout.open(filepath);
    fout<<formatsBlueprint<<endl;
    fout.close();

    return true;
}

bool ColorFormatManager::SaveFormatFile(string formats)
{
    ofstream fout;
    fout.open(filepath);
    fout << trim(formats) << endl;
    fout.close();

    LoadFormats();
    return true;
}

void ColorFormatManager::LoadFormats()
{
    formatsString = "";
    formats.clear();

    ifstream input(filepath);
    for( std::string line; getline( input, line ); )
    {
        formatsString.append(line);
        formatsString.append("\n");

        if(line.size() == 0)
        {
            continue;
        }
        else if(line.at(0) == '#')
        {
            continue;
        }
        else if(line.at(0) != '[')
        {
            continue;
        }
        else
        {
            string key = FindFormatKey(line);
            string format = GetFormatString(line);
            formats.insert(make_pair(key, format));
        }
    }
}

string ColorFormatManager::FindFormatKey(string line)
{
    size_t startOfKey = line.find_first_of("[");
    size_t endOfKey = line.find_first_of("]");

    if(startOfKey != string::npos && endOfKey != string::npos)
    {
        return line.substr(startOfKey + 1, endOfKey - 1);
    }

    return "";
}

string ColorFormatManager::GetFormatString(string line)
{
    size_t endOfKey = line.find_first_of("]");

    if(endOfKey != string::npos)
    {
        return line.substr(endOfKey + 1);
    }

    return "";
}

vector<string> ColorFormatManager::GetFormatsKeys()
{
    vector<string> v;
    for(map<string,string>::iterator it = formats.begin(); it != formats.end(); ++it) {
       v.push_back(it->first);
    }

    return v;
}

vector<string> ColorFormatManager::GetVariableKeys()
{
    vector<string> v;
    for(map<string,string>::iterator it = variables.begin(); it != variables.end(); ++it) {
        v.push_back(it->first);
    }

    return v;
}

void ColorFormatManager::SetVariables(map<string, string> vars)
{
    variables = vars;
}

string ColorFormatManager::GetFormat(string key)
{
    string format = formats[key];
    vector<string> variableKeys = GetVariableKeys();
    for(string &value: variableKeys)
    {
        string variable = variables[value];
        format = ReplaceVariable(format, value, variable);
    }

    return format;
}

string ColorFormatManager::ReplaceVariable(string original, string var, string replacement)
{
    auto pos = original.find(var);

    string newString = "";

    if(pos == string::npos)
    {
        return original;
    }
    else
    {
        while (pos != string::npos)
        {
            newString = original.replace(original.find(var), var.length(), replacement);
            pos = original.find(var);

            return newString;
        }
        return newString;
    }
}
