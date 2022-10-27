#include "Node.hpp"

#include <ostream>
#include <iostream>

#include <SFML/Graphics/RenderTarget.hpp>

Node::Node(const FontManager& fontManager, const int x, const int y, const int& nodeSize) :
    m_x(x), m_y(y), m_nodeSize(nodeSize), m_cost(1)
{
    m_costText.setString(std::to_string(m_cost));
    m_costText.setFont(fontManager.get(Assets::Font::ArialBlack));

    m_costText.setPosition(static_cast<float>(x * m_nodeSize), static_cast<float>(y * m_nodeSize));
    m_costText.setCharacterSize(10);
}

int Node::getCost() const
{
    return m_cost;
}

void Node::setCost(int cost)
{
    m_cost = cost;

    m_costText.setString(std::to_string(m_cost));
}

const sf::Text& Node::getCostText() const
{
    return m_costText;
}

int Node::getX() const
{
    return m_x;
}

int Node::getY() const
{
    return m_y;
}
