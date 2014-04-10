#ifndef GOREFEREE_H
#define GOREFEREE_H

#include "Goban.h"

class GoReferee
{
public:
    GoReferee();

    bool IsLegal(StoneState stone, const sf::Vector2i& r_mapPosition, Goban &r_goban);
    StoneState Opponent(StoneState stone) const;
};

#endif // GOREFEREE_H
