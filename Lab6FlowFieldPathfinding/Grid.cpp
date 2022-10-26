#include "Grid.hpp"

#include <iostream>

#include <SFML/Graphics/RenderTarget.hpp>

Grid::Grid(const int width, const int height, const int nodeSize) :
    m_width(width),
    m_height(height),
    m_vertices(sf::Quads, width * height * 4),
    m_outlineVertices(sf::LinesStrip, width * height * 5),
    m_nodeSize(nodeSize)
{
    m_nodes.reserve(m_width * m_height);
    //m_nodes.assign(m_nodes.capacity(), 0);

    createVertices();

    for (int i = 0; i < m_width; i++)
    {
        for (int j = 0; j < m_height; j++)
        {
            sf::RectangleShape rectangle(sf::Vector2f(m_width, m_height));
            
            rectangle.setPosition(sf::Vector2f(i * m_width, j * m_height));
        }
    }
}

void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_vertices);
    target.draw(m_outlineVertices);
}

void Grid::createVertices()
{
    for (int i = 0; i < m_width; i++)
    {
        for (int j = 0; j < m_height; j++)
        {
            // Initialize the outline vertices (border of the node)
            createOutlineVertex(i, j);

            // Initialize the quad vertices (node fill color)
            //createVertex(i, j);
        }
    }
}

void Grid::createVertex(const int x, const int y)
{
    sf::Vertex* quad = &m_vertices[(x + y * m_width) * 4];

    quad[0].position = static_cast<sf::Vector2f>(sf::Vector2i(x * m_nodeSize, y * m_nodeSize));
    quad[1].position = static_cast<sf::Vector2f>(sf::Vector2i((x + 1) * m_nodeSize, y * m_nodeSize));
    quad[2].position = static_cast<sf::Vector2f>(sf::Vector2i((x + 1) * m_nodeSize, (y + 1) * m_nodeSize));
    quad[3].position = static_cast<sf::Vector2f>(sf::Vector2i(x * m_nodeSize, (y + 1) * m_nodeSize));

    quad[0].color = sf::Color::Blue;
    quad[1].color = sf::Color::Blue;
    quad[2].color = sf::Color::Blue;
    quad[3].color = sf::Color::Blue;
}

void Grid::createOutlineVertex(const int x, const int y)
{
    sf::Vertex* lineStrip = &m_outlineVertices[(x + y * m_width) * 5];

    lineStrip[0].position = static_cast<sf::Vector2f>(sf::Vector2i(x * m_nodeSize, y * m_nodeSize));
    lineStrip[1].position = static_cast<sf::Vector2f>(sf::Vector2i((x + 1) * m_nodeSize, y * m_nodeSize));
    lineStrip[2].position = static_cast<sf::Vector2f>(sf::Vector2i((x + 1) * m_nodeSize, (y + 1) * m_nodeSize));
    lineStrip[3].position = static_cast<sf::Vector2f>(sf::Vector2i(x * m_nodeSize, (y + 1) * m_nodeSize));
    lineStrip[4].position = static_cast<sf::Vector2f>(sf::Vector2i(x * m_nodeSize, y * m_nodeSize));

    lineStrip[0].color = sf::Color::White;
    lineStrip[1].color = sf::Color::White;
    lineStrip[2].color = sf::Color::White;
    lineStrip[3].color = sf::Color::White;
    lineStrip[4].color = sf::Color::White;
}
