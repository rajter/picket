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
                s += c;
        }

        if(s.empty())
        {
            // return s;
            s = to_string(defaultInt);
        }
        else
        {
            int value = stoi(s);
            if(value < minValue || value > maxValue)
                s = to_string(defaultInt);
        }

        return s;
    }
};