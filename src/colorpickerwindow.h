#pragma once

#include <gtkmm.h>
#include "mainwindow.h"
#include "models/color.h"
#include "cfg/config.h"
#include "utils/enums.h"

class MainWindow;

class ColorPickerWindow : public Gtk::Window
{
private:
    int screenWidth = 0;
    int screenHeight = 0;
    int x = 0;
    int y = 0;
    DisplayServer displayServer = DisplayServer::x11;

    Color color;
    shared_ptr<Config> config;

    int minPixelSize = 2;
    int maxPixelSize = 15;
    int pixelSize = 8;
    int minPixelsPerRow = 1;
    int maxPixelsPerRow = 49;
    int pixelsPerRow = 13;
    int outterBounds = 50;
    int magnifierSize = pixelSize*pixelsPerRow;

    int halfMagnifierWidth() { return (pixelsPerRow-1)/2; };

    void TakeScreenshot();
    void TakeScreenshotFromXorg();
    void TakeScreenshotFromWayland();
    void SetFullscreen();
    void GrabAndHideCursor();
    void SetInitialXAndY();
    void ChangePixelsPerRow(int change);
    void ChangePixelSize(int change);
    void CalculateMagnifierSize();
    void Redraw();
    void FinishPick(bool takeColor);
    void DrawScreenshot(const Cairo::RefPtr<Cairo::Context>& cr);
    void DrawMagnifier(const Cairo::RefPtr<Cairo::Context>& cr);
    void DrawColorInfoBox(const Cairo::RefPtr<Cairo::Context>& cr);

protected:
    Glib::RefPtr<Gtk::Application> app;
    Gtk::DrawingArea *drawingArea;
    Glib::RefPtr<Gdk::Pixbuf> screenshot;
    MainWindow *mainWindow;

    void on_showed();
    bool on_key_pressed(GdkEventKey* event);
    bool on_button_pressed(GdkEventButton* button_event);
    bool on_my_draw(const Cairo::RefPtr<Cairo::Context>& cr);
    bool on_my_motion_notify_event(GdkEventMotion* motion_event);
    bool on_scroll(GdkEventScroll* scroll_event);

    void GetPixelFromPixbuf(double x, double y, Glib::RefPtr<Gdk::Pixbuf> pixbuf, unsigned char *pixels);

public:
    ColorPickerWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
    ~ColorPickerWindow();

    void SetApp(Glib::RefPtr<Gtk::Application> _app);
    void SetMainWindow(MainWindow* _mainWindow);
    void SetConfig(shared_ptr<Config> cfg);
};
