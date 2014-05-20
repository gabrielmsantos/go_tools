#include "Goban.h"
#include "GoGameTree.h"

//===================================================================================
GoGameNode::~GoGameNode()
{    
    random_access_index& l_queue_container = m_children_container.get<0>();

    /** Deleting Sub-trees */
    while(!l_queue_container.empty())
    {
        node_entry& l_node = const_cast<node_entry&>(l_queue_container.front());
        /** It will be popped later (Because of RemoveChildFromContainer)*/
        delete l_node.m_node_ptr;
        l_node.m_node_ptr = 0;

        l_queue_container = m_children_container.get<0>();
    }

    //Delete compact_board from memory
    delete m_compact_board;
    m_compact_board = 0;

    //Remove my entry from my parent
    if(m_parent)
    {
        m_parent->RemoveChildFromContainer(m_move);
    }

}
//===================================================================================
bool GoGameNode::HasChildren() const
{
    const random_access_index& l_queue_container = m_children_container.get<0>();

    return !(l_queue_container.empty());
}

//===================================================================================
void GoGameNode::RemoveChildFromContainer(short move)
{
    hashed_code_index& l_hash_container =  m_children_container.get<move_code_t>();
    hashed_code_index::iterator it = l_hash_container.find(move);

    if( it != l_hash_container.end() )
    {
        l_hash_container.erase(it);

    }else
    {
        std::cout << "COULD NOT FIND MOVE: " << move << " IN CHILDREN OF: "<< m_move << std::endl;
    }

}
//===================================================================================
GoGameNode* GoGameNode::GetChild(short move) const
{
    const hashed_code_index& l_hash_container =  m_children_container.get<move_code_t>();
    hashed_code_index::iterator it = l_hash_container.find(move);

    if( it != l_hash_container.end() )
    {
        const node_entry& l_node = *it;
        return l_node.m_node_ptr;
    }

    return NULL;
}
//===================================================================================
GoGameNode& GoGameNode::operator=(GoGameNode& node_copy)
{
    delete m_compact_board;
    m_compact_board = new CompactBoard(*(node_copy.m_compact_board));
    m_move = node_copy.m_move;

    random_access_index& l_queue_container = node_copy.m_children_container.get<0>();
    random_access_index::iterator it = l_queue_container.begin();

    //Foreach child, copy it
    for(; it != l_queue_container.end(); ++it)
    {
        const node_entry& l_node = *it;
        GoGameNode* l_game_node = new GoGameNode(*l_node.m_node_ptr);

        InsertChild(l_game_node);
    }

    return *this;

}
//===================================================================================
GoGameNode::GoGameNode(const GoGameNode& node_copy)
{
    m_compact_board = new CompactBoard(*(node_copy.m_compact_board));
    m_move = node_copy.m_move;

    const random_access_index& l_queue_container = node_copy.m_children_container.get<0>();
    random_access_index::iterator it = l_queue_container.begin();

    //Foreach child, copy it
    for(; it != l_queue_container.end(); ++it)
    {
        const node_entry& l_node = *it;
        GoGameNode* l_game_node = new GoGameNode(*l_node.m_node_ptr);

        InsertChild(l_game_node);
    }

}
//===================================================================================
GoGameNode* GoGameNode::GetNodeInSubTree(short move, CompactBoard* l_compact_board) const
{
    if((m_move ==  move) && (l_compact_board->XOR(m_compact_board) == 0))
    {
        return const_cast<GoGameNode*>(this);
    }else
    {
        const random_access_index& l_queue_container = m_children_container.get<0>();
        random_access_index::iterator it = l_queue_container.begin();

        for(; it != l_queue_container.end(); ++it)
        {
            const node_entry& l_node = *it;
            return l_node.m_node_ptr->GetNodeInSubTree(move, l_compact_board);
        }

        return NULL;
    }
}

//===================================================================================
void GoGameNode::PrintSubTree() const
{

    std::cout << m_move << " ";
    const random_access_index& l_queue_container = m_children_container.get<0>();
    random_access_index::iterator it = l_queue_container.begin();

    for(; it != l_queue_container.end(); ++it)
    {
        const node_entry& l_node = *it;
        l_node.m_node_ptr->PrintSubTree();
    }

}
//===================================================================================
const node_container_t& GoGameNode::GetChildrenContainer() const
{
    return m_children_container;
}

//===================================================================================
//              GoGameTree Class
//===================================================================================
GoGameTree::GoGameTree()
    :m_root(NULL),
      m_current_node(m_root)
{

}
//===================================================================================
void GoGameTree::InsertNewNode(GoGameNode *child)
{
    if(m_root == NULL)
    {
        m_root = child;
        m_root->SetParent(NULL);
        m_current_node = m_root;
        return;
    }

    assert( m_current_node!= NULL);
    GoGameNode* l_node;

    l_node = m_current_node->GetChild(child->GetMove());

    if( l_node != NULL)
    {
        m_current_node = l_node;
        delete child;
    }else
    {
        m_current_node->InsertChild(child);
        m_current_node = child;
    }
}
//===================================================================================
 bool GoGameTree::TakeBack(GoGameNode** node)
{
    if(m_current_node != m_root)
    {
        m_current_node = m_current_node->GetParent();
        *node = m_current_node;
        return true;
    }

    *node = m_current_node;
    return false;
} 
//===================================================================================
 void GoGameTree::BackToNode(GoGameNode* node)
{
    m_current_node = node ;
}
//====================================================================================
GoGameTree& GoGameTree::operator=(const GoGameTree& tree_copy)
{
    //Delete previous data
    delete m_root;

    //This shall copy the Tree
    m_root = new GoGameNode(*(tree_copy.m_root));

    //Now the m_current must be duely positioned
    m_current_node = m_root->GetNodeInSubTree(tree_copy.m_current_node->GetMove(), tree_copy.m_current_node->GetCompactBoard());

    if(m_current_node == NULL)
    {
        m_current_node = m_root;
    }

    return *this;
}
//====================================================================================
GoGameTree::GoGameTree(const GoGameTree& tree_copy)
{
    //This shall copy the Tree
    m_root = new GoGameNode(*(tree_copy.m_root));
    m_root->SetParent(NULL);

    //Now the m_current must be duely positioned
    m_current_node = m_root->GetNodeInSubTree(tree_copy.m_current_node->GetMove(), tree_copy.m_current_node->GetCompactBoard());

    if(m_current_node == NULL)
    {
        m_current_node = m_root;
    }

//    //Just Testing
//    m_root->PrintSubTree();
//    std::cout << " Current: " <<m_current_node->GetMove() <<std::endl;
//    tree_copy.m_root->PrintSubTree();
//    std::cout << " Current: " <<tree_copy.m_current_node->GetMove() <<std::endl;
}
//====================================================================================
GoGameNode* GoGameTree::GetRoot()
{
    return m_root;
}
//====================================================================================
