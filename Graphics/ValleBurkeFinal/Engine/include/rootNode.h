#ifndef ROOTNODE_H_INCLUDED
#define ROOTNODE_H_INCLUDED

#include "Node.h"

class RootNode : public Node
{
public:
    ~RootNode()
    {
    }

    static RootNode *getInstance()
    {
        if (!instance)
          instance = new RootNode;
        return instance;
    }

private:
    static RootNode *instance;

    RootNode()
    {}
};

RootNode* RootNode::instance;

#endif // ROOTNODE_H_INCLUDED
