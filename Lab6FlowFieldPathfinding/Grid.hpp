#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

class Grid : public sf::Drawable
{
private:
    int m_width;
    int m_height;
    int m_cellSize;

    std::vector<int> m_cells;
    sf::VertexArray m_vertices;

public:
    Grid(int width, int height, int cellSize);

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
