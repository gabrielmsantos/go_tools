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

typedef struct _go_game_info
{
    unsigned short m_komi10; //Komi should be divide by 10 to get gameKomi
    std::string m_black_player;
    std::string m_white_player;
    std::string m_black_rating;
    std::string m_white_rating;
    std::string m_game_result;
    std::string m_date;

}GoGameInfo;

class GoGame
{
public:

    static CompactBoard* cb_static;

    explicit GoGame(int board_size, boost::shared_ptr<GoReferee> l_referee);

    ~GoGame();

    void PlayMove(int i, int j ,StoneState stone);

    //For handicap games
    void PlayMove(sf::Vector2i &l_map_pos ,StoneState stone);

    /** This method plays a move and invert the current player*/
    void PlayMove(sf::Vector2i &l_mapPos);

    /** This method takes back one move and invert the current player*/
    void TakeBack();

    Goban& GetMainBoard() ;

    void AddInfluenceAnalysis(I_InfluenceModel* influence_model);

    void RegisterObserver(I_GobanObserver* observer);

    I_InfluenceModel& GetActiveInfluenceModel();

    GoGameTree& GetGameTree();

    GoGameInfo& GetGameInfo();

private:

    Goban  m_goban;
    //It is not owned by GoGame
    boost::shared_ptr<GoReferee> m_goReferee_sptr;

    //TODO DELETE THOSE GUYS
    std::unordered_set<I_InfluenceModel*> m_influences_analysis;

    StoneState m_currentPlayer;
    GoGameTree m_game_tree;
    GoGameInfo m_game_info;
};

#endif // GOGAME_H
