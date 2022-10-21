#include "Grid.hpp"

#include <iostream>

#include <SFML/Graphics/RenderTarget.hpp>

Grid::Grid(const int width, const int height, const int cellSize) :
    m_width(width), m_height(height),
    m_cellSize(cellSize),
    m_vertices(sf::Triangles, width * height * 3)
{
    m_cells.reserve(m_width * m_height);
    m_cells.assign(m_cells.capacity(), 0);

    for (int i = 0; i < m_width; ++i)
    {
        for (int j = 0; j < m_height; ++j)
        {
            int cellNumber = m_cells.at(i + j * width);

            sf::Vertex* triangles = &m_vertices[(i + j * width) * 6];

            triangles[0].position = static_cast<sf::Vector2f>(sf::Vector2i(i * cellSize, j * cellSize));
            triangles[1].position = static_cast<sf::Vector2f>(sf::Vector2i((i + 1) * cellSize, j * cellSize));
            triangles[2].position = static_cast<sf::Vector2f>(sf::Vector2i((i + 1) * cellSize, (j + 1) * cellSize));
            triangles[3].position = static_cast<sf::Vector2f>(sf::Vector2i(i * cellSize, (j + 1) * cellSize));
            
            triangles[4].position = static_cast<sf::Vector2f>(sf::Vector2i(i * cellSize, j * cellSize));
            triangles[5].position = static_cast<sf::Vector2f>(sf::Vector2i(i * cellSize, j * cellSize));

            triangles[0].color = sf::Color::Blue;
            triangles[1].color = sf::Color::Blue;
            triangles[2].color = sf::Color::Blue;
            triangles[3].color = sf::Color::Blue;
            
            triangles[4].color = sf::Color::Blue;
            triangles[5].color = sf::Color::Blue;
        }
    }
}

void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_vertices);
}
