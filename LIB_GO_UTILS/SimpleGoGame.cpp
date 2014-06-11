#include "SimpleGoGame.h"
#include "GoUtils.h"


//===============================================================================================
SimpleGoGame::SimpleGoGame(int board_size, boost::shared_ptr<GoReferee> l_referee):
    m_goban(board_size),
    m_goReferee_sptr(l_referee)
{
    m_currentPlayer = BLACK;
}
//===============================================================================================
void SimpleGoGame::PlayMove(int l_mapPos_x, int l_mapPos_y, StoneState stone)
{
    int board_size = m_goban.GetGoban().size();

    if(stone != EMPTY)
        m_currentPlayer = stone;

    if(l_mapPos_x >= 0 && l_mapPos_y >= 0 && l_mapPos_x < board_size && l_mapPos_y < board_size)
    {
        if(m_goReferee_sptr->IsLegal(m_currentPlayer,l_mapPos_x, l_mapPos_y, m_goban, m_simple_ko_restriction))
        {

            std::pair<bool, short> l_pair = m_goban.PutStone(l_mapPos_x, l_mapPos_y, m_currentPlayer);
            if(l_pair.first)//Succeded in putting the stone
            {
                    //Update simple_ko_restriction
                    m_simple_ko_restriction = abs(l_pair.second);

                    m_currentPlayer = (StoneState) ((int) m_currentPlayer * (-1));

            }

        }
    }
}
//===============================================================================================
std::vector<short> SimpleGoGame::GenerateAllLegalMoves()
{
    std::vector<short> result;
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

    return result;

}
//===============================================================================================
void SimpleGoGame::SetFromCompactBoard(CompactBoard* cb)
{
    m_goban.SetFromCompactBoard(cb);
}
//===============================================================================================
