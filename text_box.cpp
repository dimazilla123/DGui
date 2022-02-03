#include "text_box.h"
#include "dgui_config.h"

#include "widget_manager.h"

#include <cstring>

namespace DGui
{
    TextBox::TextBox(WidgetManager *manager, const char *text_content)
        : Widget(manager), text(text_content)
    {
        text_shape.setFont(manager->getFont());
        text_shape.setString(text_content);
        text_shape.setCharacterSize(FONT_SIZE);
        text_shape.setFillColor(sf::Color::White);

        position.y_length = FONT_SIZE;
        position.x_length = FONT_SIZE_HOR * std::strlen(text_content);
    }
    void TextBox::draw(sf::RenderTarget &render)
    {
        // TODO:
        sf::RectangleShape rect;
        rect.setSize(sf::Vector2f{(float)position.x_length, (float)position.y_length});
        rect.setPosition(position.x0, position.y0);
        rect.setOutlineThickness(1);
        rect.setFillColor(sf::Color::Black);
        rect.setOutlineColor(sf::Color::White);
        render.draw(rect);

        text_shape.setPosition(position.x0, position.y0);
        render.draw(text_shape);
    }
};