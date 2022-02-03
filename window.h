#ifndef DGUI_WINDOW_H
#define DGUI_WINDOW_H

#include "widget.h"
#include "widget_manager.h"

#include "button.h"

#include "boundered_rect.h"

namespace DGui
{
    class Window;

    class TitleBar : public DGui::Widget
    {
    public:
        TitleBar(WidgetManager *manager, Window *window, const char *text, int w);

        void setText(const char s[]);

        void moveSelf(int dx, int dy);

        virtual void setSize(int x_length, int y_length);
        void draw(sf::RenderTarget &render);

        virtual bool onMousePress(const DGui::Event::MouseButtonStatus &ev) override;
        virtual bool onMouseRelease(const DGui::Event::MouseButtonStatus &ev) override;
        virtual bool onMouseMove(const DGui::Event::MouseMoveStatus &ev) override;
    private:
        Window *wind;
        sf::Text title;
        sf::RectangleShape sh;

        class CloseButton : public Button
        {
        public:
            CloseButton(WidgetManager *manager, Window *to_close) : Button(manager, "Close"), to_close(to_close)
            {
                rect.setColor(sf::Color::Red);
            }

            virtual bool onMouseClick(const Event::MouseButtonStatus &ev) override;
        private:
            Window *to_close;
        };

        CloseButton close_button;

        int grab_x = -1, grab_y = -1;
        bool grabbed = false;

    };

    class Window : public DGui::Widget
    {
    public:
        Window(WidgetManager *manager, const char *title, int w, int h);

        void moveSelf(int dx, int dy);

        void setSize(int x_length, int y_length);

        void draw(sf::RenderTarget &render);

        virtual void addWidget(Widget *w);

        bool onClose();

        void setTitle(const char s[]);

        /*
        void onMousePress(const DGui::Event &ev);
        void onMouseRelease(const DGui::Event &ev);
        void onMouseMove(const DGui::Event &ev);
         */
    private:
        const int border_size = 10;

        TitleBar bar;
        //sf::RectangleShape sh;

        BounderedRect sh;
    };
};

#endif