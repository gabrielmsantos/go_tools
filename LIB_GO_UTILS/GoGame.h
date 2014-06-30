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


    explicit GoGame(int board_size, boost::shared_ptr<GoReferee> l_referee);

    ~GoGame();

    bool PlayMove(int i, int j ,StoneState stone);

    //TODO: Remove dependencies of SF
    //For handicap games
    bool PlayMove(sf::Vector2i &l_map_pos ,StoneState stone);

    /** This method plays a move and invert the current player*/
    bool PlayMove(unsigned int l_mapPos_x, unsigned int l_mapPos_y);

    /** This method plays a move and invert the current player*/
    bool PlayMove(short move);

    /** This method takes back one move and invert the current player*/
    void TakeBack();

    /** This method takes back one move and invert the current player*/
    void TakeBack(unsigned short number);

    Goban& GetMainBoard();

    //TODO: REMOVE ANALYSIS FROM HERE
    void AddInfluenceAnalysis(I_InfluenceModel* influence_model);

    void RegisterObserver(I_GobanObserver* observer);

    I_InfluenceModel& GetActiveInfluenceModel();

    GoGameTree& GetGameTree();

    GoGameInfo& GetGameInfo();

    std::vector<short> GenerateAllLegalMoves();

    /**
    * It is important to notice that the root of the Game tree will be
    * the cb state and the game info will be reseted.
    */
    void SetFromCompactBoard(CompactBoard* cb);

    void SetCurrentPlayer(StoneState to_play);

    StoneState GetCurrentPlayer() const;

    unsigned short GetTotalStones() const;

private:

    Goban  m_goban;

    //It is not owned by GoGame
    boost::shared_ptr<GoReferee> m_goReferee_sptr;

    //TODO DELETE THOSE GUYS
    std::unordered_set<I_InfluenceModel*> m_influences_analysis;

    StoneState m_currentPlayer;
    GoGameTree m_game_tree;
    GoGameInfo m_game_info;

    short m_simple_ko_restriction;
};

#endif // GOGAME_H
