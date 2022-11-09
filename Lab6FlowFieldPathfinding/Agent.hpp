#ifndef LAB6FLOWFIELD_AGENT_HPP
#define LAB6FLOWFIELD_AGENT_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "Grid.hpp"

class Agent : public sf::Drawable
{
public:
    Agent(Grid& grid, sf::Vector2f startPosition, float maxSpeed, float maxForce);

    sf::Vector2f getPosition();

    void setPosition(sf::Vector2f newPosition);

    void setPosition(float x, float y);

    float getRotation();

    void setRotation(float newRotation);

    void update(sf::Time dt);

private:
    sf::Vector2f steeringBehaviourFlowField();

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    sf::CircleShape m_shape;

    Grid& m_grid;

    sf::Vector2f m_velocity;

    float m_maxSpeed;
    float m_maxForce;
};


#endif //LAB6FLOWFIELD_AGENT_HPP
