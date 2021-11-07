#pragma once

#include <gtkmm.h>
#include "colorpickerwindow.h"
#include "settingswindow.h"
#include "formateditwindow.h"
#include "models/color.h"
#include "models/colorformatmanager.h"
#include "models/historymanager.h"
#include "cfg/config.h"
#include "utils/enums.h"

class ColorPickerWindow;
class SettingsWindow;
class FormatEditWindow;

using namespace std;

class MainWindow : public Gtk::Window
{
private:
    bool showColorPickerWindow = false;
    Color color;
    shared_ptr<Config> config;
    ColorPickerWindow *colorPickerWindow;
    SettingsWindow *settingsWindow;
    FormatEditWindow *formatEditWindow;

    string CreateFormat();
    void SaveConfiguration();
    ColorFormatManager colorFormatManager = ColorFormatManager((string)getenv("HOME")+"/.picket/formats");
    HistoryManager historyManager = HistoryManager((string)getenv("HOME")+"/.picket/history");

    sigc::connection redScaleSignal;
    sigc::connection greenScaleSignal;
    sigc::connection blueScaleSignal;
    sigc::connection rgbAlphaScaleSignal;
    sigc::connection hueScaleSignal;
    sigc::connection saturationScaleSignal;
    sigc::connection lightnessScaleSignal;
    sigc::connection hslAlphaScaleSignal;
    sigc::connection cyanScaleSignal;
    sigc::connection magentaScaleSignal;
    sigc::connection yellowScaleSignal;
    sigc::connection keyScaleSignal;

    sigc::connection redEntrySignal;
    sigc::connection greenEntrySignal;
    sigc::connection blueEntrySignal;
    sigc::connection rgbAlphaEntrySignal;
    sigc::connection hueEntrySignal;
    sigc::connection saturationEntrySignal;
    sigc::connection lightnessEntrySignal;
    sigc::connection hslAlphaEntrySignal;
    sigc::connection cyanEntrySignal;
    sigc::connection magentaEntrySignal;
    sigc::connection yellowEntrySignal;
    sigc::connection keyEntrySignal;

protected:
    Glib::RefPtr<Gtk::Application> app;
    Gtk::Window *window;
    Gtk::DrawingArea *colorArea;

    Gtk::Button *settingsBtn;
    Gtk::Button *colorPickerBtn;
    Gtk::Button *editBtn;
    Gtk::Button *clipboardBtn;
    Gtk::ComboBoxText *formatComboBox;

    Gtk::Scale *redScale;
    Gtk::Scale *greenScale;
    Gtk::Scale *blueScale;
    Gtk::Scale *rgbAlphaScale;
    Gtk::Scale *hueScale;
    Gtk::Scale *saturationScale;
    Gtk::Scale *lightnessScale;
    Gtk::Scale *hslAlphaScale;
    Gtk::Scale *cyanScale;
    Gtk::Scale *magentaScale;
    Gtk::Scale *yellowScale;
    Gtk::Scale *keyScale;

    Gtk::Entry *redEntry;
    Gtk::Entry *greenEntry;
    Gtk::Entry *blueEntry;
    Gtk::Entry *alphaEntry;
    Gtk::Entry *hueEntry;
    Gtk::Entry *saturationEntry;
    Gtk::Entry *lightnessEntry;
    Gtk::Entry *hslAlphaEntry;
    Gtk::Entry *cyanEntry;
    Gtk::Entry *magentaEntry;
    Gtk::Entry *yellowEntry;
    Gtk::Entry *keyEntry;

public:
    MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
    ~MainWindow();

    void Show(int response);
    void SetApp(Glib::RefPtr<Gtk::Application> _app);
    void SetConfig(shared_ptr<Config> cfg);
    void SetColorPickerWindow(ColorPickerWindow* _colorPickerWindow);
    void SetSettingsWindow(SettingsWindow* _settingsWindow);
    void SetFormatEditWindow(FormatEditWindow* _formatEditWindow);
    void SetPickedColor(Color pickedColor);
    void PopulateComboWithFormats();
    void InitColorFormatManager();
    void SyncColorWithUi(ColorSpace colorspace, UiEditType editType);
    void SyncUiWithColor(Color syncColor, UiEditType editType);
    void BlockUiSignals();
    void UnblockUiSignals();
    void SanitizeEntryInputs(ColorSpace colorspace);
    void SanitizeEntryInput(Gtk::Entry *entry, int minValue, int maxValue, int defaultValue);

    bool on_colorArea_draw(const Cairo::RefPtr<Cairo::Context>& cr);
    void on_exitButton_clicked();
    void on_settingsButton_clicked();
    void on_colorPickerButton_clicked();
    void on_clipboardButton_clicked();
    void on_editButton_clicked();
    void on_rgb_color_changed();
    void on_hsl_color_changed();
    void on_cmyk_color_changed();
    void on_format_changed();
    bool on_key_pressed(GdkEventKey* event);
    bool on_window_state(GdkEventWindowState *window_state_event);
    void on_rgb_entry_changed();
    void on_hsl_entry_changed();
    void on_cmyk_entry_changed();
    bool on_entry_key_press(GdkEventKey *event);
};
