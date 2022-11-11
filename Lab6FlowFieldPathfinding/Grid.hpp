#ifndef LAB6FLOWFIELD_GRID_HPP
#define LAB6FLOWFIELD_GRID_HPP

#include <vector>
#include <list>

#include <SFML/Graphics/Drawable.hpp>

#include "ResourceManager/ResourceManager.hpp"
#include "ResourceManager/ResourceIdentifiers.hpp"
#include "Node.hpp"

class Grid : public sf::Drawable
{
public:
    Grid(const FontManager& fontManager, int width, int height, float nodeSize, std::list<sf::Vector2i> obstacles);

    const std::vector<std::shared_ptr<Node>>& getNodes() const;

    int getWidth() const;
    int getHeight() const;

    float getNodeSize() const;

    /**
     * \brief Calculate the flow field pathfinding
     * \details Full algorithm that calculate and compute the full flow field pathfinding.
     * 1. Calculate cost field
     * 2. Compute integration field
     * 3. Compute vector field (set direction to goal in each cell, etc)
     */
    void calculateFlowField();

    /**
     * \brief Find a node by its grid coordinates
     * \warning Not world pixel positions, it is actual grid coordinates
     * \param coordinates coordinates on the grid
     * \return pointer to the found node, nullptr if not found
     */
    std::shared_ptr<Node> findNode(const sf::Vector2i& coordinates);

    /**
     * \brief Find a node by world position
     * \param worldPosition world position in the view/window
     * \return pointer to the found node nullptr is not found
     */
    std::shared_ptr<Node> findNodeByPosition(const sf::Vector2f& worldPosition);

    /**
     * \brief Convert a world position to a local grid coordinate
     * \param worldPosition world position in the view/window
     * \return position converted to grid coordinates
     */
    sf::Vector2i convertWorldToGridCoordinates(const sf::Vector2f& worldPosition);

    /**
     * \brief convert world position to the center world position of a grid cell 
     * \param worldPosition world position in the view/window
     * \return grid cell center calculated from a world position
     */
    sf::Vector2f convertWorldToGridPosition(const sf::Vector2f& worldPosition);

    void setGoalCoordinates(sf::Vector2i goalCoordinates);
    sf::Vector2i getGoalCoordinates();


    std::list<sf::Vector2i> getObstacles();
    void addObstacle(int x, int y);
    void removeObstacle(int x, int y);

    void setStartPosition(sf::Vector2i coordinates);
    void calculatePathFromStart();

    /**
     * \brief Toggle on/off visualisation to display debug data
     */
    void toggleDebugData();

private:
    /**
     * \brief Calculate the cost field for each nodes
     */
    void createCostField();
    /**
     * \brief Compute the integration field by using the previously calculated cost field
     */
    void createIntegrationField();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    std::vector<std::shared_ptr<Node>> m_nodes;

    // Grid size
    int m_width;
    int m_height;

    // Size of each node
    float m_nodeSize;

    sf::Vector2i m_goalCoordinates;

    /**
     * \brief Store list of coordinates from start to goal
     */
    std::vector<sf::Vector2i> m_pathFromStart;

    std::list<sf::Vector2i> m_obstacles;
};


#endif //LAB6FLOWFIELD_GRID_HPP
