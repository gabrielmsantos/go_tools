#ifndef GOGAMETREE_H
#define GOGAMETREE_H


#include <boost/multi_index_container.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>

using boost::multi_index_container;
using namespace boost::multi_index;

struct CompactBoard;
class GoGameNode;

//=============== Node Entry struct ======================
typedef struct node_entry_t
{
    short m_move;
    //    boost::shared_ptr<GoGameNode > m_node_ptr;
    GoGameNode * m_node_ptr;

    node_entry_t(short move, GoGameNode* node):m_move(move), m_node_ptr(node){}

} node_entry;

//======================================================
/**
 * Declaring the multi_index_container regarding to the node structure
*/

/** Tag for accessing by move*/
struct move_code_t{};

typedef multi_index_container<
node_entry,
indexed_by<
random_access<>,
hashed_unique<tag <move_code_t>, member <node_entry, short, &node_entry::m_move > >
>
> node_container_t;

/** Creating an index to access through random_access (like Queue) */
typedef node_container_t::nth_index<0>::type random_access_index;

/** Creating an index to access by hashed move_code*/
typedef node_container_t::index<move_code_t>::type hashed_code_index;

//======================================================
/** Class GoGameNode
@description: The class contains :

m_move (represents the code of the action took)
m_parent (representing its parent node)
m_children_container (representing the container for my children)
m_compact_board (representing the board state for this node)
*/

class GoGameNode
{
public:

    explicit GoGameNode(short move, CompactBoard* compact_board)
        :m_move(move),
          m_parent(NULL),
          m_compact_board(compact_board)
    {
    }

    void ClearNode();

    GoGameNode(const GoGameNode& node_copy);

    ~GoGameNode();

    bool HasChildren() const;

    GoGameNode* GetChild(short move) const;

    inline GoGameNode* FirstChild() const
    {
        const random_access_index& l_queue_container = m_children_container.get<0>();
        const node_entry& l_node = l_queue_container.front();
        return l_node.m_node_ptr;
    }

    inline void SetParent(GoGameNode* parent)
    {
        m_parent = parent;
    }

    inline void InsertChild(GoGameNode* child)
    {
        child->SetParent(this);
        random_access_index& l_queue_container = m_children_container.get<0>();
        l_queue_container.push_back(node_entry(child->GetMove(),child));
    }

    inline CompactBoard* GetCompactBoard() const
    {
        return m_compact_board;
    }

    inline short GetMove() const
    {
        return m_move;
    }

    inline GoGameNode* GetParent() const
    {
        return m_parent;
    }

    GoGameNode& operator=(GoGameNode& node_copy);

    GoGameNode* GetNodeInSubTree(short move, CompactBoard* l_compact_board) const;

    const node_container_t& GetChildrenContainer() const;

    void PrintSubTree() const;

private:

    void RemoveChildFromContainer(short move);

    /** The move that created the state represented by compact board
      If the move is negative then it was made by the white player, otherwise it the black player
    */
    short m_move;
    GoGameNode* m_parent;
    node_container_t m_children_container;
    CompactBoard* m_compact_board;
};

//======================================================
class GoGameTree
{
public:

    GoGameTree();
    GoGameTree(const GoGameTree& tree_copy);

    ~GoGameTree()
    {
        delete m_root;
        m_root = NULL;
        m_current_node = NULL;
    }

    GoGameTree& operator=(const GoGameTree& tree_copy);

    inline GoGameNode* GetCurrentNode() const
    {
        return m_current_node;
    }

    void InsertNewNode(GoGameNode* child);

    bool TakeBack(GoGameNode **node);

    bool TakeBack(GoGameNode **node, unsigned short number);

    void BackToNode(GoGameNode *node);

    GoGameNode* GetRoot();

private:
    GoGameNode* m_root;
    GoGameNode* m_current_node;

};
//=====================================
#endif // GOGAMETREE_H
