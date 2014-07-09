#ifndef RANDOMPOLICY_H
#define RANDOMPOLICY_H

#include "I_PlayOutPolicy.h"

class RandomPolicy: public I_PlayoutPolicy
{
public:
    explicit RandomPolicy();

//    virtual short GeneratePlayOutMove();
    virtual short GeneratePlayOutMove(SimpleGoGame& go_game_ref);
};

#endif // RANDOMPOLICY_H
