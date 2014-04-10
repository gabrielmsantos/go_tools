#include "GoGame.h"
#include "GoInfluenceModel/CrossInfluence.h"

//===============================================================================================
GoGame::GoGame(int board_size):
    m_influenceMap(board_size)
{
    m_game_tree.InsertNewNode(new GoGameNode(0, m_influenceMap.GetCompactBoard()));
    m_currentPlayer = BLACK;
}
//===============================================================================================
void GoGame::PlayMove(sf::Vector2i& l_mapPos)
{
    int board_size = m_influenceMap.GetGoban().size();

    CompactBoard* l_previous_state = 0;
    CompactBoard* l_next_state = 0;
    CompactBoard* l_current_state = 0;

    GoGameNode* l_node = 0;

    if(l_mapPos.x >= 0 && l_mapPos.y >= 0 && l_mapPos.x < board_size && l_mapPos.y < board_size)
    {
        if(m_goReferee.IsLegal(m_currentPlayer,l_mapPos, m_influenceMap))
        {
            l_node = m_game_tree.GetCurrentNode()->GetParent();

            if(l_node)
            {
                l_previous_state = l_node->GetCompactBoard();
            }

            l_current_state = m_influenceMap.GetCompactBoard();


            if(m_influenceMap.PutStone(l_mapPos, m_currentPlayer))
            {
                l_next_state = m_influenceMap.GetCompactBoard();

                //Verify KO
                if( (l_previous_state) && ( l_next_state->XOR(l_previous_state) == 0) ) //KO
                {
                    m_influenceMap.SetFromCompactBoard(l_current_state);
                    delete l_next_state;
                    delete l_current_state;
                    return;

                }else
                {
                    m_currentPlayer = (StoneState) ((int) m_currentPlayer * (-1));

                    unsigned short move = ((l_mapPos.x+1)* (MAX_BOARD+1)) + (l_mapPos.y+1);

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
    return m_influenceMap;
}

//===============================================================================================
GoGame::~GoGame()
{
}
//===============================================================================================
void GoGame::TakeBack(bool invert_player)
{
    GoGameNode* l_node = 0;
    bool took = m_game_tree.TakeBack(&l_node);

    if(took)
    {
        assert(l_node!= NULL);
        m_influenceMap.SetFromCompactBoard(l_node->GetCompactBoard());

        if(invert_player)
        {
            m_currentPlayer = (StoneState) ((int) m_currentPlayer * (-1));
        }
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
    m_influenceMap.RegisterObserver(observer);
}
//===============================================================================================
I_InfluenceModel& GoGame::GetActiveInfluenceModel()
{
    std::unordered_set<I_InfluenceModel*>::iterator it = m_influences_analysis.begin();
    return *(*it);
}
