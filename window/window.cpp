#include "window.h"

#include <cassert>

namespace DGui
{
    Window::Window(WidgetManager *manager, const char *title, int w, int h)
        : Widget(manager, w, h),
          bar(manager, this, title, w + border_size * 2)
    {
        //w = position.x0 += 0;
        //h = position.y_length += bar.getPositionRect().y_length;

        Widget::addWidget(&bar);

        /*
        sh.setFillColor(sf::Color::White);
        sh.setOutlineThickness(1);
        sh.setOutlineColor(sf::Color::Black);
         */
        const Style &st = manager->getStyle();

        /*
            0  x0 x1 x2

             _______  0
            /_|___|_\  y0
            | |   | |  
            |_|___|_|  y1
            \_|___|_/  y2
         */

        #if 0
        sf::IntRect rect = {};
        sh.setTexture(st.getTextureWithBounds("window-i", rect));
        sh.setTextureRect(rect);
        #else
        sh.load("window", st);
        #endif

        setSize(position.x_length, position.y_length);
    }

    void Window::moveSelf(int dx, int dy)
    {
        sh.move(dx, dy);
    }

    void Window::setSize(int x_length, int y_length)
    {
        y_length += bar.getSize().h;
        Widget::setSize(x_length, y_length);
        bar.setSize(x_length, bar.getMinimumSize().h);

        sh.setSize(x_length, y_length);
    }

    void Window::draw(sf::RenderTarget &render)
    {
        render.draw(sh);
    }

    void Window::addWidget(Widget *w)
    {
        Widget::addWidget(w);
        w->move(0, bar.getSize().h);
        *last() = &bar;
        *(--last()) = w;
    }

    bool Window::onClose()
    {
        getParent()->erase(this);
        return true;
    }

    void Window::setTitle(const char *s)
    {
        bar.setText(s);
    }

    /*
    void Window::onMousePress(const DGui::Event &ev)
    {

    }

    void Window::onMouseRelease(const DGui::Event &ev)
    {

    }

    void Window::onMouseMove(const DGui::Event &ev)
    {

    }
     */
};