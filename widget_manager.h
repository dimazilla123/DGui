#ifndef DGUI_WIDGET_MANAGER_H
#define DGUI_WIDGET_MANAGER_H

#include "widget.h"

#include "style.h"

#include "screen_widget.h"

#include <queue>

namespace DGui
{
    class WidgetManager
    {
    public:
        WidgetManager(const char *font_name, int w = 480, int h = 360);
        ~WidgetManager();

        void run();

        //void setRoot(Widget *w) { root = w; };
        void addWidget(Widget *w) { root.addWidget(w); }
        void draw(sf::RenderTarget &render);

        const sf::Font &getFont() { return font; }

        class Mouse
        {
        public:
            static sf::Vector2i getCoords()
            {
                return sf::Vector2i(0, 0); // TODO
            }
        private:
        };

        const Style &getStyle() const
        {
            return style;
        }

        void pushEvent(const Event &ev)
        {
            event_queue.push_back(ev);
        }
    private:

        Event popEvent()
        {
            Event ev = event_queue.front();
            event_queue.pop_front();
            return ev;
        }

        void handleEvent(const sf::Event &ev);


        bool draw_carcas = false;
        void drawCarcas(sf::RenderTarget &render, Widget *w);

        Style style;

        std::deque<Event> event_queue;

        ScreenWidget root;
        sf::Font font;
        sf::RenderWindow window;
    };
};

#endif