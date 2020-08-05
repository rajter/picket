#pragma once

#include <string>
#include "ini.h"

using namespace std;

class Config
{
private:

    mINI::INIStructure ini;
    int width = 0;
    int height = 0;
    int lastFormat = 1;
    int screenShotDelay = 300;
    bool startImmediatePick = false;
    bool copyToClipboardAfterPick = false;
    bool quitAfterPick = false;
    bool displayHexString = true;
    bool displayColorFormat = true;
    bool displayColorInfoBox = true;

    int pixelSize = 8;
    int pixelsPerRow = 13;
    string filePath = (string)getenv("HOME")+"/.picket/config.ini";

    string Get(string section, string name, string default_value);
    long GetInteger(string section, string name, long default_value);
    double GetReal(string section, string name, double default_value);
    bool GetBoolean(string section, string name, bool default_value);

public:

    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    int GetLastFormat() const { return lastFormat; }
    int GetScreenShotDelay() const { return screenShotDelay; }
    bool ShouldStartImmediatePick() const { return startImmediatePick; }
    bool ShouldCopyAfterPick() const { return copyToClipboardAfterPick; }
    bool ShouldQuitAfterPick() const { return quitAfterPick; }
    int GetPixelSize() const { return pixelSize; }
    int GetPixelsPerRow() const { return pixelsPerRow; }
    bool ShouldDisplayHexString() const { return displayHexString; }
    bool ShouldDisplayColorFormat() const { return displayColorFormat; }
    bool ShouldDisplayColorInfoBox() const { return displayColorInfoBox; }

    void SetWidth(int w) { width = w; }
    void SetHeight(int h) { height = h; }
    void SetLastFormat(int lastFormatIndex) { lastFormat = lastFormatIndex; }
    void SetScreenShotDelay(int delay) { screenShotDelay = delay; }
    void SetImmediatePick(bool pick) { startImmediatePick = pick; }
    void SetCopyAfterPick(bool copy) { copyToClipboardAfterPick = copy; }
    void SetQuitAfterPick(bool quit) { quitAfterPick = quit; }
    void SetPixelSize(int pixSize) { pixelSize = pixSize; }
    void SetPixelsPerRow(int pixPerRow) { pixelsPerRow = pixPerRow; }
    void SetDisplayHexString(bool display) { displayHexString = display; }
    void SetDisplayColorFormat(bool display) { displayColorFormat = display; }
    void SetDisplayColorInfoBox(bool display) { displayColorInfoBox = display; }

    bool LoadConfiguration();
    bool SaveConfiguration();
};
