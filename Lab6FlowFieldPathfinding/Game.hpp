#ifndef GAME_HPP
#define GAME_HPP

#include <memory>

#include <SFML/Graphics.hpp>

#include "ResourceManager/ResourceManager.hpp"
#include "ResourceManager/ResourceIdentifiers.hpp"
#include "Grid.hpp"
#include "Agent.hpp"

class Game
{
public:
    Game();

    ~Game();
    
    void run();

private:

    void processEvents();

    void processKeys(sf::Event event);

    void processMouse(const sf::Event &event) const;

    void update(sf::Time deltaTime);

    void render();

    void loadFonts();

    unsigned int static constexpr ScreenSize = 800U;

    sf::RenderWindow m_window;

    FontManager m_fontManager;
    
    bool m_exitGame;

    Grid* m_grid;
    Agent* m_agent;
};

#endif // !GAME_HPP
