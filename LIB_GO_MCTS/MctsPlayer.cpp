#include "MctsPlayer.h"
#include "UCTPolicy.h"

MctsPlayer::MctsPlayer(int board_size, boost::shared_ptr<GoReferee> l_referee)
    :m_go_game(board_size, l_referee),
      m_playout_searcher(board_size, l_referee)
{
    m_minimum_simulations = 1000;
    m_tree_policy = new UCTPolicy();


    for(int i = 0; i < 200; ++i)
    {
        fixed_moves.push_back(i);
    }
}

//===================================================
void MctsPlayer::StartSearchFor(CompactBoard* cb, StoneState to_play)
{
    unsigned int count_episodes = 0;
    double result = 0;

    //Set up Go Game
//    m_go_game.SetFromCompactBoard(cb);

    //SetCurrentPlayer
//    m_go_game.SetCurrentPlayer(to_play);

//    delete cb;

    std::vector<short> move_sequence;

    //Starts a new tree for every search process.
    m_tree.Clear();

    //Start episodes of simulation
    while( count_episodes < m_minimum_simulations || (false)) //TODO:Insert time Limit
    {

        std::cout << "Episode " << count_episodes << std::endl;

        //Reseting Game
        //Set up Go Game
        m_go_game.SetFromCompactBoard(cb);
        //SetCurrentPlayer
        m_go_game.SetCurrentPlayer(to_play);

        //Start SELECT + EXPAND PHASE
        PlayInTree(count_episodes, move_sequence);  //O(BOARD_SIZE ^ 2)

        //Start PLAY-OUT PHASE (Light Play-out)
        //result = m_playout_searcher.PlayOutGameFrom(m_go_game.GetMainBoard().GetCompactBoard(), m_go_game.GetCurrentPlayer());
        result = 1;

        //Update the choosen path in the tree, coming from the current node till the root node.
        UpdateSequenceInTree(result);

        //m_go_game.TakeBack(move_sequence.size());

        move_sequence.clear();

        count_episodes++;
    }
}

//===================================================
unsigned int MctsPlayer::ExpandTree()
{
//    std::vector<short> legal_moves;
    //m_go_game.GenerateAllLegalMoves(legal_moves); //O(BOARD_SIZE ^ 2)
    std::vector<short>::iterator it;

    MCNode* node;
    //for(it = legal_moves.begin(); it != legal_moves.end(); ++it) //O(BOARD_SIZE ^ 2) It can be optimized if I create the node into GenerateLegalMoves
    for(it = fixed_moves.begin(); it != fixed_moves.end(); ++it)
    {
        node = new MCNode(*it);
        /**
         * It will add the new nodes as children of the current_node in the tree
        */
        m_tree.AddNode(node);
    }

    //return legal_moves.size();
    return fixed_moves.size();
}

//===================================================
void MctsPlayer::PlayInTree(unsigned int count_sim, std::vector<short> &move_sequence)
{
    unsigned int expand_threshold = 1;
    unsigned int num_children;
    bool stop_sim;
    MCNode* current_node;
    MCNode* parent_node;

    MCNode* selected_node;

    //Always start from root
    m_tree.SetCurrentNode(m_tree.GetRoot()); //O(1)

    //Start to play in tree and stop when you Expand a node
    while(true)
    {
        current_node = m_tree.GetCurrentNode();  //O(1)
        parent_node = current_node->GetParent(); //O(1)

        //IF it was not expanded before neither it is final.
        if( !(current_node->HasChildren() || current_node->IsFinal() ) ) //O(1)
        {
            //
            /**
             * IF the node was not expanded before and it is not a final node the expand it.
             * See if it is the second time that this node is choosen.
             *
             * ELSE IF count =< than expand_threshold, do not expand it and pass it to the play-out phase.
             * Remember that the count starts at 1.
             *
            */
            if((current_node->GetCount() > expand_threshold) || ( parent_node == NULL)) //O(1)
            {
                //Expand the tree
                num_children = ExpandTree(); //O(BOARD_SIZE ^ 2)

                //If I can't expand it, it is final position.
                if(num_children == 0)   //O(1)
                {
                    current_node->SetFinal();  //O(1)
                    break;
                }

                //set break: After select a child pass it to the play-out phase.
                stop_sim = true; //O(1)

            }else
            {
                break;
            }
        }

        //Select a child from the current node according to the current Tree Policy.
        unsigned int parent_visits;
        if(parent_node != NULL)
        {
            parent_visits = parent_node->GetCount();
        }else
        {
            parent_visits = count_sim;
        }                                           //O(1)

        selected_node = m_tree_policy->SelectChild(current_node, parent_visits); //O(BOARD_SIZE ^  2)

        //Execute the selected move
        if(true || m_go_game.PlayMove(selected_node->GetMove()))
        //if(true)//O(1)
        {
            //Set Current Node
            m_tree.SetCurrentNode(selected_node);

            //Update path
            move_sequence.push_back(selected_node->GetMove());            
        }else
        {
            std::cout << "SHOULD NOT HAPPEN: COULD NOT PLAY THE MOVE " << std::endl;
        }

        //If Stop_SIM or is it final? Then return
        if((stop_sim) || selected_node->IsFinal())
            break;

    }
}

//===================================================
void MctsPlayer::UpdateSequenceInTree(double result)
{
    //From current node till the root
    MCNode* current_node;

    //If it is the root stop it
    while((current_node = m_tree.GetCurrentNode()) && current_node->GetParent() != nullptr)
    {
        //Update value
        current_node->AddValue(result);

        //Update count (Increment 1)
        current_node->AddCount(1);

        //Set the parent as the current_node!
        m_tree.SetCurrentNode(current_node->GetParent());
    }
}

//===================================================
std::vector<short> MctsPlayer::GetBestMovesFound()
{
    std::vector<MCNode*>& possible_moves = m_tree.GetRoot()->GetChildren();

    //Sort according to the most visited moves.
    std::sort(possible_moves.begin(), possible_moves.end(), compByCount);

    std::vector<short> best_moves;

    for(int i = 0; i< possible_moves.size(); ++i)
    {
        best_moves.push_back(possible_moves[i]->GetMove());
    }

    return best_moves;
}
//===================================================
