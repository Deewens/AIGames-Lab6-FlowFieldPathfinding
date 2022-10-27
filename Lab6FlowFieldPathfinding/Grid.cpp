#include "Grid.hpp"

#include <iostream>

#include <SFML/Graphics/RenderTarget.hpp>

Grid::Grid(const FontManager& fontManager, const int width, const int height, const int nodeSize) :
    m_width(width),
    m_height(height),
    m_vertices(sf::Quads, width * height * 4),
    m_outlineVertices(sf::Lines, width * height * 8),
    m_nodeSize(nodeSize)
{
    m_nodes.reserve(m_width * m_height);

    for (int i = 0; i < m_width; i++)
    {
        for (int j = 0; j < m_height; j++)
        {
            m_nodes.emplace_back(fontManager, i, j, m_nodeSize);
        }
    }

    createVertices();
    createHeatmap(sf::Vector2i(5, 10));
}

void Grid::update(sf::Time dt)
{
}

std::vector<Node>& Grid::getNodes()
{
    return m_nodes;
}

void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_vertices);
    target.draw(m_outlineVertices);

    for (auto& node : m_nodes)
    {
        target.draw(node.getCostText());
    }
}

void Grid::createHeatmap(sf::Vector2i goalPosition)
{
    auto goal = std::find_if(m_nodes.begin(), m_nodes.end(), [=](const Node& node)
    {
        if (node.getX() == goalPosition.x && node.getY() == goalPosition.y)
        {
            return true;
        }

        return false;
    });
    if (goal != m_nodes.end())
        goal->setCost(0);

    sf::Vertex* quad = &m_vertices[(goalPosition.x + goalPosition.y * m_width) * 4];
    quad[0].color = sf::Color(255, 0, 0);
    quad[1].color = sf::Color(255, 0, 0);
    quad[2].color = sf::Color(255, 0, 0);
    quad[3].color = sf::Color(255, 0, 0);


    for (auto& node : m_nodes)
    {
        if (node.getX() == goalPosition.x && node.getY() == goalPosition.y) continue;

        auto distance = std::max(std::abs(goalPosition.x - node.getX()), std::abs(goalPosition.y - node.getY()));
        node.setCost(distance);

        sf::Vertex* quad = &m_vertices[(node.getX() + node.getY() * m_width) * 4];
        const auto heatmapColor = static_cast<sf::Uint8>((50 - node.getCost())*255/50);

        quad[0].color = sf::Color(0, 0, heatmapColor);
        quad[1].color = sf::Color(0, 0, heatmapColor);
        quad[2].color = sf::Color(0, 0, heatmapColor);
        quad[3].color = sf::Color(0, 0, heatmapColor);
    }
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
            createVertex(i, j);
        }
    }
}

void Grid::createVertex(const int x, const int y)
{
    sf::Vertex* quad = &m_vertices[(x + y * m_width) * 4];
    const Node nodeIdx = m_nodes[x + y * m_width];

    quad[0].position = static_cast<sf::Vector2f>(sf::Vector2i(x * m_nodeSize, y * m_nodeSize));
    quad[1].position = static_cast<sf::Vector2f>(sf::Vector2i((x + 1) * m_nodeSize, y * m_nodeSize));
    quad[2].position = static_cast<sf::Vector2f>(sf::Vector2i((x + 1) * m_nodeSize, (y + 1) * m_nodeSize));
    quad[3].position = static_cast<sf::Vector2f>(sf::Vector2i(x * m_nodeSize, (y + 1) * m_nodeSize));

    // Color the individual quad of the heatmap according to the cost (cost 0 = light bue, cost 255 = black)
    // Inversion of the cost to conform to a [0, 255] range (eg: 0 cost = black = 255 in term of color, so we invert 0 to be 255)
    const auto heatmapColor = static_cast<sf::Uint8>((50 - nodeIdx.getCost())*255/50);

    // TODO: To set the color value of the heatmap, take the maximum cost value possible on the graph, and then, for each cost, calculate the rule of three
    // TODO: (MaxCost - cost) * 255 / MaxCost
    // TODO: I use 50 above, but it is because I'm using Chebyshev Distance to calculate the cost, which is wrong, I need to do the wavefront algorithm when I create the heatmap, and then update the color with the above calcul
    // see: https://gamedevelopment.tutsplus.com/tutorials/understanding-goal-based-vector-field-pathfinding--gamedev-9007

    quad[0].color = sf::Color(0, 0, heatmapColor);
    quad[1].color = sf::Color(0, 0, heatmapColor);
    quad[2].color = sf::Color(0, 0, heatmapColor);
    quad[3].color = sf::Color(0, 0, heatmapColor);
}

void Grid::createOutlineVertex(const int x, const int y)
{
    sf::Vertex* line = &m_outlineVertices[(x + y * m_width) * 8];

    line[0].position = static_cast<sf::Vector2f>(sf::Vector2i(x * m_nodeSize, y * m_nodeSize));
    line[1].position = static_cast<sf::Vector2f>(sf::Vector2i((x + 1) * m_nodeSize, y * m_nodeSize));

    line[2].position = static_cast<sf::Vector2f>(sf::Vector2i((x + 1) * m_nodeSize, y * m_nodeSize));
    line[3].position = static_cast<sf::Vector2f>(sf::Vector2i((x + 1) * m_nodeSize, (y + 1) * m_nodeSize));

    line[4].position = static_cast<sf::Vector2f>(sf::Vector2i((x + 1) * m_nodeSize, (y + 1) * m_nodeSize));
    line[5].position = static_cast<sf::Vector2f>(sf::Vector2i(x * m_nodeSize, (y + 1) * m_nodeSize));

    line[6].position = static_cast<sf::Vector2f>(sf::Vector2i(x * m_nodeSize, (y + 1) * m_nodeSize));
    line[7].position = static_cast<sf::Vector2f>(sf::Vector2i(x * m_nodeSize, y * m_nodeSize));

    line->color = sf::Color::White;
}
