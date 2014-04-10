#ifndef CROSSINFLUENCE_H
#define CROSSINFLUENCE_H

#define INFLUENCE 10
#define DIRECT .8f
#define DIAGONAL .7f
#define OSJ .6f
#define KNIGHT .5f
#define TSJ .4f
#define LKNIGHT .3f

#include "I_InfluenceModel.h"

class CrossInfluence: public I_InfluenceModel, public I_GobanObserver
{
public:
    CrossInfluence(unsigned int board_size);
    virtual void PutInfluence(const sf::Vector2i& pos, int infl_value);
    virtual void Notify(const sf::Vector2i& pos, int infl_value);
};

#endif // CROSSINFLUENCE_H
