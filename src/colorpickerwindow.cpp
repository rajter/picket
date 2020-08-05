#include <gtkmm.h>
#include <glib.h>
#include <iostream>
#include <utility>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/X.h>
#include "colorpickerwindow.h"
#include "utils/drawinghelpers.cpp"

ColorPickerWindow::ColorPickerWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder): Gtk::Window(cobject)
{
    Display* display = XOpenDisplay(nullptr);
    Screen*  defaultScreen = DefaultScreenOfDisplay(display);

    screenWidth = defaultScreen->width;
    screenHeight = defaultScreen->height;

    cout << "Screen: " << screenWidth << "x" << screenHeight << endl;

    string xdgSessionType = (string)getenv("XDG_SESSION_TYPE");
    if(xdgSessionType == "x11")
    {
        displayServer = DisplayServer::x11;
        cout << "Display Server: x11" << endl;
    }
    else if(xdgSessionType == "wayland")
    {
        displayServer = DisplayServer::Wayland;
        cout << "Display Server: Wayland" << endl;
    }

    SetFullscreen();

    color = Color(0,0,0);

    this->signal_show().connect(sigc::mem_fun(this, &ColorPickerWindow::on_showed));
    this->signal_key_press_event().connect(sigc::mem_fun(this, &ColorPickerWindow::on_key_pressed), false);
    this->signal_button_press_event().connect(sigc::mem_fun(this, &ColorPickerWindow::on_button_pressed));
    this->signal_scroll_event().connect(sigc::mem_fun(this, &ColorPickerWindow::on_scroll));

    this->set_events(Gdk::EXPOSURE_MASK | Gdk::POINTER_MOTION_MASK | Gdk::POINTER_MOTION_HINT_MASK | Gdk::BUTTON_MOTION_MASK |
            Gdk::BUTTON1_MOTION_MASK | Gdk::BUTTON2_MOTION_MASK | Gdk::BUTTON3_MOTION_MASK | Gdk::BUTTON_PRESS_MASK |
            Gdk::BUTTON_RELEASE_MASK | Gdk::KEY_PRESS_MASK | Gdk::KEY_RELEASE_MASK | Gdk::ENTER_NOTIFY_MASK |
            Gdk::FOCUS_CHANGE_MASK | Gdk::STRUCTURE_MASK | Gdk::PROPERTY_CHANGE_MASK | Gdk::VISIBILITY_NOTIFY_MASK |
            Gdk::PROXIMITY_IN_MASK | Gdk::PROXIMITY_OUT_MASK | Gdk::SUBSTRUCTURE_MASK | Gdk::SCROLL_MASK | Gdk::TOUCH_MASK |
            Gdk::SMOOTH_SCROLL_MASK | Gdk::TOUCHPAD_GESTURE_MASK);

    refBuilder->get_widget("DrawingArea", drawingArea);
    drawingArea->signal_draw().connect(sigc::mem_fun(this, &ColorPickerWindow::on_my_draw));
    drawingArea->signal_motion_notify_event().connect(sigc::mem_fun(this, &ColorPickerWindow::on_my_motion_notify_event));
    drawingArea->add_events(Gdk::KEY_PRESS_MASK | Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_PRESS_MASK);
}

ColorPickerWindow::~ColorPickerWindow()
{
    delete drawingArea;
}

void ColorPickerWindow::SetApp(Glib::RefPtr<Gtk::Application> _app)
{
    app = std::move(_app);
}

void ColorPickerWindow::SetMainWindow(MainWindow* _mainWindow)
{
    mainWindow = _mainWindow;
}

void ColorPickerWindow::SetConfig(shared_ptr<Config> cfg)
{
    config = std::move(cfg);
    pixelSize = config->GetPixelSize();
    pixelsPerRow = config->GetPixelsPerRow();
    CalculateMagnifierSize();
}

void ColorPickerWindow::TakeScreenshot()
{
    if(displayServer == DisplayServer::x11)
        TakeScreenshotFromXorg();
    else if(displayServer == DisplayServer::Wayland)
        TakeScreenshotFromWayland();
}

void ColorPickerWindow::TakeScreenshotFromXorg()
{
    screenshot = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8, screenWidth+outterBounds*2, screenHeight+outterBounds*2);

    auto *display = XOpenDisplay(nullptr);
    auto root = DefaultRootWindow(display);

    XImage *image = XGetImage(display,root, 0,0 , screenWidth, screenHeight, AllPlanes, ZPixmap);

    unsigned char *array = new unsigned char[screenWidth * screenHeight * 3];

    unsigned long red_mask = image->red_mask;
    unsigned long green_mask = image->green_mask;
    unsigned long blue_mask = image->blue_mask;

    for (int xScreen = 0; xScreen < screenWidth; xScreen++)
    {
        for (int yScreen = 0; yScreen < screenHeight ; yScreen++)
        {
            unsigned long pixel = XGetPixel(image, xScreen, yScreen);

            unsigned char blue = pixel & blue_mask;
            unsigned char green = (pixel & green_mask) >> 8;
            unsigned char red = (pixel & red_mask) >> 16;

            array[(xScreen + screenWidth * yScreen) * 3] = red;
            array[(xScreen + screenWidth * yScreen) * 3 + 1] = green;
            array[(xScreen + screenWidth * yScreen) * 3 + 2] = blue;
        }
    }

    auto pic = Gdk::Pixbuf::create_from_data(array, Gdk::Colorspace::COLORSPACE_RGB, false, 8, screenWidth, screenHeight, screenWidth*3);
    pic->copy_area(0,0,screenWidth, screenHeight, screenshot, outterBounds, outterBounds);

    cout << "Xorg screenshot." << endl;
}

void ColorPickerWindow::TakeScreenshotFromWayland()
{
    screenshot = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, screenWidth+outterBounds*2, screenHeight+outterBounds*2);

    string filename = (string)getenv("HOME");
    filename.append("/.picket/screenshot.png");

    Glib::RefPtr<Gio::DBus::Connection> connection = app->get_dbus_connection();

    const Glib::ustring&                object_path = "/org/gnome/Shell/Screenshot";
    const Glib::ustring&                interface_name = "org.gnome.Shell.Screenshot";
    const Glib::ustring&                method_name = "Screenshot";
    const Glib::VariantContainerBase*   parameters = new Glib::VariantContainerBase(g_variant_new("(bbs)", false, false, filename.c_str()));
    const Glib::ustring&                bus_name = "org.gnome.Shell.Screenshot";
    int                                 timeout_msec = -1;
    auto                                flags = Gio::DBus::CALL_FLAGS_NONE;
    const Glib::VariantType&            reply_type = Glib::VariantType();

    connection->call_sync(object_path, interface_name, method_name, *parameters, bus_name, timeout_msec, flags, reply_type);

    Glib::RefPtr<Gdk::Pixbuf> pic = Gdk::Pixbuf::create_from_file(filename);
    pic->copy_area(0, 0, screenWidth, screenHeight, screenshot, outterBounds, outterBounds);

    cout << "Wayland screenshot." << endl;
}

void ColorPickerWindow::SetFullscreen()
{
    this->fullscreen();
    this->set_size_request(screenWidth, screenHeight);
}

void ColorPickerWindow::GrabAndHideCursor()
{
    this->set_modal(true);
    auto display = this->get_display();
    auto window = this->get_window();
    auto screen = this->get_screen();
    display->get_default_seat()->grab(window, Gdk::SEAT_CAPABILITY_ALL, true, Gdk::Cursor::create(display, Gdk::BLANK_CURSOR));
}

void ColorPickerWindow::SetInitialXAndY()
{
    auto display = this->get_display();
    auto device = display->get_default_seat()->get_pointer();
    int initialX = 0;
    int initialY = 0;

    auto screen = this->get_screen();
    device->get_position(screen, initialX, initialY);
    x = initialX;
    y = initialY;
}

void ColorPickerWindow::on_showed()
{
    TakeScreenshot();
    GrabAndHideCursor();
    SetInitialXAndY();
}

bool ColorPickerWindow::on_my_motion_notify_event(GdkEventMotion* motion_event)
{
    x = (int)motion_event->x_root;
    y = (int)motion_event->y_root;

    GetPixelFromPixbuf(x+outterBounds, y+outterBounds, screenshot, screenshot->get_pixels());
    Redraw();

    return true;
}

bool ColorPickerWindow::on_my_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    DrawScreenshot(cr);

    DrawMagnifier(cr);

    if(config->ShouldDisplayColorInfoBox())
        DrawColorInfoBox(cr);

    return true;
}

bool ColorPickerWindow::on_key_pressed(GdkEventKey* event)
{
    // move mouse pointer
    auto display = this->get_display();
    auto device = display->get_default_seat()->get_pointer();

    switch(event->keyval)
    {
        case GDK_KEY_w:
        case GDK_KEY_Up:
            device->warp(this->get_screen(),x,y-1);
            break;
        case GDK_KEY_s:
        case GDK_KEY_Down:
            device->warp(this->get_screen(),x,y+1);
            break;
        case GDK_KEY_a:
        case GDK_KEY_Left:
            device->warp(this->get_screen(),x-1,y);
            break;
        case GDK_KEY_d:
        case GDK_KEY_Right:
            device->warp(this->get_screen(),x+1,y);
            break;
        case GDK_KEY_e:
        case GDK_KEY_KP_Add:
            ChangePixelsPerRow(2);
            break;
        case GDK_KEY_q:
        case GDK_KEY_KP_Subtract:
            ChangePixelsPerRow(-2);
            break;
        case GDK_KEY_space:
        case GDK_KEY_Return:
            FinishPick(true);
            break;
        case GDK_KEY_Escape:
            FinishPick(false);
            break;
        default:
            break;
    }

    Redraw();
    return true;
}

bool ColorPickerWindow::on_button_pressed(GdkEventButton* button_event)
{
    if(button_event->button == 1)
    {
        FinishPick(true);
    }

    return true;
}

bool ColorPickerWindow::on_scroll(GdkEventScroll* scroll_event)
{
    if(scroll_event->direction == GDK_SCROLL_UP)
        ChangePixelSize(1);
    else if(scroll_event->direction == GDK_SCROLL_DOWN)
        ChangePixelSize(-1);

    Redraw();
    return true;
}

void ColorPickerWindow::DrawScreenshot(const Cairo::RefPtr<Cairo::Context>& cr)
{
    Gdk::Cairo::set_source_pixbuf(cr, screenshot, -outterBounds, -outterBounds);
    cr->paint();
}

void ColorPickerWindow::DrawMagnifier(const Cairo::RefPtr<Cairo::Context>& cr)
{
    // draw magnified image
    Glib::RefPtr<Gdk::Pixbuf> subImage = Gdk::Pixbuf::create_subpixbuf(screenshot, x+outterBounds-pixelsPerRow/2, y+outterBounds-pixelsPerRow/2, pixelsPerRow, pixelsPerRow);
    Glib::RefPtr<Gdk::Pixbuf> scaledImage = subImage->scale_simple(magnifierSize, magnifierSize, Gdk::INTERP_NEAREST);
    Gdk::Cairo::set_source_pixbuf(cr, scaledImage, x-magnifierSize/2, y-magnifierSize/2);
    cr->rectangle(x-magnifierSize/2,y-magnifierSize/2,magnifierSize,magnifierSize);
    cr->fill();

    // draw magnifier outline
    DrawingHelpers::DrawRectangleOutline(x, y, magnifierSize, magnifierSize, cr);

    // draw center pixel
    DrawingHelpers::FillCenterRectangle(x, y, pixelSize, pixelSize, &color, cr);
    DrawingHelpers::DrawRectangleOutline(x, y, pixelSize, pixelSize, cr);
}

void ColorPickerWindow::DrawColorInfoBox(const Cairo::RefPtr<Cairo::Context>& cr)
{
    // calculate variables for info text
    auto backColor = Color(0,0,0,175);
    auto textColor = Color(255,255,255,255);
    int infoX = x;
    int infoY = y+magnifierSize/2+50;

    if(infoY+(75/2) > screenHeight)
    {
        infoY = y-magnifierSize/2-50;
    }
    if((x-100) < 0)
    {
        infoX = x+(100-x);
    }
    if((x+100) > screenWidth)
    {
        infoX = x-(100-(screenWidth-x));
    }

    int textSize = 16;
    int textX = infoX-25;
    int textY = infoY+textSize*2-2;

    // draw transparent background rectangle
    DrawingHelpers::FillCenterRectangle(infoX, infoY, 200, 75, &backColor, cr);

    // draw center color rectangle
    DrawingHelpers::FillCenterRectangle(infoX-60, infoY, 60, 60, &color, cr);
    DrawingHelpers::DrawRectangleOutline(infoX-60, infoY, 60, 60, cr);

    // draw info text
    string rh = "R:"+ to_string(color.GetRed()) + " H:" + to_string(color.GetHue()) + "°";
    string gs = "G:"+ to_string(color.GetGreen()) + " S:" + to_string(color.GetSaturation()) + "%";
    string bl = "B:"+ to_string(color.GetBlue()) + " L:" + to_string(color.GetLightness()) + "%";
    DrawingHelpers::DrawText(textX, textY-textSize*3, rh, "Monospace", textSize, Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD, &textColor, cr);
    DrawingHelpers::DrawText(textX, textY-textSize*2, gs, "Monospace", textSize, Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD, &textColor, cr);
    DrawingHelpers::DrawText(textX, textY-textSize, bl, "Monospace", textSize, Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD, &textColor, cr);
    DrawingHelpers::DrawText(textX, textY, color.GetHexString(), "Monospace", textSize, Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD, &textColor, cr);
}

void ColorPickerWindow::GetPixelFromPixbuf(double xPixel, double yPixel, const Glib::RefPtr<Gdk::Pixbuf> &pixbuf, unsigned char *pixels)
{
    guchar *p;
    p = pixels + ((int)yPixel) * pixbuf->get_rowstride() + ((int)xPixel) * pixbuf->get_n_channels();

    color.SetRGB((p[0]<<8) / 256, (p[1]<<8) / 256, (p[2]<<8) / 256);
}

void ColorPickerWindow::ChangePixelsPerRow(int change)
{
    auto newPixelsPerRow = pixelsPerRow + change;
    if(newPixelsPerRow > maxPixelsPerRow)
        pixelsPerRow = maxPixelsPerRow;
    else if(newPixelsPerRow < minPixelsPerRow)
        pixelsPerRow = minPixelsPerRow;
    else
        pixelsPerRow = newPixelsPerRow;

    CalculateMagnifierSize();
}

void ColorPickerWindow::ChangePixelSize(int change)
{
    auto newPixelSize = pixelSize + change;
    if(newPixelSize > maxPixelSize)
        pixelSize = maxPixelSize;
    else if(newPixelSize < minPixelSize)
        pixelSize = minPixelSize;
    else
        pixelSize = newPixelSize;

    CalculateMagnifierSize();
}

void ColorPickerWindow::CalculateMagnifierSize()
{
    magnifierSize = pixelSize*pixelsPerRow;
}

void ColorPickerWindow::Redraw()
{
    drawingArea->queue_draw();
}

void ColorPickerWindow::FinishPick(bool takeColor)
{
    auto display = this->get_display();
    display->get_default_seat()->ungrab();

    config->SetPixelSize(pixelSize);
    config->SetPixelsPerRow(pixelsPerRow);

    if(takeColor)
    {
        mainWindow->SetPickedColor(color);
        mainWindow->Show(Gtk::RESPONSE_ACCEPT);
    }
    else
    {
        mainWindow->Show(Gtk::RESPONSE_CANCEL);
    }

    app->hold();
    close();
}