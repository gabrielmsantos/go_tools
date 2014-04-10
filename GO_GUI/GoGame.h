#ifndef GOGAME_H
#define GOGAME_H

#include "Goban.h"
#include "GoReferee.h"
#include "GoInfluenceStatistics/I_GobanStatistics.h"
#include "GoInfluenceModel/I_InfluenceModel.h"
#include "GoGameTree.h"
#include <boost/shared_ptr.hpp>
#include <unordered_set>

class I_GobanObserver;

class GoGame
{
public:

    explicit GoGame(int board_size);

    ~GoGame();

    //For handicap games
    void ForceMove(sf::Vector2i l_map_pos ,StoneState stone);

    /** This method plays a move and invert the current player*/
    void PlayMove(sf::Vector2i &l_mapPos);

    /** This method takes back one move and invert the current player*/
    void TakeBack(bool invert_player);

    Goban& GetMainBoard() ;

    void AddInfluenceAnalysis(I_InfluenceModel* influence_model);

    void RegisterObserver(I_GobanObserver* observer);

    I_InfluenceModel& GetActiveInfluenceModel();

private:

    GoReferee m_goReferee;
    Goban  m_influenceMap;
    //TODO DELETE THOSE GUYS
    std::unordered_set<I_InfluenceModel*> m_influences_analysis;
    StoneState m_currentPlayer;
    std::vector<Move> m_movesSequence;
    GoGameTree m_game_tree;
};

#endif // GOGAME_H
