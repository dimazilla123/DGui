#ifndef DGUI_BOUNDERED_RECT_H
#define DGUI_BOUNDERED_RECT_H

#include <SFML/Graphics.hpp>

#include "style.h"

namespace DGui
{
    class BounderedRect : public sf::Drawable, public sf::Transformable
    {
    public:
        BounderedRect();
        void setSize(int x_length, int y_length);
        void setColor(const sf::Color &c)
        {
            color = c;
            updateColor();
        }
        bool load(const char *tex_name, const Style &st);
    private:

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
        {
            states.transform *= getTransform();
            states.texture = tex;
            target.draw(verteces, states);
        }

        void updateColor();
        void updateSkin();

        sf::Color color;
        const sf::Texture *tex = nullptr;
        sf::IntRect bounds[9] = {};
        int border = 0;
        int x_length = 0, y_length = 0;

        sf::VertexArray verteces;
    };
};

#endif