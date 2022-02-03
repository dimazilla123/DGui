#include "slider.h"

namespace DGui
{
    Slider::Slider(WidgetManager *manager, int w, int h, Orientation orient)
        : Widget(manager, w, h),
          middle(manager, orient == Horizontal ? w - 20 : h - 20, orient)
    {
        rect.setSize(sf::Vector2f(w, h));
        rect.setTexture(manager->getStyle().getTexture(Style::Textures::ButtonTexture));

        addWidget(&middle);
    }

    void Slider::moveSelf(int dx, int dy)
    {
        rect.move(dx, dy);
    }
};