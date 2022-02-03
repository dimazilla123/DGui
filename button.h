#ifndef DGUI_BUTTON_H
#define DGUI_BUTTON_H

#include "widget.h"
#include "boundered_rect.h"

namespace DGui
{
    class Button : public Widget
    {
    public:
        Button(WidgetManager *manager, const char *text);

        virtual void draw(sf::RenderTarget &render);

        virtual bool onFocusGain() override;
        virtual bool onFocusLose() override;
 
        virtual void moveSelf(int dx, int dy);

        virtual void setSize(int x_length, int y_length);
    protected:
        const char *text;
        sf::Text text_shape;
        BounderedRect rect;
    private:
        void refit();
    };
};

#endif