#include "GoReferee.h"

//================================================================================
GoReferee::GoReferee()
{
}

//================================================================================
bool GoReferee::IsLegal(StoneState stone, const sf::Vector2i& r_mapPosition,  Goban& r_goban)
{
    int x;
    int y;
    StoneState adj;
    int l_count;

    for(unsigned int i=0; i<4; ++i)
    {
        if(i%2 == 0)
        {
            x = r_mapPosition.x-1+i;
            y = r_mapPosition.y;

            if((x < 0)||(x>=r_goban.Dimension()))
                continue;
        }else
        {
            x = r_mapPosition.x;
            y = r_mapPosition.y-2+i;

            if((y < 0)||(y>=r_goban.Dimension()))
                continue;
        }

        //I have divided in 3 segments to be better to read and understand.
        /** Getting the Dragon*/
        GobanIntersection* l_dragon = r_goban.GetDragon(x,y);
        l_count = l_dragon->DragonLibertiesCount();
        //delete l_dragon;

        /** Getting the adjacent stone*/
        adj = r_goban.GetStone(x,y);

        //If there is any liberty it is legal
        if(adj == EMPTY)
        {
            return true;
        }//Else if the neighbours are not Empty but im gonna connect to another dragon (with liberties) then it is legal
        else if( (adj == stone) && ( l_count > 1))
        {

            return true;
        }
        //Else if im gonna capture some stones (snapback for example) it is also legal
        else if( (adj == Opponent(stone)) && ( l_count == 1 ) )
        {
            return true;
        }
    }
    return false;
}

//================================================================================
StoneState GoReferee::Opponent(StoneState stone) const
{
    return (StoneState)((int)stone * (-1));
}
////================================================================================
