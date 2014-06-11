#include "GoGame.h"
#include "GoUtils.h"
#include "GoInfluenceModel/CrossInfluence.h"

//===============================================================================================
GoGame::GoGame(int board_size, boost::shared_ptr<GoReferee> l_referee):
    m_goban(board_size),
    m_goReferee_sptr(l_referee)
{
    m_game_tree.InsertNewNode(new GoGameNode(0, m_goban.GetCompactBoard()));
    m_currentPlayer = BLACK;
}
//===============================================================================================
void GoGame::PlayMove(int i, int j ,StoneState stone)
{
    sf::Vector2i l_map_pos(i,j);
    PlayMove(l_map_pos, stone);
}
//===============================================================================================
void GoGame::PlayMove(sf::Vector2i &l_map_pos ,StoneState stone)
{
    m_currentPlayer = stone;
    PlayMove(l_map_pos.x,l_map_pos.y);
}
//===============================================================================================
void GoGame::PlayMove(unsigned int l_mapPos_x, unsigned int l_mapPos_y)
{
    int board_size = m_goban.GetGoban().size();

    CompactBoard* l_previous_state = 0;
    CompactBoard* l_next_state = 0;
    CompactBoard* l_current_state = 0;

    GoGameNode* l_node = 0;

    if(l_mapPos_x >= 0 && l_mapPos_y >= 0 && l_mapPos_x < board_size && l_mapPos_y < board_size)
    {
        if(m_goReferee_sptr->IsLegal(m_currentPlayer,l_mapPos_x, l_mapPos_y, m_goban, m_simple_ko_restriction))
        {
            l_node = m_game_tree.GetCurrentNode()->GetParent();

            if(l_node)
            {
                l_previous_state = l_node->GetCompactBoard();
            }

            l_current_state = m_goban.GetCompactBoard();

            std::pair<bool, short> l_pair = m_goban.PutStone(l_mapPos_x, l_mapPos_y, m_currentPlayer);
            if(l_pair.first)//Succeded in putting the stone
            {
                l_next_state = m_goban.GetCompactBoard();

                //Complex KO checking
                if( (l_previous_state) && ( l_next_state->XOR(l_previous_state) == 0) ) //KO
                {
                    m_goban.SetFromCompactBoard(l_current_state);
                    delete l_next_state;
                    delete l_current_state;
                    return;

                }else
                {
                    /** The move that leads to the next state and the player who made it.
                      If the move is negative then it was made by the white player, otherwise it the black player
                    */
                    short move = GoUtils::BoardPositionToMove(l_mapPos_x, l_mapPos_y, m_currentPlayer);

                    //Update simple_ko_restriction
                    m_simple_ko_restriction = abs(l_pair.second);

//                    std::cout << "x: " << l_mapPos.x << std::endl;
//                    std::cout << "y: " << l_mapPos.y << std::endl;
//                    std::cout << "move: " << move << std::endl;
//                    std::cout << "---  "  << std::endl;

                    m_currentPlayer = (StoneState) ((int) m_currentPlayer * (-1));

                    m_game_tree.InsertNewNode(new GoGameNode(move, l_next_state));
                }
            }

            delete l_current_state;
        }
    }
}
//===============================================================================================

Goban& GoGame::GetMainBoard()
{
    return m_goban;
}

//===============================================================================================
GoGame::~GoGame()
{
    //std::cout <<"Destructing GoGame" << std::endl;
}
//===============================================================================================
void GoGame::TakeBack()
{

    /**
      Currently, the TakeBack method deactivates the Previously CheckSimpleKo
      In order to change that it would be necessary to save this variavel together
      with its state. At the moment it is a unnecessary cost.
    */
    m_simple_ko_restriction =0;

    GoGameNode* l_node = 0;
    bool took = m_game_tree.TakeBack(&l_node);

    if(took)
    {
        assert(l_node!= NULL);
        m_goban.SetFromCompactBoard(l_node->GetCompactBoard());

        if(l_node->GetMove() > 0)
            m_currentPlayer = WHITE;
        else
            m_currentPlayer = BLACK;
    }
}
//===============================================================================================
void GoGame::AddInfluenceAnalysis(I_InfluenceModel* influence_model)
{
    m_influences_analysis.insert(influence_model);
}
//===============================================================================================
void GoGame::RegisterObserver(I_GobanObserver* observer)
{
    m_goban.RegisterObserver(observer);
}
//===============================================================================================

GoGameTree& GoGame::GetGameTree()
{
    return m_game_tree;
}
//===============================================================================================
GoGameInfo& GoGame::GetGameInfo()
{
    return m_game_info;
}
//===============================================================================================
I_InfluenceModel& GoGame::GetActiveInfluenceModel()
{
    std::unordered_set<I_InfluenceModel*>::iterator it = m_influences_analysis.begin();
    return *(*it);
}
//===============================================================================================
void GoGame::SetFromCompactBoard(CompactBoard* cb)
{

    m_goban.SetFromCompactBoard(cb);
    m_game_tree = GoGameTree();
    m_game_info = GoGameInfo();
}
//===============================================================================================
std::vector<short> GoGame::GenerateAllLegalMoves()
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
//=================================================================================================
