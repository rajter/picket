#include <iostream>
#include "settingswindow.h"

using namespace std;

SettingsWindow::SettingsWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder): Gtk::Dialog(cobject)
{
    refBuilder->get_widget("CloseBtn", closeButton);
    refBuilder->get_widget("SaveBtn", saveButton);

    saveButton->signal_clicked().connect(sigc::mem_fun(this, &SettingsWindow::on_saveButton_clicked) );
    closeButton->signal_clicked().connect(sigc::mem_fun(this, &SettingsWindow::on_closeButton_clicked) );

    refBuilder->get_widget("Pick1CheckBox", startImmediatePickCheckBox);
    refBuilder->get_widget("Pick2CheckBox", copyToClipboardAfterPickCheckBox);
    refBuilder->get_widget("Pick3CheckBox", quitAfterPickCheckBox);

    refBuilder->get_widget("Display1CheckBox", displayHexStringCheckBox);
    refBuilder->get_widget("Display2CheckBox", displayColorFormatCheckBox);
    refBuilder->get_widget("Display3CheckBox", displayColorInfoBoxCheckBox);

    refBuilder->get_widget("ScreenShotDelaySpinButton", screenshotDelaySpinButton);

    screenshotDelaySpinButton->set_range(0,5000);
    screenshotDelaySpinButton->set_increments(50,50);
}

SettingsWindow::~SettingsWindow()
{
    delete saveButton;
    delete closeButton;
    delete startImmediatePickCheckBox;
    delete copyToClipboardAfterPickCheckBox;
    delete quitAfterPickCheckBox;
    delete displayHexStringCheckBox;
    delete displayColorFormatCheckBox;
    delete displayColorInfoBoxCheckBox;
    delete screenshotDelaySpinButton;

    std::cout << "SettingsWindow destructor called." << std::endl;
}

void SettingsWindow::SetConfig(Config* cfg)
{
    config = cfg;

    startImmediatePickCheckBox->set_active(config->ShouldStartImmediatePick());
    copyToClipboardAfterPickCheckBox->set_active(config->ShouldCopyAfterPick());
    quitAfterPickCheckBox->set_active(config->ShouldQuitAfterPick());
    displayHexStringCheckBox->set_active(config->ShouldDisplayHexString());
    displayColorFormatCheckBox->set_active(config->ShouldDisplayColorFormat());
    displayColorInfoBoxCheckBox->set_active(config->ShouldDisplayColorInfoBox());
    screenshotDelaySpinButton->set_value(config->GetScreenShotDelay());
}

void SettingsWindow::on_saveButton_clicked()
{
    config->SetImmediatePick(startImmediatePickCheckBox->get_active());
    config->SetCopyAfterPick(copyToClipboardAfterPickCheckBox->get_active());
    config->SetQuitAfterPick(quitAfterPickCheckBox->get_active());
    config->SetDisplayHexString(displayHexStringCheckBox->get_active());
    config->SetDisplayColorFormat(displayColorFormatCheckBox->get_active());
    config->SetDisplayColorInfoBox(displayColorInfoBoxCheckBox->get_active());
    config->SetScreenShotDelay((int)screenshotDelaySpinButton->get_value());

    response(Gtk::RESPONSE_ACCEPT);
}

void SettingsWindow::on_closeButton_clicked()
{
    response(Gtk::RESPONSE_CANCEL);
}