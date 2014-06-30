//
//  GoInfluenceMap.h
//  GoInfluenceMap
//
//  Created by Giordano Bruno on 25/01/14.
//
//

#ifndef __GoInfluenceMap__GoInfluenceMap__
#define __GoInfluenceMap__GoInfluenceMap__

#include <SFML/Graphics.hpp>
#include "GoGUI/GobanGUI.h"
#include "GoGame.h"
#include "GoDatabase.h"
#include "MctsPlayer.h"


class GoInfluenceMap {
public:
    static void Start();

private:
    enum GameState { UNINITIALIZED, PLAYING, EXITING, MINIMIZED };

    static bool IsExiting();
    static void GameLoop();

    static void InputEventListener(const sf::Event& rEvent);
    static void MapClick(const sf::Event& rEvent);
    static void CloseGame(const sf::Event& rEvent);

    //State of the game
    static GameState            m_gameState;
    //Window where the game will be rendered
    static sf::RenderWindow     m_mainWindow;

    //Current player Statistics
    static I_GobanStatistics* m_goban_statistics;

    static GobanGUI m_gobanGUI;
    static GoDatabase m_go_database;
    static boost::shared_ptr<GoReferee> m_go_referee_ptr;
    static GoGame m_gogame;
    static std::vector<short> m_advisor_map;
    static MctsPlayer* m_automatic_player;
};

#endif /* defined(__GoInfluenceMap__GoInfluenceMap__) */
