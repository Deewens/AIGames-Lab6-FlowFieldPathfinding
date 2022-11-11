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
    Node(const FontManager& fontManager, Grid& grid, sf::Vector2i coordinates, float size);

    /**
     * \brief Get the list of neighbours of this node
     * \param includeDiagonals include neighbours in diagonal
     * \return list of neighbours for this node
     */
    std::vector<std::shared_ptr<Node>> getNeighbours(bool includeDiagonals = true) const;

    /**
     * \brief Look for all this neighbour's node and return the one that the flow field direction points to
     * \return node where the flow field direction points to
     */
    std::shared_ptr<Node> findNextNode() const;

    /**
     * \brief Set the color for this node
     * \param color RGBA Color
     */
    void setQuadColor(sf::Color color);

    /**
     * \brief Get Local grid coordinates of this node
     * \return the local grid coordinates (not the world position)
     */
    sf::Vector2i getCoordinates() const;

    int getCostDistance() const;
    void setCostDistance(int cost);

    int getIntegrationField() const;
    void setIntegrationField(int integrationField);

    sf::Vector2f getFlowFieldDirection() const;
    void setFlowFieldDirection(sf::Vector2f fieldDirection);

    bool isVisualDebugEnabled();
    void setVisualDebugEnabled(bool enabled);

private:
    void createQuadVertices();
    void createOutlineVertices();

    void updateQuadColor();

    void setupDebugText(const FontManager& fontManager);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    // Reference to the grid linked to this node
    Grid& m_grid;
 
    // (X,Y) coordinates of the node in the grid
    sf::Vector2i m_coordinates;

    /**
     * \brief Size of the node
     */
    float m_size;

    int m_costDistance;
    int m_integrationField;

    // Store the direction of the flow in the direction of the goal
    // (also called vector field)
    sf::Vector2f m_flowFieldDirection;

    /*
     * DEBUG PROPERTIES
     */
 
    sf::CircleShape m_positionPoint;

    sf::VertexArray m_vertices;
    sf::VertexArray m_outlineVertices;

    Arrow m_arrow;

    sf::Text m_costText;
    sf::Text m_integrationFieldText;

    bool m_isVisualDebugEnabled;
};


#endif //LAB6FLOWFIELD_NODE_HPP
