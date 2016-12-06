#ifndef BOUNDINGBOX_H_INCLUDED
#define BOUNDINGBOX_H_INCLUDED

class BoundingBox
{
public:
    BoundingBox(float width, float height, float depth) :
        width(width),
        height(height),
        depth(depth)
    {
    }

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
