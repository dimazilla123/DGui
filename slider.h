#ifndef DGUI_SLIDER_H
#define DGUI_SLIDER_H

#include "abstract_slider.h"
#include "widget_manager.h"

namespace DGui
{
    class Slider : public Widget
    {
    public:
        enum Orientation
        {
            Horizontal,
            Vertical
        };

        Slider(WidgetManager *manager, int w, int h, Orientation orient);

        void onThumbChange(const Thumb *thumb) {}

        void moveSelf(int dx, int dy);

        void draw(sf::RenderTarget &render) {}
    private:

        struct LinearSlider : public AbstractSlider
        {
            LinearSlider(WidgetManager *manager, int l, Orientation orient)
                : AbstractSlider(manager, (orient == Horizontal ? l : 2 * Thumb::radius),
                                          (orient == Vertical   ? l : 2 * Thumb::radius)),
                  orientation(orient),
                  thumb(manager, this)
            {
                line[0].position = sf::Vector2f(Thumb::radius, Thumb::radius);
                switch (orient)
                {
                    case Horizontal:
                    {
                        line[1].position = sf::Vector2f(l, position.y_length / 2);
                    } break;
                    case Vertical:
                    {
                        line[1].position = sf::Vector2f(position.x_length / 2, l);
                    } break;
                }
                addWidget(&thumb);
            }

            void moveSelf(int dx, int dy)
            {
                for (int i = 0; i < sizeof(line) / sizeof(line[0]); ++i)
                {
                    line[i].position.x += dx;
                    line[i].position.y += dy;
                }
            }

            void draw(sf::RenderTarget &render)
            {
                render.draw(&line[0], sizeof(line) / sizeof(line[0]), sf::LineStrip);
            }

            void onThumbChange(Thumb *thumb)
            {
                if (orientation == Vertical)
                    value = thumb->getValueX();
                else
                    value = thumb->getValueY();
            }

            Orientation orientation;
            sf::Vertex line[2] = {};

            float value = 0.5;
            Thumb thumb;
        };

        sf::RectangleShape rect;

        LinearSlider middle;
    };
};

#endif