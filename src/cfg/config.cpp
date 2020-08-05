#include <string>
#include "config.h"
#include "ini.h"

using namespace std;

bool Config::LoadConfiguration()
{
    // create a file instance
    mINI::INIFile file(filePath);

    // create a data structure
    if(file.read(ini))
    {
        width = GetInteger("global", "width", 0);
        height = GetInteger("global", "height", 0);
        startImmediatePick = GetBoolean("global", "start_immediate_pick", false);
        copyToClipboardAfterPick = GetBoolean("global", "copy_to_clipboard_after_pick", false);
        quitAfterPick = GetBoolean("global", "quit_after_pick", false);
        lastFormat = GetInteger("global", "last_format", 1);
        screenShotDelay = GetInteger("global", "screenshot_delay", 300);
        pixelSize = GetInteger("magnifier", "pixel_size", -1);
        pixelsPerRow = GetInteger("magnifier", "pixels_per_row", -1);
        displayHexString = GetBoolean("display", "display_hex_string", true);
        displayColorFormat = GetBoolean("display", "display_color_format", true);
        displayColorInfoBox = GetBoolean("display", "displa_color_info_box", true);

        return true;
    }

    return false;
}

bool Config::SaveConfiguration()
{
    mINI::INIFile file(filePath);

    ini["global"]["width"] = to_string(width);
    ini["global"]["height"] = to_string(height);
    ini["global"]["start_immediate_pick"] = to_string(startImmediatePick);
    ini["global"]["copy_to_clipboard_after_pick"] = to_string(copyToClipboardAfterPick);
    ini["global"]["quit_after_pick"] = to_string(quitAfterPick);
    ini["global"]["last_format"] = to_string(lastFormat);
    ini["global"]["screenshot_delay"] = to_string(screenShotDelay);
    ini["magnifier"]["pixel_size"] = to_string(pixelSize);
    ini["magnifier"]["pixels_per_row"] = to_string(pixelsPerRow);
    ini["display"]["display_hex_string"] = to_string(displayHexString);
    ini["display"]["display_color_format"] = to_string(displayColorFormat);
    ini["display"]["displa_color_info_box"] = to_string(displayColorInfoBox);

    return file.write(ini, true);
}

string Config::Get(string section, string name, string default_value)
{
    if(ini.has(section))
    {
        if(ini[section].has(name))
        {
            return ini.get(section).get(name);
        }
    }

    return default_value;
}

long Config::GetInteger(string section, string name, long default_value)
{
    string valstr = Get(section, name, "");
    const char* value = valstr.c_str();
    char* end;
    // This parses "1234" (decimal) and also "0x4D2" (hex)
    long n = strtol(value, &end, 0);
    return end > value ? n : default_value;
}

double Config::GetReal(string section, string name, double default_value)
{
    string valstr = Get(section, name, "");
    const char* value = valstr.c_str();
    char* end;
    double n = strtod(value, &end);
    return end > value ? n : default_value;
}

bool Config::GetBoolean(string section, string name, bool default_value)
{
    string valstr = Get(section, name, "");
    // Convert to lower case to make string comparisons case-insensitive
    transform(valstr.begin(), valstr.end(), valstr.begin(), ::tolower);
    if (valstr == "true" || valstr == "yes" || valstr == "on" || valstr == "1")
        return true;
    else if (valstr == "false" || valstr == "no" || valstr == "off" || valstr == "0")
        return false;
    else
        return default_value;
}
