#ifndef IGOBANSTATISTICS_H
#define IGOBANSTATISTICS_H

#include "../Goban.h"

class I_InfluenceModel;

class I_GobanStatistics{
public:
    virtual ~I_GobanStatistics(){}
    virtual double GetInfluenceAmount(StoneState stone,const Goban& r_goban, I_InfluenceModel& r_influence)   = 0;
    virtual int GetTotalInfluencedCells(StoneState stone,const Goban& r_goban, I_InfluenceModel& r_influence) = 0;
    virtual int GetEstimativeTerritory(StoneState stone,const Goban& r_goban, I_InfluenceModel& r_influence)  = 0;
};
#endif // IGOBANSTATISTICS_H
