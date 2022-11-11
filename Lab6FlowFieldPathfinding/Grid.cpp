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

    auto node = findNode({2, 14});
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
    if (coordinates.x >= m_width || coordinates.y >= m_height)
    {
        return nullptr;
    }

    try
    {
        return m_nodes.at(coordinates.y + m_width * coordinates.x);
    }
    catch (std::out_of_range const&)
    {
        // The node does not exist in the grid so we return null
        return nullptr;
    }
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
    const int xCoord = static_cast<int>(std::round(gridPosition.x));
    const int yCoord = static_cast<int>(std::round(gridPosition.y));

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

void Grid::setGoalCoordinates(sf::Vector2i goalCoordinates)
{
    m_goalCoordinates = goalCoordinates;
}

sf::Vector2i Grid::getGoalCoordinates() const
{
    return m_goalCoordinates;
}

std::list<sf::Vector2i> Grid::getObstacles()
{
    return m_obstacles;
}

void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (auto& node : m_nodes)
    {
        target.draw(*node);
    }
}

void Grid::computeVectorField() const
{
    for (const auto& node : m_nodes)
    {
        if (node->getCostDistance() == INT_MAX) continue;
        const auto neighbours = node->getNeighbours();

        std::shared_ptr<Node> lowestDistance = nullptr;
        for (size_t i = 0; i < neighbours.size(); i++)
        {
            if (neighbours[i]->getCostDistance() == INT_MAX) continue;
            if (lowestDistance == nullptr)
            {
                lowestDistance = neighbours[i];
                continue;
            }

            if (neighbours[i]->getIntegrationField() < lowestDistance->getIntegrationField())
            {
                lowestDistance = neighbours[i];
            }
        }

        const auto directionToClosestNeighbour = VectorUtils::normalize(
            lowestDistance->getPosition() - node->getPosition());
        if (node->getCostDistance() != 0) node->setFlowFieldDirection(directionToClosestNeighbour);
    }
}

void Grid::calculateFlowField()
{
    // Refresh the grid with default values
    for (const auto& node : m_nodes)
    {
        node->setCostDistance(-1);
        node->setIntegrationField(-1);
        node->setFlowFieldDirection({0, 0});
    }

    // Add obstacles to the grid
    for (auto obstacle : m_obstacles)
    {
        auto node = findNode(obstacle);
        node->setCostDistance(INT_MAX);
        node->setIntegrationField(INT_MAX);
    }

    createCostField();
    createIntegrationField();
    computeVectorField();
}

void Grid::createCostField()
{
    auto goal = findNode(m_goalCoordinates);
    goal->setCostDistance(0);

    std::queue<std::shared_ptr<Node>> unmarkedNodes;
    unmarkedNodes.push(goal);

    while (!unmarkedNodes.empty())
    {
        const auto current = unmarkedNodes.front();
        unmarkedNodes.pop();

        auto neighbours = current->getNeighbours(true);
        for (auto& neighbour : neighbours)
        {
            if (neighbour->getCostDistance() == -1)
            {
                neighbour->setCostDistance(current->getCostDistance() + 1);
                unmarkedNodes.push(neighbour);
            }
        }
    }
}

void Grid::createIntegrationField()
{
    const std::shared_ptr<Node> goal = findNode(m_goalCoordinates);
    goal->setIntegrationField(0);

    std::queue<std::shared_ptr<Node>> unmarkedNodes;
    unmarkedNodes.push(goal);

    while (!unmarkedNodes.empty())
    {
        const auto current = unmarkedNodes.front();
        unmarkedNodes.pop();

        auto neighbours = current->getNeighbours();
        for (auto& neighbour : neighbours)
        {
            if (neighbour->getIntegrationField() == -1 && neighbour->getIntegrationField() != INT_MAX)
            {
                const int distance =
                    VectorUtils::getLength(neighbour->getPosition() - goal->getPosition());
                neighbour->setIntegrationField(neighbour->getCostDistance() * 100 + distance);
                unmarkedNodes.push(neighbour);
            }
        }
    }
}

void Grid::setStartPosition(sf::Vector2i coordinates)
{
    m_pathFromStart.clear();
    m_pathFromStart.push_back(coordinates);
    calculatePathFromStart();
}

void Grid::toggleDebugData()
{
    for (const auto node : m_nodes)
    {
        node->setVisualDebugEnabled(!node->isVisualDebugEnabled());
    }
}

void Grid::calculatePathFromStart()
{
    const auto startCoordinates = m_pathFromStart[0];

    auto currentNode = findNode(startCoordinates);
    currentNode->setQuadColor(sf::Color::Green);
    while (currentNode->getCostDistance() != 0)
    {
        const auto nextNode = currentNode->findNextNode();
        if (nextNode == nullptr) break;

        m_pathFromStart.push_back(nextNode->getCoordinates());
        if (nextNode->getCostDistance() != 0)
        {
            nextNode->setQuadColor(sf::Color::Yellow);
        }


        currentNode = nextNode;
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
