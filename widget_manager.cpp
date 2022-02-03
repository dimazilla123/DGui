#include "widget_manager.h"

#include <SFML/Window.hpp>

#include <sys/stat.h>
#include <cstdlib>
#include <cstring>

static void rec_draw(sf::RenderTarget &render, DGui::Widget *v)
{
    v->draw(render);
    DGui::Widget::Iterator u = v->begin();
    int x_offset = v->getPositionRect().x0;
    int y_offset = v->getPositionRect().y0;
    while (u != v->end())
    {
        int x = (*u)->getPositionRect().x0;
        int y = (*u)->getPositionRect().y0;
        //(*u)->setPosition(x + x_offset, y + y_offset);
        rec_draw(render, *u);
        //(*u)->setPosition(x, y);
        ++u;
    }
}

static bool rec_handle_event(const DGui::Event &ev, DGui::Widget *v)
{
    DGui::Widget::Iterator u = v->last();
    while (u != v->end())
    {
        if (rec_handle_event(ev, *u))
            return true;
        --u;
    }
    return v->handleEvent(ev);
}

static char *readfile(const char *name)
{
    struct stat st = {};
    if (stat(name, &st) == -1)
    {
        perror("Cannot get file size with stat");
        return nullptr;
    }

    FILE *f = fopen(name, "r");
    if (f == nullptr)
        return nullptr;

    char *buf = new char[st.st_size + 1];
    memset(buf, 0, st.st_size + 1);

    fread(buf, st.st_size, 1, f);
    fclose(f);

    return buf;
}

namespace DGui
{
    WidgetManager::WidgetManager(const char *font_name, int w, int h)
        : window(sf::VideoMode(w, h), "DGui"),
          root(this, w, h)
    {
        if (!font.loadFromFile(font_name))
            fprintf(stderr, "Cannot load %s for gui!\n", font_name);

        if (style.loadTextures() < 0)
            fprintf(stderr, "Cannot load textures for current style!\n");

        char *config = readfile("img/textures.json");

        style.loadTextures(config);

        delete [] config;

        //window = new sf::RenderWindow(sf::VideoMode(w, h), "DGui", sf::Style::Titlebar | sf::Style::Close);
    }
    WidgetManager::~WidgetManager()
    {}

    void WidgetManager::run()
    {
        window.display();

        while (window.isOpen())
        {
            sf::Event ev;
            while (window.pollEvent(ev))
                handleEvent(ev);
            window.clear();
            draw(window);
            window.display();
        }
    }

    void WidgetManager::drawCarcas(sf::RenderTarget &render, Widget *v)
    {
        sf::RectangleShape rec; // TODO: one rec instance for recursion
        rec.setOutlineColor(sf::Color::Yellow);
        rec.setFillColor(sf::Color::Transparent);
        rec.setOutlineThickness(1);

        rec.setSize(sf::Vector2f(v->getSize().w, v->getSize().h));
        rec.setPosition(v->getPositionRect().x0, v->getPositionRect().y0);

        render.draw(rec);

        for (Widget::Iterator it = v->begin(); it != v->end(); ++it)
        {
            drawCarcas(render, *it);
        }
    }

    void WidgetManager::draw(sf::RenderTarget &render)
    {
        Widget *v = &root;
        rec_draw(render, v);

        if (draw_carcas)
            drawCarcas(render, v);
    }

    void WidgetManager::handleEvent(const sf::Event &ev)
    {
        Event dgui_event = {};
        switch (ev.type)
        {
            case sf::Event::Closed:
            {
                window.close();
            } break;
            case sf::Event::Resized:
            {
                sf::View v = window.getView();
                sf::Vector2u size = window.getSize();
                v.setSize(size.x, size.y);
                v.setCenter(size.x / 2, size.y / 2);
                window.setView(v);

                root.setSize(size.x, size.y);
            } break;
            case sf::Event::KeyPressed:
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::F5))
                {
                    static int dumpn = 0;
                    char filename[256] = {};
                    snprintf(filename, sizeof(filename), "graph%d.gv", dumpn);
                    FILE *f = fopen(filename, "w");
                    root.dump(f);
                    fclose(f);
                    snprintf(filename, sizeof(filename), "dot -Tpng graph%d.gv -o graph%d.png", dumpn, dumpn);
                    system(filename);
                    ++dumpn;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::F6))
                {
                    draw_carcas ^= 1;
                }
            } break;
            case sf::Event::MouseButtonPressed:
            {
                dgui_event.type = DGui::Event::MouseButtonEvent;
                dgui_event.mouse_button_event.state = DGui::Event::MouseButtonStatus::Pressed;
                sf::Vector2f mpos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                dgui_event.mouse_button_event.x = mpos.x;
                dgui_event.mouse_button_event.y = mpos.y;

                fprintf(stderr, "Pressed at (%3d, %3d)\n", dgui_event.mouse_button_event.x, dgui_event.mouse_button_event.y);
                
                pushEvent(dgui_event);
            } break;

            case sf::Event::MouseButtonReleased:
            {
                dgui_event.type = DGui::Event::MouseButtonEvent;
                dgui_event.mouse_button_event.state = DGui::Event::MouseButtonStatus::Released;

                sf::Vector2f mpos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                dgui_event.mouse_button_event.x = mpos.x;
                dgui_event.mouse_button_event.y = mpos.y;

                fprintf(stderr, "Released at (%3d, %3d)\n", dgui_event.mouse_button_event.x, dgui_event.mouse_button_event.y);
                
                pushEvent(dgui_event);
            } break;
            case sf::Event::MouseMoved:
            {
                //static int me = 0;
                //fprintf(stderr, "MoveEvent %d\n", ++me);
                dgui_event.type = DGui::Event::MouseMoveEvent;
                sf::Vector2f mpos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                dgui_event.mouse_move_event.x = mpos.x;
                dgui_event.mouse_move_event.y = mpos.y;
                
                pushEvent(dgui_event);
            } break;
            default:
                break;
        }
        while (!event_queue.empty())
        {
            rec_handle_event(popEvent(), &root);
        }
    }

};