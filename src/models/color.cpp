#include <iostream>
#include <vector>
#include "color.h"
#include "../utils/colorspace.cpp"
#include "../utils/stringutils.cpp"

using namespace std;
using namespace DataUtilities;

Color::Color()
{
    SetRGB(0,0,0);
    ColorSpace::RgbToHsl(red, green, blue, &hue, &saturation, &lightness);
    ColorSpace::RgbToCmyk(red, green, blue, &cyan, &magenta, &yellow, &key);
}

Color::Color(int r, int g, int b)
{
    SetRGB(r, g, b);
    ColorSpace::RgbToHsl(red, green, blue, &hue, &saturation, &lightness);
    ColorSpace::RgbToCmyk(red, green, blue, &cyan, &magenta, &yellow, &key);
}

Color::Color(int r, int g, int b, int a)
{
    SetRGB(r, g, b, a);
    ColorSpace::RgbToHsl(red, green, blue, &hue, &saturation, &lightness);
    ColorSpace::RgbToCmyk(red, green, blue, &cyan, &magenta, &yellow, &key);
}

void Color::SetRGB(int r, int g, int b, int a)
{
    red = r;
    green = g;
    blue = b;
    alpha = a;
    ColorSpace::RgbToHsl(red, green, blue, &hue, &saturation, &lightness);
    ColorSpace::RgbToCmyk(red, green, blue, &cyan, &magenta, &yellow, &key);
}

void Color::SetHSL(int h, int s, int l, int a)
{
    hue = h;
    saturation = s;
    lightness = l;
    alpha = a;
    ColorSpace::HslToRgb(hue, (float)GetSaturationAsDouble(), (float)GetLightnessAsDouble(), &red, &green, &blue);
    ColorSpace::RgbToCmyk(red, green, blue, &cyan, &magenta, &yellow, &key);
}

void Color::SetCmyk(int c, int m, int y, int k)
{
    cyan = c;
    magenta = m;
    yellow = y;
    key = k;
    ColorSpace::CmykToRgb(cyan, magenta, yellow, key, &red, &green, &blue);
    ColorSpace::RgbToHsl(red, green, blue, &hue, &saturation, &lightness);
}

string Color::GetHexString(bool withAplha)
{
    std::stringstream sstream;
    sstream << "#" << GetRedHex() << GetGreenHex() << GetBlueHex();

    if(withAplha)
        sstream << GetAlphaHex();

    return sstream.str();
}

Color Color::GetContrastColor()
{
    auto luminance = 1 - ( 0.299 * GetRed() + 0.587 * GetGreen() + 0.114 * GetBlue()) / 255;

    auto d = (luminance > 0.5) ? 255 : 0;

    return Color(d,d,d);
}

string Color::ToString() const
{
    time_t now = time(0);
    string date = ctime(& now);

    string colorString = "";
    colorString.append(trim(date));
    colorString.append("|");
    colorString.append(to_string(red));
    colorString.append(";");
    colorString.append(to_string(green));
    colorString.append(";");
    colorString.append(to_string(blue));
    colorString.append(";");
    colorString.append(to_string(alpha));

    return trim(colorString);
}

bool Color::FromString(const string &colorString)
{
    return true;
}

map<string, string> Color::GetVariables()
{
    map<string, string> variables;
    variables.insert(make_pair("$r$", to_string(red)));
    variables.insert(make_pair("$g$", to_string(green)));
    variables.insert(make_pair("$b$", to_string(blue)));
    variables.insert(make_pair("$a$", to_string(alpha)));
    variables.insert(make_pair("$r_d$", DoubleToString(GetRedAsDouble(),6)));
    variables.insert(make_pair("$g_d$", DoubleToString(GetGreenAsDouble(),6)));
    variables.insert(make_pair("$b_d$", DoubleToString(GetBlueAsDouble(),6)));
    variables.insert(make_pair("$a_d$", DoubleToString(GetAlphaAsDouble(),6)));
    variables.insert(make_pair("$r_d1$", DoubleToString(GetRedAsDouble(),1)));
    variables.insert(make_pair("$g_d1$", DoubleToString(GetGreenAsDouble(),1)));
    variables.insert(make_pair("$b_d1$", DoubleToString(GetBlueAsDouble(),1)));
    variables.insert(make_pair("$a_d1$", DoubleToString(GetAlphaAsDouble(),1)));
    variables.insert(make_pair("$r_d2$", DoubleToString(GetRedAsDouble(),2)));
    variables.insert(make_pair("$g_d2$", DoubleToString(GetGreenAsDouble(),2)));
    variables.insert(make_pair("$b_d2$", DoubleToString(GetBlueAsDouble(),2)));
    variables.insert(make_pair("$a_d2$", DoubleToString(GetAlphaAsDouble(),2)));
    variables.insert(make_pair("$r_d3$", DoubleToString(GetRedAsDouble(),3)));
    variables.insert(make_pair("$g_d3$", DoubleToString(GetGreenAsDouble(),3)));
    variables.insert(make_pair("$b_d3$", DoubleToString(GetBlueAsDouble(),3)));
    variables.insert(make_pair("$a_d3$", DoubleToString(GetAlphaAsDouble(),3)));
    variables.insert(make_pair("$r_d4$", DoubleToString(GetRedAsDouble(),4)));
    variables.insert(make_pair("$g_d4$", DoubleToString(GetGreenAsDouble(),4)));
    variables.insert(make_pair("$b_d4$", DoubleToString(GetBlueAsDouble(),4)));
    variables.insert(make_pair("$a_d4$", DoubleToString(GetAlphaAsDouble(),4)));
    variables.insert(make_pair("$r_d5$", DoubleToString(GetRedAsDouble(),5)));
    variables.insert(make_pair("$g_d5$", DoubleToString(GetGreenAsDouble(),5)));
    variables.insert(make_pair("$b_d5$", DoubleToString(GetBlueAsDouble(),5)));
    variables.insert(make_pair("$a_d5$", DoubleToString(GetAlphaAsDouble(),5)));
    variables.insert(make_pair("$r_d6$", DoubleToString(GetRedAsDouble(),6)));
    variables.insert(make_pair("$g_d6$", DoubleToString(GetGreenAsDouble(),6)));
    variables.insert(make_pair("$b_d6$", DoubleToString(GetBlueAsDouble(),6)));
    variables.insert(make_pair("$a_d6$", DoubleToString(GetAlphaAsDouble(),6)));
    variables.insert(make_pair("$r_hex$", GetRedHex()));
    variables.insert(make_pair("$g_hex$", GetGreenHex()));
    variables.insert(make_pair("$b_hex$", GetBlueHex()));
    variables.insert(make_pair("$a_hex$", GetAlphaHex()));
    variables.insert(make_pair("$h$", to_string(hue)));
    variables.insert(make_pair("$s$", to_string(saturation)));
    variables.insert(make_pair("$l$", to_string(lightness)));
    variables.insert(make_pair("$s_d$", to_string(GetSaturationAsDouble())));
    variables.insert(make_pair("$l_d$", to_string(GetLightnessAsDouble())));
    variables.insert(make_pair("$c$", to_string(cyan)));
    variables.insert(make_pair("$m$", to_string(magenta)));
    variables.insert(make_pair("$y$", to_string(yellow)));
    variables.insert(make_pair("$k$", to_string(key)));
    variables.insert(make_pair("$c_d$", DoubleToString(GetCyanAsDouble(),6)));
    variables.insert(make_pair("$m_d$", DoubleToString(GetMagentaAsDouble(),6)));
    variables.insert(make_pair("$y_d$", DoubleToString(GetYellowAsDouble(),6)));
    variables.insert(make_pair("$k_d$", DoubleToString(GetKeyAsDouble(),6)));

    return variables;
}

void Color::Log()
{
    cout << "Color" << endl;
    cout << "Red: " << GetRed() << ", Green: " << GetGreen() << ", Blue: " << GetBlue() << endl;
    cout << "Hue: " << GetHue() << "°, Saturation: " << GetSaturation() << "%, Lightness: " << GetLightness() << "%" << endl;
    cout << "Cyan: " << GetCyan() << ", Magenta: " << GetMagenta() << ", Yellow: " << GetYellow() << ", Key: " << GetKey() << endl;
}
