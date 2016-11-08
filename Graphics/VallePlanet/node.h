#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include <deque>

#include <glm/gtc/type_ptr.hpp>

#include "transform.h"
#include "drawable.h"
#include "animator.h"

using namespace std;

class Node
{
public:
    Node()
    {
    }

    Node(Transform t) :
        transform(t)
    {
    }

    ~Node()
    {
        // Delete all drawables attached to this node
        for (unsigned i = 0; i < drawables.size(); ++i)
            delete drawables.at(i);
        drawables.clear();

        // Delete all child nodes
        for (unsigned i = 0; i < children.size(); ++i)
            delete children.at(i);
        children.clear();
    }

    inline void addChild(Node* node)
        { children.push_back(node); }

    inline void addChild(Node* node, Animator* anim)
        {
            children.push_back(node);
            animators.push_back(anim);
        }

    inline void addDrawable(Drawable* draw)
        { drawables.push_back(draw); }

    void draw(mat4 modelViewMat)
    {
        // Create base model view matrix
        modelViewMat = translate(modelViewMat, transform.getTranslate());
        modelViewMat = rotate(modelViewMat, transform.getXAngle(), vec3(1., 0., 0.));
        modelViewMat = rotate(modelViewMat, transform.getYAngle(), vec3(0., 1., 0.));
        modelViewMat = rotate(modelViewMat, transform.getZAngle(), vec3(0., 0., 1.));
        modelViewMat = scale(modelViewMat, transform.getScale());

        // Draw all drawables attached to this node
        for (unsigned i = 0; i < drawables.size(); ++i)
            drawables.at(i)->draw(modelViewMat);

        // Draw all child nodes
        for (unsigned i = 0; i < children.size(); ++i)
            children.at(i)->draw(modelViewMat);
    }

    void animate()
    {
        // Call all animators attached to this node
        for (unsigned i = 0; i < animators.size(); ++i)
            animators.at(i)->animate();

        // Animate all child nodes
        for (unsigned i = 0; i < children.size(); ++i)
            children.at(i)->animate();
    }

    inline Transform& getTransform()
        { return transform; }

protected:
    deque<Node*> children;
    Transform transform;

    deque<Drawable*> drawables;

    deque<Animator*> animators;
};

#endif // NODE_H_INCLUDED
