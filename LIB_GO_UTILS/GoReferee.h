#ifndef GOREFEREE_H
#define GOREFEREE_H

#include "Goban.h"

class GoReferee
{
public:
    GoReferee();

    bool IsLegal(StoneState stone, unsigned int map_x, unsigned int map_y,  Goban& r_goban, short ko_restriction);
    StoneState Opponent(StoneState stone) const;
};

#endif // GOREFEREE_H
