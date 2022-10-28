#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>

#include "Grid.hpp"

#include "ResourceManager/ResourceManager.hpp"
#include "ResourceManager/ResourceIdentifiers.hpp"

class Game // NOLINT(cppcoreguidelines-special-member-functions)
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
    void processKeys(sf::Event t_event);
    void processMouse(const sf::Event& t_event) const;
    void update(sf::Time t_deltaTime);
    void render();

    void loadFonts();

    unsigned int static constexpr ScreenSize = 1200U;

    sf::RenderWindow m_window; // main SFML window

    FontManager m_fontManager;

    Grid* m_grid;

    bool m_exitGame; // control exiting game
};

#endif // !GAME_HPP
