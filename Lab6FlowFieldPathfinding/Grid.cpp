#include "Grid.hpp"

#include <iostream>
#include <queue>

#include <SFML/Graphics/RenderTarget.hpp>

Grid::Grid(const FontManager& fontManager, const int width, const int height, const int nodeSize) :
    m_width(width),
    m_height(height),
    m_nodeSize(nodeSize)
{
    m_nodes.reserve(m_width * m_height);

    for (int i = 0; i < m_width; i++)
    {
        for (int j = 0; j < m_height; j++)
        {
            m_nodes.emplace_back(fontManager, i, j, m_nodeSize, m_nodes);
        }
    }

    createHeatmap(sf::Vector2i(5, 10));
}

void Grid::update(sf::Time dt)
{
}

std::vector<Node>& Grid::getNodes()
{
    return m_nodes;
}

int Grid::getNodeSize() const
{
    return m_nodeSize;
}

void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (auto& node : m_nodes)
    {
        target.draw(node);
    }
}

void Grid::createHeatmap(const sf::Vector2i goalPosition)
{
    auto goal = std::find_if(m_nodes.begin(), m_nodes.end(), [=](const Node& node)
    {
        return node.getPosition() == goalPosition;
    });
    if (goal == m_nodes.end()) return;

    goal->setCost(0);

    for (auto& node : m_nodes)
    {
        auto position = node.getPosition();

        
    }

    /*
    for (auto& node : m_nodes)
    {
        if (node.getPosition()) continue;

        
        auto distance = std::max(std::abs(goalPosition.x - node.getX()), std::abs(goalPosition.y - node.getY()));
        node.setCost(distance);
    }*/
}