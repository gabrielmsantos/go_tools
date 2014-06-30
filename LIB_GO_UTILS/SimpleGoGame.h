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
    bool PlayMove(int l_mapPos_x, int l_mapPos_y, StoneState stone = EMPTY);

    /** This method plays a move and invert the current player*/
    bool PlayMove(short move);

    void GenerateAllLegalMoves(std::vector<short>& result);

    void SetFromCompactBoard(CompactBoard* cb);

    inline StoneState GetCurrentPlayer()
    {
        return m_currentPlayer;
    }

    unsigned short Dimension();

    void PlayPassMove();

    const Goban& GetMainBoard();

    void SetCurrentPlayer(StoneState to_play);

private:

    Goban  m_goban;

    //It is not owned by GoGame
    boost::shared_ptr<GoReferee> m_goReferee_sptr;

    //Who is the turn to move
    StoneState m_currentPlayer;

    /*variable to early check simple ko*/
    short m_simple_ko_restriction;

};

#endif // SIMPLEGOGAME_H
