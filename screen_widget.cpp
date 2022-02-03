#include "screen_widget.h"

namespace DGui
{
    ScreenWidget::ScreenWidget(WidgetManager *manager, int w, int h) : Widget(manager)
    {
        Widget::setSize(w, h);
        rsh.setSize(sf::Vector2f(w, h));
        rsh.setFillColor(sf::Color(60, 60, 60));
    }

    void ScreenWidget::draw(sf::RenderTarget &render)
    {
        render.draw(rsh);
    }

    void ScreenWidget::setSize(int x_length, int y_length)
    {
        Widget::setSize(x_length, y_length);
        rsh.setSize(sf::Vector2f(x_length, y_length));
    }
};
