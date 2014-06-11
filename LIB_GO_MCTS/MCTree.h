#ifndef MCTREE_H
#define MCTREE_H
#include<vector>

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

    MCNode() {}
    ~MCNode()
    {
        //Delete the sub-tree below
    }

    void AddChild(MCNode* child);

private:


MCNode* m_parent;
move_info m_info;
std::vector<MCNode*> m_children;

};
//===================================================
class MCTree
{
public:
    MCTree();

    ~MCTree()
    {
        delete m_root;
    }

//TODO: Create constructor Copy, operator =
private:

MCNode* m_root;
MCNode* m_current_node;
};
//===================================================

#endif // MCTREE_H
