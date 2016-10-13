#ifndef ASTEROID_H_INCLUDED
#define ASTEROID_H_INCLUDED

// AsteroidBuilder class
class AsteroidBuilder
{
public:
    AsteroidBuilder(float x, float y, float z, float r, unsigned char colorR,
             unsigned char colorG, unsigned char colorB);

    friend class Asteroid;

protected:
    float centerX, centerY, centerZ, radius;
    unsigned char color[3];
};

// Asteroid class.
class Asteroid
{
public:
    Asteroid();
    Asteroid(AsteroidBuilder builder);
    Asteroid(float x, float y, float z, float r, unsigned char colorR,
             unsigned char colorG, unsigned char colorB);
    virtual ~Asteroid() {}

    float getCenterX() { return centerX; }
    float getCenterY() { return centerY; }
    float getCenterZ() { return centerZ; }
    float getRadius()  { return radius; }
    virtual void draw();

protected:
    float centerX, centerY, centerZ, radius;
    unsigned char color[3];
};

class NormalAsteroid : public Asteroid
{
public:
    NormalAsteroid();
    NormalAsteroid(AsteroidBuilder builder);
    NormalAsteroid(float x, float y, float z, float r, unsigned char colorR,
                   unsigned char colorG, unsigned char colorB);
    ~NormalAsteroid() {}
};

class GoldAsteroid : public Asteroid
{
public:
    GoldAsteroid();
    GoldAsteroid(AsteroidBuilder builder);
    GoldAsteroid(float x, float y, float z, float r, unsigned char colorR,
                unsigned char colorG, unsigned char colorB);
    ~GoldAsteroid();

    friend void animate(int);

protected:
    unsigned char oscillate; // Ranges from 0-360 for the sin function
    bool isAnimating; // Whether the asteroid is glowing
};

#endif // ASTEROID_H_INCLUDED
