#include "node.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

Node::Node()
{
}

Node::Node(Transform t) :
    transform(t)
{
}

Node::~Node()
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

void Node::draw(mat4 modelViewMat)
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

void Node::animate()
{
    // Call all animators attached to this node
    for (unsigned i = 0; i < animators.size(); ++i)
        animators.at(i)->animate();

    // Animate all child nodes
    for (unsigned i = 0; i < children.size(); ++i)
        children.at(i)->animate();
}
