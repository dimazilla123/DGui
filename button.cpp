#include "button.h"

#include "widget_manager.h"
#include "dgui_config.h"

#include <cstring>

namespace DGui
{
    Button::Button(WidgetManager *manager, const char *text_content)
        : Widget(manager), text(text_content)
    {
        text_shape.setFont(manager->getFont());
        text_shape.setString(text_content);
        text_shape.setCharacterSize(FONT_SIZE);
        text_shape.setFillColor(sf::Color::White);

        rect.load("button", manager->getStyle());

        position.y_length = 1.5 * FONT_SIZE;
        position.x_length = FONT_SIZE_HOR * std::strlen(text_content);

        rect.setSize(position.x_length, position.y_length);
        rect.setPosition(position.x0, position.y0);

        /*
        rect.setOutlineThickness(1);
        rect.setFillColor(sf::Color::Black);
        rect.setOutlineColor(sf::Color::White);
         */
    }

    bool Button::onFocusGain()
    {
        //rect.setTexture(manager->getStyle().getTexture(Style::ButtonPressedTexture));
        rect.load("button-pressed", manager->getStyle());
        return true;
    }

    bool Button::onFocusLose()
    {
        rect.load("button", manager->getStyle());
        return true;
    }

    void Button::moveSelf(int dx, int dy)
    {
        text_shape.move(dx, dy);
        rect.move(dx, dy);
    }

    void Button::refit()
    {
        int offset_x = (position.x_length - text_shape.getString().getSize() * FONT_SIZE_HOR) / 2;
        int offset_y = (position.y_length - FONT_SIZE) / 2;
        text_shape.setPosition(offset_x + position.x0, offset_y + position.y0);
        rect.setPosition(position.x0, position.y0);
    }

    void Button::setSize(int x_length, int y_length)
    {
        Widget::setSize(x_length, y_length);
        rect.setSize(x_length, y_length);
    }

    void Button::draw(sf::RenderTarget &render)
    {

        render.draw(rect);
        render.draw(text_shape);
    }
};