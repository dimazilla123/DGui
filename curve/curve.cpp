#include "curve.h"

namespace DGui
{
    Curve::Curve(WidgetManager *manager, int w, int h)
        : AbstractSlider(manager, w, h)
    {
        const char frag[] = 
            "#version 410 core\n"

            "out vec4 frag_colour;\n"
            
            "void main()\n"
            "{\n"
            "    frag_colour = vec4(1.f, 1.f, 0.f, 1.f);\n"
            "}";
        const char vert[] = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;

            void main()
            {
                gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
            }
        )";
        const char geom[] = R"(#version 460

            layout (lines_adjacency) in;
            layout (line_strip, max_vertices = 4) out;
            
            void main ()
            {
                gl_Position = gl_in[0].gl_Position;
                EmitVertex();
                gl_Position = gl_in[1].gl_Position;
                EmitVertex();
                gl_Position = gl_in[2].gl_Position;
                EmitVertex();
                gl_Position = gl_in[3].gl_Position;
                EmitVertex();
                EndPrimitive();
            }
        )";

        shader.loadFromMemory(vert, sf::Shader::Type::Vertex);
        shader.loadFromMemory(frag, sf::Shader::Type::Fragment);
        //shader.loadFromMemory(geom, sf::Shader::Type::Geometry);

        varray.setPrimitiveType(sf::LineStrip);
        //points.reserve(32);
    }

    Curve::~Curve()
    {
        Widget::Iterator it = begin();
        while (it != end())
        {
            Widget::Iterator jt = it;
            ++jt;
            delete (Marker*)*it;
            it = jt;
        }
    }

    bool vector_x_cmp(const Vector2d &a, const Vector2d &b)
    {
        return a.x < b.x;
    }

    int Curve::addPoint(const Vector2d &v)
    {
        assert(0 <= v.x && v.x <= 1 && 0 <= v.y && v.y <= 1);
        auto it = std::lower_bound(points.begin(), points.end(), v, vector_x_cmp);
        int pos = std::distance(points.begin(), it);
        points.insert(it, v);

        changed = true;

        return pos;
    }

    void Curve::draw(sf::RenderTarget &render)
    {
        if (changed)
        {
            redraw();
            changed = false;
        }

        render.draw(varray, &shader);
    }

    Vector2d Curve::pixel2pos(const sf::Vector2i &v) const
    {
        return Vector2d((v.x - position.x0) / ((float)position.x_length), 1 - ((v.y - position.y0) / ((float)position.y_length)));
    }

    sf::Vector2f Curve::pos2pixel(const Vector2d &v) const
    {
        return sf::Vector2f(position.x_length * v.x + position.x0, (position.y_length * (1 - v.y) + position.y0));
    }

    void Curve::redraw()
    {
        varray.clear();

        for (int i = 0; i < points.size(); ++i)
        {
            sf::Vertex vert(pos2pixel(points[i]));
            varray.append(vert);
        }
    }

    void Curve::moveSelf(int dx, int dy)
    {
        for (int i = 0; i < varray.getVertexCount(); ++i)
        {
            varray[i].position.x += dx;
            varray[i].position.y += dy;
        }
    }

    float Curve::getValue(float x) const
    {
        assert(0 <= x && x <= 1);
        assert(!points.empty());

        auto it = std::lower_bound(points.begin(), points.end(), Vector2d(x, 0), vector_x_cmp);

        if (it == points.end() - 1)
            return it->y;

        Vector2d a = *it,
                 b = *(it++);

        return ((b - a) * (a.x - x) + b).y;
    }

    bool Curve::onMouseClick(const Event::MouseButtonStatus &ev)
    {
        fprintf(stderr, "Curve::onMouseClick\n");
        int pos = addPoint(pixel2pos(ev.x, ev.y));

        Widget::Iterator it = begin();
        for (int i = 0; i < pos; ++i)
            ++it;

        Marker *mk = new Marker(manager, this, pos);
        //thumbs.push_back(Marker(manager, this, pos));
        addWidget(mk);

        int cnt = 0;
        for (Widget::Iterator jt = begin(); jt != end(); ++jt)
        {
            assert(dynamic_cast<Marker*>(*jt));
            Marker *mk = static_cast<Marker*>(*jt);

            mk->setPointIndex(cnt);

            sf::Vector2f p = pos2pixel(points[cnt]);
            mk->setPosition(p.x, p.y);
            ++cnt;
        }

        return true;
    }

    void Curve::onThumbChange(Thumb *th)
    {
        fprintf(stderr, "Curve::onThumbChange\n");
        assert(dynamic_cast<const Marker*>(th));

        const Marker *mk = static_cast<const Marker*>(th);

        Rect<int> bounds = position;
        Widget::Iterator it(th),
                         left = it,
                         right = it;
        --left;
        ++right;

        if (*left)
            bounds.x0 = (*left)->getPositionRect().x0;
        if (*right)
            bounds.x_length = (*right)->getPositionRect().x0 - bounds.x0;
        fprintf(stderr, "bounds = {.x0 = %d, .y0 = %d, .x_length = %d, .y_length = %d}\n", bounds.x0, bounds.y0, bounds.x_length, bounds.y_length);
        fprintf(stderr, "left = %p, right = %p\n", *left, *right);
        th->moveInBounds(bounds);

        points[mk->getPointIndex()] = Vector2d(mk->getValueX(), mk->getValueY());
        changed = true;
    }
};