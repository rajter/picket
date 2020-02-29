#pragma once

#include <iostream>
#include <map>
#include <string>
#include <iterator>
#include <vector>

using namespace std;

class ColorFormatManager
{
 private:
    string filepath = "";
    map<string, string> formats;
    map<string, string> variables;
    string ReplaceVariable(string original, string var, string replacement);
    vector<string> GetVariableKeys();
    string formatsString = "";
    string formatsBlueprint = "#FORMATS\n"
        "# Formats\n"
        "[HEX]#$r_hex$$g_hex$$b_hex$\n"
        "[RGB]$r$, $g$, $b$\n"
        "[HSL]$h$, $s$%, $l$%\n"
        "# CSS Formats\n"
        "[CSS HEX]#$r_hex$$g_hex$$b_hex$\n"
        "[CSS RGB]rgb($r$, $g$, $b$)\n"
        "[CSS HSL]hsl($h$, $s$%, $l$%)\n"
        "# .Net Formats\n"
        "[.Net Color]Color.FromRgb($r$, $g$, $b$)\n"
        "# This is a comment\n";

 protected:

 public:
    ColorFormatManager(string path);
    string FormatFilePath();
    string FormatsString();
    bool FormatFileExists();
    bool CreateFormatFile();
    bool SaveFormatFile(string formats);
    void LoadFormats();
    void SetVariables(map<string, string> vars);
    string FindFormatKey(string line);
    string GetFormatString(string line);
    vector<string> GetFormatsKeys();
    string GetFormat(string key);
};
