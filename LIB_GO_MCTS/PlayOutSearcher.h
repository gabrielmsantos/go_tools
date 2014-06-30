#ifndef PLAYOUTSEARCHER_H
#define PLAYOUTSEARCHER_H

#include "SimpleGoGame.h"
#include "I_PlayOutPolicy.h"

class PlayOutSearcher
{
public:

    PlayOutSearcher(unsigned int board_size, boost::shared_ptr<GoReferee> l_referee);
    double PlayOutGameFrom(CompactBoard* cb, StoneState to_play);
    short GeneratePlayOutMove();

private:

SimpleGoGame m_simple_go_game;
I_PlayoutPolicy* m_playout_policy_ptr;

};

#endif // PLAYOUTSEARCHER_H
