#pragma once

// Forward declaration of SFML classes
namespace sf
{
    class Font;
}

namespace Assets
{
    enum class Font
    {
        ArialBlack
    };
}

// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceManager;

typedef ResourceManager<sf::Font, Assets::Font> FontManager;
