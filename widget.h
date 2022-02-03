#ifndef DGUI_WIDGET_H
#define DGUI_WIDGET_H

#include <SFML/Graphics.hpp>

#include "event.h"

#include <cassert>

namespace DGui
{
    template<typename T>
    struct Rect
    {
        T x0, y0, x_length, y_length;
    };

    class WidgetManager;
    class Widget;

    typedef void (*CallbackPtr)(Widget *w, const Event &ev);

    class Widget
    {
    public:
        virtual void draw(sf::RenderTarget &render) {}; // draws itself

        void dump(FILE *f) const;

        bool handleEvent(const Event &ev);
        void addWidget(Widget *child);

        virtual bool onMouseClick(const Event::MouseButtonStatus &ev) { return true; }
        virtual bool onMousePress(const Event::MouseButtonStatus &ev) { return true; }
        virtual bool onMouseRelease(const Event::MouseButtonStatus &ev) { return true; }
        virtual bool onMouseMove(const Event::MouseMoveStatus &ev) { return false; }

        virtual bool onFocusGain() { return true; }
        virtual bool onFocusLose() { return true; }

        virtual bool onClose() { return false; }

        virtual void refit();

        struct Size
        {
            int w, h;
        };

        Size getSize() const
        {
            return {position.x_length, position.y_length};
        }

        virtual Size getMinimumSize() const
        {
            return getSize();
        }

        void setMinimumSize()
        {
            Size s = getMinimumSize();
            setSize(s.w, s.h);
        }

        void setPosition(int x0, int y0)
        {
            move(x0 - position.x0, y0 - position.y0);
        }

        void move(int dx, int dy)
        {
            position.x0 += dx;
            position.y0 += dy;
            moveSelf(dx, dy);
            Iterator it = begin();
            while (it != end())
            {
                (*it)->move(dx, dy);
                ++it;
            }
        }

        virtual void setSize(int x_length, int y_length)
        {
            position.x_length = x_length;
            position.y_length = y_length;
        }

        Rect<int> getPositionRect() const
        {
            return position;
        }

        bool isInBonds(int x, int y) const
        {
            return position.x0 <= x && x <= position.x0 + position.x_length
                && position.y0 <= y && y <= position.y0 + position.y_length;
        }

        void moveInBounds(const Rect<int> &p)
        {
            if (position.x_length > p.x_length || position.y_length > p.y_length)
            {
                fprintf(stderr, "[WARNING] Widget::moveInBounds: bounds are smaller than object\n");
            }
            int nx = position.x0;
            int ny = position.y0;
            if (p.x0 + p.x_length < position.x0 + position.x_length)
                nx = p.x0 + p.x_length - position.x_length;
            if (position.x0 < p.x0)
                nx = p.x0;

            if (p.y0 + p.y_length < position.y0 + position.y_length)
                ny = p.y0 + p.y_length - position.y_length;
            if (position.y0 < p.y0)
                ny = p.y0;

            setPosition(nx, ny);
        }

        struct Iterator
        {
            Widget *current;

            Iterator(Widget *v) : current(v) {}
            Iterator &operator++()
            {
                current = current->next_brother;
                return *this;
            }
            Iterator &operator--()
            {
                current = current->prev_brother;
                return *this;
            }

            bool operator==(Iterator other) const
            {
                return current == other.current;
            }
            bool operator!=(Iterator other) const
            {
                return !operator==(other);
            }

            Widget* operator*() const
            {
                return current;
            }

            Widget* &operator*()
            {
                return current;
            }
        };

        Iterator begin() const
        {
            return Iterator(first_child);
        }

        Iterator last() const
        {
            return Iterator(last_child);
        }

        Iterator end() const
        {
            return Iterator(nullptr);
        }

        void addWidget(Widget *child, Iterator pos);

        Widget *erase(Widget *v);
        Widget *erase(Iterator it); // removes from list, but does not delete

        Widget *getParent() const
        {
            return parent;
        }

        bool isPressed() const
        {
            return press_status == Pressed;
        }

    protected:
        WidgetManager *manager;

        Widget(WidgetManager *manager, int w = 0, int h = 0) : manager(manager), position{0, 0, w, h} {}

        virtual void moveSelf(int dx, int dy) {}
        virtual void dumpSelf(FILE *f, int id) const;

        Rect<int> position;
    private:
        Widget *parent = nullptr;
        Widget *next_brother = nullptr;
        Widget *prev_brother = nullptr;
        Widget *last_child = nullptr;
        Widget *first_child = nullptr;
        bool focused = false;
        enum {
            Pressed,
            Released,
        } press_status = Released;
    };
};

#endif