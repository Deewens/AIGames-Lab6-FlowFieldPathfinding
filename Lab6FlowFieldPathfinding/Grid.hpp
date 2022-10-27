#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Time.hpp>

#include "Node.hpp"
#include "ResourceManager/ResourceManager.hpp"
#include "ResourceManager/ResourceIdentifiers.hpp"

class Grid : public sf::Drawable
{
private:
    int m_width;
    int m_height;

    sf::VertexArray m_vertices;
    sf::VertexArray m_outlineVertices;

    std::vector<Node> m_nodes;
    int m_nodeSize;

public:
    Grid(const FontManager& fontManager, int width, int height, int nodeSize);

    void update(sf::Time dt);

    std::vector<Node>& getNodes();

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void createHeatmap(sf::Vector2i goalPosition);
    
    void createVertices();
    void createVertex(int x, int y);
    void createOutlineVertex(int x, int y);
};
