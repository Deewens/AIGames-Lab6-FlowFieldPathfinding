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

    int getCostDistance() const;

    void setCostDistance(int cost);

    int getIntegrationField() const;

    void setIntegrationField(int integrationField);

    sf::Vector2f getFlowFieldDirection() const;

    void setFlowFieldDirection(sf::Vector2f fieldDirection);

    std::vector<std::shared_ptr<Node>> getNeighbours(bool includeDiagonals = true) const;

    sf::Vector2i getCoordinates() const;

    std::shared_ptr<Node> findNextNode() const;

    void setQuadColor(sf::Color color);

    bool isVisualDebugEnabled();
    void setVisualDebugEnabled(bool enabled);
private:

    void updateQuadColor();

    void createQuadVertices();

    void createOutlineVertices();

    void setupDebugText(const FontManager& fontManager);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::CircleShape m_positionPoint;

    float m_size;

    // (X,Y) coordinates of the node in the grid
    sf::Vector2i m_coordinates;

    Grid& m_grid;

    int m_costDistance;
    int m_integrationField;

    // Store the direction of the flow in the direction of the goal
    // (also called vector field)
    sf::Vector2f m_flowFieldDirection;
    Arrow m_arrow;

    sf::Text m_costText;
    sf::Text m_integrationFieldText;

    sf::VertexArray m_vertices;
    sf::VertexArray m_outlineVertices;

    bool m_isVisualDebugEnabled;
};


#endif //LAB6FLOWFIELD_NODE_HPP
