#include "Game.hpp"

#include <iostream>

/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup the image
/// </summary>
Game::Game() :
    m_window{sf::VideoMode{ScreenSize, ScreenSize, 32U}, "SFML Game"},
    m_exitGame{false} //when true game will exit
{
    loadFonts();
    m_grid = new Grid(m_fontManager, ScreenSize, ScreenSize, 100);
}

/// <summary>
/// default destructor we did not dynamically allocate anything
/// so we don't need to free it, but method needs to be here
/// </summary>
Game::~Game()
{
    delete m_grid;
};


/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Game::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    constexpr float fps{60.0f};
    const sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
    while (m_window.isOpen())
    {
        processEvents(); // as many as possible
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;
        while (timeSinceLastUpdate > timePerFrame)
        {
            timeSinceLastUpdate -= timePerFrame;
            processEvents(); // at least 60 fps
            update(timePerFrame); //60 fps
        }
        
        updateFPSCounter(elapsedTime);
        
        render(); // as many as possible
    }
}

/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
    sf::Event newEvent{};
    while (m_window.pollEvent(newEvent))
    {
        if (sf::Event::Closed == newEvent.type) // window message
        {
            m_exitGame = true;
        }
        if (sf::Event::KeyPressed == newEvent.type) //user pressed a key
        {
            processKeys(newEvent);
        }
    }
}


/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(sf::Event t_event)
{
    if (sf::Keyboard::Escape == t_event.key.code)
    {
        m_exitGame = true;
    }
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
    m_grid->update(t_deltaTime);

    if (m_exitGame)
    {
        m_window.close();
    }
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
    m_window.clear(sf::Color::Black);

    m_window.draw(*m_grid);

    m_window.display();
}

void Game::loadFonts()
{
    m_fontManager.load(Assets::Font::ArialBlack, "ASSETS/FONTS/ariblk.ttf");
}

void Game::updateFPSCounter(sf::Time elapsedTime)
{
    m_updateTime += elapsedTime;
    m_framesPerSecond += 1;
    
    std::cout << "FPS: " + std::to_string(m_framesPerSecond) << std::endl;

    m_updateTime -= sf::seconds(1.0f);
    m_framesPerSecond = 0;
}
