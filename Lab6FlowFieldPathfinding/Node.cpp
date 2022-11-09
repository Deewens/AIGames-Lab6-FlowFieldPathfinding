#include "Node.hpp"

#include <iostream>

#include "Grid.hpp"

Node::Node(const FontManager& fontManager, Grid& grid, sf::Vector2i localCoordinate, float size) :
    m_size(size),
    m_localPosition(localCoordinate),
    m_grid(grid),
    m_cost(-1),
    m_integrationField(-1),
    m_vertices(sf::Quads, 4),
    m_outlineVertices(sf::Lines, 8)
{
    const float halfSize = m_size / 2;
    // Offset from border of window because the origin point of each node is centered
    const float gridOffset = halfSize / 2;

    setPosition(static_cast<float>(localCoordinate.x) * m_size + halfSize,
                static_cast<float>(localCoordinate.y) * m_size + halfSize);

    // Origin is at the center of each node
    setOrigin(halfSize, halfSize);

    setupDebugText(fontManager);

    createQuadVertices();
    createOutlineVertices();

    m_positionPoint.setRadius(2);
    m_positionPoint.setFillColor(sf::Color::Red);
    m_positionPoint.setOrigin(m_positionPoint.getRadius(), m_positionPoint.getRadius());
    m_positionPoint.setPosition(getPosition());

    //std::cout << getPosition().x << " " << getPosition().y << std::endl;
    m_arrow.setPosition(getPosition());
    m_arrow.setLength(halfSize);
}

sf::Vector2i Node::getLocalCoordinates() const
{
    return m_localPosition;
}

sf::Vector2f Node::getLocalOrigin() const
{
    return {static_cast<float>(m_localPosition.x) + 0.5f, static_cast<float>(m_localPosition.y) + 0.5f};
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

void Node::updateQuadColor()
{
    if (m_cost == 0)
    {
        setQuadColor(sf::Color::Red);
    }
    else if (m_cost == INT_MAX)
    {
        setQuadColor(sf::Color::Magenta);
    }
    else
    {
        const auto heatmapColor = static_cast<sf::Uint8>((50 - m_cost) * 255 / 50);

        setQuadColor(sf::Color(0, 0, heatmapColor));
    }
}

void Node::createQuadVertices()
{
    const auto x = getPosition().x;
    const auto y = getPosition().y;

    const float halfSize = m_size / 2;

    m_vertices[0].position = sf::Vector2f(x - halfSize, y - halfSize);
    m_vertices[1].position = sf::Vector2f(x + halfSize, y - halfSize);
    m_vertices[2].position = sf::Vector2f(x + halfSize, y + halfSize);
    m_vertices[3].position = sf::Vector2f(x - halfSize, y + halfSize);

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
    const float halfSize = m_size / 2;
    const auto x = getLocalCoordinates().x;
    const auto y = getLocalCoordinates().y;


    m_outlineVertices[0].position = sf::Vector2f(
        x * m_grid.getNodeSize(), y * m_grid.getNodeSize());
    m_outlineVertices[1].position = sf::Vector2f(
        (x + 1) * m_grid.getNodeSize(), y * m_grid.getNodeSize());

    m_outlineVertices[2].position = sf::Vector2f(
        (x + 1) * m_grid.getNodeSize(), y * m_grid.getNodeSize());
    m_outlineVertices[3].position = sf::Vector2f(
        (x + 1) * m_grid.getNodeSize(), (y + 1) * m_grid.getNodeSize());

    m_outlineVertices[4].position = sf::Vector2f(
        (x + 1) * m_grid.getNodeSize(), (y + 1) * m_grid.getNodeSize());
    m_outlineVertices[5].position = sf::Vector2f(
        x * m_grid.getNodeSize(), (y + 1) * m_grid.getNodeSize());

    m_outlineVertices[6].position = sf::Vector2f(
        x * m_grid.getNodeSize(), (y + 1) * m_grid.getNodeSize());
    m_outlineVertices[7].position = sf::Vector2f(
        x * m_grid.getNodeSize(), y * m_grid.getNodeSize());

    for (size_t i = 0; i < m_outlineVertices.getVertexCount(); i++)
    {
        m_outlineVertices[i].color = sf::Color::White;
    }
}

void Node::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(m_vertices, states);
    target.draw(m_outlineVertices);
    //if (m_cost != INT_MAX) target.draw(m_costText, states);
    //target.draw(m_integrationFieldText, states);

    if (m_flowFieldDirection != sf::Vector2f(0, 0)) target.draw(m_arrow, states);

    target.draw(m_positionPoint);
}

void Node::setupDebugText(const FontManager& fontManager)
{
    m_costText.setString(std::to_string(m_cost));
    m_costText.setFont(fontManager.get(Assets::Font::ArialBlack));
    m_costText.setPosition(getPosition());
    m_costText.setCharacterSize(15);

    m_integrationFieldText.setFont(fontManager.get(Assets::Font::ArialBlack));
    m_integrationFieldText.setCharacterSize(15);
    m_integrationFieldText.setPosition(getPosition().x + m_size - 30,
                                       getPosition().y);
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

        const int neighbourX = m_localPosition.x + ((direction % 3) - 1);
        const int neighbourY = m_localPosition.y + ((direction / 3) - 1);

        if (neighbourX < 0 || neighbourX > m_grid.getWidth() - 1 ||
            neighbourY < 0 || neighbourY > m_grid.getHeight() - 1)
        {
            continue;
        }

        // Calculate the node cell in the list
        auto neighbour = m_grid.findNode(neighbourX, neighbourY);
        neighbours.push_back(neighbour);
    }

    return neighbours;
}
