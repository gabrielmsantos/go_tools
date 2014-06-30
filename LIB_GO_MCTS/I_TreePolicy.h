#ifndef I_TREEPOLICY_H
#define I_TREEPOLICY_H

#include "MCTree.h"

/**
 * Interface fot the Tree Policy used to Select a Child at each step of the selection
 * phase
*/
class I_TreePolicy
{
public:
    virtual ~I_TreePolicy(){}
    virtual MCNode* SelectChild(MCNode* node, unsigned int parent_visits) = 0;
};

#endif // I_TREEPOLICY_H

