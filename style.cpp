#include "style.h"
#include <cstdlib>
#include <cstring>

namespace DGui
{
    Style::Style()
    {

    }

    Style::~Style()
    {
        if (config_buf)
            delete [] config_buf;
        if (toks)
            delete [] toks;

        for (auto [s, st] : bounds)
        {
            if (st.tex != &tex)
                delete st.tex;
        }
    }

    int Style::loadTextures()
    {
        static const char *tex_pathes[Textures::Count] = {
            [Textures::ButtonTexture]        = "img/button.png",
            [Textures::ButtonPressedTexture] = "img/button_pressed.png",

            [Textures::TitleTexture]         = "img/title.png",
            [Textures::WindowTexture]        = "img/window.png",

            //[Textures::ThumbTexture]         = "img/thumb.png",
        };

        for (int i = 0; i < Textures::Count; ++i)
        {
            if (!textures[i].loadFromFile(tex_pathes[i]))
                return -i;
            textures[i].setRepeated(true);
        }
        return 1;
    }

    int Style::loadTextures(const char *config)
    {
        int len = std::strlen(config) + 1;
        config_buf = new char[len];
        std::memcpy(config_buf, config, len);

        json_strip(config_buf);

        json_parser parser = {};
        json_parser_construct(&parser, config_buf, NULL);

        if (json_parse(&parser) == -1)
        {
            fprintf(stderr, "Style::loadTextures(const char *config): invalid json!\n");
            return -1;
        }

        toks = new json_token[parser.it];
        json_parser_construct(&parser, config_buf, toks);
        root = json_parse(&parser);
        if (root == -1)
        {
            fprintf(stderr, "Invalid json!\n");
            return -1;
        }

        json_token_id filename_tok = json_find_by_key(toks, root, "file");
        if (filename_tok == -1 || toks[filename_tok].type != JSON_STRING)
        {
            fprintf(stderr, "No texture file mentioned!\n");
            return -1;
        }

        tex.loadFromFile(std::string("img/") + toks[filename_tok].string);

        json_token_id textures = json_find_by_key(toks, root, "textures");
        if (textures == -1 || toks[textures].type != JSON_ARRAY)
        {
            fprintf(stderr, "No valid textures list!\n");
            return -1;
        }

        for (json_token_id tex_it = toks[textures].array.first; tex_it != -1; tex_it = toks[tex_it].next)
        {
            json_token_id name_id   = json_find_by_key(toks, tex_it, "name");
            json_token_id bounds_id = json_find_by_key(toks, tex_it, "pos");
            json_token_id type_id   = json_find_by_key(toks, tex_it, "type");
            if (name_id == -1)
            {
                fprintf(stderr, "Warning: no name for texture\n");
                continue;
            }
            if (bounds_id == -1)
            {
                fprintf(stderr, "Warning: no texture position for %s\n", toks[name_id].string);
                continue;
            }

            const char *name = toks[name_id].string;
            Subtexture st;
            sf::IntRect crop = sf::IntRect(
                        toks[json_find_by_index(toks, bounds_id, 0)].number,
                        toks[json_find_by_index(toks, bounds_id, 1)].number,
                        toks[json_find_by_index(toks, bounds_id, 2)].number,
                        toks[json_find_by_index(toks, bounds_id, 3)].number
                    );

            if (type_id == -1)
            {
                st.tex = &tex;
                st.rect = crop;
                bounds[name] = st;
            }
            else if (!strcmp(toks[type_id].string, "rect-with-border"))
            {
                json_token_id border_id = json_find_by_key(toks, tex_it, "border-size");
                if (border_id == -1)
                {
                    fprintf(stderr, "Warning: no border size specificated for rect-with border %s\n", toks[name_id].string);
                    continue;
                }

                int border = toks[border_id].number;


                struct CropName
                {
                    const char *name;
                    sf::IntRect crop;
                };
                CropName offsets[9] = {
                    {"nw", {crop.left,                       crop.top,                        border,                  border}},
                    {"n",  {crop.left + border,              crop.top,                        crop.width - 2 * border, border}},
                    {"ne", {crop.left + crop.width - border, crop.top,                        border,                  border}},
                    {"e",  {crop.left + crop.width - border, crop.top + border,               border,                  crop.height - 2 * border}},
                    {"se", {crop.left + crop.width - border, crop.top + crop.height - border, border,                  border}},
                    {"s",  {crop.left + border,              crop.top + crop.height - border, crop.width - 2 * border, border}},
                    {"sw", {crop.left,                       crop.top + crop.height - border, border,                  border}},
                    {"w",  {crop.left,                       crop.top + border,               border,                  crop.height - 2 * border}},
                    {"i",  {crop.left + border,              crop.top + border,               crop.width - 2 * border, crop.height - 2 * border}}
                };

                sf::Image img = tex.copyToImage();

                for (int i = 0; i < sizeof(offsets) / sizeof(offsets[0]); ++i)
                {
                    #if 0
                    st.tex = new sf::Texture;
                    st.tex->loadFromImage(img, offsets[i].crop);
                    st.tex->setRepeated(true);
                    st.rect = offsets[i].crop;
                    st.rect.left = st.rect.top = 0;
                    #else
                    st.tex = &tex;
                    st.rect = offsets[i].crop;
                    #endif
                    //st.rect.left = st.rect.top = 0;

                    bounds[std::string(name) + "-" + offsets[i].name] = st;
                }
            }
        }

        return 1;
    }

    const sf::Texture *Style::getTextureWithBounds(const char *name, sf::IntRect &rect) const
    {
        auto it = bounds.find(name);
        if (it == bounds.end())
            return nullptr;
        rect = it->second.rect;
        return it->second.tex;
    }
};