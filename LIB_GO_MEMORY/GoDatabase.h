#ifndef GODATABASE_H
#define GODATABASE_H

#include "GoGame.h"
#include "SGFParser.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>

using boost::multi_index_container;
using namespace boost::multi_index;

//===============================================================
class InfoTree
{
public:
    explicit InfoTree(GoGameInfo* info, GoGameTree* tree):
        m_game_info(info),
        m_game_tree(tree)
    {

    }

    explicit InfoTree(InfoTree& tree_copy)
    {
        m_game_info = new GoGameInfo(tree_copy.GetGameInfo());
        m_game_tree = new GoGameTree(tree_copy.GetGameTree());
    }

    InfoTree& operator=(InfoTree& tree_copy)
    {
        /**Delete previous data*/
        delete m_game_info;
        delete m_game_tree;

        m_game_info = new GoGameInfo(tree_copy.GetGameInfo());
        m_game_tree = new GoGameTree(tree_copy.GetGameTree());

        return *this;
    }

    ~InfoTree()
    {
        delete m_game_info;
        delete m_game_tree;
    }

    GoGameInfo& GetGameInfo()
    {
        return *m_game_info;
    }

    GoGameTree& GetGameTree()
    {
        return *m_game_tree;
    }

private:
    GoGameInfo* m_game_info;
    GoGameTree* m_game_tree;

};
//===============================================================
/**Creating the structs in order to improve the search*/

/**
    The game index struct will enable games be searched by:
        -(Non-Unique) Number of stones hash: This will enable to search for states with a specific number of stones
        -(Non-Unique) InfoTree hash (It will list all entries for a specific game)
        -(Ordered Unique) InfoTree hash (It will list all games)

*/
//-----------------------------------------------
typedef struct  _game_index
{
    unsigned short m_number_of_stones; //The number of stones at a particular state
    InfoTree* m_info_tree_ptr;
    GoGameNode* m_game_node_ptr;

    _game_index(unsigned short num_stones, InfoTree* tree, GoGameNode* node)
        :m_number_of_stones(num_stones),
        m_info_tree_ptr(tree),
        m_game_node_ptr(node)
    {}


}game_index_entry;
//-----------------------------------------------

/** Tags for ...*/
struct number_of_stones_t{}; //accessing by number of stones in the board
struct non_unique_info_tree_ptr_t{}; //accessing by info_tree pointer
struct ordered_unique_info_tree_ptr_t{}; //accessing by info_tree pointer


typedef multi_index_container<
game_index_entry,

indexed_by<
hashed_non_unique< tag <number_of_stones_t>, member<game_index_entry, unsigned short, &game_index_entry::m_number_of_stones > >,
hashed_non_unique< tag <non_unique_info_tree_ptr_t>,member<game_index_entry, InfoTree*, &game_index_entry::m_info_tree_ptr > >,
ordered_non_unique< tag <ordered_unique_info_tree_ptr_t>, member<game_index_entry, InfoTree*, &game_index_entry::m_info_tree_ptr > >
>
> game_index_container;

/**Creating indexes to access through... */

/** Number of stones: It will return all entries with a specific number of stones on the board*/
typedef game_index_container::index<number_of_stones_t>::type number_of_stones_index;
//typedef game_index_container::index<number_of_stones_t>::type number_of_stones_index;

///** NON-UNIQUE: Hashed game: It will return all entries with that pointer*/
typedef game_index_container::nth_index<1>::type all_entries_ptr_index;

////@TODO: Change this later to access ordered through Game Date uniquely
///** UNIQUE: Ordered game: It will enable to access the games ordered by ptr*/
///Since it is ordered by pointer (the pointer acting as an Id) the order
/// of return will be completly random
typedef game_index_container::nth_index<2>::type all_entries_ptr_distinct_index;

//===============================================================
/** Struct returned by a search.
 *It contains:
 * - A pointer to the gameTree
 * - A pointer to the gameNode(Belonging to the tree) which matchs the result.
 * - A char indicating how many clockwise rotations were performed to the original state:
 * - A bool indicating if the state is color inversed.
 */
typedef struct _match_state
{
    InfoTree* m_tree;
    GoGameNode* m_node;
    unsigned char m_rotations;
    bool m_inversed;
    bool m_mirrored;

}DB_MatchState;
//===============================================================
class GoDatabase
{
public:
    ~ GoDatabase();
    GoDatabase();
    void InsertGamesFrom(std::string directory, bool recursive);
    std::vector< DB_MatchState> SearchFullBoardState(CompactBoard* cb_state);
    std::vector<short> NextMoveAdvisor(CompactBoard* cb_state);

private:
    void CreateIndexesFor(InfoTree* l_info_tree_ptr, GoGameNode* l_node_ptr);

    game_index_container m_game_index_container;
    SGFParser m_parser;
    void DisposeData();
};

#endif // GODATABASE_H
