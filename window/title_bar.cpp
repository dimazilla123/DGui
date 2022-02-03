#include "window.h"
#include "dgui_config.h"

namespace DGui
{
    TitleBar::TitleBar(WidgetManager *manager, Window *window, const char *text, int w)
        : Widget(manager),
          wind(window),
          close_button(manager, window)
    {
        setSize(w, FONT_SIZE);
        title.setFont(manager->getFont());
        title.setString(text);
        title.setCharacterSize(FONT_SIZE);
        title.setFillColor(sf::Color::White);

        /*
        sh.setFillColor(sf::Color::Magenta);
         */
        //sh.setTexture(manager->getStyle().getTexture(Style::Textures::TitleTexture));
        //sh.setSize(sf::Vector2f(w, FONT_SIZE));

        close_button.move(w - close_button.getMinimumSize().w, 0);
        addWidget(&close_button);
    }

    void TitleBar::moveSelf(int dx, int dy)
    {
        title.move(dx, dy);
        sh.move(dx, dy);
    }

    void TitleBar::setSize(int x_length, int y_length)
    {
        Widget::setSize(x_length, y_length);
        sh.setSize(sf::Vector2f(position.x_length, position.y_length));
        close_button.setPosition(position.x0 + position.x_length - close_button.getMinimumSize().w, position.y0);
    }

    void TitleBar::draw(sf::RenderTarget &render)
    {
        //render.draw(sh);
        render.draw(title);
    }

    bool TitleBar::onMousePress(const DGui::Event::MouseButtonStatus &ev)
    {
        grabbed = true;
        grab_x = ev.x;
        grab_y = ev.y;
        fprintf(stderr, "Grabbed at (%2d, %2d)\n", grab_x, grab_y);

        return true;
    }

    bool TitleBar::onMouseRelease(const DGui::Event::MouseButtonStatus &ev)
    {
        grabbed = false;
        grab_x = grab_y = -1;
        fprintf(stderr, "Ungrabbed at (%2d, %2d)\n", ev.x, ev.y);

        return true;
    }

    bool TitleBar::onMouseMove(const DGui::Event::MouseMoveStatus &ev)
    {
        if (!grabbed)
            return false;
        int dx = ev.x - grab_x;
        int dy = ev.y - grab_y;
        fprintf(stderr, "moved for (%3d, %3d)\n", dx, dy);
        wind->move(dx, dy);
        fprintf(stderr, "new positon (%3d, %3d)\n", position.x0, position.y0);
        grab_x = ev.x;
        grab_y = ev.y;

        return false;
    }

    bool TitleBar::CloseButton::onMouseClick(const Event::MouseButtonStatus &ev)
    {
        // TODO: push event "CloseEvent" into the WidgetManager queue
        //Widget::Iterator it(dynamic_cast<Widget*>(to_close));

        //Event cl_ev = {};
        //cl_ev.type = Event::CloseEvent;
        //manager->pushEvent(cl_ev);

        to_close->onClose();

        return true;
    }

    void TitleBar::setText(const char *s)
    {
        title.setString(s);
    }
};