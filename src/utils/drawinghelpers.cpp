#include <string>
#include <gtkmm.h>
#include "../models/color.h"

using namespace std;

namespace DrawingHelpers
{
    static void DrawRectangleOutline(double x, double y, double width, double height, const Cairo::RefPtr<Cairo::Context>& cr)
    {
        cr->set_line_width(1);
        cr->set_source_rgba(0.3,0.3,0.3,1);
        cr->rectangle(x-width/2-1,y-height/2-1,width+2,height+2);
        cr->stroke();

        cr->set_line_width(2);
        cr->set_source_rgba(1,1,1,1);
        cr->rectangle(x-width/2,y-height/2,width,height);
        cr->stroke();
    }

    static void FillRectangle(double x, double y, double width, double height, Color *color, const Cairo::RefPtr<Cairo::Context>& cr)
    {
        cr->set_source_rgba(color->GetRedAsDouble(), color->GetGreenAsDouble(), color->GetBlueAsDouble(), color->GetAlphaAsDouble());
        cr->rectangle(x,y,width,height);
        cr->fill();
    }

    static void FillCenterRectangle(double x, double y, double width, double height, Color *color, const Cairo::RefPtr<Cairo::Context>& cr)
    {
        cr->set_source_rgba(color->GetRedAsDouble(), color->GetGreenAsDouble(), color->GetBlueAsDouble(), color->GetAlphaAsDouble());
        cr->rectangle(x-width/2,y-height/2,width,height);
        cr->fill();
    }

    static void DrawText(double x, double y, string text, string font, double fontSize, Cairo::FontSlant fontslant, Cairo::FontWeight fontWeight, Color *color, const Cairo::RefPtr<Cairo::Context>& cr)
    {
        cr->select_font_face(font, fontslant, fontWeight);
        cr->set_font_size(fontSize);
        cr->set_source_rgba(color->GetRedAsDouble(), color->GetGreenAsDouble(), color->GetBlueAsDouble(), color->GetAlphaAsDouble());
        cr->move_to(x,y);
        cr->show_text(text);
    }

    static void DrawCenterText(double x, double y, const string text, const string font, double fontSize, Cairo::FontSlant fontslant, Cairo::FontWeight fontWeight, Color *color, const Cairo::RefPtr<Cairo::Context>& cr)
    {
        Cairo::TextExtents extents;

        cr->select_font_face(font, fontslant, fontWeight);
        cr->set_font_size(fontSize);
        cr->get_text_extents(text, extents);
        cr->set_source_rgba(color->GetRedAsDouble(), color->GetGreenAsDouble(), color->GetBlueAsDouble(), color->GetAlphaAsDouble());
        cr->move_to(x-extents.width/2,y-extents.height/2);
        cr->show_text(text);
    }
};