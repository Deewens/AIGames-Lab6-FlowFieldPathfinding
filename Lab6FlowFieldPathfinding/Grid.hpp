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
    Grid(const FontManager &fontManager, int width, int height, float nodeSize, std::list<sf::Vector2i> obstacles);

    const std::vector<std::shared_ptr<Node>> &getNodes() const;

    int getWidth() const;

    int getHeight() const;

    float getNodeSize() const;

    void calculateFlowField();

    std::shared_ptr<Node> findNode(const sf::Vector2i& coordinates);
    
    std::shared_ptr<Node> findNodeByPosition(const sf::Vector2f& worldPosition);

    sf::Vector2i convertWorldToGridCoordinates(const sf::Vector2f& worldPosition);
    sf::Vector2f convertWorldToGridPosition(const sf::Vector2f& worldPosition);

    void setGoalCoordinates(sf::Vector2i goalCoordinates);
    sf::Vector2i getGoalCoordinates();



    std::list<sf::Vector2i> getObstacles();
    void addObstacle(int x, int y);
    void removeObstacle(int x, int y);

    void setStartPosition(sf::Vector2i coordinates);
    void calculatePathFromStart();
    
    void toggleDebugData();

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    std::vector<std::shared_ptr<Node>> m_nodes;
    
    void createCostField();

    void createIntegrationField();


    // Number of columns of the grid
    int m_width;
    // Number of rows of the grid
    int m_height;

    float m_nodeSize;

    sf::Vector2i m_goalCoordinates;

    std::vector<sf::Vector2i> m_pathFromStart;
    
    std::list<sf::Vector2i> m_obstacles;
};


#endif //LAB6FLOWFIELD_GRID_HPP
