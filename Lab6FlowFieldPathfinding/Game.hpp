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

    /// <summary>
    /// main method for game
    /// </summary>
    void run();

private:

    void processEvents();

    void processKeys(sf::Event event);

    void processMouse(const sf::Event &event) const;

    void update(sf::Time deltaTime);

    void render();

    void loadFonts();

    unsigned int static constexpr ScreenSize = 840U;

    sf::RenderWindow m_window; // main SFML window
    bool m_exitGame; // control exiting game

    Grid* m_grid;

    Agent* m_agent;

    FontManager m_fontManager;

};

#endif // !GAME_HPP
