#include "GoGame.h"
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
    PlayMove(l_map_pos);
}
//===============================================================================================
void GoGame::PlayMove(sf::Vector2i& l_mapPos)
{
    int board_size = m_goban.GetGoban().size();

    CompactBoard* l_previous_state = 0;
    CompactBoard* l_next_state = 0;
    CompactBoard* l_current_state = 0;

    GoGameNode* l_node = 0;

    if(l_mapPos.x >= 0 && l_mapPos.y >= 0 && l_mapPos.x < board_size && l_mapPos.y < board_size)
    {
        if(m_goReferee_sptr->IsLegal(m_currentPlayer,l_mapPos, m_goban))
        {
            l_node = m_game_tree.GetCurrentNode()->GetParent();

            if(l_node)
            {
                l_previous_state = l_node->GetCompactBoard();
            }

            l_current_state = m_goban.GetCompactBoard();


            if(m_goban.PutStone(l_mapPos, m_currentPlayer))
            {
                l_next_state = m_goban.GetCompactBoard();

                //Verify KO
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
                    //short move = (((l_mapPos.x+1)* (MAX_BOARD+1)) + (l_mapPos.y+1)) * m_currentPlayer;

                    short move = (((l_mapPos.x)* (MAX_BOARD)) + (l_mapPos.y)) * m_currentPlayer;

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

CompactBoard* GoGame::cb_static = NULL;
