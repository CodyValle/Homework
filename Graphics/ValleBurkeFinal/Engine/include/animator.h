#ifndef ANIMATOR_H_INCLUDED
#define ANIMATOR_H_INCLUDED

#include "transform.h"

class Animator
{
public:
    // Constructor
    Animator();

    // Destructor
    ~Animator();

    // To be defined to child classes
    virtual void animate() = 0;

    // Sets a translation offset from the final calculation
    inline void setTranslation(glm::vec3 trans)
        { transform.setTranslate(trans); }

protected:
    Transform transform; // Tracks offsets
};

#endif // ANIMATOR_H_INCLUDED
