#include "UCTPolicy.h"
#include <math.h>

UCTPolicy::UCTPolicy()
{
}


MCNode* UCTPolicy::SelectChild(MCNode* node, unsigned int parent_visits)
{

    double constant = 0.7f;
    const std::vector<MCNode*>& children_vec = node->GetChildren();
    std::vector<MCNode*>::const_iterator it;
    MCNode* selected = nullptr;
    MCNode* aux_node = nullptr;
    double logpos = log2(parent_visits+2);
    double result = 0;
    double max_result = 0;

    //Setting First Play Urgency value
    /**
     * At the time that each node is created it must be simulated at least once
     * so we enforce a large value to each node and make sure that the search will no
     * be stuck at just one branch of the tree.
    */
    double fpu = 10000;


    //UCT_VALUE: constant * sqrt(  logpos / ( 5 * aux_node->GetCount() ) )
    //UCT formula: Win_rate + UCT_VALUE
    for(it = children_vec.begin(); it != children_vec.end(); ++it)
    {
        aux_node = *it;

        if(aux_node->GetCount() == 1)
            result = fpu;
        else
            result = (aux_node->GetValue()/aux_node->GetCount()) +
                ( constant * sqrt( logpos / ( 5 * (aux_node->GetCount()) ) ) );

        if(result > max_result)
        {
            max_result = result;
            selected = aux_node;
        }
    }

    return selected;
}
