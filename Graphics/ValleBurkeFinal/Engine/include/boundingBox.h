#ifndef BOUNDINGBOX_H_INCLUDED
#define BOUNDINGBOX_H_INCLUDED

/**
 * A bounding box is attached to a node via a collider, and is used by the collider to
 *  check whether an object is inside the bounding box.
**/

class BoundingBox
{
public:
    BoundingBox(float width, float height, float depth);

    inline float getWidth()
        { return width; }

    inline float getHeight()
        { return height; }

    inline float getDepth()
        { return depth; }

private:
    friend class Collider;
    float width;
    float height;
    float depth;
};

#endif // BOUNDINGBOX_H_INCLUDED
