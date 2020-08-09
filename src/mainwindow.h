#pragma once

#include <gtkmm.h>
#include "colorpickerwindow.h"
#include "settingswindow.h"
#include "formateditwindow.h"
#include "models/color.h"
#include "models/colorformatmanager.h"
#include "cfg/config.h"

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

    sigc::connection redScaleSignal;
    sigc::connection greenScaleSignal;
    sigc::connection blueScaleSignal;
    sigc::connection rgbAlphaScaleSignal;
    sigc::connection hslAlphaScaleSignal;
    sigc::connection hueScaleSignal;
    sigc::connection saturationScaleSignal;
    sigc::connection lightnessScaleSignal;
    sigc::connection cyanScaleSignal;
    sigc::connection magentaScaleSignal;
    sigc::connection yellowScaleSignal;
    sigc::connection keyScaleSignal;

protected:
    Glib::RefPtr<Gtk::Application> app;
    Gtk::ToolButton *exitBtn;
    Gtk::ToolButton *settingsBtn;
    Gtk::ToolButton *colorPickerBtn;
    Gtk::Button *editBtn;
    Gtk::Button *clipboardBtn;
    Gtk::Window *window;
    Gtk::DrawingArea *colorArea;
    Gtk::Scale *redScale;
    Gtk::Scale *greenScale;
    Gtk::Scale *blueScale;
    Gtk::Scale *rgbAlphaScale;
    Gtk::Scale *hslAlphaScale;
    Gtk::Scale *hueScale;
    Gtk::Scale *saturationScale;
    Gtk::Scale *lightnessScale;
    Gtk::Scale *cyanScale;
    Gtk::Scale *magentaScale;
    Gtk::Scale *yellowScale;
    Gtk::Scale *keyScale;
    Gtk::ComboBoxText *formatComboBox;

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
    void SyncColorWithUi(const string &colorspace);
    void SyncUiWithColor(Color syncColor);
    void BlockUiSignals();
    void UnblockUiSignals();

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
};
