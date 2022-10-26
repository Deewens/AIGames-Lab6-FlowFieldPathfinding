#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "Node.hpp"

class Grid : public sf::Drawable
{
private:
    int m_width;
    int m_height;

    sf::VertexArray m_vertices;
    sf::VertexArray m_outlineVertices;
    std::vector<sf::RectangleShape> m_rectangles;

    std::vector<Node> m_nodes;
    int m_nodeSize;


public:
    Grid(int width, int height, int nodeSize);

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    void createVertices();

    void createVertex(int x, int y);
    void createOutlineVertex(int x, int y);
};
