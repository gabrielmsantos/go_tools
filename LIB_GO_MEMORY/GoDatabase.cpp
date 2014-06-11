#define BOOST_FILESYSTEM_VERSION 3
#define BOOST_FILESYSTEM_NO_DEPRECATED

#include "GoDatabase.h"
#include "GoUtils.h"
#include <boost/filesystem.hpp>
#include <vector>
#include <math.h>

namespace fs = ::boost::filesystem;

//=============================================================================================
// return the filenames of all files that have the specified extension
// in the specified directory and all subdirectories
void GetAllSgfFrom(const fs::path& root, const std::string& ext, std::vector<fs::path>& ret, bool recursive)
{
    if (!fs::exists(root)) return;

    if (fs::is_directory(root))
    {
        if(recursive)
        {
            fs::recursive_directory_iterator it(root);
            fs::recursive_directory_iterator endit;

            while(it != endit)
            {
                if (fs::is_regular_file(*it) and it->path().extension() == ext)
                {
                    ret.push_back(it->path().string());
                }
                ++it;
            }
        }else
        {
            fs::directory_iterator it(root);
            fs::directory_iterator endit;
            while(it != endit)
            {
                if (fs::is_regular_file(*it) and it->path().extension() == ext)
                {
                    ret.push_back(it->path().string());
                }
                ++it;
            }
        }
    }
}

//=============================================================================================
GoDatabase::GoDatabase()
{
}
//=============================================================================================
GoDatabase::~GoDatabase()
{
    DisposeData();
}
//=============================================================================================
void GoDatabase::InsertGamesFrom(std::string directory, bool recursive)
{

    std::vector<fs::path> sgf_collection;
    std::ifstream in_file;

    boost::filesystem::path l_path(directory);
    std::string extension = ".sgf";
    GetAllSgfFrom(l_path,extension,sgf_collection, recursive);

    std::vector<fs::path>::iterator it = sgf_collection.begin();
    boost::shared_ptr<GoReferee> l_referee = boost::shared_ptr<GoReferee>(new GoReferee());

    for(; it != sgf_collection.end(); ++it)
    {
        std::cout <<  (*it).string() << std::endl;
        in_file.open((*it).string());

        //TODO: SOLVE THE PROBLEM of BOARD SIZE
        GoGame* l_go_game = new GoGame(19, l_referee);
        m_parser.ParseGame(in_file, l_go_game);

        GoGameTree* l_tree_ptr = new GoGameTree(l_go_game->GetGameTree());
        GoGameInfo* l_game_info_ptr =  new GoGameInfo(l_go_game->GetGameInfo());
        //GoGameInfo* l_game_info_ptr =  0;

        InfoTree* l_info_tree_ptr = new InfoTree(l_game_info_ptr, l_tree_ptr);

        CreateIndexesFor(l_info_tree_ptr, l_tree_ptr->GetRoot());

        delete l_go_game;
        in_file.close();
    }
}
//=============================================================================================
void GoDatabase::CreateIndexesFor(InfoTree* l_info_tree_ptr, GoGameNode* l_node_ptr)
{
    unsigned short num_stones = l_node_ptr->GetCompactBoard()->GetNumberOfStones();
    m_game_index_container.insert(game_index_entry(num_stones, l_info_tree_ptr, l_node_ptr));

    const node_container_t& l_children = l_node_ptr->GetChildrenContainer();
    const random_access_index& l_queue_container = l_children.get<0>();
    random_access_index::iterator it = l_queue_container.begin();

    for(; it != l_queue_container.end(); ++it)
    {
        const node_entry& l_node = *it;
        CreateIndexesFor(l_info_tree_ptr, l_node.m_node_ptr);
    }
}
//=============================================================================================
void GoDatabase::DisposeData()
{

    number_of_stones_index& l_search_states = m_game_index_container.get<number_of_stones_t>();

    //Get all states containing number_of_stones = 0 (Root of each tree)
    /** Is it correct to suppose that every SGF starts empty ? Even the ones which have AB and AW (add stones) should start empty.*/
    std::pair<number_of_stones_index::iterator,number_of_stones_index::iterator>  ret = l_search_states.equal_range(0);

    number_of_stones_index::iterator it0;

    std::cout <<  "Disposing Data at GoDatabase" << std::endl;
    int i = 0;
    for(it0 = ret.first; it0 != ret.second; ++it0)
    {
        game_index_entry& l_entry = const_cast<game_index_entry&>(*it0);
        delete l_entry.m_info_tree_ptr;
        l_entry.m_info_tree_ptr = NULL;
        ++i;
    }
    std::cout <<  "Deleted " << i << " entries" << std::endl;

    /** Clearing container */
    m_game_index_container.clear();
}
//=============================================================================================
std::vector< DB_MatchState> GoDatabase::SearchFullBoardState(CompactBoard* cb_state)
{
    //Get number of stones from cb_state.
    //The search process will use this number as index
    unsigned short num_of_stones = cb_state->GetNumberOfStones();

    //Resulting vector
    std::vector< DB_MatchState> result_states;

    //Performing the SEARCH
    /** Now, it will search, in the entire database, for all states containing
     * exactly "num_of_stones" stones on the board.*/
    number_of_stones_index& l_search_states = m_game_index_container.get<number_of_stones_t>();

    //Get all states containing number_of_stones
    std::pair<number_of_stones_index::iterator,number_of_stones_index::iterator> ret = l_search_states.equal_range(num_of_stones);

    number_of_stones_index::iterator it0;

    /** Getting all variations to search for.(It will be deleted in the end of the search)*/
    std::vector<CompactBoard* > variations_to_search = cb_state->GetAllVariations();

    DB_MatchState match_state;
    CompactBoard* cb_to_compare;
    CompactBoard* l_entry_cb;

    /*Iterate trhough the range of states that matched the search*/
    for(it0 = ret.first; it0 != ret.second; ++it0)
    {
        game_index_entry& l_entry = const_cast<game_index_entry&>(*it0);

        //do not delete it
        l_entry_cb = l_entry.m_game_node_ptr->GetCompactBoard();

        //Comparing each variation
        for(unsigned int i = 0; i < variations_to_search.size(); ++i)
        {
            cb_to_compare = variations_to_search[i];

            if(l_entry_cb->XOR(cb_to_compare) == 0 )
            {
                match_state.m_tree = l_entry.m_info_tree_ptr;
                match_state.m_node = l_entry.m_game_node_ptr;
                match_state.m_inversed = cb_to_compare->IsInversed();
                match_state.m_rotations = cb_to_compare->GetRotation();
                match_state.m_mirrored = cb_to_compare->IsMirrored();

                result_states.push_back(match_state);
            }
        }
    }

    //Deleting variations
    for(unsigned int i = 0; i < variations_to_search.size(); ++i)
    {
        delete variations_to_search[i];
    }

    return result_states;
}
//=============================================================================================
std::vector<short> GoDatabase::NextMoveAdvisor(CompactBoard* cb_state)
{

    //result moves
    std::vector<short> result_moves;

    /** Getting all states that fully match cb_state */
    std::vector< DB_MatchState> result_states = SearchFullBoardState(cb_state);

    std::vector< DB_MatchState>::iterator it = result_states.begin();

    short move;

    unsigned int num_anticlock_rotations;

    for(; it!= result_states.end(); ++it)
    {
        DB_MatchState& match_state = *it;

        const node_container_t& l_children = match_state.m_node->GetChildrenContainer();

        const random_access_index& l_queue_container = l_children.get<0>();

        random_access_index::iterator it_child = l_queue_container.begin();

        for(; it_child != l_queue_container.end(); ++it_child)
        {
            const node_entry& l_node = *it_child;

            move = l_node.m_move;

            if(match_state.m_mirrored)
            {
                move = GoUtils::GetMirrorForMove(move);
            }

            if(match_state.m_rotations != 0)
            {
                num_anticlock_rotations = 4 - match_state.m_rotations;
            }
            else
            {
                num_anticlock_rotations = 0;
            }

            //@TODO: Create Anticlockwise: Seriously, do it.
            for(unsigned int i = 0 ; i < num_anticlock_rotations; ++i)
            {
                move = GoUtils::GetClockwiseRotationForMove(move);
            }

            if(match_state.m_inversed)
            {
                move*=-1;
            }

            result_moves.push_back(move);
        }
    }

    return result_moves;
}
//=============================================================================================
unsigned int GoDatabase::GetNumberOfNodes() const
{
    return m_game_index_container.size();
}
