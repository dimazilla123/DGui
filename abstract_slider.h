#ifndef DGUI_ABSTRACT_SLIDER_H
#define DGUI_ABSTRACT_SLIDER_H

#include "widget.h"
#include "button.h"

#include <SFML/Graphics.hpp>

namespace DGui
{
    class AbstractSlider;

    class Thumb : public Widget
    {
    public:
        Thumb(WidgetManager *manager, AbstractSlider *slider);

        void moveSelf(int dx, int dy)
        {
            sh.move(dx, dy);
        }

        bool onMousePress(const Event::MouseButtonStatus &ev);
        bool onMouseRelease(const Event::MouseButtonStatus &ev);
        bool onMouseMove(const Event::MouseMoveStatus &ev);

        float getValueX() const;
        float getValueY() const;

        void draw(sf::RenderTarget &render);

        static const int radius = 4;
    private:
        AbstractSlider *slider;

        bool grabbed = false;
        int grab_x = -1;
        int grab_y = -1;


        sf::CircleShape sh;
    };

    class AbstractSlider : public Widget
    {
    public:
        AbstractSlider(WidgetManager *manager, int w, int h) : Widget(manager, w, h) {}
        virtual void onThumbChange(Thumb *thumb) {}
    private:
    };
};

#endif