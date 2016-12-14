#ifndef ROOTNODE_H_INCLUDED
#define ROOTNODE_H_INCLUDED

/**
 * RootNode is a special node that is the entry point for the entire scene.
**/

#include "Node.h"

class RootNode : public Node
{
public:
    ~RootNode();

    static RootNode *getInstance()
    {
        if (!instance)
          instance = new RootNode;
        return instance;
    }

private:
    static RootNode *instance;

    RootNode();
};

#endif // ROOTNODE_H_INCLUDED
