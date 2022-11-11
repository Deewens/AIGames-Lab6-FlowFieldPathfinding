#pragma once

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Arrow.hpp"

class Arrow : public sf::Drawable
{
public:
    Arrow();
    Arrow(sf::Vector2f position, sf::Vector2f direction, float length);

    void setDirection(sf::Vector2f direction);

    void setPosition(sf::Vector2f position);

    void setLength(float length);

private:
    /**
     * \brief Calculate vertices position to draw an arrow
     */
    void updateVertices();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Vector2f m_position;
    sf::Vector2f m_direction;
    float m_length;

    sf::VertexArray m_arrow;
};
