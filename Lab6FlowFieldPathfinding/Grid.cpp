#include "Grid.hpp"
#include "utils/VectorUtils.hpp"

#include <iostream>
#include <queue>

Grid::Grid(const FontManager& fontManager, int width, int height, float nodeSize, std::list<sf::Vector2i> obstacles) :
    m_width(width),
    m_height(height),
    m_nodeSize(nodeSize),
    m_obstacles(obstacles)
{
    m_nodes.reserve(width * height);

    for (int i = 0; i < m_width; i++)
    {
        for (int j = 0; j < m_height; j++)
        {
            m_nodes.emplace_back(std::make_shared<Node>(
                fontManager,
                *this,
                sf::Vector2i(i, j),
                m_nodeSize
            ));
        }
    }
}

const std::vector<std::shared_ptr<Node>>& Grid::getNodes() const
{
    return m_nodes;
}

int Grid::getWidth() const
{
    return m_width;
}

int Grid::getHeight() const
{
    return m_height;
}

float Grid::getNodeSize() const
{
    return m_nodeSize;
}

std::shared_ptr<Node> Grid::findNode(const sf::Vector2i& coordinates)
{
    return m_nodes.at(coordinates.y + m_width * coordinates.x);
}

std::shared_ptr<Node> Grid::findNodeByPosition(const sf::Vector2f& worldPosition)
{
    const sf::Vector2i nodeCoordinates = convertWorldToGridCoordinates(worldPosition);
    return findNode(nodeCoordinates);
}

sf::Vector2i Grid::convertWorldToGridCoordinates(const sf::Vector2f& worldPosition)
{
    // Calculate grid coordinate from world position
    const sf::Vector2f gridPosition = convertWorldToGridPosition(worldPosition);
    const int xCoord = static_cast<int>(std::floor(gridPosition.x));
    const int yCoord = static_cast<int>(std::floor(gridPosition.y));

    return {xCoord, yCoord};
}

sf::Vector2f Grid::convertWorldToGridPosition(const sf::Vector2f& worldPosition)
{
    // Calculate grid coordinate from world position
    const float halfSize = m_nodeSize / 2;
    const float xGridPos = (worldPosition.x - halfSize) / m_nodeSize;
    const float yGridPos = (worldPosition.y - halfSize) / m_nodeSize;

    return {xGridPos, yGridPos};
}

void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (auto& node : m_nodes)
    {
        target.draw(*node);
    }
}

void Grid::calculateFlowField(sf::Vector2<int> goalPosition)
{
    // Refresh
    for (const auto& node : m_nodes)
    {
        node->setCost(-1);
        node->setIntegrationField(-1);
        node->setFlowFieldDirection({0, 0});
    }

    for (auto obstacle : m_obstacles)
    {
        auto node = findNode(obstacle);
        node->setCost(INT_MAX);
        node->setIntegrationField(INT_MAX);
    }

    createCostField(goalPosition);
    createIntegrationField(goalPosition);

    for (const auto& node : m_nodes)
    {
        if (node->getCost() == INT_MAX)
        {
            continue;
        }

        const auto neighbours = node->getNeighbours();

        auto lowestIntegrationField = neighbours[0];
        for (size_t i = 1; i < neighbours.size(); i++)
        {
            if (neighbours[i]->getIntegrationField() < lowestIntegrationField->getIntegrationField())
            {
                lowestIntegrationField = neighbours[i];
            }
        }

        // The direction is already normalised
        sf::Vector2f direction;
        direction = VectorUtils::normalize(lowestIntegrationField->getPosition() - node->getPosition());

        /*std::cout << "Lowest: " << lowestIntegrationField->getPosition().x << ", " << lowestIntegrationField->getPosition().y << std::endl;
        std::cout << "Node: " << node->getPosition().x << ", " << node->getPosition().y << std::endl;*/
        //std::cout << direction.x << ", " << direction.y << std::endl;
        if (node->getCost() != 0) node->setFlowFieldDirection(direction);
    }
}

void Grid::createCostField(sf::Vector2<int> goalPosition)
{
    auto goal = findNode(goalPosition);
    goal->setCost(0);

    std::queue<std::shared_ptr<Node>> leftNodes;
    leftNodes.push(goal);

    while (!leftNodes.empty())
    {
        const auto current = leftNodes.front();
        leftNodes.pop();

        auto neighbours = current->getNeighbours(false);
        for (auto& neighbour : neighbours)
        {
            if (neighbour->getCost() == -1)
            {
                neighbour->setCost(current->getCost() + 1);
                leftNodes.push(neighbour);
            }
        }
    }
}

void Grid::createIntegrationField(const sf::Vector2i goalPosition)
{
    const std::shared_ptr<Node> goal = findNode(goalPosition);
    goal->setIntegrationField(0);

    std::queue<std::shared_ptr<Node>> leftNodes;
    leftNodes.push(goal);

    while (!leftNodes.empty())
    {
        const auto current = leftNodes.front();
        leftNodes.pop();

        if (current->getCost() == INT_MAX) continue;

        auto neighbours = current->getNeighbours();
        for (auto& neighbour : neighbours)
        {
            if (neighbour->getCost() == INT_MAX) continue;

            if (neighbour->getIntegrationField() == -1)
            {
                leftNodes.push(neighbour);

                const int distance = std::max(std::abs(neighbour->getPosition().x - goal->getPosition().x),
                                              std::abs(neighbour->getPosition().y - goal->getPosition().y));

                neighbour->setIntegrationField(neighbour->getCost() + distance);
            }
        }
    }
}

void Grid::addObstacle(int x, int y)
{
    m_obstacles.emplace_back(x, y);
}

void Grid::removeObstacle(int x, int y)
{
    m_obstacles.remove(sf::Vector2i(x, y));
}
