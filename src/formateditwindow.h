#pragma once

#include <gtkmm.h>
#include "mainwindow.h"

class MainWindow;

class FormatEditWindow : public Gtk::Dialog
{
private:
    MainWindow* mainWindow;
    Gtk::Button *saveButton;
    Gtk::Button *closeButton;
    Gtk::TextView *formatTextView;
    Glib::RefPtr<Gtk::TextBuffer> formatTextBuffer;

    string oldFormats;
    string newFormats;

public:
    FormatEditWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);

    void SetFormat(string formats);
    string GetFormat();

    void on_saveButton_clicked();
    void on_closeButton_clicked();
};
