#include <gtkmm.h>
#include <memory>
#include "colorpickerwindow.h"
#include "mainwindow.h"
#include "formateditwindow.h"
#include "settingswindow.h"
#include "cfg/config.h"
#include "utils/dirutils.h"
#include "cfg/paths.h"

using namespace std;

int main(int argc, char *argv[])
{
    MainWindow *window;
    ColorPickerWindow *colorPickerWindow;
    SettingsWindow *settingsWindow;
    FormatEditWindow *formatEditWindow;
    string userDirectory = (string)getenv("HOME")+"/.picket";

    if(!DirUtilities::DirectoryExists(userDirectory))
        DirUtilities::CreateUserDirectory(userDirectory);

    shared_ptr<Config> config = make_shared<Config>();
    config->LoadConfiguration();

    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.examples.base");
    auto mainWinBuilder = Gtk::Builder::create_from_file(DATADIR "/picket/MainWindow.glade");
    auto colorPickerWinBuilder = Gtk::Builder::create_from_file(DATADIR "/picket/ColorPickerWindow.glade");
    auto settingWindowBuilder = Gtk::Builder::create_from_file(DATADIR "/picket/SettingsWindow.glade");
    auto formatEditWindowBuilder = Gtk::Builder::create_from_file(DATADIR "/picket/FormatEditWindow.glade");

    mainWinBuilder->get_widget_derived("MainWindow", window);
    colorPickerWinBuilder->get_widget_derived("ColorPickerWindow", colorPickerWindow);
    settingWindowBuilder->get_widget_derived("SettingsWindow", settingsWindow);
    formatEditWindowBuilder->get_widget_derived("FormatEditWindow", formatEditWindow);

    window->SetApp(app);
    window->SetConfig(config);
    window->InitColorFormatManager();
    window->PopulateComboWithFormats();
    window->SetColorPickerWindow(colorPickerWindow);
    window->SetFormatEditWindow(formatEditWindow);

    colorPickerWindow->SetApp(app);
    colorPickerWindow->SetMainWindow(window);
    colorPickerWindow->SetConfig(config);

    settingsWindow->SetConfig(config);
    window->SetSettingsWindow(settingsWindow);

    if(config->ShouldStartImmediatePick())
        app->run(*colorPickerWindow);
    else
        app->run(*window);

    delete window;
    delete settingsWindow;
    delete colorPickerWindow;
    delete formatEditWindow;

    return 0;
}
