#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>

#include "Grid.hpp"

class Game  // NOLINT(cppcoreguidelines-special-member-functions)
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
    void update(sf::Time t_deltaTime);
    void render();

    sf::RenderWindow m_window; // main SFML window
    sf::Font m_arialBlackFont; // font used by message

    Grid m_grid;
    
    bool m_exitGame; // control exiting game

};

#endif // !GAME_HPP

