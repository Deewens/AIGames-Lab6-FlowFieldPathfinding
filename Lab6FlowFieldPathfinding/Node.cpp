#include "Node.hpp"

#include <ostream>
#include <iostream>

#include <SFML/Graphics/RenderTarget.hpp>

#include "Direction.hpp"

Node::Node(const FontManager& fontManager, const int x, const int y, const int& nodeSize,
           std::vector<Node>& gridNodes) :
    m_x(x), m_y(y), m_nodeSize(nodeSize),
    m_cost(1),
    m_vertices(sf::Quads, 4),
    m_outlineVertices(sf::Lines, 8),
    m_nodes(gridNodes)
{
    m_costText.setString(std::to_string(m_cost));
    m_costText.setFont(fontManager.get(Assets::Font::ArialBlack));

    m_costText.setPosition(static_cast<float>(x * m_nodeSize), static_cast<float>(y * m_nodeSize));
    m_costText.setCharacterSize(10);

    createQuadVertices();
    createOutlineVertices();
}

int Node::getCost() const
{
    return m_cost;
}

void Node::setCost(const int cost)
{
    m_cost = cost;

    m_costText.setString(std::to_string(m_cost));

    updateQuadColor();
}

const sf::Text& Node::getCostText() const
{
    return m_costText;
}

sf::Vector2i Node::getPosition() const
{
    return {m_x, m_y};
}

int Node::getX() const
{
    return m_x;
}

int Node::getY() const
{
    return m_y;
}

std::vector<Node> Node::getNeighbours()
{
    auto northPosition = getPosition() + sf::Vector2i(0, 1);
    auto eastPosition = getPosition() + sf::Vector2i(1, 0);
    auto southPosition = getPosition() + sf::Vector2i(0, -1);
    auto westPosition = getPosition() + sf::Vector2i(-1, 0);

    auto northEastPosition = getPosition() + sf::Vector2i(1, 1);
    auto southEastPosition = getPosition() + sf::Vector2i(1, -1);
    auto southWestPosition = getPosition() + sf::Vector2i(-1, -1);
    auto northWestPosition = getPosition() + sf::Vector2i(-1, 1);

    for (auto element : Direction::CardinalDirections<int>)
    {
        
    }
    
    auto found = std::find_if(m_nodes.begin(), m_nodes.end(), [=](Node& node)
    {
        return 
    });
}


void Node::setQuadColor(sf::Color color)
{
    for (size_t i = 0; i < m_vertices.getVertexCount(); i++)
    {
        m_vertices[i].color = color;
    }
}

void Node::updateQuadColor()
{
    if (m_cost == 0)
    {
        setQuadColor(sf::Color::Red);
    }
    else
    {
        const auto heatmapColor = static_cast<sf::Uint8>((50 - m_cost) * 255 / 50);

        setQuadColor(sf::Color(0, 0, heatmapColor));
    }
}


void Node::createQuadVertices()
{
    m_vertices[0].position = static_cast<sf::Vector2f>(sf::Vector2i(m_x * m_nodeSize, m_y * m_nodeSize));
    m_vertices[1].position = static_cast<sf::Vector2f>(sf::Vector2i((m_x + 1) * m_nodeSize, m_y * m_nodeSize));
    m_vertices[2].position = static_cast<sf::Vector2f>(sf::Vector2i((m_x + 1) * m_nodeSize, (m_y + 1) * m_nodeSize));
    m_vertices[3].position = static_cast<sf::Vector2f>(sf::Vector2i(m_x * m_nodeSize, (m_y + 1) * m_nodeSize));

    // Color the individual quad of the heatmap according to the cost (cost 0 = light bue, cost 255 = black)
    // Inversion of the cost to conform to a [0, 255] range (eg: 0 cost = black = 255 in term of color, so we invert 0 to be 255)
    const auto heatmapColor = static_cast<sf::Uint8>((50 - m_cost) * 255 / 50);

    // TODO: To set the color value of the heatmap, take the maximum cost value possible on the graph, and then, for each cost, calculate the rule of three
    // TODO: (MaxCost - cost) * 255 / MaxCost
    // TODO: I use 50 above, but it is because I'm using Chebyshev Distance to calculate the cost, which is wrong, I need to do the wavefront algorithm when I create the heatmap, and then update the color with the above calcul
    // see: https://gamedevelopment.tutsplus.com/tutorials/understanding-goal-based-vector-field-pathfinding--gamedev-9007

    for (size_t i = 0; i < m_vertices.getVertexCount(); i++)
    {
        m_vertices[i].color = sf::Color(0, 0, heatmapColor);
    }
}

void Node::createOutlineVertices()
{
    m_outlineVertices[0].position = static_cast<sf::Vector2f>(sf::Vector2i(m_x * m_nodeSize, m_y * m_nodeSize));
    m_outlineVertices[1].position = static_cast<sf::Vector2f>(sf::Vector2i((m_x + 1) * m_nodeSize, m_y * m_nodeSize));

    m_outlineVertices[2].position = static_cast<sf::Vector2f>(sf::Vector2i((m_x + 1) * m_nodeSize, m_y * m_nodeSize));
    m_outlineVertices[3].position = static_cast<sf::Vector2f>(sf::Vector2i(
        (m_x + 1) * m_nodeSize, (m_y + 1) * m_nodeSize));

    m_outlineVertices[4].position = static_cast<sf::Vector2f>(sf::Vector2i(
        (m_x + 1) * m_nodeSize, (m_y + 1) * m_nodeSize));
    m_outlineVertices[5].position = static_cast<sf::Vector2f>(sf::Vector2i(m_x * m_nodeSize, (m_y + 1) * m_nodeSize));

    m_outlineVertices[6].position = static_cast<sf::Vector2f>(sf::Vector2i(m_x * m_nodeSize, (m_y + 1) * m_nodeSize));
    m_outlineVertices[7].position = static_cast<sf::Vector2f>(sf::Vector2i(m_x * m_nodeSize, m_y * m_nodeSize));

    for (size_t i = 0; i < m_outlineVertices.getVertexCount(); i++)
    {
        m_outlineVertices[i].color = sf::Color::White;
    }
}

void Node::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_vertices);
    target.draw(m_outlineVertices);
}
