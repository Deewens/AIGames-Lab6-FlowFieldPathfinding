#ifndef LAB6FLOWFIELD_AGENT_HPP
#define LAB6FLOWFIELD_AGENT_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "Grid.hpp"

class Agent : public sf::Drawable
{
public:
    Agent(Grid& grid, sf::Vector2f startPosition, float maxSpeed, float maxForce);

    sf::Vector2f getPosition() const;
    void setPosition(sf::Vector2f newPosition);
    void setPosition(float x, float y);

    float getRotation() const;
    void setRotation(float newRotation);

    float getRadius() const;

    float getOutlineThickness() const;
    
    void update(sf::Time dt);

private:
    /**
     * \brief Steering behaviour using Bilinear Interpolation
     * \details https://en.wikipedia.org/wiki/Bilinear_interpolation
     * \return calculated velocity to be assigned to the agent
     */
    sf::Vector2f steeringBehaviourFlowField() const;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    // Reference of the grid where the agent navigate in
    Grid& m_grid;

    sf::CircleShape m_shape;

    sf::Vector2f m_velocity;
    float m_maxSpeed;
    float m_maxForce;
};


#endif //LAB6FLOWFIELD_AGENT_HPP
