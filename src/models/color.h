#pragma once

#include<iterator>
#include<map>
#include<string>
#include<math.h>
#include<iostream>
#include<sstream>
#include<iomanip>
#include"../utils/datautils.h"

using namespace std;

class Color
{
private:
    int red = 0;
    int green = 0;
    int blue = 0;
    int alpha = 0;
    int hue = 0;
    int saturation = 0;
    int lightness = 0;
    int cyan = 0;
    int magenta = 0;
    int yellow = 0;
    int key = 0;

public:
    Color();
    Color(int r, int g, int b);
    Color(int r, int g, int b, int a);
    void SetRGB(int r, int g, int b, int a = 255);
    void SetHSL(int h, int s, int l, int a = 255);
    void SetCmyk(int c, int m, int y, int k);

    int GetRed() {return red;}
    int GetGreen() {return green;}
    int GetBlue() {return blue;}
    int GetAlpha() {return alpha;}
    double GetRedAsDouble() {return (double)red/255;}
    double GetGreenAsDouble() {return (double)green/255;}
    double GetBlueAsDouble() {return (double)blue/255;}
    double GetAlphaAsDouble() {return (double)alpha/255;}
    string GetRedHex() {return DataUtilities::IntToHex(red);}
    string GetGreenHex() {return DataUtilities::IntToHex(green);}
    string GetBlueHex() {return DataUtilities::IntToHex(blue);}
    string GetAlphaHex() {return DataUtilities::IntToHex(alpha);}
    string GetHexString();
    int GetHue() { return hue; }
    int GetSaturation() { return saturation; }
    int GetLightness() { return lightness; }
    double GetHueAsDouble() { return (double)hue/360; }
    double GetSaturationAsDouble() { return (double)saturation/100; }
    double GetLightnessAsDouble() { return (double)lightness/100; }
    int GetCyan() { return cyan; }
    int GetMagenta() { return magenta; }
    int GetYellow() { return yellow; }
    int GetKey() { return key; }
    double GetCyanAsDouble() { return (double)cyan/100; }
    double GetMagentaAsDouble() { return (double)magenta/100; }
    double GetYellowAsDouble() { return (double)yellow/100; }
    double GetKeyAsDouble() { return (double)key/100; }

    void Log();
    Color GetContrastColor();
    map<string, string> GetVariables();
};
