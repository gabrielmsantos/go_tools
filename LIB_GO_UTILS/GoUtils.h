#ifndef GOUTILS_H
#define GOUTILS_H

#include"Goban.h"

#define MAX_BOARD 19
#define MIN_BOARD 4

class GoUtils
{
public:
//====================================================================================
    static short BoardPositionToMove(unsigned int x, unsigned int y, int currentPlayer)
    {
        return (short)((((x+1)* MAX_BOARD) + (y+1)) * currentPlayer);
    }
//====================================================================================
    static std::pair<unsigned short,unsigned short> MoveToBoardPosition(short move)
    {
        unsigned short row = (abs(move)/MAX_BOARD) -1;
        unsigned short column = (abs(move)%MAX_BOARD) -1;

        return std::pair<unsigned short,unsigned short>(row, column);
    }
//====================================================================================

    static short GetClockwiseRotationForMove(short move)
    {

        //short row = abs(move)/MAX_BOARD;
        //short column = abs(move)%MAX_BOARD;

        std::pair<short, short> l_pair = GoUtils::MoveToBoardPosition(move);
        short row = l_pair.first;
        short column = l_pair.second;

        short new_column = row;
        short new_row = MAX_BOARD - 1 - column;

        //return   (new_row* MAX_BOARD) + new_column ) * move/abs(move);
        return BoardPositionToMove(new_row , new_column,move/abs(move));
    }

    //=============================================================================================
    static short GetMirrorForMove(short move)
    {
//        short row = abs(move)/MAX_BOARD;
//        short column = abs(move)%MAX_BOARD;

        std::pair<short, short> l_pair = GoUtils::MoveToBoardPosition(move);
        short row = l_pair.first;
        short column = l_pair.second;

        short new_column = column;
        short new_row = (MAX_BOARD - 1) - row;

        //return ((new_row* MAX_BOARD) + new_column ) * move/abs(move);
        return BoardPositionToMove(new_row , new_column,move/abs(move));
    }

};
#endif // GOUTILS_H
