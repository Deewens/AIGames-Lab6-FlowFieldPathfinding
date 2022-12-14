#include "Node.hpp"

#include <iostream>

#include "Grid.hpp"
#include "utils/VectorUtils.hpp"

Node::Node(const FontManager& fontManager, Grid& grid, const sf::Vector2i coordinates, const float size) :
    m_grid(grid),
    m_coordinates(coordinates),
    m_size(size),
    m_costDistance(-1),
    m_integrationField(-1),
    m_vertices(sf::Quads, 4),
    m_outlineVertices(sf::Lines, 4),
    m_isVisualDebugEnabled(false)
{
    // Calculate an offset from the border of the window because the origin point of each node is centered
    const float halfSize = m_size / 2;
    setPosition(static_cast<float>(m_coordinates.x) * m_size + halfSize,
                static_cast<float>(m_coordinates.y) * m_size + halfSize);

    // Origin is at the center of each node
    setOrigin(halfSize, halfSize);

    // Initialize debugging visualisation
    setupDebugText(fontManager);

    createQuadVertices();
    createOutlineVertices();

    m_positionPoint.setRadius(2);
    m_positionPoint.setFillColor(sf::Color::Red);
    m_positionPoint.setOrigin(m_positionPoint.getRadius(), m_positionPoint.getRadius());
    m_positionPoint.setPosition(getPosition());

    m_arrow.setPosition(getPosition());
    m_arrow.setLength(halfSize);
}

int Node::getCostDistance() const
{
    return m_costDistance;
}

void Node::setCostDistance(const int cost)
{
    m_costDistance = cost;

    m_costText.setString(std::to_string(m_costDistance));
    updateQuadColor();
}

int Node::getIntegrationField() const
{
    return m_integrationField;
}

void Node::setIntegrationField(const int integrationField)
{
    m_integrationField = integrationField;

    m_integrationFieldText.setString(std::to_string(m_integrationField));
}

sf::Vector2f Node::getFlowFieldDirection() const
{
    return m_flowFieldDirection;
}

void Node::setFlowFieldDirection(const sf::Vector2f fieldDirection)
{
    m_flowFieldDirection = fieldDirection;

    m_arrow.setDirection(sf::Vector2f(fieldDirection));
}

void Node::setQuadColor(sf::Color color)
{
    for (size_t i = 0; i < m_vertices.getVertexCount(); i++)
    {
        m_vertices[i].color = color;
    }
}

bool Node::isVisualDebugEnabled() const
{
    return m_isVisualDebugEnabled;
}

void Node::setVisualDebugEnabled(const bool enabled)
{
    m_isVisualDebugEnabled = enabled;
}

void Node::updateQuadColor()
{
    // 0 = Goal Node
    if (m_costDistance == 0)
    {
        setQuadColor(sf::Color::Red);
    }
    // INT_MAX = Impassable node
    else if (m_costDistance == INT_MAX)
    {
        setQuadColor(sf::Color::Magenta);
    }
    // Otherwise, assign a color according to the cost to create the heatmap
    else
    {
        const auto heatmapColor = static_cast<sf::Uint8>((50 - m_costDistance) * 255 / 50);

        setQuadColor(sf::Color(0, 0, heatmapColor));
    }
}

void Node::createQuadVertices()
{
    m_vertices[0].position = sf::Vector2f(0, 0);
    m_vertices[1].position = sf::Vector2f(m_size, 0);
    m_vertices[2].position = sf::Vector2f(m_size, m_size);
    m_vertices[3].position = sf::Vector2f(0, m_size);

    // Color the individual quad of the heatmap according to the cost (cost 0 = light bue, cost 255 = black)
    // Inversion of the cost to conform to a [0, 255] range (eg: 0 cost = black = 255 in term of color, so we invert 0 to be 255)
    // const auto heatmapColor = static_cast<sf::Uint8>((50 - m_cost) * 255 / 50);
    //
    // To set the color value of the heatmap, take the maximum cost value possible on the graph, and then, for each cost, calculate the rule of three
    // (MaxCost - cost) * 255 / MaxCost
    const int maxCost = (m_grid.getWidth() - 1) + (m_grid.getHeight() - 1);
    const auto heatmapColor = static_cast<sf::Uint8>((maxCost - m_costDistance) * 255 / maxCost);

    for (size_t i = 0; i < m_vertices.getVertexCount(); i++)
    {
        m_vertices[i].color = sf::Color(0, 0, heatmapColor);
    }
}

void Node::createOutlineVertices()
{
    // Draw vertices according to local coordinates of the cell
    m_outlineVertices[0].position = sf::Vector2f(0, 0);
    m_outlineVertices[1].position = sf::Vector2f(m_size, 0);

    m_outlineVertices[2].position = sf::Vector2f(m_size, 0);
    m_outlineVertices[3].position = sf::Vector2f(m_size, m_size);

    // 4 vertices are enough to draw the outline, more vertex will overlap anyways
    /*m_outlineVertices[4].position = sf::Vector2f(m_size, m_size);
    m_outlineVertices[5].position = sf::Vector2f(0, m_size);

    m_outlineVertices[6].position = sf::Vector2f(0, m_size);
    m_outlineVertices[7].position = sf::Vector2f(0, 0);*/

    for (size_t i = 0; i < m_outlineVertices.getVertexCount(); i++)
    {
        m_outlineVertices[i].color = sf::Color::White;
    }
}

void Node::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(m_vertices, states);
    target.draw(m_outlineVertices, states);

    if (m_isVisualDebugEnabled)
    {
        if (m_costDistance != INT_MAX) target.draw(m_costText, states);
        target.draw(m_integrationFieldText, states);
        if (m_flowFieldDirection != sf::Vector2f(0, 0)) target.draw(m_arrow, states);

        target.draw(m_positionPoint);
    }
}

void Node::setupDebugText(const FontManager& fontManager)
{
    m_costText.setString(std::to_string(m_costDistance));
    m_costText.setFont(fontManager.get(Assets::Font::ArialBlack));
    m_costText.setPosition(0, 0);
    m_costText.setCharacterSize(15);

    m_integrationFieldText.setFont(fontManager.get(Assets::Font::ArialBlack));
    m_integrationFieldText.setCharacterSize(15);
    m_integrationFieldText.setPosition(30, 0);
}

std::vector<std::shared_ptr<Node>> Node::getNeighbours(bool includeDiagonals) const
{
    std::vector<std::shared_ptr<Node>> neighbours;

    // Loop through the 8 directions to find the neighbours of a rectangular cell
    for (int direction = 0; direction < 9; direction++)
    {
        // Skip 4, this is ourself
        if (direction == 4) continue;

        if (!includeDiagonals)
        {
            if (direction == 0 || direction == 2 || direction == 6 || direction == 8) continue;
        }

        const int neighbourX = m_coordinates.x + ((direction % 3) - 1);
        const int neighbourY = m_coordinates.y + ((direction / 3) - 1);

        if (neighbourX < 0 || neighbourX > m_grid.getWidth() - 1 ||
            neighbourY < 0 || neighbourY > m_grid.getHeight() - 1)
        {
            continue;
        }

        // Calculate the node cell in the list
        auto neighbour = m_grid.findNode({neighbourX, neighbourY});
        neighbours.push_back(neighbour);
    }

    return neighbours;
}

sf::Vector2i Node::getCoordinates() const
{
    return m_coordinates;
}

std::shared_ptr<Node> Node::findNextNode() const
{
    const auto neighbours = getNeighbours();

    for (auto neighbour : neighbours)
    {
        auto directionToNeighbour = VectorUtils::normalize(neighbour->getPosition() - getPosition());
        if (directionToNeighbour == m_flowFieldDirection)
        {
            return neighbour;
        }
    }

    return nullptr;
}
