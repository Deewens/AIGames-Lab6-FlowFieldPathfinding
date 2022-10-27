#include "Node.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

Node::Node(const FontManager& fontManager, const int x, const int y, const int& nodeSize) :
    m_x(x), m_y(y), m_nodeSize(nodeSize)
{
    m_costText.setString("C");
    m_costText.setFont(fontManager.get(Assets::Font::ArialBlack));
    m_costText.setPosition(static_cast<float>(x) + (static_cast<float>(nodeSize) * 0.5f),
                           static_cast<float>(y) + (static_cast<float>(nodeSize) * 0.5f));
    m_costText.setCharacterSize(10);
}

int Node::getCost() const
{
    return m_cost;
}

void Node::setCost(const int cost)
{
    m_cost = cost;
    m_costText.setString(std::to_string(cost));
}

const sf::Text& Node::getCostText() const
{
    return m_costText;
}
