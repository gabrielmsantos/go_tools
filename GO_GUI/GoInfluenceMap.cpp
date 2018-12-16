//
//  GoInfluenceMap.cpp
//  GoInfluenceMap
//
//  Created by Giordano Bruno on 25/01/14.
//  Modified by Gabriel Santos on 18/12/18
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

    //m_go_database.InsertGamesFrom("/home/gabriel/Downloads/games/",true);
        //m_go_database.InsertGamesFrom("/home/gabriel/DBGames",true);
    if(using_database_advisor)
    {
        m_go_database.InsertGamesFrom("/home/gabriel/DBGames/FewGames",true);
        m_advisor_map = m_go_database.NextMoveAdvisor(m_gogame.GetGameTree().GetCurrentNode()->GetCompactBoard());
        std::cout <<m_go_database.GetNumberOfNodes() << " nodes inserted"<<std::endl;
    }

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

    delete m_goban_influence;
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
            m_gobanGUI.Draw(m_mainWindow, m_gogame.GetMainBoard(), *m_goban_statistics, m_gogame.GetActiveInfluenceModel(), m_advisor_map);
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

        m_gogame.PlayMove(l_mapPos.x,l_mapPos.y);

//        #####################  ATTENTION: This is the Automatic Player code, leave it inactive while the engine is not done

        if(using_automatic_player)
        {
            m_automatic_player->StartSearchFor(m_gogame.GetMainBoard().GetCompactBoard(), m_gogame.GetCurrentPlayer());
            std::vector<short> moves = m_automatic_player->GetBestMovesFound();
            if(moves.size() > 0 )
            {
                std::pair<unsigned short, unsigned short> pos = GoUtils::MoveToBoardPosition(moves[0]);
                m_gogame.PlayMove(pos.first,pos.second);
            }else
            {
                std::cout << "PASS -- " << std::endl;
            }
        }
//        ##########################  END Automatic PLayer ##############################################


//        std::vector<short>::iterator it;
//        std::vector<short> legal_moves = m_gogame.GenerateAllLegalMoves();

//        for(it = legal_moves.begin(); it != legal_moves.end(); ++it)
//        {
//            std::cout << *it << " "<<  std::flush;
//        }
//        std::cout << std::endl;

//        #####################  ATTENTION: This is the DataBase Advisor #####################
        //Get Advisors
        if(using_database_advisor)
            m_advisor_map = m_go_database.NextMoveAdvisor(m_gogame.GetGameTree().GetCurrentNode()->GetCompactBoard());

//        ##########################  END Database Advisor ##############################################
        //Put Engine here

    }else if (rEvent.type == sf::Event::MouseButtonReleased && rEvent.mouseButton.button == sf::Mouse::Right)
    {
        m_gogame.TakeBack();
//        m_advisor_map = m_go_database.NextMoveAdvisor(m_gogame.GetGameTree().GetCurrentNode()->GetCompactBoard());
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


boost::shared_ptr<GoReferee> GoInfluenceMap::m_go_referee_ptr = boost::shared_ptr<GoReferee>(new GoReferee());
GoGame GoInfluenceMap::m_gogame = GoGame(BOARD_SIZE, m_go_referee_ptr);
GobanGUI GoInfluenceMap::m_gobanGUI = GobanGUI(BOARD_SIZE);
I_GobanStatistics* GoInfluenceMap::m_goban_statistics = new BasicGobanStatistics();
GoInfluenceMap::GameState GoInfluenceMap::m_gameState = GoInfluenceMap::UNINITIALIZED;
sf::RenderWindow GoInfluenceMap::m_mainWindow;
std::vector<short> GoInfluenceMap::m_advisor_map;
GoDatabase GoInfluenceMap::m_go_database;
MctsPlayer* GoInfluenceMap::m_automatic_player = new MctsPlayer(BOARD_SIZE, m_go_referee_ptr);

bool GoInfluenceMap::using_database_advisor =false;
bool GoInfluenceMap::using_automatic_player =false;


