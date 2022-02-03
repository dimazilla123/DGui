#include "abstract_slider.h"

namespace DGui
{
    Thumb::Thumb(WidgetManager *manager, AbstractSlider *slider)
        : Widget(manager, 2 * radius, 2 * radius),
          slider(slider)
    {
        Size sz = slider->getSize();
        move(sz.w / 2, sz.h / 2);

        sh.setRadius(radius);
        sh.setFillColor(sf::Color::White);
        sh.setOutlineColor(sf::Color::Black);
        sh.setOutlineThickness(2);
    }

    void Thumb::draw(sf::RenderTarget &render)
    {
        render.draw(sh);
    }


    bool Thumb::onMousePress(const Event::MouseButtonStatus &ev)
    {
        if (!grabbed)
        {
            grab_x = ev.x;
            grab_y = ev.y;
            grabbed = true;
        }
        return true;
    }

    bool Thumb::onMouseRelease(const Event::MouseButtonStatus &ev)
    {
        if (grabbed)
        {
            grab_x = grab_y = -1;
            grabbed = false;
        }
        return true;
    }

    bool Thumb::onMouseMove(const Event::MouseMoveStatus &ev)
    {
        if (!grabbed)
            return false;
        int dx = ev.x - grab_x;
        int dy = ev.y - grab_y;

        move(dx, dy);

        grab_x = ev.x;
        grab_y = ev.y;

        moveInBounds(slider->getPositionRect());

        slider->onThumbChange(this);

        return true;
    }

    float Thumb::getValueX() const
    {
        Rect<int> p = slider->getPositionRect();
        return (position.x0 + position.x_length / 2 - p.x0) / ((float)p.x_length);
    }

    float Thumb::getValueY() const
    {
        Rect<int> p = slider->getPositionRect();
        return 1.f - (position.y0 + position.y_length / 2 - p.y0) / ((float)p.y_length);
    }
};