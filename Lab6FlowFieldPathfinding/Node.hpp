#ifndef LAB6FLOWFIELD_NODE_HPP
#define LAB6FLOWFIELD_NODE_HPP

#include <SFML/Graphics.hpp>

#include "Arrow.hpp"

#include "ResourceManager/ResourceManager.hpp"
#include "ResourceManager/ResourceIdentifiers.hpp"

class Grid;

class Node : public sf::Drawable, public sf::Transformable
{
public:
    Node(const FontManager& fontManager, Grid& grid, sf::Vector2i localCoordinate, float size);

    /**
    * \brief Local position of the node in grid coordinate
    * \return (X, Y) vector local position of the node
    */
    sf::Vector2i getLocalCoordinates() const;

    /**
     * \brief Get local center position in grid coordinate
     * \return Center point of the node
     */
    sf::Vector2f getLocalOrigin() const;

    int getCost() const;

    void setCost(int cost);

    int getIntegrationField() const;

    void setIntegrationField(int integrationField);

    sf::Vector2f getFlowFieldDirection() const;

    void setFlowFieldDirection(sf::Vector2f fieldDirection);

    std::vector<std::shared_ptr<Node>> getNeighbours(bool includeDiagonals = true) const;

private:
    void setQuadColor(sf::Color color);

    void updateQuadColor();

    void createQuadVertices();

    void createOutlineVertices();

    void setupDebugText(const FontManager& fontManager);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::CircleShape m_positionPoint;
    
    float m_size;

    // Local coordinate of the node in the grid
    sf::Vector2i m_localPosition;

    Grid& m_grid;

    int m_cost;
    int m_integrationField;

    // Store the direction of the flow in the direction of the goal
    // (also called vector field)
    sf::Vector2f m_flowFieldDirection;
    Arrow m_arrow;

    sf::Text m_costText;
    sf::Text m_integrationFieldText;

    sf::VertexArray m_vertices;
    sf::VertexArray m_outlineVertices;
};


#endif //LAB6FLOWFIELD_NODE_HPP
