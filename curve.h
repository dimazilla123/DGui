#ifndef DGUI_CURVE_H
#define DGUI_CURVE_H

#include "widget.h"
#include "widget_manager.h"

#include "curve/vector2d.h"
#include "abstract_slider.h"

#include "event.h"
#include <vector>
#include <list>

namespace DGui
{
    class Curve : public AbstractSlider
    {
    public:
        Curve(WidgetManager *manager, int w, int h);
        ~Curve();

        void draw(sf::RenderTarget &render);

        int addPoint(const Vector2d &p);
        bool onMouseClick(const Event::MouseButtonStatus &ev);

        void moveSelf(int dx, int dy);

        void onThumbChange(Thumb *th);

        float getValue(float x) const;
    private:

        bool changed = false;
        void redraw();

        sf::Shader shader;

        sf::VertexArray varray;

        class Marker : public Thumb
        {
        public:
            Marker(WidgetManager *manager, AbstractSlider *slider, int point_index)
                : Thumb(manager, slider),
                  point_index(point_index) {}

            #if 1
            void draw(sf::RenderTarget &render)
            {
                Thumb::draw(render);

                sf::Text t;
                t.setFont(manager->getFont());
                t.setCharacterSize(22);
                t.setFillColor(sf::Color::White);
                t.setPosition(position.x0, position.y0);
                char buf[3] = {};
                snprintf(buf, sizeof buf, "%d", point_index);
                t.setString(buf);

                render.draw(t);
            }
            #endif

            int getPointIndex() const
            {
                return point_index;
            }
            void setPointIndex(int index)
            {
                point_index = index;
            }
        private:
            int point_index;
        };

        Vector2d pixel2pos(const sf::Vector2i &v) const;
        Vector2d pixel2pos(int x, int y) const
        {
            return pixel2pos(sf::Vector2i(x, y));
        }
        sf::Vector2f pos2pixel(const Vector2d &v) const;

        std::vector<Vector2d> points;
        //std::list<Marker> thumbs;
    };
};

#endif