#ifndef UCTPOLICY_H
#define UCTPOLICY_H

#include "I_TreePolicy.h"

class UCTPolicy: public I_TreePolicy
{
public:

    UCTPolicy();

    virtual MCNode* SelectChild(MCNode* node, unsigned int parent_visits);
};

#endif // UCTPOLICY_H
