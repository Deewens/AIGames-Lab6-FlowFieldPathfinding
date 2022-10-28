#pragma once
#include <array>
#include <SFML/System/Vector2.hpp>

static class Direction
{
    static const std::array<sf::Vector2i, 4> CardinalDirections{
        sf::Vector2i(0, 1), // UP
        sf::Vector2i(1, 0), // RIGHT
        sf::Vector2i(0, -1), // DOWN
        sf::Vector2i(-1, 0) // LEFT
    };

    static const std::array<sf::Vector2i, 4> OrdinalDirections{
        sf::Vector2i(1, 1), // RIGHT-UP
        sf::Vector2i(1, -1), // RIGHT-DOWN
        sf::Vector2i(-1, -1), // LEFT-DOWN
        sf::Vector2i(-1, 1) // LEFT-UP
    };

    static const std::array<sf::Vector2i, 8> All8WindDirections{
        sf::Vector2i(0, 1), // UP
        sf::Vector2i(1, 0), // RIGHT
        sf::Vector2i(0, -1), // DOWN
        sf::Vector2i(-1, 0), // LEFT,
        sf::Vector2i(1, 1), // RIGHT-UP
        sf::Vector2i(1, -1), // RIGHT-DOWN
        sf::Vector2i(-1, -1), // LEFT-DOWN
        sf::Vector2i(-1, 1) // LEFT-UP
    };


    template<typename T>
    static sf::Vector2<T> getCardinalDirections()
    {
        
    }
};
