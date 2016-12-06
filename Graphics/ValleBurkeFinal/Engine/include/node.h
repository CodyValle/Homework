#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include <deque>
#include <iostream>

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
    virtual ~Node();

    // Adds a child to this Node
    inline void addChild(Node* node)
        {
            node->setParent(this);
            children.push_back(node);
        }

    // Adds a child to this node, whose Transform is controlled by an Animator
    inline void addChild(Node* node, Animator* anim)
        {
            node->setParent(this);
            node->setAnimator(anim);
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
    virtual void draw(glm::mat4 modelViewMat);

    // Will call animate for every Animator, then animate for every attached Node
    void animate(float deltaTime);

    // Removes the node from the scene
    void destroy()
    {
        parent->remove(this);
    }

    // Removes a node from this node
    void remove(Node* node)
    {
        for (std::deque<Node*>::iterator it = children.begin(); it != children.end(); ++it)
            if (*it == node)
            {
                children.erase(it);
                break;
            }

        Animator* animator = node->getAnimator();
        if (animator)
            for (std::deque<Animator*>::iterator it = animators.begin(); it != animators.end(); ++it)
                if (*it == animator)
                {
                    animators.erase(it);
                    break;
                }
    }

    // Enabled getter/setter
    inline bool getEnabled()
        { return enabled; }

    inline void setEnabled(bool e)
        { enabled = e; }

protected:
    inline void setParent(Node* parent)
        { this->parent = parent; }

    inline void setAnimator(Animator* anim)
        { this->anim = anim; }

    inline Animator* getAnimator()
        { return anim; }

    Transform transform; // The Transform of this Node
    Node* parent;
    Animator* anim;

    // Children
    deque<Node*> children;
    deque<Drawable*> drawables;
    deque<Animator*> animators;

    // Node is active or not
    bool enabled;
};

#endif // NODE_H_INCLUDED
