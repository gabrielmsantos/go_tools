#ifndef SIMPLEGOGAME_H
#define SIMPLEGOGAME_H

#include "Goban.h"
#include "GoReferee.h"
#include <boost/shared_ptr.hpp>

/**
  This class was mainly created to be used on play-out simulations.
  There is no GameTree
  There is no GameInfo
  There is no takeback
  No complex ko checking just simple ko checking.
*/

class SimpleGoGame
{
public:        

    explicit SimpleGoGame(int board_size, boost::shared_ptr<GoReferee> l_referee);

    /** Execute a move in the m_goban according to the "stone" player*/
    void PlayMove(int l_mapPos_x, int l_mapPos_y, StoneState stone = EMPTY);

    std::vector<short> GenerateAllLegalMoves();

    void SetFromCompactBoard(CompactBoard* cb);

private:

    Goban  m_goban;

    //It is not owned by GoGame
    boost::shared_ptr<GoReferee> m_goReferee_sptr;

    StoneState m_currentPlayer;

    /*variable to early check simple ko*/
    short m_simple_ko_restriction;

};

#endif // SIMPLEGOGAME_H
