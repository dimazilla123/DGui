#ifndef DGUI_DIRECTIONAL_LAYOUT_H
#define DGUI_DIRECTIONAL_LAYOUT_H

#include "widget.h"

#include "dgui_config.h"

namespace DGui
{
    class DirectionalLayout : public Widget
    {
    public:
        DirectionalLayout(WidgetManager *manager, int w, int h)
            : Widget(manager)
        {
            position.x_length = w;
            position.y_length = h;
        }

        void draw(sf::RenderTarget &r)
        {
            if (changed)
            {
                refit();
                changed = false;
            }
        }

        void addWidget(Widget *w)
        {
            Widget::addWidget(w);
            changed = true;
        }

        void setSize(int w, int h)
        {
            Widget::setSize(w, h);
            refit();
        }

        bool isChanged() const
        {
            return changed;
        }

        virtual void refit() = 0;
    private:
        int border_space = BORDER_SIZE;
        bool changed = false;
    };

    class VerticalLayout : public DirectionalLayout
    {
    public:
        VerticalLayout(WidgetManager *manager, int w, int h)
            : DirectionalLayout(manager, w, h)
        {

        }
        void refit() override;
    private:
    };

    class HorizontalLayout : public DirectionalLayout
    {
    public:
        HorizontalLayout(WidgetManager *manager, int w, int h)
            : DirectionalLayout(manager, w, h)
        {

        }
        void refit() override;
    private:
    };
};

#endif