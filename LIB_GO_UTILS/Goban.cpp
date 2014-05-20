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
bool Goban::PutStone(const sf::Vector2i& r_mapPosition, StoneState stone)
{
    if ( ( GetStone(r_mapPosition.x, r_mapPosition.y) == EMPTY )  && ( stone != EMPTY ) )
    {
        m_intersections[r_mapPosition.x][r_mapPosition.y].PutStone(stone);

        //Distribute the influence over the board
        //m_influence_model->PutInfluence(r_mapPosition, stone, *this);
        NotifyObservers(r_mapPosition, stone);

        //Settle the Dragons (Groups of Stones) {Remove Stones, Join Dragons, etc..} Go Rules Here
        CheckDragons(&(m_intersections[r_mapPosition.x][r_mapPosition.y]));

        return true;
    }

    return false;
}
//================================================================================
void Goban::CheckDragons(GobanIntersection* intersection)
{
    int x;
    int y;
    Dragon* l_dragon;

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

        if(m_intersections[x][y].GetStone() == Opponent(intersection->GetStone()))
        {
            l_dragon = GetDragon(x,y);
            if(l_dragon->LibertyCount() == 0)
            {

                StoneState stone = m_intersections[x][y].GetStone();

                //It deletes the dragon inside and count prisoners
                unsigned short prisoners = RemoveDragon(l_dragon);

                if(stone  == WHITE)
                {
                    m_black_prisoners += prisoners;
                }else
                {
                    m_white_prisoners += prisoners;
                }

            }else
            {
                delete l_dragon;
            }
        }
    }

}

//================================================================================
StoneState Goban::Opponent(StoneState stone) const
{
    return (StoneState)((int)stone * (-1));
}
//================================================================================
unsigned short Goban::RemoveDragon(Dragon* l_dragon)
{
    unsigned short dragon_size = 0;
    std::unordered_set<GobanIntersection*>::iterator it;
    for(it = l_dragon->GetStones().begin(); it != l_dragon->GetStones().end();++it, ++dragon_size)
    {
        RemoveStone((*it)->GetBoardPosition());
    }

    delete l_dragon;
    l_dragon=0;

    return dragon_size;
}
//================================================================================
bool Goban::RemoveStone(const sf::Vector2i &r_mapPosition)
{
    if (GetStone(r_mapPosition.x, r_mapPosition.y)!=EMPTY) {

        //m_influence_model->PutInfluence(r_mapPosition, GetStone(r_mapPosition.x, r_mapPosition.y) * (-1), *this);

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
Dragon* Goban::GetDragon(int x, int y)
{
    Dragon* l_dragon = new Dragon();

    MakeDragon(&(m_intersections[x][y]), l_dragon);

    return l_dragon;
}
//================================================================================
void Goban::MakeDragon( GobanIntersection* p_intersection, Dragon* p_dragon)
{
    p_dragon->AddStone(p_intersection);

    StoneState adj;
    int x;
    int y;

    for(int i=0; i<4; ++i)
    {
        if(i%2 == 0)
        {
            x = p_intersection->GetX()-1+i;
            y = p_intersection->GetY();

            if((x < 0)||(x>=Dimension()))
                continue;
        }else
        {
            x = p_intersection->GetX();
            y = p_intersection->GetY()-2+i;

            if((y < 0)||(y>=Dimension()))
                continue;
        }

        adj = GetStone(x,y);

        //Add Liberty
        if(adj == EMPTY)
        {
            p_dragon->AddLiberty(x,y);

        }else if (adj == p_intersection->GetStone())
        {
            //MakeDragon Recursively
            if(! p_dragon->HasIntersection( &(m_intersections[x][y]) ) )
            {
                MakeDragon(&(m_intersections[x][y]), p_dragon);
            }
        }
    }
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
            PutStone(sf::Vector2i(x,y), (StoneState) ((2*j) -1) );
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
