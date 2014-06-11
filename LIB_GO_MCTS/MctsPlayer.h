#ifndef MCTSPLAYER_H
#define MCTSPLAYER_H

#include "GoGame.h"
#include "MCTree.h"

class MctsPlayer
{
public:
    MctsPlayer(int board_size, boost::shared_ptr<GoReferee> l_referee);

    /** All the search process starts here
     *Must return a list of moves, ordered from the best one.*/
    void StartSearchFor(CompactBoard* cb);



private:

GoGame m_go_game;
MCTree m_tree;

};

#endif // MCTSPLAYER_H
