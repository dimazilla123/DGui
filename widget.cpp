#include "widget.h"

#include "widget_manager.h"

#include <cassert>

void DGui::Widget::addWidget(Widget *w)
{
    w->parent = this;

    if (last_child == nullptr)
    {
        assert(first_child == last_child);
        first_child = last_child = w;
    }
    else
    {
        last_child->next_brother = w;
        w->prev_brother = last_child;
        last_child = w;
    }

    /*
    if (first_child == nullptr)
    {
        assert(first_child == last_child);
        first_child = last_child = w;
    }
    else
    {
        w->next_brother = first_child;
        first_child->prev_brother = w;
        first_child = w;
    }
     */

    w->move(position.x0, position.y0);
}

void DGui::Widget::addWidget(Widget *child, Iterator pos)
{
    if (pos == end())
    {
        addWidget(child);
        return;
    }
    Widget *w = *pos;
    if (w->prev_brother)
        w->prev_brother->next_brother = child;
    child->prev_brother = w->prev_brother;

    w->prev_brother = child;
    child->next_brother = w;

    if (w == last_child)
        last_child = child;
    w->move(position.x0, position.y0);
}

DGui::Widget *DGui::Widget::erase(Widget *v)
{
    if (v->next_brother)
        v->next_brother->prev_brother = v->prev_brother;
    else
        last_child = v->prev_brother;
    if (v->prev_brother)
        v->prev_brother->next_brother = v->next_brother;
    else
        first_child = v->next_brother;

    v->next_brother = v->prev_brother = v->parent = nullptr;
    return v;
}

DGui::Widget *DGui::Widget::erase(Iterator it)
{
    return erase(*it);
}

bool DGui::Widget::handleEvent(const DGui::Event &ev)
{
    switch (ev.type)
    {
        case DGui::Event::MouseButtonEvent:
        {
            switch (ev.mouse_button_event.state)
            {
                case DGui::Event::MouseButtonStatus::MouseButtonState::Pressed:
                {
                    if (isInBonds(ev.mouse_button_event.x, ev.mouse_button_event.y))
                    {
                        press_status = Pressed;
                        return onMousePress(ev.mouse_button_event);
                    }
                } break;
                case DGui::Event::MouseButtonStatus::MouseButtonState::Released:
                {
                    if (press_status == Pressed)
                    {
                        press_status = Released;

                        if (isInBonds(ev.mouse_button_event.x, ev.mouse_button_event.y))
                        {
                            DGui::Event click = ev;
                            click.mouse_button_event.state = DGui::Event::MouseButtonStatus::MouseButtonState::Clicked;
                            manager->pushEvent(click);
                        }

                        return onMouseRelease(ev.mouse_button_event);
                    }
                } break;

                case DGui::Event::MouseButtonStatus::MouseButtonState::Clicked:
                {
                    if (isInBonds(ev.mouse_button_event.x, ev.mouse_button_event.y))
                        return onMouseClick(ev.mouse_button_event);
                    return false;
                } break;
            }
        } break;

        case DGui::Event::MouseMoveEvent:
        {
            if (isInBonds(ev.mouse_move_event.x, ev.mouse_move_event.y))
            {
                if (!focused)
                {
                    focused = true;
                    onFocusGain();
                }
            }
            else if (focused)
            {
                focused = false;
                onFocusLose();
            }
            //press_status = Released;
            return onMouseMove(ev.mouse_move_event);
        } break;

        case DGui::Event::CloseEvent:
        {
            fprintf(stderr, "Close event\n");
            return onClose();
        } break;

        default:
        {
        }
    }
    return false;
}

void DGui::Widget::dump(FILE *f) const
{
    fprintf(f, "digraph G {\n"
               //"    rankdir=\"RL\"\n"
               "    node [shape=record];\n");
    const Widget *v = this;
    int id = 0;

    while (v != nullptr)
    {

        fprintf(f, "{rank = same;");
        for (const DGui::Widget *u = v; u != nullptr; u = u->next_brother)
        {
            fprintf(f, " struct%p;", u);
        }
        fprintf(f, "};\n");

        fprintf(f, "struct%p [label=\"type %s | id = %d\"];\n", v, typeid(*v).name(), id);

        if (v->parent != nullptr)
            fprintf(f, "struct%p -> struct%p;\n", v->parent, v);
        #if 0

        if (v->next_brother)
            fprintf(f, "struct%p -> struct%p [color=\"red\"];\n", v, v->next_brother);
        if (v->prev_brother)
            fprintf(f, "struct%p -> struct%p [color=\"blue\"];\n", v, v->prev_brother);

        if (v->first_child)
            fprintf(f, "struct%p -> struct%p [color=\"green\"];\n", v, v->first_child);
        if (v->last_child)
            fprintf(f, "struct%p -> struct%p [color=\"purple\"];\n", v, v->last_child);

        #endif

        if (v->first_child != nullptr)
            v = v->first_child;
        else if (v->next_brother != nullptr)
            v = v->next_brother;
        else
        {
            while (v->next_brother == nullptr && v->parent != nullptr)
                v = v->parent;
            v = v->next_brother;
        }
        ++id;
    }
    fprintf(f, "}\n");
}

void DGui::Widget::dumpSelf(FILE *f, int id) const
{
}

void DGui::Widget::refit()
{
    int min_x = position.x0, max_x = position.x0,
        min_y = position.y0, max_y = position.y0;
    for (DGui::Widget::Iterator it = begin(); it != end(); ++it)
    {
        DGui::Widget *w = *it;
        max_x = std::max(max_x, w->position.x0 + w->position.x_length);
        max_y = std::max(max_y, w->position.y0 + w->position.y_length);
    }

    setSize(max_x - min_x, max_y - min_y);
}