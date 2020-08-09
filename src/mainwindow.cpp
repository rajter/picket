#include <gtkmm.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <utility>
#include "colorpickerwindow.h"
#include "formateditwindow.h"
#include "models/color.h"
#include "cfg/config.h"
#include "utils/drawinghelpers.cpp"

using namespace std;

MainWindow::MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder): Gtk::Window(cobject)
{
    refBuilder->get_widget("ExitBtn", exitBtn);
    refBuilder->get_widget("SettingsBtn", settingsBtn);
    refBuilder->get_widget("ColorPickerBtn", colorPickerBtn);
    refBuilder->get_widget("ClipboardBtn", clipboardBtn);
    refBuilder->get_widget("EditBtn", editBtn);
    refBuilder->get_widget("ColorArea", colorArea);
    refBuilder->get_widget("RedScale", redScale);
    refBuilder->get_widget("GreenScale", greenScale);
    refBuilder->get_widget("BlueScale", blueScale);
    refBuilder->get_widget("RgbAlphaScale", rgbAlphaScale);
    refBuilder->get_widget("HslAlphaScale", hslAlphaScale);
    refBuilder->get_widget("HueScale", hueScale);
    refBuilder->get_widget("SaturationScale", saturationScale);
    refBuilder->get_widget("LightnessScale", lightnessScale);
    refBuilder->get_widget("LightnessScale", cyanScale);
    refBuilder->get_widget("CyanScale", cyanScale);
    refBuilder->get_widget("MagentaScale", magentaScale);
    refBuilder->get_widget("YellowScale", yellowScale);
    refBuilder->get_widget("KeyScale", keyScale);
    refBuilder->get_widget("FormatComboBox", formatComboBox);

    redScale->set_range(0, 255);
    blueScale->set_range(0, 255);
    greenScale->set_range(0, 255);
    rgbAlphaScale->set_range(0, 255);
    rgbAlphaScale->set_value(255);
    hslAlphaScale->set_range(0, 255);
    hslAlphaScale->set_value(255);
    hueScale->set_range(0, 360);
    saturationScale->set_range(0, 100);
    lightnessScale->set_range(0, 100);
    cyanScale->set_range(0, 100);
    magentaScale->set_range(0, 100);
    yellowScale->set_range(0, 100);
    keyScale->set_range(0, 100);
    redScaleSignal = redScale->signal_value_changed().connect(sigc::mem_fun(this, &MainWindow::on_rgb_color_changed));
    greenScaleSignal = greenScale->signal_value_changed().connect(sigc::mem_fun(this, &MainWindow::on_rgb_color_changed));
    blueScaleSignal = blueScale->signal_value_changed().connect(sigc::mem_fun(this, &MainWindow::on_rgb_color_changed));
    rgbAlphaScaleSignal = rgbAlphaScale->signal_value_changed().connect(sigc::mem_fun(this, &MainWindow::on_rgb_color_changed));
    hueScaleSignal = hueScale->signal_value_changed().connect(sigc::mem_fun(this, &MainWindow::on_hsl_color_changed));
    saturationScaleSignal = saturationScale->signal_value_changed().connect(sigc::mem_fun(this, &MainWindow::on_hsl_color_changed));
    lightnessScaleSignal = lightnessScale->signal_value_changed().connect(sigc::mem_fun(this, &MainWindow::on_hsl_color_changed));
    hslAlphaScaleSignal = hslAlphaScale->signal_value_changed().connect(sigc::mem_fun(this, &MainWindow::on_hsl_color_changed));
    cyanScaleSignal = cyanScale->signal_value_changed().connect(sigc::mem_fun(this, &MainWindow::on_cmyk_color_changed));
    magentaScaleSignal = magentaScale->signal_value_changed().connect(sigc::mem_fun(this, &MainWindow::on_cmyk_color_changed));
    yellowScaleSignal = yellowScale->signal_value_changed().connect(sigc::mem_fun(this, &MainWindow::on_cmyk_color_changed));
    keyScaleSignal = keyScale->signal_value_changed().connect(sigc::mem_fun(this, &MainWindow::on_cmyk_color_changed));

    colorArea->signal_draw().connect(sigc::mem_fun(this, &MainWindow::on_colorArea_draw));
    exitBtn->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_exitButton_clicked) );
    settingsBtn->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_settingsButton_clicked) );
    colorPickerBtn->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_colorPickerButton_clicked) );
    clipboardBtn->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_clipboardButton_clicked) );
    editBtn->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_editButton_clicked) );
    formatComboBox->signal_changed().connect(sigc::mem_fun(this, &MainWindow::on_format_changed));

    signal_key_press_event().connect(sigc::mem_fun(this, &MainWindow::on_key_pressed), false);
    signal_window_state_event().connect(sigc::mem_fun(this, &MainWindow::on_window_state), false);

    set_title("Picket");
    set_size_request(100, 350);
    color = Color(0,0,0);
}

MainWindow::~MainWindow()
{
    delete exitBtn;
    delete settingsBtn;
    delete colorPickerBtn;
    delete editBtn;
    delete clipboardBtn;
    delete colorArea;
    delete redScale;
    delete greenScale;
    delete blueScale;
    delete rgbAlphaScale;
    delete hslAlphaScale;
    delete hueScale;
    delete saturationScale;
    delete lightnessScale;
    delete cyanScale;
    delete magentaScale;
    delete yellowScale;
    delete keyScale;
    delete formatComboBox;
}

string MainWindow::CreateFormat()
{
    colorFormatManager.SetVariables(color.GetVariables());
    string selectedFormat = formatComboBox->get_active_text();
    string format = colorFormatManager.GetFormat(selectedFormat);

    return format;
}

void MainWindow::SaveConfiguration()
{
    bool configurationSaved = config->SaveConfiguration();

    if(configurationSaved)
        cout << "Configuration saved!" << endl;
    else
        cout << "Error is saving configuration." << endl;
}

void MainWindow::SetApp(Glib::RefPtr<Gtk::Application> _app)
{
    this->app = std::move(_app);
}

void MainWindow::SetConfig(shared_ptr<Config> cfg)
{
    config = std::move(cfg);
}

void MainWindow::SetColorPickerWindow(ColorPickerWindow* _colorPickerWindow)
{
    colorPickerWindow = _colorPickerWindow;
}

void MainWindow::SetSettingsWindow(SettingsWindow* _settingsWindow)
{
    settingsWindow = _settingsWindow;
}

void MainWindow::SetFormatEditWindow(FormatEditWindow* _formatEditWindow)
{
    formatEditWindow = _formatEditWindow;
}

void MainWindow::InitColorFormatManager()
{
    colorFormatManager.LoadFormats();
}

void MainWindow::PopulateComboWithFormats()
{
    formatComboBox->remove_all();
    vector<string> formatKeys = colorFormatManager.GetFormatsKeys();
    for(string &value: formatKeys)
    {
        formatComboBox->append(value);
    }
    formatComboBox->set_active(config->GetLastFormat());
}

void MainWindow::Show(int response)
{
    if(response == Gtk::RESPONSE_ACCEPT)
    {
        if(config->ShouldQuitAfterPick())
            on_exitButton_clicked();
        else
            present();
    }
    else if(response == Gtk::RESPONSE_CANCEL)
    {
        present();
    }
}

void MainWindow::SetPickedColor(Color pickedColor)
{
    pickedColor.Log();
    BlockUiSignals();
    SyncUiWithColor(pickedColor);
    UnblockUiSignals();
    color.SetRGB(pickedColor.GetRed(), pickedColor.GetGreen(), pickedColor.GetBlue());

    if(config->ShouldCopyAfterPick())
        on_clipboardButton_clicked();
}

void MainWindow::SyncColorWithUi(const string &colorspace)
{
    if(colorspace == "RGB")
    {
        color.SetRGB(static_cast<int>(redScale->get_value()), static_cast<int>(greenScale->get_value()), static_cast<int>(blueScale->get_value()), static_cast<int>(rgbAlphaScale->get_value()));
    }
    else if(colorspace == "HSL")
    {
        color.SetHSL(static_cast<int>(hueScale->get_value()), static_cast<int>(saturationScale->get_value()), static_cast<int>(lightnessScale->get_value()));
    }
    else if(colorspace == "CMYK")
    {
        color.SetCmyk(static_cast<int>(cyanScale->get_value()), static_cast<int>(magentaScale->get_value()), static_cast<int>(yellowScale->get_value()), static_cast<int>(keyScale->get_value()));
    }

    colorArea->queue_draw();
}

void MainWindow::SyncUiWithColor(Color syncColor)
{
    redScale->set_value(syncColor.GetRed());
    greenScale->set_value(syncColor.GetGreen());
    blueScale->set_value(syncColor.GetBlue());
    rgbAlphaScale->set_value(syncColor.GetAlpha());
    hueScale->set_value(syncColor.GetHue());
    saturationScale->set_value(syncColor.GetSaturation());
    lightnessScale->set_value(syncColor.GetLightness());
    hslAlphaScale->set_value(syncColor.GetAlpha());
    cyanScale->set_value(syncColor.GetCyan());
    magentaScale->set_value(syncColor.GetMagenta());
    yellowScale->set_value(syncColor.GetYellow());
    keyScale->set_value(syncColor.GetKey());

    colorArea->queue_draw();
}

void MainWindow::BlockUiSignals()
{
    redScaleSignal.block();
    greenScaleSignal.block();
    blueScaleSignal.block();
    rgbAlphaScaleSignal.block();
    hueScaleSignal.block();
    saturationScaleSignal.block();
    lightnessScaleSignal.block();
    hslAlphaScaleSignal.block();
    cyanScaleSignal.block();
    magentaScaleSignal.block();
    yellowScaleSignal.block();
    keyScaleSignal.block();
}

void MainWindow::UnblockUiSignals()
{
    redScaleSignal.unblock();
    greenScaleSignal.unblock();
    blueScaleSignal.unblock();
    rgbAlphaScaleSignal.unblock();
    hueScaleSignal.unblock();
    saturationScaleSignal.unblock();
    lightnessScaleSignal.unblock();
    hslAlphaScaleSignal.unblock();
    cyanScaleSignal.unblock();
    magentaScaleSignal.unblock();
    yellowScaleSignal.unblock();
    keyScaleSignal.unblock();
}

void MainWindow::on_rgb_color_changed()
{
    BlockUiSignals();
    SyncColorWithUi("RGB");
    SyncUiWithColor(color);
    UnblockUiSignals();
}

void MainWindow::on_hsl_color_changed()
{
    BlockUiSignals();
    SyncColorWithUi("HSL");
    SyncUiWithColor(color);
    UnblockUiSignals();
}

void MainWindow::on_cmyk_color_changed()
{
    BlockUiSignals();
    SyncColorWithUi("CMYK");
    SyncUiWithColor(color);
    UnblockUiSignals();
}

bool MainWindow::on_colorArea_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    cr->set_source_rgba(color.GetRedAsDouble(), color.GetGreenAsDouble(), color.GetBlueAsDouble(), color.GetAlphaAsDouble());
    cr->paint();

    auto textColor = color.GetContrastColor();
    int x = colorArea->get_width() - 110;
    int y = colorArea->get_height() - 7;

    if(config->ShouldDisplayHexString())
        DrawingHelpers::DrawText(x, 25, color.GetHexString(), "Monospace", 18, Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD, &textColor, cr);
    if(config->ShouldDisplayColorFormat())
        DrawingHelpers::DrawText(10, y, CreateFormat(), "Monospace", 18, Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD, &textColor, cr);

    return true;
}

void MainWindow::on_exitButton_clicked()
{
    SaveConfiguration();
    app->quit();
}

void MainWindow::on_settingsButton_clicked()
{
    int result = settingsWindow->run();
    if(result == Gtk::RESPONSE_ACCEPT)
    {
        SaveConfiguration();
        colorPickerWindow->SetConfig(config);
        queue_draw();
    }

    settingsWindow->SetConfig(config);
    settingsWindow->close();
}

void MainWindow::on_colorPickerButton_clicked()
{
    showColorPickerWindow = true;
    iconify();
}

void MainWindow::on_clipboardButton_clicked()
{
    string format = CreateFormat();

    Glib::RefPtr<Gtk::Clipboard> refClipboard = Gtk::Clipboard::get();
    refClipboard->set_text(format);

    auto Icon = Gio::ThemedIcon::create("color-picker");
    auto Notification = Gio::Notification::create(format);
    Notification->set_body(formatComboBox->get_active_text());
    Notification->set_icon (Icon);
    app->send_notification(Notification);
}

void MainWindow::on_editButton_clicked()
{
    int activeFormatId = formatComboBox->get_active_row_number();
    formatEditWindow->SetFormat(colorFormatManager.FormatsString());
    int result = formatEditWindow->run();
    if(result == Gtk::RESPONSE_ACCEPT)
    {
        colorFormatManager.SaveFormatFile(formatEditWindow->GetFormat());
        PopulateComboWithFormats();
        formatComboBox->set_active(activeFormatId > -1 ? activeFormatId : 0);
    }

    formatEditWindow->close();
}

void MainWindow::on_format_changed()
{
    config->SetLastFormat(formatComboBox->get_active_row_number());
    queue_draw();
}

bool MainWindow::on_key_pressed(GdkEventKey* event)
{
    switch(event->keyval)
    {
        case GDK_KEY_p:
        case GDK_KEY_space:
            on_colorPickerButton_clicked();
            break;
        case GDK_KEY_1:
            formatComboBox->set_active(0); queue_draw();
            break;
        case GDK_KEY_2:
            formatComboBox->set_active(1); queue_draw();
            break;
        case GDK_KEY_3:
            formatComboBox->set_active(2); queue_draw();
            break;
        case GDK_KEY_4:
            formatComboBox->set_active(3); queue_draw();
            break;
        case GDK_KEY_5:
            formatComboBox->set_active(4); queue_draw();
            break;
        case GDK_KEY_6:
            formatComboBox->set_active(5); queue_draw();
            break;
        case GDK_KEY_7:
            formatComboBox->set_active(6); queue_draw();
            break;
        case GDK_KEY_8:
            formatComboBox->set_active(7); queue_draw();
            break;
        case GDK_KEY_9:
            formatComboBox->set_active(8); queue_draw();
            break;
        case GDK_KEY_0:
            formatComboBox->set_active(9); queue_draw();
            break;
        case GDK_KEY_Tab:
            on_clipboardButton_clicked();
            break;
        case GDK_KEY_q:
            on_exitButton_clicked();
            break;
        default:
            break;
    }

    return true;
}

bool MainWindow::on_window_state(GdkEventWindowState *window_state_event)
{
    if((window_state_event->changed_mask & GdkWindowState::GDK_WINDOW_STATE_ICONIFIED) == GdkWindowState::GDK_WINDOW_STATE_ICONIFIED)
    {
        if(showColorPickerWindow)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(config->GetScreenShotDelay()));
            colorPickerWindow->present();
            showColorPickerWindow = false;
        }
    }

    return true;
}