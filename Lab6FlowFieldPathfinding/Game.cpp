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

    constexpr int gridSize = ScreenSize / 60;

    m_grid = new Grid(m_fontManager, gridSize, gridSize, 60, {{5, 10}, {10, 5}});
    /*m_grid->calculateFlowField(sf::Vector2i(10, 10));*/

    m_agent = new Agent(*m_grid, m_grid->findNode({2, 2})->getPosition(), 50.f, 40.f);
}

/// <summary>
/// default destructor we did not dynamically allocate anything
/// so we don't need to free it, but method needs to be here
/// </summary>
Game::~Game()
{
    delete m_grid;
    delete m_agent;
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
        timeSinceLastUpdate += clock.restart();
        while (timeSinceLastUpdate > timePerFrame)
        {
            timeSinceLastUpdate -= timePerFrame;
            processEvents(); // at least 60 fps
            update(timePerFrame); //60 fps
        }
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
        if (sf::Event::MouseButtonPressed == newEvent.type)
        {
            processMouse(newEvent);
        }
    }
}


/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(sf::Event event)
{
    if (sf::Keyboard::Escape == event.key.code)
    {
        m_exitGame = true;
    }
}

void Game::processMouse(const sf::Event &event) const
{
    if (event.mouseButton.button == sf::Mouse::Left)
    {
        const sf::Vector2i mousePosition = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
        // Convert window pixel coordinates to the grid coordinates
        const sf::Vector2i mouseGridPosition = mousePosition / static_cast<int>(m_grid->getNodeSize());

        m_grid->calculateFlowField(mouseGridPosition);
    }
    // Place/remove walls
    if (event.mouseButton.button == sf::Mouse::Right)
    {
        const sf::Vector2i mousePosition = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
        // Convert window pixel coordinates to the grid coordinates
        const sf::Vector2i mouseGridPosition = mousePosition / static_cast<int>(m_grid->getNodeSize());

        m_grid->addObstacle(mouseGridPosition.x, mouseGridPosition.y);
        m_grid->calculateFlowField(sf::Vector2i(10, 10));
    }
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time deltaTime)
{
    m_agent->update(deltaTime);

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
    m_window.draw(*m_agent);

    m_window.display();
}

void Game::loadFonts()
{
    m_fontManager.load(Assets::Font::ArialBlack, "ASSETS/FONTS/ariblk.ttf");
}
