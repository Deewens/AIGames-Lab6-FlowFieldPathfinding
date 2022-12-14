#include "Game.hpp"

#include <iostream>

Game::Game() :
    m_window{sf::VideoMode{ScreenSize, ScreenSize, 32U}, "SFML Game"},
    m_exitGame{false} //when true game will exit
{
    loadFonts();

    constexpr int gridSize = 50;

    m_grid = new Grid(m_fontManager, gridSize, gridSize, ScreenSize / gridSize, {{5, 10}, {10, 5}});
    /*m_grid->calculateFlowField(sf::Vector2i(10, 10));*/

    m_agent = new Agent(*m_grid, m_grid->findNode({2, 2})->getPosition(), 60.f, 150.f);
}

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
/// and user
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
/// <param name="event">key press event</param>
void Game::processKeys(sf::Event event)
{
    // Toggle debug on/off
    if (sf::Keyboard::D == event.key.code)
    {
        m_grid->toggleDebugData();
    }

    if (sf::Keyboard::Escape == event.key.code)
    {
        m_exitGame = true;
    }
}

void Game::processMouse(const sf::Event& event) const
{
    // Left-click in a cell to set the coordinates of the goal
    if (event.mouseButton.button == sf::Mouse::Left)
    {
        const sf::Vector2i mousePosition = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
        // Convert window pixel coordinates to the grid coordinates
        const sf::Vector2i mouseGridPosition = mousePosition / static_cast<int>(m_grid->getNodeSize());

        m_grid->setGoalCoordinates(mouseGridPosition);
        m_grid->calculateFlowField();
        m_grid->setStartPosition(m_grid->convertWorldToGridCoordinates(m_agent->getPosition()));
    }

    // Middle-click in a cell to set the coordinates of the start cell
    if (event.mouseButton.button == sf::Mouse::Middle)
    {
        const sf::Vector2i mousePosition = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
        // Convert window pixel coordinates to the grid coordinates
        const sf::Vector2i mouseGridPosition = mousePosition / static_cast<int>(m_grid->getNodeSize());

        m_grid->calculateFlowField();

        m_grid->setStartPosition(mouseGridPosition);
        m_agent->setPosition(m_grid->findNode(mouseGridPosition)->getPosition());
    }

    // Place/remove walls
    if (event.mouseButton.button == sf::Mouse::Right)
    {
        const sf::Vector2i mousePosition = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
        // Convert window pixel coordinates to the grid coordinates
        const sf::Vector2i mouseGridPosition = mousePosition / static_cast<int>(m_grid->getNodeSize());

        auto obstacles = m_grid->getObstacles();
        const auto find = std::find_if(obstacles.begin(), obstacles.end(), [=](sf::Vector2i obstaclePos)
        {
            return obstaclePos.x == mouseGridPosition.x && obstaclePos.y == mouseGridPosition.y;
        });

        if (find == obstacles.end())
        {
            m_grid->addObstacle(mouseGridPosition.x, mouseGridPosition.y);
        }
        else
        {
            m_grid->removeObstacle(mouseGridPosition.x, mouseGridPosition.y);
        }

        m_grid->calculateFlowField();
        m_grid->calculatePathFromStart();
    }
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="deltaTime">time interval per frame</param>
void Game::update(const sf::Time deltaTime)
{
    // Ugly code that does collision check against window border (prevent some bugs where the Agent just go through the border of the window and never come back)
    if (m_agent->getPosition().x - m_agent->getRadius() - m_agent->getOutlineThickness() < 0 || m_agent->getPosition().x
        + m_agent->getRadius() + m_agent->getOutlineThickness() > ScreenSize || m_agent->getPosition().y - m_agent->
        getRadius() - m_agent->getOutlineThickness() < 0 || m_agent->getPosition().y + m_agent->getRadius() + m_agent->
        getOutlineThickness() > ScreenSize)
    {
        sf::Vector2f newAgentPosition = m_agent->getPosition();

        if (m_agent->getPosition().x - m_agent->getRadius() - m_agent->getOutlineThickness() < 0)
        {
            newAgentPosition.x = 0 + m_agent->getRadius() + m_agent->getOutlineThickness();
        }
        else if (m_agent->getPosition().x + m_agent->getRadius() + m_agent->getOutlineThickness() > ScreenSize)
        {
            newAgentPosition.x = ScreenSize - m_agent->getRadius() - m_agent->getOutlineThickness();
        }

        if (m_agent->getPosition().y - m_agent->getRadius() - m_agent->getOutlineThickness() < 0)
        {
            newAgentPosition.y = 0 + m_agent->getRadius() + m_agent->getOutlineThickness();
        }
        else if (m_agent->getPosition().y + m_agent->getRadius() + m_agent->getOutlineThickness() > ScreenSize)
        {
            newAgentPosition.y = ScreenSize - m_agent->getRadius() - m_agent->getOutlineThickness();
        }

        m_agent->setPosition(newAgentPosition);
    }

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
