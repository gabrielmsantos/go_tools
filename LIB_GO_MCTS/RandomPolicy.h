#ifndef RANDOMPOLICY_H
#define RANDOMPOLICY_H

#include "I_PlayOutPolicy.h"

class RandomPolicy: public I_PlayoutPolicy
{
public:
    explicit RandomPolicy(SimpleGoGame& go_game);

    virtual short GeneratePlayOutMove();
};

#endif // RANDOMPOLICY_H
