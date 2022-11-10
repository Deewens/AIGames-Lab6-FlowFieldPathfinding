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

float Agent::getRadius()
{
    return m_shape.getRadius();
}

float Agent::getOutlineThickness()
{
    return m_shape.getOutlineThickness();
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

    auto direction = VectorUtils::normalize((top * (1 - yWeight)) + (bottom * yWeight));
        /*
    if (m_grid.findNode({nodeCoords.x, nodeCoords.y + 1}) == nullptr)
    {
        direction = sf::Vector2f(0, -1);
    }*/
    //std::cout << "\r" << direction.x << " " << direction.y << "                ";

    if (std::isnan(VectorUtils::getLength(direction)))
    {
        return {0, 0};
    }

    const auto desiredVelocity = direction * m_maxSpeed;

    const auto velocityChange = desiredVelocity - m_velocity;

    return velocityChange * (m_maxForce / m_maxSpeed);
}
