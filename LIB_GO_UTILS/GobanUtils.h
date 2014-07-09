#ifndef GOBANUTILS_H
#define GOBANUTILS_H

#include "Goban.h"

namespace GobanUtils
{
//==========================================================================================
inline bool IsPotentialOnePointEye(unsigned short move_x, unsigned short move_y, const Goban& goban_ref, StoneState stone)
{
    std::vector<std::pair<unsigned short, unsigned short> > adjacencies = goban_ref.GetAdjacencies(move_x, move_y);
    std::vector<std::pair<unsigned short, unsigned short> >::iterator it;

    for(it = adjacencies.begin(); it != adjacencies.end(); ++it)
    {
        if( goban_ref.GetStone(it->first, it->second) != stone )
            return false;
    }

    return true;
}

//==========================================================================================
inline bool IsOnePointEye(unsigned short x, unsigned short y, const Goban& goban_ref, StoneState stone)
{
    //Verifies adjacencies
    if( !IsPotentialOnePointEye(x,y,goban_ref,stone) )
    {
        return false;
    }

    //Verifies Diagonals
    std::vector<std::pair<unsigned short, unsigned short> > diagonals = goban_ref.GetDiagonals(x, y);
    std::vector<std::pair<unsigned short, unsigned short> >::iterator it;
    unsigned short limit = 0;

    for(it = diagonals.begin(); it != diagonals.end(); ++it)
    {
        if( goban_ref.GetStone(it->first,it->second) != stone )
            limit++;

        if(limit > 1)
            return false;
    }

    return true;
}

//======================================================================================
inline std::pair<double, double> ScoreSimpleEndPosition(const Goban& l_goban)
{
    double white_score = 0;
    double black_score = 0;

    const std::vector<std::vector<GobanIntersection> >& intersections = l_goban.GetGoban();

    std::vector<std::vector<GobanIntersection> >::const_iterator it;
    std::vector<GobanIntersection>::const_iterator second_it;

    StoneState stone;

    for(it = intersections.begin(); it != intersections.end(); ++it)
    {
        for(second_it = (*it).begin(); second_it!= (*it).end(); ++second_it)
        {
            stone = (*second_it).GetStone();

            if(stone == BLACK)
            {
                black_score++;

            }else if(stone == WHITE)
            {
                white_score++;

            }else
            {
                //One point eye black
                if( GobanUtils::IsOnePointEye((*second_it).GetX(), (*second_it).GetY(), l_goban, BLACK ))
                {
                    black_score++;
                }else if( GobanUtils::IsOnePointEye((*second_it).GetX(), (*second_it).GetY(), l_goban,  WHITE )) //One point eye white
                {
                    white_score++;
                }

            }
        }
    }
    return std::pair<double, double>(black_score, white_score);
}
//======================================================================================
inline StoneState GetStoneState(short move)
{
    if(move < 0)
        return WHITE;
    else if (move > 0)
        return BLACK;
    else
        return EMPTY;
}
//======================================================================================
}
#endif // GOBANUTILS_H
