#ifndef MCTREE_H
#define MCTREE_H
#include<vector>
#include <algorithm>

//===================================================
typedef struct move_info_t
{

short m_move;
unsigned int m_count;
double m_value;

} move_info;

//===================================================
class MCNode
{
public:

    explicit MCNode(short move, MCNode* parent = nullptr)
    {
        //Setting info
        m_info = move_info();
        m_info.m_move = move;

        //To do not mess with the UCT rule.
        m_info.m_count = 1;
        m_info.m_value = 0;

        m_is_final = false;

        //Set Parent
        m_parent = parent;
    }

    /**
     * Notice that the parent is not copied.
    */
    MCNode(const MCNode& node_copy)
    {
        m_parent = nullptr;

        m_info = node_copy.m_info;
        m_is_final = node_copy.m_is_final;


        MCNode* node;

        std::vector<MCNode*>::const_iterator it;
        for(it = node_copy.m_children.begin(); it != node_copy.m_children.end(); ++it)
        {
            node = new MCNode(*(*it));
            AddChild(node);
        }
    }

    /**
     * Notice that the parent is not copied.
    */
    MCNode& operator=(const MCNode& node_copy)
    {
        //Destroy previous data;
        ClearNode();

        m_parent = nullptr;
        m_info = node_copy.m_info;
        m_is_final = node_copy.m_is_final;

        MCNode* node;

        std::vector<MCNode*>::iterator it;
        for(it = m_children.begin(); it != m_children.end(); ++it)
        {
            node = new MCNode(*(*it));
            AddChild(node);
        }
    }

    void ClearNode()
    {
        // Deleting sub-tree
        std::vector<MCNode*>::iterator it = m_children.begin();
        while(( it != m_children.end() ) )
        {
            delete *it;
            it = m_children.begin();
        }

        //Remove it from parent
        if(m_parent != nullptr)
            m_parent->RemoveChild(this);
    }

    ~MCNode()
    {
        ClearNode();
    }

    inline void AddChild(MCNode* child)
    {
        child->m_parent = this;
        m_children.push_back(child);
    }

    inline void RemoveChild(MCNode* child)
    {
        m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end() );
    }

    inline std::vector<MCNode*>& GetChildren()
    {
        return m_children;
    }

    inline short GetMove() const
    {
        return m_info.m_move;
    }

    inline unsigned int GetCount() const
    {
        return m_info.m_count;
    }

    inline double GetMean() const
    {
        return m_info.m_value/m_info.m_count;
    }

    inline double GetValue() const
    {
        return m_info.m_value;
    }

    inline bool HasChildren() const
    {
        return (m_children.size() > 0);
    }

    inline void SetFinal()
    {
        m_is_final = true;
    }

    inline bool IsFinal() const
    {
        return m_is_final;
    }

    inline MCNode* GetParent() const
    {
        return m_parent;
    }

    inline void AddValue(double addition)
    {
        m_info.m_value += addition;
    }

    inline void AddCount(unsigned int addition)
    {
        m_info.m_count += addition;
    }

    friend class MCTree;
private:

MCNode* m_parent;
move_info m_info;
std::vector<MCNode*> m_children;
bool m_is_final;
};
//===================================================
/**
 * The MCTree is responsible for managing all nodes that will compose the Monte Carlo Tree Search
 * Notice that the root node is always 0. It does not contain the last move played.
*/
class MCTree
{
public:

    MCTree()
    {
        //Starting the root
        m_root = new MCNode(0, nullptr);

        //Setting up a large count value for the root, so it will be expanded right on.
        m_root->m_info.m_count = 10000;

        m_current_node = m_root;
    }

    ~MCTree()
    {
        delete m_root;
    }

    inline void SetCurrentNode(MCNode* node)
    {
        m_current_node = node;
    }

    inline MCNode* GetCurrentNode()
    {
        return m_current_node;
    }

    inline MCNode* GetRoot()
    {
        return m_root;
    }

    inline void AddNode(MCNode* node)
    {
        m_current_node->AddChild(node);
    }

    inline void Clear()
    {
        delete m_root;
        m_root = new MCNode(0, nullptr);
        m_current_node = m_root;
    }

    MCTree(MCTree& tree_copy)
    {
        m_root = new MCNode( *(tree_copy.GetRoot()));

        //Must set the current_node
        //Current Node set to the root
        m_current_node = m_root;
    }


    MCTree& operator=( MCTree& tree_copy)
    {
        //Delete previous data;
        delete m_root;

        m_root = new MCNode( *(tree_copy.GetRoot()));

        //Must set the current_node
        //Current Node set to the root
        m_current_node = m_root;
    }

private:

MCNode* m_root;
MCNode* m_current_node;
};
//===================================================
//Compare by Count
inline bool compByCount(MCNode* node_a, MCNode* node_b)
{
    return node_a->GetCount() < node_b->GetCount();
}

//===================================================
//Compare by Mean
inline bool compByMean(MCNode* node_a, MCNode* node_b)
{
    return  node_a->GetMean() < node_b->GetMean();
}


#endif // MCTREE_H
