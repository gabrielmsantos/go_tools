#include "Goban.h"
#include "GoGameTree.h"

//===================================================================================
GoGameNode::~GoGameNode()
{
    random_access_index& l_queue_container = m_children_container.get<0>();

    /** Deleting Sub-trees */
    while(!l_queue_container.empty())
    {
        /** It will be popped from the child*/
        const node_entry& l_node = l_queue_container.front();
        delete l_node.m_node_ptr;

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
void GoGameNode::RemoveChildFromContainer(unsigned short move)
{
    hashed_code_index& l_hash_container =  m_children_container.get<move_code_t>();
    hashed_code_index::iterator it = l_hash_container.find(move);

    if( it != l_hash_container.end() )
    {
        l_hash_container.erase(it);
    }

}
//===================================================================================
GoGameNode* GoGameNode::GetChild(unsigned short move) const
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
