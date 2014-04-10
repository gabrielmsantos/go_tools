#ifndef GOBAN_H
#define GOBAN_H


#include <math.h>
#include <algorithm>
#include <unordered_set>
#include <boost/functional/hash.hpp>
#include <boost/shared_ptr.hpp>
#include "GoInfluenceModel/I_InfluenceModel.h"
#include "I_GobanSubject.h"

#define SIZE_BYTE 8
#define MAX_BOARD 19

//=================== Stone State ================================
enum StoneState{
    WHITE =-1,
    EMPTY,
    BLACK
};

//=================== Move Struct ================================
typedef struct Move
{
    Move(StoneState s,const sf::Vector2i& pos)
        :m_stone(s),
          m_position(pos)

    {
    }

    StoneState  m_stone;
    sf::Vector2i m_position;
}Move;

//=================== CompactBoard Struct ================================
typedef struct CompactBoard
{
    unsigned int* m_compact_board;
    unsigned int  m_size;
    unsigned short m_black_prisoners;
    unsigned short m_white_prisoners;

    //============================== The Big Three =======================
    CompactBoard()
    {
        /**This representation supports up to 19x19 boardsize. In case of changing this specification, MAX_BOARD should be changed.*/
        unsigned int one_board_bytes = ceil( (double)(MAX_BOARD*MAX_BOARD)/(sizeof(unsigned int) * SIZE_BYTE) );

        m_size = 2*one_board_bytes;

        m_compact_board =  new unsigned int[m_size]; //( [0-11] -> White space [12-23]->Black Space )

        Clear();
    }

    CompactBoard(const CompactBoard& compact_copy)
    {
        m_size = compact_copy.m_size;

        //copy prisoners
        m_black_prisoners = compact_copy.m_black_prisoners;
        m_white_prisoners = compact_copy.m_white_prisoners;

        for(unsigned int i=0; i< m_size; ++i)
        {
            m_compact_board[i] = compact_copy.m_compact_board[i];
        }

    }

    CompactBoard& operator=(const CompactBoard& compact_copy)
    {
        m_size = compact_copy.m_size;

        //copy prisoners
        m_black_prisoners = compact_copy.m_black_prisoners;
        m_white_prisoners = compact_copy.m_white_prisoners;

        for(unsigned int i=0; i< m_size; ++i)
        {
            m_compact_board[i] = compact_copy.m_compact_board[i];
        }

        return *this;
    }
    //============================== The Big Three =======================

    void Clear()
    {
        m_black_prisoners = 0;
        m_white_prisoners = 0;
        for(unsigned int i = 0; i< m_size; ++i)
        {
            m_compact_board[i] = 0;
        }
    }

    int HalfSize()
    {
        return m_size/2;
    }

    ~CompactBoard()
    {
        delete[] m_compact_board;
    }

    /** Return the number of different bits from another board_representation (Including prisoners)*/
    unsigned int XOR(const CompactBoard * p_compact_board)
    {
        if(p_compact_board == NULL)
        {
            return 1;
        }
        unsigned int total = 0;
        unsigned int l_xor;
        for(unsigned int i = 0; i < m_size; ++i)
        {
            l_xor= m_compact_board[i] ^ p_compact_board->m_compact_board[i];

            for(unsigned int j=0; j < ( sizeof(unsigned int) * SIZE_BYTE ); ++j)
            {
                if( ( l_xor & ( (unsigned int ) 1 << j ) ) > 0 )
                {
                    ++total;
                }
            }
        }
        return total;
    }

}CompactBoard;

//========================================================================

class Dragon;

//=================== Intersection Class ================================
class GobanIntersection
{
public:
    GobanIntersection() :
        m_stone(EMPTY)
    {
    }

    StoneState GetStone() const
    {
        return m_stone;
    }

    inline void SetCoord(sf::Vector2i position)
    {
        m_board_position = position;
    }

    inline bool IsAdjacentTo(GobanIntersection* intersection) const
    {
        if((abs(m_board_position.x - intersection->GetX()) ==1 ) || (abs(m_board_position.y - intersection->GetY()) ==1))
            return true;

        return false;
    }

    inline int GetX() const
    {
        return m_board_position.x;
    }

    inline int GetY() const
    {
        return m_board_position.y;
    }

    inline const sf::Vector2i& GetBoardPosition() const
    {
        return m_board_position;
    }

    friend class Goban;

private:

    //Which Stone currently is on the intersection
    StoneState m_stone; // -1(black), 0(empty), 1(white)

    //The intersection board position [0 -- (BOARD_SIZE-1)][0 -- (BOARD_SIZE-1)]
    sf::Vector2i m_board_position;

    void Clear()
    {
        m_stone =EMPTY;
    }

    inline void PutStone(StoneState stone)
    {
        m_stone = stone;
    }

};

//=================== Dragon Class ================================
class Dragon
{
public:

    Dragon()
    {
    }

    bool IsAdjacentTo(GobanIntersection* intersection) const
    {
        std::unordered_set<GobanIntersection*>::const_iterator it;
        for(it = m_stones.begin(); it != m_stones.end(); ++it)
        {
            if(intersection->IsAdjacentTo(*it))
                return true;
        }

        return false;
    }

    void AddStone(GobanIntersection* intersection)
    {
        m_stones.insert(intersection);
    }

    void RemoveLiberty(int x, int y)
    {
        m_liberties.erase(std::make_pair(x,y));
    }

    void AddLiberty(int x, int y)
    {
        m_liberties.insert(std::make_pair(x,y));
    }

    int LibertyCount() const
    {
        return m_liberties.size();
    }

    void PrintLiberties() const
    {
        std::unordered_set< std::pair<int, int>, boost::hash< std::pair<int, int> > >::const_iterator it;
        const std::pair<int, int>* l_pair;
        for(it = m_liberties.begin(); it!= m_liberties.end();++it)
        {
            l_pair = &(*it);
            std::cout << l_pair->first << "-"<< l_pair->second << "  ";
        }
        std::cout << std::endl;
    }

    std::unordered_set<GobanIntersection*>& GetStones()
    {
        return m_stones;
    }

    inline bool HasIntersection(GobanIntersection* p_intersection)
    {
        return ( m_stones.find(p_intersection) != m_stones.end() ) ;
    }

    friend class Goban;

private:
    std::unordered_set<GobanIntersection*> m_stones;
    std::unordered_set< std::pair<int, int>, boost::hash< std::pair<int, int> > > m_liberties;
};

//=================== Influence Goban ====================================
class Goban: public I_GobanSubject
{
public:
    explicit Goban(int board_size);

    ~Goban();

    bool PutStone(const sf::Vector2i& r_mapPosition, StoneState stone);

    inline int Dimension() const
    {
        return m_intersections.size();
    }

    inline StoneState GetStone(int x, int y) const
    {
        return m_intersections[x][y].GetStone();
    }

    inline StoneState GetStone(sf::Vector2i pos) const
    {
        return m_intersections[pos.x][pos.y].GetStone();
    }

    inline int GetLayer(int x, int y) const
    {
        int a = std::min(x+1,y+1);
        int b = std::min( abs(x - Dimension()), abs( y - Dimension()) );
        return std::min(a,b);
    }

    Dragon* GetDragon(int x, int y);

    inline const std::vector<std::vector<GobanIntersection> >& GetGoban() const
    {
        return m_intersections;
    }

    inline int CountStones(StoneState stone) const
    {
        int total = 0;
        std::vector<std::vector<GobanIntersection> >::const_iterator it;
        std::vector<GobanIntersection>::const_iterator second_it;

        for(it = m_intersections.begin(); it != m_intersections.end(); ++it)
        {
            for(second_it = (*it).begin(); second_it!= (*it).end(); ++second_it)
            {
                if(stone == (*second_it).GetStone())
                {
                    ++total;
                }
            }
        }
        return total;
    }

    inline unsigned short GetWhitePrisoners() const
    {
        return m_white_prisoners;
    }

    inline unsigned short GetBlackPrisoners() const
    {
        return m_black_prisoners;
    }

    bool RemoveStone(const sf::Vector2i& r_mapPosition);

    void ClearBoard();

    Goban& operator=(const Goban& influence_goban_copy);

    CompactBoard *GetCompactBoard() const;

    void SetFromCompactBoard(CompactBoard *p_compact_board);

private:

    std::vector<std::vector<GobanIntersection> > m_intersections;
    //boost::shared_ptr<InfluenceModel> m_influence_model;

    unsigned short m_black_prisoners;
    unsigned short m_white_prisoners;

    void MakeDragon(GobanIntersection *p_intersection, Dragon* p_dragon);

    void CheckDragons(GobanIntersection* intersection);

    StoneState Opponent(StoneState stone) const;

    unsigned short RemoveDragon(Dragon* l_dragon);
};

#endif // GOBAN_H
