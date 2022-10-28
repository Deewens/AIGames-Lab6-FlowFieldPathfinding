#pragma once

#include <SFML/Graphics/Text.hpp>

#include "ResourceManager/ResourceManager.hpp"
#include "ResourceManager/ResourceIdentifiers.hpp"

class Node : public sf::Drawable
{
private:
    int m_x = 0, m_y = 0;
    int m_nodeSize;

    int m_cost;

    sf::Text m_costText;

    sf::VertexArray m_vertices;
    sf::VertexArray m_outlineVertices;

    // Reference to the list of all the nodes in the grid
    std::vector<Node>& m_nodes;
public:
    Node(const FontManager& fontManager, int x, int y, const int& nodeSize, std::vector<Node>& gridNodes);

    int getCost() const;
    void setCost(int cost);

    const sf::Text& getCostText() const;

    sf::Vector2i getPosition() const;
    int getX() const;
    int getY() const;

    std::vector<Node> getNeighbours();
    
    void setQuadColor(sf::Color color);

    void updateQuadColor();

    void createQuadVertices();
    void createOutlineVertices();


    friend bool operator==(const Node& t_lhs, const Node& t_rhs)
    {
        return t_lhs.m_x == t_rhs.m_x
            && t_lhs.m_y == t_rhs.m_y;
    }

    friend bool operator!=(const Node& t_lhs, const Node& t_rhs)
    {
        return !(t_lhs == t_rhs);
    }

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
