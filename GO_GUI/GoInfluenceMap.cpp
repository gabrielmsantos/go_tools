//
//  GoInfluenceMap.cpp
//  GoInfluenceMap
//
//  Created by Giordano Bruno on 25/01/14.
//

#include "GoInfluenceMap.h"
#include "GoGUI/HSL.hpp"
#include "Config.h"
#include "GoInfluenceStatistics/BasicGobanStatistics.h"
#include "GoInfluenceModel/CrossInfluence.h"

#include <iostream>

void GoInfluenceMap::Start()
{

    CrossInfluence* m_goban_influence = new CrossInfluence(m_gogame.GetMainBoard().Dimension());
    m_gogame.AddInfluenceAnalysis(m_goban_influence);
    m_gogame.RegisterObserver(m_goban_influence);
    //Game was initialized before
    if(m_gameState != GoInfluenceMap::UNINITIALIZED)
        return;

    // Create the main window
    m_mainWindow.create(sf::VideoMode(700, 480), "Influence Goban", sf::Style::Titlebar | sf::Style::Close);
    m_mainWindow.setVerticalSyncEnabled(true);

    m_gameState = GoInfluenceMap::PLAYING;

    //Main loop of the game
    while(!IsExiting())
    {
        GameLoop();
    }

    //Close the window
    m_mainWindow.close();
}

bool GoInfluenceMap::IsExiting()
{
    return m_gameState == GoInfluenceMap::EXITING;
}

void GoInfluenceMap::GameLoop()
{
        sf::Event event;
    switch (m_gameState) {
        case GoInfluenceMap::PLAYING:
        {
            //Clear/Draw/Display
            m_mainWindow.clear(sf::Color(0,0,0));
            m_gobanGUI.Draw(m_mainWindow, m_gogame.GetMainBoard(), *m_goban_statistics, m_gogame.GetActiveInfluenceModel());
            m_mainWindow.display();

            break;
        }
        case GoInfluenceMap::MINIMIZED:
        {
            m_mainWindow.waitEvent(event);
            InputEventListener(event);
            break;
        }
        default:
            break;
    }

    // Process input events
    while (m_mainWindow.pollEvent(event))
    {
       InputEventListener(event);
    }
}

void GoInfluenceMap::InputEventListener(const sf::Event& rEvent)
{
    //Verify if the mouse was clicked
    MapClick(rEvent);

    //Verify if the game was closed
    CloseGame(rEvent);
}

void GoInfluenceMap::MapClick(const sf::Event& rEvent)
{
    //:TODO: VERIFY BOUNDARIES
    if (rEvent.type == sf::Event::MouseButtonReleased && rEvent.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2i l_mousePos(rEvent.mouseButton.x, rEvent.mouseButton.y);
        sf::Vector2i l_mapPos;
        m_gobanGUI.MapClick(l_mousePos, l_mapPos);

        m_gogame.PlayMove(l_mapPos);
    }else if (rEvent.type == sf::Event::MouseButtonReleased && rEvent.mouseButton.button == sf::Mouse::Right)
    {
        m_gogame.TakeBack(true);
    }
}

void GoInfluenceMap::CloseGame(const sf::Event& rEvent)
{
    if(rEvent.type == sf::Event::Closed)
    {
        m_gameState = GoInfluenceMap::EXITING;
    }
    if(rEvent.type == sf::Event::KeyPressed && rEvent.key.code == sf::Keyboard::Escape)
    {
        m_gameState = GoInfluenceMap::EXITING;
    }
    if(rEvent.type == sf::Event::LostFocus)
    {
        m_gameState = GoInfluenceMap::MINIMIZED;
    }
    if(rEvent.type == sf::Event::GainedFocus)
    {
        m_gameState = GoInfluenceMap::PLAYING;
    }

}

GoGame GoInfluenceMap::m_gogame = GoGame(BOARD_SIZE);
I_GobanStatistics* GoInfluenceMap::m_goban_statistics = new BasicGobanStatistics();
GobanGUI GoInfluenceMap::m_gobanGUI = GobanGUI(BOARD_SIZE);
GoInfluenceMap::GameState GoInfluenceMap::m_gameState = GoInfluenceMap::UNINITIALIZED;
sf::RenderWindow GoInfluenceMap::m_mainWindow;
