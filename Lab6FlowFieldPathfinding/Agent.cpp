#include "Agent.hpp"

#include <iostream>

#include "utils/VectorUtils.hpp"

Agent::Agent(Grid& grid, sf::Vector2f startPosition, float maxSpeed, float maxForce) :
    m_grid(grid),
    m_maxSpeed(maxSpeed),
    m_maxForce(maxForce)
{
    m_shape.setFillColor(sf::Color::Transparent);
    m_shape.setOutlineThickness(3);
    m_shape.setOutlineColor(sf::Color::White);
    m_shape.setPosition(startPosition);
    m_shape.setRadius(12);
    m_shape.setOrigin(m_shape.getRadius(), m_shape.getRadius());
}

sf::Vector2f Agent::getPosition()
{
    return m_shape.getPosition();
}

void Agent::setPosition(sf::Vector2f newPosition)
{
    m_shape.setPosition(newPosition);
}

void Agent::setPosition(float x, float y)
{
    m_shape.setPosition({x, y});
}

float Agent::getRotation()
{
    return m_shape.getRotation();
}

void Agent::setRotation(float newRotation)
{
    m_shape.setRotation(newRotation);
}


void Agent::update(const sf::Time dt)
{
    auto forceToApply = steeringBehaviourFlowField();

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

sf::Vector2f Agent::steeringBehaviourFlowField()
{
    const sf::Vector2i nodeCoords = m_grid.convertWorldToGridCoordinates(getPosition());

    const auto f00 = sf::Vector2f(m_grid.findNode({nodeCoords.x, nodeCoords.y})->getFlowFieldDirection());
    const auto f01 = sf::Vector2f(m_grid.findNode({nodeCoords.x, nodeCoords.y + 1})->getFlowFieldDirection());
    const auto f10 = sf::Vector2f(m_grid.findNode({nodeCoords.x + 1, nodeCoords.y})->getFlowFieldDirection());
    const auto f11 = sf::Vector2f(m_grid.findNode({nodeCoords.x + 1, nodeCoords.y + 1})->getFlowFieldDirection());

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
