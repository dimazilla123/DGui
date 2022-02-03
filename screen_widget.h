#ifndef DGUI_SCREEN_WIDGET
#define DGUI_SCREEN_WIDGET

#include "widget.h"

namespace DGui
{
    class ScreenWidget : public Widget
    {
    public:
        ScreenWidget(WidgetManager *manager, int w, int h);
        void draw(sf::RenderTarget &render);
        void setSize(int x_length, int y_length);
    private:
        sf::RectangleShape rsh;
    };
};

#endif