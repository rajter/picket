#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <math.h>
#include <unistd.h>
#include <sys/stat.h>

using namespace std;

namespace DataUtilities
{
    static double RoundTo(double value, int n)
    {
        return floor(value*pow(10,n)+0.5)/pow(10,n);
    }

    static string DoubleToString(double value, int precision)
    {
        std::stringstream sstream;
        sstream << fixed << setprecision(precision) << value;
        return sstream.str();
    }

    static string IntToHex(int i)
    {
        std::stringstream sstream;
        sstream << std::setfill('0') << std::setw(2) << std::hex << i;
        return sstream.str();
    }

    static string SanitizeStringToInt(const string &input, int minValue, int maxValue, int defaultInt)
    {
        string s = "";

        for(char c : input)
        {
            if(isdigit(c))
            {
                s += c;
            }
        }

        // Remove first zero char
        if(s.length() > 1 && s[0] == '0')
        {
            s.erase(0,1);
        }

        if(!s.empty())
        {
            int value = stoi(s);
            if(value < minValue)
            {
                s = to_string(minValue);
            }
            else if( value > maxValue)
            {
                s = to_string(maxValue);
            }
        }

        return s;
    }
};