#include <gtkmm.h>
#include <iostream>
#include <utility>
#include "formateditwindow.h"

using namespace std;

FormatEditWindow::FormatEditWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder): Gtk::Dialog(cobject)
{
    refBuilder->get_widget("CloseBtn", closeButton);
    refBuilder->get_widget("SaveBtn", saveButton);
    refBuilder->get_widget("FormatTextView", formatTextView);
    formatTextBuffer = Gtk::TextBuffer::create();
    formatTextView->set_buffer(formatTextBuffer);

    saveButton->signal_clicked().connect(sigc::mem_fun(this, &FormatEditWindow::on_saveButton_clicked) );
    closeButton->signal_clicked().connect(sigc::mem_fun(this, &FormatEditWindow::on_closeButton_clicked) );
}

FormatEditWindow::~FormatEditWindow()
{
    delete saveButton;
    delete closeButton;
    delete formatTextView;
}

void FormatEditWindow::SetFormat(string formats)
{
    oldFormats = std::move(formats);
    formatTextBuffer->set_text(oldFormats);
    cout << oldFormats << endl;
}

string FormatEditWindow::GetFormat()
{
    return newFormats;
}

void FormatEditWindow::on_saveButton_clicked()
{
    newFormats = formatTextBuffer->get_text();
    response(Gtk::RESPONSE_ACCEPT);
}

void FormatEditWindow::on_closeButton_clicked()
{
    response(Gtk::RESPONSE_CANCEL);
}