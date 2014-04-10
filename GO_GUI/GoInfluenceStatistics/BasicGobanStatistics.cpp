#include "BasicGobanStatistics.h"

//================================================================================================================
BasicGobanStatistics::BasicGobanStatistics()
{
}

//================================================================================================================
double BasicGobanStatistics::GetInfluenceAmount(StoneState stone, const Goban& r_goban, I_InfluenceModel& r_influence)
{
    double total = 0;
    for(unsigned int i = 0; i< r_influence.GetDimension(); ++i)
        for(unsigned int j = 0; j< r_influence.GetDimension(); ++j)
    {
            if((stone > 0 ) && (r_influence.GetInfluence(i,j) > 0) && (r_goban.GetStone(i,j) != stone))
            {
                total+= r_influence.GetInfluence(i,j);
            }
            else if ((stone < 0 ) && (r_influence.GetInfluence(i,j) < 0)  && (r_goban.GetStone(i,j)!= stone))
            {
                total+= r_influence.GetInfluence(i,j);
            }
    }
    return total;
}

//================================================================================================================
int BasicGobanStatistics::GetTotalInfluencedCells(StoneState stone, const Goban& r_goban, I_InfluenceModel& r_influence)
{
    int total = 0;

    for(unsigned int i = 0; i< r_influence.GetDimension(); ++i)
        for(unsigned int j = 0; j< r_influence.GetDimension(); ++j)
    {
            if((stone > 0 ) && (r_influence.GetInfluence(i,j) > 0))
            {
                total++;
            }
            else if ((stone < 0 ) && (r_influence.GetInfluence(i,j) < 0))
            {
                total++;
            }
    }
    return total;
}

//================================================================================================================
int BasicGobanStatistics::GetEstimativeTerritory(StoneState stone,const Goban& r_goban, I_InfluenceModel& r_influence)
{
    int total = 0;

    for(unsigned int i = 0; i< r_influence.GetDimension(); ++i)
        for(unsigned int j = 0; j< r_influence.GetDimension(); ++j)
    {
        if((stone > 0 ) && ((r_influence.GetInfluence(i,j) >= 13) || ((r_goban.GetLayer(i,j)<=2)&&(r_influence.GetInfluence(i,j) >= 8))) && (r_goban.GetStone(i,j)!= stone))
        {
            total++;
        }
        else if ((stone < 0 ) && ((r_influence.GetInfluence(i,j) <= -13) || ((r_goban.GetLayer(i,j)<=2)&&(r_influence.GetInfluence(i,j) <= -8)))  && (r_goban.GetStone(i,j)!= stone))
        {
            total++;
        }
    }

    return total;
}
//================================================================================================================
