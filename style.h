#ifndef DGUI_STYLE_H
#define DGUI_STYLE_H

#include <SFML/Graphics.hpp>
#include <cassert>

#include <unordered_map> // for texture bound access
#include <string_view>

#include "json/json.h"

namespace DGui
{
    class Style
    {
    public:
        Style();
        ~Style();

        int loadTextures();
        int loadTextures(const char *config);

        enum Textures
        {
            ButtonTexture,
            ButtonPressedTexture,
            TitleTexture,
            WindowTexture,

            Count
        };

        const sf::Texture *getTexture(Textures tid) const
        {
            assert(0 <= tid && tid < Textures::Count);
            return &textures[tid];
        }

        const sf::Texture *getTextureWithBounds(const char *name, sf::IntRect &rect) const;

    private:
        sf::Texture textures[Textures::Count] = {};

        sf::Texture tex;

        struct Subtexture
        {
            sf::Texture *tex;
            sf::IntRect rect;
        };

        
        std::unordered_map<std::string, Subtexture> bounds;

        char *config_buf = nullptr;
        json_token *toks = nullptr;
        json_token_id root = -1;
    };
};

#endif