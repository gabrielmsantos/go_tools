#ifndef BASICGOBANSTATISTICS_H
#define BASICGOBANSTATISTICS_H

#include "I_GobanStatistics.h"

class BasicGobanStatistics: public I_GobanStatistics
{

public:
    BasicGobanStatistics();
    double GetInfluenceAmount(StoneState stone,const Goban& r_goban, I_InfluenceModel& r_influence);
    int GetTotalInfluencedCells(StoneState stone,const Goban& r_goban, I_InfluenceModel& r_influence);
    int GetEstimativeTerritory(StoneState stone,const Goban& r_goban, I_InfluenceModel& r_influence);
};

#endif // BASICGOBANSTATISTICS_H
