#include <string>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <math.h>

using namespace std;

namespace ColorSpace
{
    static void RgbToHsl(int red, int green, int blue, int* hue, int* saturation, int* lightness)
    {
        double r = (double)red/255;
        double g = (double)green/255;
        double b = (double)blue/255;

        double colors[] = { r,g,b };

        auto max = *std::max_element(colors, colors + 3);
        auto min = *std::min_element(colors, colors + 3);
        auto h = (max + min) / 2;
        auto s = (max + min) / 2;
        auto l = (max + min) / 2;

        if (max == min)
        {
            h = s = 0; // achromatic
        }
        else
        {
            auto d = max - min;
            s = l > 0.5 ? d / (2 - max - min) : d / (max + min);

            if(max == r)
                h = (g - b) / d + (g < b ? 6 : 0);
            else if(max == g)
                h = (b - r) / d + 2;
            else if(max == b)
                h = (r - g) / d + 4;
        }

        h /= 6;

        *hue = (int)(h*360);
        *saturation = (int)round(s*100);
        *lightness = (int)round(l*100);
    }

    static void RgbToCmyk(int red, int green, int blue, int* cyan, int* magenta, int* yellow, int* key)
    {
        double r = (double)red/255.0f;
        double g = (double)green/255.0f;
        double b = (double)blue/255.0f;

        auto k = 1.0f-std::max({r,g,b});
        auto c = (1.0f-r-k)/(1.0f-k);
        auto m = (1.0f-g-k)/(1.0f-k);
        auto y = (1.0f-b-k)/(1.0f-k);

        *cyan = isnan(c) ? 0 : (int)(c*100);
        *magenta = isnan(m) ? 0 : (int)(m*100);
        *yellow = isnan(y) ? 0 : (int)(y*100);
        *key = isnan(k) ? 0 : (int)(k*100);
    }

    static void CmykToRgb(int cyan, int magenta, int yellow, int key, int* red, int* green, int* blue)
    {
        double c = (double)cyan/100.0f;
        double m = (double)magenta/100.0f;
        double y = (double)yellow/100.0f;
        double k = (double)key/100.0f;

        int r = (int)(255 * (1 - c) * (1 - k));
        int g = (int)(255 * (1 - m) * (1 - k));
        int b = (int)(255 * (1 - y) * (1 - k));

        *red = r;
        *green = g;
        *blue = b;
    }

    static float HueToRgb(float v1, float v2, float vH)
    {
        if (vH < 0)
            vH += 1;

        if (vH > 1)
            vH -= 1;

        if ((6 * vH) < 1)
            return (v1 + (v2 - v1) * 6 * vH);

        if ((2 * vH) < 1)
            return v2;

        if ((3 * vH) < 2)
            return (v1 + (v2 - v1) * ((2.0f / 3) - vH) * 6);

        return v1;
    }

    static void HslToRgb(int hue, float saturation, float lightness, int* red, int* green, int* blue)
    {
        if (saturation == 0)
        {
            *red = *green = *blue = lightness; // achromatic
        }
        else
        {
            float v1, v2;
            float h = (float)hue / 360;

            v2 = (lightness < 0.5) ? (lightness * (1 + saturation)) : ((lightness + saturation) - (lightness * saturation));
            v1 = 2 * lightness - v2;

            *red = (unsigned char)(255 * HueToRgb(v1, v2, h + (1.0f / 3)));
            *green = (unsigned char)(255 * HueToRgb(v1, v2, h));
            *blue = (unsigned char)(255 * HueToRgb(v1, v2, h - (1.0f / 3)));
        }
    }
};