#ifndef I_PLAYOUTPOLICY_H
#define I_PLAYOUTPOLICY_H

#include "SimpleGoGame.h"

/**
 * Interface for Play-Out Policy
*/
class I_PlayoutPolicy
{

public:

    explicit I_PlayoutPolicy(SimpleGoGame& go_game):m_go_game_ref(go_game)
    {
    }

    virtual short GeneratePlayOutMove() = 0;

    virtual ~I_PlayoutPolicy(){}
    //======================================================================================

protected:

    SimpleGoGame& m_go_game_ref;
};

#endif // I_PLAYOUTPOLICY_H
