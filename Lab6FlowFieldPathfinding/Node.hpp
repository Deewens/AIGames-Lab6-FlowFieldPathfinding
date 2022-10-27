#pragma once

#include <SFML/Graphics/Text.hpp>

#include "ResourceManager/ResourceManager.hpp"
#include "ResourceManager/ResourceIdentifiers.hpp"

class Node
{
private:
    int m_x = 0, m_y = 0;
    int m_nodeSize;

    int m_cost;

    sf::Text m_costText;

public:
    Node(const FontManager& fontManager, int x, int y, const int& nodeSize);

    int getCost() const;
    void setCost(int cost);
    
    const sf::Text& getCostText() const;

    int getX() const;
    int getY() const;
};
