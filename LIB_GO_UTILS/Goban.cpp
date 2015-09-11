#include "Goban.h"
#include <iostream>
#include <math.h>
#include "GoInfluenceModel/CrossInfluence.h"

//================================================================================
Goban::Goban(int board_size):
    m_intersections(board_size),
    m_black_prisoners(0),
    m_white_prisoners(0)
{
    assert ( ( board_size >= MIN_BOARD) && (board_size <= MAX_BOARD) );
    for(unsigned int i = 0; i < m_intersections.size(); ++i)
    {
        m_intersections[i] = std::vector<GobanIntersection>( m_intersections.size());
        for(unsigned int j =0; j < m_intersections[i].size(); ++j)
        {
            (m_intersections[i][j]).SetCoord(sf::Vector2i(i,j));
        }
    }
}
//================================================================================
Goban::~Goban()
{
    //std::cout <<"Destructing Goban" << std::endl;
}
//================================================================================
std::pair<bool,short> Goban::PutStone(unsigned int r_mapPosition_x, unsigned int r_mapPosition_y , StoneState stone)
{
    short move_ko_restriction = 0;
    if ( ( GetStone(r_mapPosition_x, r_mapPosition_y) == EMPTY )  && ( stone != EMPTY ) )
    {
        m_intersections[r_mapPosition_x][r_mapPosition_y].PutStone(stone);

        //TODO THINK ABOUT THIS
        //#### This method notify all observers of the game INFLUENCE & STATISTICS!!!
        NotifyObservers(sf::Vector2i(r_mapPosition_x,r_mapPosition_y), stone);

        //Settle the Dragons (Groups of Stones) {Remove Stones, Join Dragons, etc..} Go Rules Here
        move_ko_restriction = CheckDragons(&(m_intersections[r_mapPosition_x][r_mapPosition_y]));

        return std::pair<bool, short>(true, move_ko_restriction);
    }
    return std::pair<bool, short>(false, move_ko_restriction);
}
//================================================================================
short Goban::CheckDragons(GobanIntersection* intersection)
{
    int x;
    int y;
    int total_prisoners=0;
    short move=0;

    intersection->m_dragon_size+=1;

    for(int i=0; i<4; ++i)
    {
        if(i%2 == 0)
        {
            x = intersection->GetX()-1+i;
            y = intersection->GetY();

            if((x < 0)||(x>=Dimension()))
                continue;
        }else
        {
            x = intersection->GetX();
            y = intersection->GetY()-2+i;

            if((y < 0)||(y>=Dimension()))
                continue;
        }

        if(m_intersections[x][y].GetStone() == EMPTY)
        {
            intersection->DragonAddLiberty(x,y);

        }else if(m_intersections[x][y].GetStone() == intersection->GetStone())//Friend Stone
        {
            //if it is not in the same group already
            if(  m_intersections[x][y].GetSize() < intersection->GetSize())
            {
                intersection->DragonAddIntersection(&(m_intersections[x][y]));
            }else
            {
                m_intersections[x][y].DragonAddIntersection(intersection);
            }
        }
        else//Oponent
        {
            GobanIntersection* l_opponent_dragon = m_intersections[x][y].GetRoot();
            l_opponent_dragon->DragonRemoveLiberty(intersection->GetX(), intersection->GetY());

            if(l_opponent_dragon->DragonLibertiesCount() == 0)
            {
                StoneState stone = m_intersections[x][y].GetStone();

                //It deletes the dragon inside and count prisoners
                if(l_opponent_dragon->m_dragon_size==1)
                {
                    move =  GoUtils::BoardPositionToMove(l_opponent_dragon->GetX(), l_opponent_dragon->GetY(), l_opponent_dragon->GetStone() );
                }

                unsigned short prisoners = RemoveDragon(l_opponent_dragon);

                total_prisoners+=prisoners;

                if(stone  == WHITE)
                {
                    m_black_prisoners += prisoners;
                }else
                {
                    m_white_prisoners += prisoners;
                }
            }
        }
    }

    //Check Here for Simple KO Just Used For Play-out Simulations
    //If total Prisoners == 1 and intersection.dragon_size ==1
    if( (total_prisoners == 1) && (intersection->m_dragon_size==1) )
    {
        //std::cout << "simple Ko: "<< move << std::endl;
        return move;
    }
    else
    {
        return 0;
    }


    //std::cout << intersection->GetX() << " XX " << intersection->GetY() << std::endl;
    //intersection->GetRoot()->PrintLiberties();
    //std::cout << intersection->GetRoot()->m_dragon_liberties.size() << std::endl;

}

//================================================================================
StoneState Goban::Opponent(StoneState stone) const
{
    return (StoneState)((int)stone * (-1));
}
//================================================================================
unsigned short Goban::RemoveDragon(GobanIntersection* l_dragon)
{
    unsigned short dragon_size = 0;

    int x;
    int y;
    std::unordered_set<GobanIntersection*>::iterator it;

    for(it = l_dragon->m_dragon_children.begin(); it != l_dragon->m_dragon_children.end(); ++it)
    {
        dragon_size+= RemoveDragon( (*it) );
    }

    //Add Liberties to Adjacency

    for(int i=0; i<4; ++i)
    {
        if(i%2 == 0)
        {
            x = l_dragon->GetX()-1+i;
            y = l_dragon->GetY();

            if((x < 0)||(x>=Dimension()))
                continue;
        }else
        {
            x = l_dragon->GetX();
            y = l_dragon->GetY()-2+i;

            if((y < 0)||(y>=Dimension()))
                continue;
        }

        if(m_intersections[x][y].GetStone() == Opponent(l_dragon->GetStone()))
        {
            m_intersections[x][y].DragonAddLiberty(l_dragon->GetX(), l_dragon->GetY());
        }
    }

    RemoveStone(l_dragon->GetBoardPosition());

    dragon_size++;

    return dragon_size;
}
//================================================================================
bool Goban::RemoveStone(const sf::Vector2i &r_mapPosition)
{
    if (GetStone(r_mapPosition.x, r_mapPosition.y)!=EMPTY) {

        NotifyObservers(r_mapPosition,  GetStone(r_mapPosition.x, r_mapPosition.y) * (-1));

        m_intersections[r_mapPosition.x][r_mapPosition.y].Clear();

        return true;
    }

    return false;
}
//================================================================================
Goban& Goban::operator=(const Goban& influence_goban_copy)
{
    m_black_prisoners = influence_goban_copy.m_black_prisoners;
    m_white_prisoners = influence_goban_copy.m_white_prisoners;

    //For each intersection update the Dragon
    for(unsigned int i = 0; i< m_intersections.size(); ++i)
    {
        for(unsigned int j = 0; j < m_intersections[i].size(); ++j)
        {
            m_intersections[i][j] = influence_goban_copy.m_intersections[i][j];
        }
    }

    return *this;
}
//================================================================================
GobanIntersection *Goban::GetDragon(int x, int y)
{
    return m_intersections[x][y].GetRoot();
}
//================================================================================
CompactBoard* Goban::GetCompactBoard() const
{
    unsigned int pos;       // position of the bit
    unsigned int byte_pos;  // position that is gonna be modified in the array
    unsigned int offset;    // offset regarding to the bit to be modified
    unsigned int start;     // indicates if it has to be inserted in white or black space
    int stone;              // the integer representation of the stone.

    CompactBoard* l_compact_board = new CompactBoard(GetBoardSize());

    l_compact_board->m_black_prisoners = m_black_prisoners;
    l_compact_board->m_white_prisoners = m_white_prisoners;
    l_compact_board->m_info = 0;

    //Compacting the board representation
    for(unsigned int i = 0; i< m_intersections.size(); ++i)
    {
        for(unsigned int j = 0; j < m_intersections[i].size(); ++j)
        {
            stone = (int) m_intersections[i][j].GetStone();

            if(stone == 0)
                continue;

            start = (((stone+1)/2)*l_compact_board->HalfSize()) ;

            pos = i* Dimension()+ j;

            byte_pos = start + ( pos/(sizeof(unsigned int) * SIZE_BYTE) );

            offset = pos % (sizeof(unsigned int) * SIZE_BYTE);

            l_compact_board->m_compact_board[byte_pos] |= ( (unsigned int) 1 << offset );
        }
    }

    return l_compact_board;
}
//================================================================================
void Goban::SetFromCompactBoard(CompactBoard* p_compact_board)
{
    assert(p_compact_board);
    ClearBoard();

    unsigned int offset;    //Represents the bit to be checked.
    unsigned int turned_on; //Check if a specified bit is turned on.

    unsigned int x; //map coordinate
    unsigned int y; //map coordinate
    unsigned int i;
    unsigned int j;
    unsigned int p;

    unsigned int number_of_intersections = (m_intersections.size()*m_intersections.size());

    //Setting up prisoners
    m_black_prisoners = p_compact_board->m_black_prisoners;
    m_white_prisoners = p_compact_board->m_white_prisoners;

    //Setting up stones
    for(i = 0, j = 0, p = 0; j < 2 ; ++p,  i = ( p % number_of_intersections) , j = ( p/number_of_intersections) )
    {
        offset = i % (sizeof(unsigned int) * SIZE_BYTE);
        turned_on = p_compact_board->m_compact_board[ (j*p_compact_board->HalfSize()) + (i/(sizeof(unsigned int) * SIZE_BYTE)) ] & ( (unsigned int) 1 << offset );
        if(turned_on)
        {
            x = i/Dimension();
            y = i % Dimension();
            PutStone(x,y, (StoneState) ((2*j) -1) );
        }
    }

}
//================================================================================
void Goban::ClearBoard()
{
    m_black_prisoners =0;
    m_white_prisoners =0;

    for(unsigned int i = 0; i < m_intersections.size(); ++i)
        for(unsigned int j = 0; j < m_intersections[i].size(); ++j)
        {
            RemoveStone(sf::Vector2i(i,j));
        }

}

//================================================================================
std::vector<std::pair<unsigned short, unsigned short> > Goban::GetAdjacencies(unsigned short move_x, unsigned short move_y) const
{
    unsigned short x;
    unsigned short y;

    std::vector<std::pair<unsigned short, unsigned short> > adjacencies;

    for(unsigned int i=0; i<4; ++i)
    {
        if(i%2 == 0)
        {
            x = move_x -1+i;
            y = move_y;

            if((x < 0)||(x>=Dimension()))
                continue;
        }else
        {
            x = move_x;
            y = move_y-2+i;

            if((y < 0)||(y >= Dimension()))
                continue;
        }

        adjacencies.push_back(std::pair<unsigned short, unsigned short>(x,y));
    }

    return adjacencies;
}

//================================================================================
std::vector<std::pair<unsigned short, unsigned short> > Goban::GetDiagonals(unsigned short move_x, unsigned short move_y) const
{
    unsigned short x;
    unsigned short y;

    std::vector<std::pair<unsigned short, unsigned short> > diagonals;


    //-------------------------------------------
    x = move_x+1;
    y = move_y+1;

    if(IsValidCoordinates(x,y))
        diagonals.push_back(std::pair<unsigned short, unsigned short>(x,y));
    //-------------------------------------------

    //-------------------------------------------
    x = move_x+1;
    y = move_y-1;

    if(IsValidCoordinates(x,y))
        diagonals.push_back(std::pair<unsigned short, unsigned short>(x,y));
    //-------------------------------------------

    //-------------------------------------------
    x = move_x-1;
    y = move_y+1;

    if(IsValidCoordinates(x,y))
        diagonals.push_back(std::pair<unsigned short, unsigned short>(x,y));
    //-------------------------------------------

    //-------------------------------------------
    x = move_x-1;
    y = move_y-1;

    if(IsValidCoordinates(x,y))
        diagonals.push_back(std::pair<unsigned short, unsigned short>(x,y));
    //-------------------------------------------

    return diagonals;
}

//================================================================================
bool Goban::IsValidCoordinates(unsigned short x, unsigned short y) const
{
    return !( (x < 0) || (x>=Dimension()) || (y < 0) || (y >= Dimension()) );
}

//================================================================================
