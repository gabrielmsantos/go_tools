#ifndef GOBAN_H
#define GOBAN_H


#include <math.h>
#include <algorithm>
#include <utility>
#include <unordered_set>
#include <boost/functional/hash.hpp>
#include <boost/shared_ptr.hpp>
#include "GoInfluenceModel/I_InfluenceModel.h"
#include "I_GobanSubject.h"

#define SIZE_BYTE 8
#define MAX_BOARD 19
#define MIN_BOARD 4

//=================== Stone State ================================
enum StoneState{
    WHITE =-1,
    EMPTY,
    BLACK
};
//=================== Flags to be used on variations =============
enum CBFlag
{
    R0 = 0,
    R90 = 1,
    R180 = 2,
    R270 = 3,
    MIRRORED = 1 << 2,
    INVERSED = 1 << 3
};

//The purpose of this struct is to be a very compact board representation
//=================== CompactBoard Struct ================================
typedef struct CompactBoard
{
    unsigned int* m_compact_board;
    unsigned short m_black_prisoners;
    unsigned short m_white_prisoners;
    unsigned char  m_info;

    //============================== The Big Three =======================
    CompactBoard(unsigned int board_size)
    {
        /**This representation supports up to 19x19 boardsize. In case of changing this specification, MAX_BOARD should be changed.*/
        /** For 19x19 boards total size of CompactBoard struct:
        - 24 bytes for the board representation ( [0-11] -> White space [12-23]->Black Space )
        - 1 byte m_size
        - 2 bytes m_black_prisoners
        - 2 bytes m_white prisoners
        = 29 bytes
        */
        m_info = 0;
        SetBoardSize(board_size);

        m_compact_board =  new unsigned int[GetSizeBytesAllocatedToBoard()]; //( [0-11] -> White space [12-23]->Black Space )

        Clear();
    }

    //================================================================================
    CompactBoard(const CompactBoard& compact_copy)
    {
        m_info = compact_copy.m_info;

        m_compact_board =  new unsigned int[GetSizeBytesAllocatedToBoard()]; //( [0-11] -> White space [12-23]->Black Space )

        //copy prisoners
        m_black_prisoners = compact_copy.m_black_prisoners;
        m_white_prisoners = compact_copy.m_white_prisoners;

        for(unsigned int i=0; i< GetSizeBytesAllocatedToBoard(); ++i)
        {
            m_compact_board[i] = compact_copy.m_compact_board[i];
        }
    }

    //================================================================================
    //It returns the number of bytes allocated to the board (m_compact_board)
    unsigned int GetSizeBytesAllocatedToBoard()
    {
        //For now this value will be the MAX_BOARD, but itshould be GetBoardSize();
        unsigned int one_board_bytes = ceil( (double)(MAX_BOARD*MAX_BOARD)/(sizeof(unsigned int) * SIZE_BYTE) );

        return 2*one_board_bytes;
    }

    //================================================================================
    void SetBoardSize(unsigned int board_size)
    {
        /** The board_size stored is equivalent to MAX_BOARD - board_size, because it was made
        to fit 4 bits (maping 16 positions). In this fashion the maximum size supported is MAX_BOARD
        and the MINIMUM is MIN_BOARD*/
        assert ( ( board_size >= MIN_BOARD) && (board_size <= MAX_BOARD) );
        m_info  = (MAX_BOARD - board_size) << 4;
    }

    //================================================================================
    unsigned char GetBoardSize()
    {
        return (unsigned char) MAX_BOARD - (m_info >> 4);
    }

    //================================================================================
    CompactBoard& operator=(const CompactBoard& compact_copy)
    {
        m_info = compact_copy.m_info;

        //delete previous data
        delete[] m_compact_board;

        m_compact_board =  new unsigned int[GetSizeBytesAllocatedToBoard()]; //( [0-11] -> White space [12-23]->Black Space )

        //copy prisoners
        m_black_prisoners = compact_copy.m_black_prisoners;
        m_white_prisoners = compact_copy.m_white_prisoners;


        for(unsigned int i=0; i< GetSizeBytesAllocatedToBoard(); ++i)
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
        for(unsigned int i = 0; i< GetSizeBytesAllocatedToBoard(); ++i)
        {
            m_compact_board[i] = 0;
        }
    }

    unsigned int HalfSize()
    {
        return GetSizeBytesAllocatedToBoard()/2;
    }

    ~CompactBoard()
    {
        delete[] m_compact_board;
    }

    //================================================================================
    /** Return the number of different bits from another board_representation (Including prisoners)*/
    unsigned int XOR(const CompactBoard * p_compact_board)
    {
        if(p_compact_board == NULL)
        {
            return 1;
        }
        unsigned int total = 0;
        unsigned int l_xor;
        for(unsigned int i = 0; i < GetSizeBytesAllocatedToBoard(); ++i)
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

    //================================================================================
    /** Return the number of stones present in the current state*/
    unsigned short GetNumberOfStones()
    {
        unsigned short total = 0;
        unsigned int l_xor;
        for(unsigned int i = 0; i < GetSizeBytesAllocatedToBoard(); ++i)
        {
            l_xor= m_compact_board[i] ^ 0;

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

    //================================================================================
    std::vector<CompactBoard* > GetAllVariations()
    {

        std::vector<CompactBoard*> all_variations;

        CompactBoard* cb = new CompactBoard(*this);
        CompactBoard* cb_inv;
        CompactBoard* cb_mirr;
        CompactBoard* cb_inv_mirr;

        //For each rotation version get its inversed and mirrored version as well.
        for(unsigned int i = 0; i < 4; ++i )
        {
            if(i>0)
            {
                cb = cb->GetRotatedClockwiseVariation();
            }

            //Just the original rotated (0, 90, 180 or 270 degrees)
            cb->SetInfo((CBFlag)i);
            all_variations.push_back(cb);

            //Get its mirrored variation
            cb_mirr = cb->GetMirroredVariation();
            cb_mirr->SetInfo((CBFlag) ( i | MIRRORED));
            all_variations.push_back(cb_mirr);

            //Get its inversed version
            cb_inv = cb->GetInversedVariation();
            cb_inv->SetInfo((CBFlag) (i | INVERSED ));
            all_variations.push_back(cb_inv );

            cb_inv_mirr = cb_inv->GetMirroredVariation();
            cb_inv_mirr->SetInfo((CBFlag) (i | INVERSED | MIRRORED) );
            all_variations.push_back(cb_inv_mirr );
        }

        return all_variations;
    }


    unsigned char GetRotation()
    {
        return m_info & (unsigned char)3;
    }

    bool IsMirrored()
    {
        return m_info & MIRRORED;
    }

    bool IsInversed()
    {
        return m_info & INVERSED;
    }

    void SetInfo(CBFlag flag)
    {
        //It should preserve the board size and set the flags passed in flag
        char mask = 15 << 4;//11110000
        m_info = (m_info & mask) | flag;
    }

    //================================================================================
private:

    //================================================================================
    /*Verify if a specific position is set (due a specific color) */
    bool IsSet(int row, int column, StoneState stone)
    {
        unsigned int position = ( row * MAX_BOARD) + column;
        unsigned int start = (((stone+1)/2)*HalfSize()) ;
        unsigned int byte_pos = start + ( position/(sizeof(unsigned int) * SIZE_BYTE) );
        unsigned int offset = position % (sizeof(unsigned int) * SIZE_BYTE);

        assert(byte_pos < GetSizeBytesAllocatedToBoard());
        return ( (m_compact_board[byte_pos] & (unsigned int) 1 << offset)  > 0 );
    }

    //================================================================================
    /* Set a specific position of the CompactBoard (due a specific color) **/
    void SetPosition(int row, int column, StoneState stone)
    {
        unsigned int position = ( row * MAX_BOARD) + column;
        unsigned int start = (((stone+1)/2)*HalfSize()) ;
        unsigned int byte_pos = start + ( position/(sizeof(unsigned int) * SIZE_BYTE) );
        unsigned int offset = position % (sizeof(unsigned int) * SIZE_BYTE);

        assert(byte_pos < GetSizeBytesAllocatedToBoard());

        m_compact_board[byte_pos] |= ( (unsigned int) 1 << offset );
    }


    //================================================================================
    /*Rotates the CompactBoard clockwise and returns a new (rotated) CompactBoard*/
    CompactBoard* GetRotatedClockwiseVariation()
    {
        CompactBoard* cb_rotated = new CompactBoard(GetBoardSize());

        /**Copying properties*/
        cb_rotated->m_black_prisoners = m_black_prisoners;
        cb_rotated->m_white_prisoners = m_white_prisoners;
        cb_rotated->m_info = m_info;

        //Rotating the board
        for(int i = 0; i < MAX_BOARD; ++i)
        {
            for(int j =0; j< MAX_BOARD; ++j)
            {
                if(IsSet(i,j,BLACK))
                {
                    cb_rotated->SetPosition(MAX_BOARD-1-j, i,BLACK);
                }

                else if(IsSet(i,j,WHITE))
                {
                    cb_rotated->SetPosition(MAX_BOARD-1-j, i,WHITE);
                }
            }
        }

        return cb_rotated;
    }

    //======================== Test purposes ==========================================
    void PrintCompactBoard()
    {
        //Rotating the board
        for(int i = 0; i < MAX_BOARD; ++i)
        {
            for(int j =0; j< MAX_BOARD; ++j)
            {
                if(IsSet(i,j,BLACK))
                {
                    std::cout << i << " ** " << j << " B" << std::endl;
                }
                else if(IsSet(i,j,WHITE))
                {
                    std::cout << i << " ** " << j << " W" << std::endl;
                }
            }
        }

    }

    //================================================================================
    /**Returns its inversed variation*/
    CompactBoard* GetInversedVariation()
    {
        CompactBoard* cb_inversed= new CompactBoard(GetBoardSize());

        /**Copying properties*/
        cb_inversed->m_black_prisoners = m_white_prisoners;
        cb_inversed->m_white_prisoners = m_black_prisoners;
        cb_inversed->m_info = m_info;

        //Rotating the board
        for(int i = 0; i < MAX_BOARD; ++i)
        {
            for(int j =0; j< MAX_BOARD; ++j)
            {
                if(IsSet(i,j,BLACK))
                {
                    cb_inversed->SetPosition(i,j,WHITE);
                }
                else if(IsSet(i,j,WHITE))
                {
                    cb_inversed->SetPosition(i,j,BLACK);
                }
            }
        }

        return cb_inversed;
    }
    //================================================================================
    /**Returns its mirrored variation*/
    CompactBoard* GetMirroredVariation()
    {
        CompactBoard* cb_mirrored= new CompactBoard(GetBoardSize());

        /**Copying properties*/
        cb_mirrored->m_black_prisoners = m_white_prisoners;
        cb_mirrored->m_white_prisoners = m_black_prisoners;
        cb_mirrored->m_info = m_info;

        //Rotating the board
        for(int i = 0; i < MAX_BOARD; ++i)
        {
            for(int j =0; j< MAX_BOARD; ++j)
            {
                if(IsSet(i,j,BLACK))
                {
                    cb_mirrored->SetPosition((GetBoardSize()-1)-i,j,BLACK);
                }
                else if(IsSet(i,j,WHITE))
                {
                    cb_mirrored->SetPosition((GetBoardSize()-1)-i,j,WHITE);
                }

            }
        }

        return cb_mirrored;
    }
}CompactBoard;

//=================== Intersection Class ================================
class GobanIntersection
{
public:
    GobanIntersection() :
        m_stone(EMPTY)
    {
        m_dragon_parent = this;
        m_dragon_size = 0;
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

    /*Add a liberty to the root intersection*/
    void DragonAddLiberty(int x, int y)
    {

        GobanIntersection* root = GetRoot();
        root->m_dragon_liberties.insert(std::make_pair(x,y));
    }

    unsigned short DragonLibertiesCount()
    {
        return m_dragon_liberties.size();
    }

    void DragonRemoveLiberty(int x, int y)
    {
        GobanIntersection* root = GetRoot();
        root->m_dragon_liberties.erase(std::make_pair(x,y));
    }

    void DragonMergeLiberties(std::unordered_set< std::pair<int, int>, boost::hash< std::pair<int, int> > >& liberties)
    {
        std::unordered_set< std::pair<int, int>, boost::hash< std::pair<int, int> > >::const_iterator it;
        std::pair<int, int> l_pair;
        for(it = liberties.begin(); it!= liberties.end();++it)
        {
            l_pair = (*it);
            m_dragon_liberties.insert(l_pair);
        }
    }

    void DragonAddIntersection(GobanIntersection* intersection)
    {

        GobanIntersection* root_larger = GetRoot();
        GobanIntersection* root_smaller = intersection->GetRoot();

        if(root_larger == root_smaller)
            return;

        root_smaller->DragonRemoveLiberty(GetX(), GetY());
        //Remove the liberty regarding to intersection position
        root_larger->DragonRemoveLiberty(intersection->GetX(), intersection->GetY());

        //Add liberties from the smaller Dragon
        root_larger->DragonMergeLiberties(root_smaller->m_dragon_liberties);

        //Larger Dragon swallows small dragon
        root_smaller->SetParent(root_larger);

        //Updating size
        root_larger->m_dragon_size+=root_smaller->m_dragon_size;

    }

    void SetParent(GobanIntersection* new_parent)
    {
        m_dragon_parent->m_dragon_children.erase(this);
        m_dragon_parent = new_parent;
        m_dragon_parent->m_dragon_children.insert(this);
    }

    unsigned short GetSize()
    {
        return GetRoot()->m_dragon_size;
    }

    void PrintLiberties() const
    {
        std::unordered_set< std::pair<int, int>, boost::hash< std::pair<int, int> > >::const_iterator it;
        const std::pair<int, int>* l_pair;
        for(it = m_dragon_liberties.begin(); it!= m_dragon_liberties.end();++it)
        {
            l_pair = &(*it);
            std::cout << l_pair->first << "-"<< l_pair->second << "  ";
        }
        std::cout << std::endl;
    }

    /** Recursively get the root*/
    GobanIntersection* GetRoot()
    {
        if(m_dragon_parent==this)
        {
            return m_dragon_parent;
        }

        GobanIntersection* root = m_dragon_parent->GetRoot();

        //PATH COMPRESSION
        SetParent(root);

        return root;
    }



private:

    //Which Stone currently is on the intersection
    StoneState m_stone; // -1(black), 0(empty), 1(white)

    //The intersection board position [0 -- (BOARD_SIZE-1)][0 -- (BOARD_SIZE-1)]
    sf::Vector2i m_board_position;

    void Clear()
    {
        m_stone = EMPTY;
        m_dragon_parent = this;
        m_dragon_size = 0;
        m_dragon_children.clear();
        m_dragon_liberties.clear();
    }

    inline void PutStone(StoneState stone)
    {
        m_stone = stone;
    }

    //Regarding to the dragon
    std::unordered_set< std::pair<int, int>, boost::hash< std::pair<int, int> > > m_dragon_liberties;
    std::unordered_set<GobanIntersection*> m_dragon_children;
    GobanIntersection* m_dragon_parent;
    unsigned short m_dragon_size;

};
//=================== Dragon Class ================================
/*class Dragon
{
public:

    Dragon(GobanIntersection* intersection)
    {
        m_intersection = intersection;
        m_root = this;
        m_size = 0;
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


    //TODO PATH COMPRESSION
    Dragon* GetMainRoot()
    {
        if(m_root==this)
        {
            return m_root;
        }

        return m_root->GetMainRoot();
    }

    friend class Goban;

private:
    Dragon* m_root;
    GobanIntersection* m_intersection;
    std::unordered_set<Dragon*> m_stones;
    std::unordered_set< std::pair<int, int>, boost::hash< std::pair<int, int> > > m_liberties;
    unsigned short m_size;
};*/

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

    GobanIntersection* GetDragon(int x, int y);

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

    inline unsigned int GetBoardSize() const
    {
        return m_intersections.size();
    }

    bool RemoveStone(const sf::Vector2i& r_mapPosition);

    void ClearBoard();

    Goban& operator=(const Goban& influence_goban_copy);

    CompactBoard *GetCompactBoard() const;

    void SetFromCompactBoard(CompactBoard *p_compact_board);


private:

    std::vector<std::vector<GobanIntersection> > m_intersections;

    //std::vector<std::vector<Dragon*> > m_dragons;

    unsigned short m_black_prisoners;
    unsigned short m_white_prisoners;

    //void MakeDragon(GobanIntersection *p_intersection, Dragon* p_dragon);

    void CheckDragons(GobanIntersection* intersection);

    StoneState Opponent(StoneState stone) const;

    unsigned short RemoveDragon(GobanIntersection *l_dragon);
};

#endif // GOBAN_H
