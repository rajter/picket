#pragma once

#include <gtkmm.h>
#include "mainwindow.h"
#include "cfg/config.h"

class MainWindow;

class SettingsWindow : public Gtk::Dialog
{
private:
    shared_ptr<Config> config;
    MainWindow* mainWindow;
    Gtk::Button *saveButton;
    Gtk::Button *closeButton;
    Gtk::CheckButton *startImmediatePickCheckBox;
    Gtk::CheckButton *copyToClipboardAfterPickCheckBox;
    Gtk::CheckButton *quitAfterPickCheckBox;
    Gtk::CheckButton *displayHexStringCheckBox;
    Gtk::CheckButton *displayColorFormatCheckBox;
    Gtk::CheckButton *displayColorInfoBoxCheckBox;
    Gtk::SpinButton *screenshotDelaySpinButton;

public:
    SettingsWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
     ~SettingsWindow();

    void SetConfig(shared_ptr<Config> cfg);

    void on_saveButton_clicked();
    void on_closeButton_clicked();
    void on_showInfoBox_toggled();
};
