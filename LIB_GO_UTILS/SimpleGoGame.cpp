#include "SimpleGoGame.h"
#include "GoUtils.h"
#include "GobanUtils.h"


//===============================================================================================
SimpleGoGame::SimpleGoGame(int board_size, boost::shared_ptr<GoReferee> l_referee):
    m_goban(board_size),
    m_goReferee_sptr(l_referee)
{
    m_currentPlayer = BLACK;
    m_simple_ko_restriction = 0;
}

//===============================================================================================
//bool SimpleGoGame::PlayMove(int l_mapPos_x, int l_mapPos_y, StoneState stone)
//{
//    bool success = false;
//    int board_size = m_goban.GetGoban().size();

//    if(stone != EMPTY)
//        m_currentPlayer = stone;

//    if(l_mapPos_x >= 0 && l_mapPos_y >= 0 && l_mapPos_x < board_size && l_mapPos_y < board_size)
//    {
//        if(m_goReferee_sptr->IsLegal(m_currentPlayer,l_mapPos_x, l_mapPos_y, m_goban, m_simple_ko_restriction))
//        {

//            std::pair<bool, short> l_pair = m_goban.PutStone(l_mapPos_x, l_mapPos_y, m_currentPlayer);
//            if(l_pair.first)//Succeded in putting the stone
//            {
//                    //Update simple_ko_restriction
//                    m_simple_ko_restriction = abs(l_pair.second);

//                    m_currentPlayer = (StoneState) ((int) m_currentPlayer * (-1));

//                    success = true;
//                    return success;
//            }

//        }
//    }

//    std::cout << "NEVER HIT HERE"<< std::endl;
//    return success;
//}

//===============================================================================================
bool SimpleGoGame::PlayMove(int l_mapPos_x, int l_mapPos_y, StoneState stone)
{

    //Let us suppose that is everything correct
    m_currentPlayer = stone;

    std::pair<bool, short> l_pair = m_goban.PutStone(l_mapPos_x, l_mapPos_y, m_currentPlayer);
    if(l_pair.first)//Succeded in putting the stone
    {
        //Update simple_ko_restriction
        m_simple_ko_restriction = abs(l_pair.second);

        m_currentPlayer = (StoneState) ((int) m_currentPlayer * (-1));

        return true;
    }

    return false;
}
//===============================================================================================

void SimpleGoGame::GenerateAllLegalMoves(std::vector<short>& result)
{    
    const std::vector<std::vector<GobanIntersection>>& l_intersections =  m_goban.GetGoban();
    std::vector<std::vector<GobanIntersection> >::const_iterator it;
    std::vector<GobanIntersection>::const_iterator second_it;

    GobanIntersection l_gint;
    for(it = l_intersections.begin(); it != l_intersections.end(); ++it)
    {
        for(second_it = (*it).begin(); second_it!= (*it).end(); ++second_it)
        {
            l_gint = *second_it;

            if((l_gint.GetStone() == EMPTY) &&  m_goReferee_sptr->IsLegal(m_currentPlayer,l_gint.GetX(), l_gint.GetY(), m_goban, m_simple_ko_restriction))
            {
                result.push_back(GoUtils::BoardPositionToMove(l_gint.GetX(), l_gint.GetY(), m_currentPlayer));
            }
        }
    }
}
//===============================================================================================
void SimpleGoGame::SetFromCompactBoard(CompactBoard* cb)
{
    m_goban.SetFromCompactBoard(cb);
    m_simple_ko_restriction = 0;
}
//===============================================================================================
unsigned short SimpleGoGame::Dimension()
{
    return m_goban.Dimension();
}
//===============================================================================================
bool SimpleGoGame::PlayMove(short move)
{
    m_currentPlayer = GobanUtils::GetStoneState(move);
    std::pair<unsigned short, unsigned short> l_map_pos = GoUtils::MoveToBoardPosition(move);
    return PlayMove(l_map_pos.first,l_map_pos.second, m_currentPlayer);
}

//===============================================================================================
void SimpleGoGame::PlayPassMove()
{
    m_currentPlayer = (StoneState) ((int) m_currentPlayer * (-1));
}

//===============================================================================================
const Goban& SimpleGoGame::GetMainBoard()
{
    return m_goban;
}
//===============================================================================================
void SimpleGoGame::SetCurrentPlayer(StoneState to_play)
{
    m_currentPlayer = to_play;
}
