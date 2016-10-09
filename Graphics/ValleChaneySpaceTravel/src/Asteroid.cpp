#  include <GL/glew.h>
#  include <GL/freeglut.h>
#ifdef __APPLE__
#  include <OpenGL/glext.h>
#else
#  include <GL/glext.h>
#endif

#include "Asteroid.h"

using namespace std;

// AsteroidBuilder constructor
AsteroidBuilder::AsteroidBuilder(float x, float y, float z, float r, unsigned char colorR,
				   unsigned char colorG, unsigned char colorB) :
    centerX(x),
    centerY(y),
    centerZ(z),
    radius(r),
    color{colorR, colorG, colorB}
{
}

// Asteroid default constructor.
Asteroid::Asteroid() :
    centerX(0.0),
    centerY(0.0),
    centerZ(0.0),
    radius(0.0), // Indicates no asteroid exists in the position.
    color{0, 0, 0}
{
}

// Asteroid constructor
Asteroid::Asteroid(AsteroidBuilder builder) :
    centerX(builder.centerX),
    centerY(builder.centerY),
    centerZ(builder.centerZ),
    radius(builder.radius),
    color{builder.color[0], builder.color[1], builder.color[2]}
{
}

// Asteroid constructor.
Asteroid::Asteroid(float x, float y, float z, float r, unsigned char colorR,
				   unsigned char colorG, unsigned char colorB) :
    centerX(x),
    centerY(y),
    centerZ(z),
    radius(r),
    color{colorR, colorG, colorB}
{
}

// Function to draw asteroid.
void Asteroid::draw()
{
    if (radius > 0.0) // If asteroid exists.
    {
        glPushMatrix();
        glTranslatef(centerX, centerY, centerZ);
        glColor3ubv(color);
        glutWireSphere(radius, (int)radius * 6, (int)radius * 6);
        glPopMatrix();
    }
}

// NormalAsteroid default constructor.
NormalAsteroid::NormalAsteroid() :
    Asteroid()
{
}

// NormalAsteroid constructor.
NormalAsteroid::NormalAsteroid(AsteroidBuilder builder) :
    Asteroid(builder)
{
}

// NormalAsteroid constructor.
NormalAsteroid::NormalAsteroid(float x, float y, float z, float r, unsigned char colorR,
                           unsigned char colorG, unsigned char colorB) :
    Asteroid(x, y, z, r, colorR, colorG, colorB)
{
}

// GoldAsteroid default constructor.
GoldAsteroid::GoldAsteroid() :
    Asteroid(),
    oscillate(0),
    isAnimating(true)
{
    color[0] = 222;
    color[1] = 192;
    color[2] = 0;
}

// GoldAsteroid constructor.
GoldAsteroid::GoldAsteroid(AsteroidBuilder builder) :
    Asteroid(builder),
    oscillate(0),
    isAnimating(true)
{
    radius *= 2;
    color[0] = 222;
    color[1] = 192;
    color[2] = 0;
}

// GoldAsteroid constructor.
GoldAsteroid::GoldAsteroid(float x, float y, float z, float r, unsigned char colorR,
                           unsigned char colorG, unsigned char colorB) :
    Asteroid(x, y, z, r * 2, colorR, colorG, colorB),
    oscillate(0),
    isAnimating(true)
{
    color[0] = 222;
    color[1] = 192;
    color[2] = 0;
}

// GoldAsteroid destructor
GoldAsteroid::~GoldAsteroid()
{
    isAnimating = false;
}
