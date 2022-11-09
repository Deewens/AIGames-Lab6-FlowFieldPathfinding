#include "Arrow.hpp"

#include <iostream>
#include <complex>

#include "utils/VectorUtils.hpp"

Arrow::Arrow() : m_length(0), m_arrow(sf::Lines, 6)
{
    updateVertices();
}

Arrow::Arrow(const sf::Vector2f position, const sf::Vector2f direction, const float length) :
    m_position(position),
    m_direction(direction),
    m_length(length),
    m_arrow(sf::Lines, 6)
{
    updateVertices();
}

void Arrow::setDirection(const sf::Vector2f direction)
{
    m_direction = direction;

    updateVertices();
}

void Arrow::setPosition(const sf::Vector2f position)
{
    m_position = position;
    updateVertices();
}

void Arrow::setLength(const float length)
{
    m_length = length;
    updateVertices();
}

void Arrow::updateVertices()
{
    const sf::Vector2f arrowheadPosition = m_position + m_direction * m_length;

    m_arrow[0].position = m_position;
    m_arrow[1].position = arrowheadPosition;

    // float height = m_length * std::sqrtf(3);
    float h = m_length * 0.75f;
    // float halfWidth = m_length * 0.5f;
    float w = h * 0.5f;

    const sf::Vector2f backwardVector = arrowheadPosition - m_position;
    sf::Vector2f U = backwardVector / VectorUtils::getLength(backwardVector);
    sf::Vector2f V = sf::Vector2f(-U.y, U.x);

    const sf::Vector2f v1 = arrowheadPosition - h * U + w * V;
    const sf::Vector2f v2 = arrowheadPosition - h * U - w * V;


    m_arrow[2].position = arrowheadPosition;
    m_arrow[3].position = v1;

    m_arrow[4].position = arrowheadPosition;
    m_arrow[5].position = v2;
}

void Arrow::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_arrow);
}
