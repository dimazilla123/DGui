#ifndef DGUI_TEXTBOX_H
#define DGUI_TEXTBOX_H

#include "widget.h"

namespace DGui
{
    class TextBox : public Widget
    {
    public:
        TextBox(WidgetManager *manager, const char *text);
        virtual void draw(sf::RenderTarget &render);
    private:
        const char *text;
        sf::Text text_shape;
    };
};

#endif