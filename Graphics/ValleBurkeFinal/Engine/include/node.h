#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include <deque>

#include "transform.h"
#include "drawable.h"
#include "animator.h"

using namespace std;

class Node
{
public:
    // Constructors
    Node();
    Node(Transform t);

    // Destructor
    ~Node();

    // Adds a child to this Node
    inline void addChild(Node* node)
        { children.push_back(node); }

    // Adds a child to this node, whose Transform is controlled by an Animator
    inline void addChild(Node* node, Animator* anim)
        {
            children.push_back(node);
            animators.push_back(anim);
        }

    // Adds a Drawable to this Node
    inline void addDrawable(Drawable* draw)
        { drawables.push_back(draw); }

    // Gets a reference to the Transform of this Node
    inline Transform& getTransform()
        { return transform; }

    // Will call draw for every attached Drawable, then draw for every attached Node
    void draw(glm::mat4 modelViewMat);

    // Will call animate for every Animator, then animate for every attached Node
    void animate();

protected:
    Transform transform; // The Transform of this Node

    // Children
    deque<Node*> children;
    deque<Drawable*> drawables;
    deque<Animator*> animators;
};

#endif // NODE_H_INCLUDED
