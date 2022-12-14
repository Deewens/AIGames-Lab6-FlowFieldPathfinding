#include "Agent.hpp"

#include <iostream>

#include "utils/VectorUtils.hpp"

Agent::Agent(Grid& grid, const sf::Vector2f startPosition, const float maxSpeed, const float maxForce) :
    m_grid(grid),
    m_maxSpeed(maxSpeed),
    m_maxForce(maxForce)
{
    m_shape.setFillColor(sf::Color::Cyan);
    m_shape.setOutlineThickness(1);
    m_shape.setOutlineColor(sf::Color::Cyan);
    m_shape.setPosition(startPosition);
    m_shape.setRadius(5);
    m_shape.setOrigin(m_shape.getRadius(), m_shape.getRadius());
}

sf::Vector2f Agent::getPosition() const
{
    return m_shape.getPosition();
}

void Agent::setPosition(const sf::Vector2f newPosition)
{
    m_shape.setPosition(newPosition);
}

void Agent::setPosition(float x, float y)
{
    m_shape.setPosition({x, y});
}

float Agent::getRotation() const
{
    return m_shape.getRotation();
}

void Agent::setRotation(const float newRotation)
{
    m_shape.setRotation(newRotation);
}

float Agent::getRadius() const
{
    return m_shape.getRadius();
}

float Agent::getOutlineThickness() const
{
    return m_shape.getOutlineThickness();
}


void Agent::update(const sf::Time dt)
{
    const auto forceToApply = steeringBehaviourFlowField();

    m_velocity = m_velocity + (forceToApply * dt.asSeconds());

    auto speed = VectorUtils::getLength(m_velocity);
    if (speed > m_maxSpeed)
    {
        m_velocity = m_velocity * (m_maxSpeed / speed);
    }

    setRotation(VectorUtils::vectorToAngle(m_velocity));

    setPosition(getPosition() + (m_velocity * dt.asSeconds()));
}

void Agent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_shape);
}

sf::Vector2f Agent::steeringBehaviourFlowField() const
{
    const sf::Vector2i nodeCoords = m_grid.convertWorldToGridCoordinates(getPosition());
    //std::cout << "\rGrid coords: " << nodeCoords.x << " " << nodeCoords.y << "\tAgent pos: " << getPosition().x << " " << getPosition().y << std::flush;

    sf::Vector2f f00;
    sf::Vector2f f01;
    sf::Vector2f f10;
    sf::Vector2f f11;

    const auto node00 = m_grid.findNode({nodeCoords.x, nodeCoords.y});
    const auto node01 = m_grid.findNode({nodeCoords.x, nodeCoords.y + 1});
    const auto node10 = m_grid.findNode({nodeCoords.x + 1, nodeCoords.y});
    const auto node11 = m_grid.findNode({nodeCoords.x + 1, nodeCoords.y + 1});
    if (node00 != nullptr)
    {
        f00 = sf::Vector2f(node00->getFlowFieldDirection());
    }
    else
    {
        // Return no velocity because the agent is not on a node parts of the grid
        return {0, 0};
    }

    if (node01 != nullptr)
    {
        f01 = sf::Vector2f(node01->getFlowFieldDirection());
    }
    else
    {
        f01 = sf::Vector2f(0, -1);
    }
    if (node10 != nullptr)
    {
        f10 = sf::Vector2f(node10->getFlowFieldDirection());
    }
    else
    {
        f10 = sf::Vector2f(-1, 0);
    }

    if (node11 != nullptr)
    {
        f11 = sf::Vector2f(node11->getFlowFieldDirection());
    }
    else
    {
        f11 = sf::Vector2f(1, 1);
    }

    const sf::Vector2f nodeGridPos = m_grid.convertWorldToGridPosition(getPosition());

    const auto xWeight = nodeGridPos.x - static_cast<float>(nodeCoords.x);

    const auto top = (f00 * (1 - xWeight)) + (f10 * xWeight);
    const auto bottom = (f01 * (1 - xWeight)) + (f11 * xWeight);

    const auto yWeight = nodeGridPos.y - static_cast<float>(nodeCoords.y);

    const auto direction = VectorUtils::normalize((top * (1 - yWeight)) + (bottom * yWeight));
    if (std::isnan(VectorUtils::getLength(direction)))
    {
        return {0, 0};
    }

    const auto desiredVelocity = direction * m_maxSpeed;

    const auto velocityChange = desiredVelocity - m_velocity;

    return velocityChange * (m_maxForce / m_maxSpeed);
}
