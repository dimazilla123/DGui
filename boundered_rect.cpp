#include "boundered_rect.h"
#include <string>

namespace DGui
{
    enum Directions
    {
        NW = 0,
        N,
        NE,
        W,
        I,
        E,
        SW,
        S,
        SE,
    };

    const char *names[] = {
        [NW] = "nw",
        [N]  = "n",
        [NE] = "ne",
        [W]  = "w",
        [I]  = "i",
        [E]  = "e",
        [SW] = "sw",
        [S]  = "s",
        [SE] = "se",
    };

    BounderedRect::BounderedRect()
    {
        color = sf::Color::White;
    }

    void BounderedRect::updateColor()
    {
        for (int i = 0; i < verteces.getVertexCount(); ++i)
            verteces[i].color = color;
    }

    void BounderedRect::updateSkin()
    {
        int iw = bounds[Directions::I].width,
            ih = bounds[Directions::I].height;

        int samples_x = (x_length - 2 * border + iw - 1) / iw,
            samples_y = (y_length - 2 * border + ih - 1) / ih;

        verteces.resize(4 * ((samples_x + 2) * (samples_y + 2)));

        std::vector<int> xs(3 + samples_x),
                         ys(3 + samples_y);
        xs[0] = 0;
        for (int i = 0; i < samples_x; ++i)
            xs[1 + i] = border + iw * i;
        xs[1 + samples_x] = x_length - border;
        xs[2 + samples_x] = x_length;
        ys[0] = 0;
        for (int i = 0; i < samples_y; ++i)
            ys[1 + i] = border + ih * i;
        ys[1 + samples_y] = y_length - border;
        ys[2 + samples_y] = y_length;

        for (int x = 0; x < samples_x + 2; ++x)
        {
            for (int y = 0; y < samples_y + 2; ++y)
            {
                int it = x + y * (2 + samples_x);
                sf::Vertex *quad = &verteces[4 * it];

                quad[0].position = sf::Vector2f(xs[x], ys[y]);
                quad[1].position = sf::Vector2f(xs[x + 1], ys[y]);
                quad[2].position = sf::Vector2f(xs[x + 1], ys[y + 1]);
                quad[3].position = sf::Vector2f(xs[x], ys[y + 1]);

                int nx = 1,
                    ny = 1;
                if (x == 0) nx = 0;
                else if (x == samples_x + 1) nx = 2;
                if (y == 0) ny = 0;
                else if (y == samples_y + 1) ny = 2;

                fprintf(stderr, "(%3d, %3d) -> (%3d, %3d)\n", x, y, nx, ny);

                it = nx + ny * 3;

                quad[0].texCoords = sf::Vector2f(bounds[it].left,                    bounds[it].top);
                quad[1].texCoords = sf::Vector2f(bounds[it].left + bounds[it].width, bounds[it].top);
                quad[2].texCoords = sf::Vector2f(bounds[it].left + bounds[it].width, bounds[it].top + bounds[it].height);
                quad[3].texCoords = sf::Vector2f(bounds[it].left,                    bounds[it].top + bounds[it].height);
            }
        }

        updateColor();
    }

    void BounderedRect::setSize(int nx_length, int ny_length)
    {
        x_length = nx_length;
        y_length = ny_length;
        updateSkin();
    }

    bool BounderedRect::load(const char *tex_name, const Style &st)
    {
        Directions suffs[] = {
            NW,
            N,
            NE,
            W,
            I,
            E,
            SW,
            S,
            SE,
        };

        for (int i = 0; i < 9; ++i)
        {
            tex = st.getTextureWithBounds((std::string(tex_name) + '-' + names[suffs[i]]).c_str(), bounds[i]);
        }

        border = bounds[Directions::NW].width;

        verteces.setPrimitiveType(sf::Quads);
        //verteces.setPrimitiveType(sf::LineStrip);

        updateSkin();

        return true;
    }
};