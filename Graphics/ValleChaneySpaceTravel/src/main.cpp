///////////////////////////////////////////////////////////////////////////////////
// spacetravel.cpp
//
// This program draws a conical spacecraft that can travel and an array of
// fixed spherical asteroids. The view in the left viewport is from a fixed
// camera; the view in the right viewport is from the spacecraft.
// There is approximate collision detection.
//
// User-defined constants:
// ROW is the number of rows of  asteroids.
// COLUMN is the number of columns of asteroids.
// FILL_PROBABILITY is the percentage probability that a particular row-column slot
// will be filled with an asteroid.
//
// Interaction:
// Press the left/right arrow keys to turn the craft.
// Press the up/down arrow keys to move the craft.
// Reach the glowing golden asteroid to win!
//
// Sumanta Guha, modified by Cody Valle and Malcolm Chaney.
///////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>
#include <ctime>

#  include <GL/glew.h>
#  include <GL/freeglut.h>
#ifdef __APPLE__
#  include <OpenGL/glext.h>
#else
#  include <GL/glext.h>
#endif

#include "Asteroid.h"

using namespace std;

static const float PI = 3.14159265;

const int ROWS = 8;  // Number of rows of asteroids.
const int COLUMNS = 5; // Number of columns of asteroids.
const int FILL_PROBABILITY = 100; // Percentage probability that a particular row-column slot will be
                             // filled with an asteroid. It should be an integer between 0 and 100.

// Globals.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.
static int width, height; // Size of the OpenGL window.
static float angle = 0.0; // Angle of the spacecraft.
static float xVal = 0, zVal = 0; // Co-ordinates of the spacecraft.
static bool isCollision = false; // Is there collision between the spacecraft and an asteroid?
static bool isGolden = false; // Did I find the gold?
static unsigned int spacecraft; // Display lists base index.
static GoldAsteroid* goldAsteroid = 0; // Pointer to the GoldAsteroid

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, const char *output)
{
    for (const char* c = output; *c != '\0'; ++c)
        glutBitmapCharacter(font, *c);
}

vector<Asteroid*> arrayAsteroids; // Global array of asteroids.

void animate(int)
{
    if (goldAsteroid && goldAsteroid->isAnimating)
        glutTimerFunc(30, animate, 0);
    else return;

    float offset = sin(goldAsteroid->oscillate * PI / 180) * 64;
    goldAsteroid->color[0] = 190 + offset;
    goldAsteroid->color[1] = 160 + offset;

    goldAsteroid->oscillate += 8;
    goldAsteroid->oscillate %= 360;

    glutPostRedisplay();
}

// Initialization routine.
void setup()
{
    srand(time(NULL));
    arrayAsteroids.reserve(ROWS * COLUMNS);

    spacecraft = glGenLists(1);
    glNewList(spacecraft, GL_COMPILE);
        glPushMatrix();
        glRotatef(180.0, 0.0, 1.0, 0.0); // To make the spacecraft point down the $z$-axis initially.
        glColor3f (1.0, 1.0, 1.0);
        glutWireCone(5.0, 10.0, 10, 10);
        glRotatef(-90, 1, 0, 0);
        glutWireCylinder(1.0,20.0,20,20);
        glPopMatrix();
    glEndList();

    // Determine randomly which will be the gold asteroid
    int rand_column = rand() % COLUMNS,
        rand_row = rand() % ROWS;

    // Initialize global arrayAsteroids.
    for (int j = 0; j < COLUMNS; j++)
        for (int i = 0; i < ROWS; i++)
            if (rand() % 100 < FILL_PROBABILITY)
            // If rand()%100 >= FILL_PROBABILITY the default constructor asteroid remains in the slot
            // which indicates that there is no asteroid there because the default's radius is 0.
            {
                // Position the asteroids depending on if there is an even or odd number of columns
                // so that the spacecraft faces the middle of the asteroid field.
                AsteroidBuilder builder = AsteroidBuilder( (COLUMNS % 2 ? 15 : 0) + 30.0 * (-COLUMNS / 2 + j),
                                                           0.0, -40.0 - 30.0 * i, 3.0,
                                                           rand() % 256, rand() % 256, rand() % 256);
                // Check if this is the gold asteroid
                if (rand_column == j && rand_row == i)
                {
                    goldAsteroid = new GoldAsteroid(builder);
                    arrayAsteroids[j * ROWS + i] = goldAsteroid;
                    glutTimerFunc(0, animate, 0); // Set up the glow animation
                }
                else
                    arrayAsteroids[j * ROWS + i] = new NormalAsteroid(builder);
            }

    glEnable(GL_DEPTH_TEST);
    glClearColor (0.0, 0.0, 0.0, 0.0);
}

// Function to check if two spheres centered at (x1,y1,z1) and (x2,y2,z2) with
// radius r1 and r2 intersect.
int checkSpheresIntersection(float x1, float y1, float z1, float r1,
						     float x2, float y2, float z2, float r2)
{
    return ( pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2) <= pow(r1 + r2, 2) );
}

// Function to check if the spacecraft collides with an asteroid when the center of the base
// of the craft is at (x, 0, z) and it is aligned at an angle a to to the -z direction.
// Collision detection is approximate as instead of the spacecraft we use a bounding sphere.
bool asteroidCraftCollision(float x, float z, float a)
{
     // Check for collision with each asteroid.
    for (int i = 0; i < COLUMNS * ROWS; ++i)
            if (arrayAsteroids[i]->getRadius() > 0 ) // If asteroid exists.
                if ( checkSpheresIntersection( x - 5 * sin( (PI / 180.0) * a), 0.0,
                     z - 5 * cos( (PI / 180.0) * a), 7.072,
                     arrayAsteroids[i]->getCenterX(), arrayAsteroids[i]->getCenterY(),
                     arrayAsteroids[i]->getCenterZ(), arrayAsteroids[i]->getRadius() ) )
                    return true;
    return false;
}

bool asteroidGoldFound(float x, float z, float a)
{
     // Check for collision with gold asteroid.
    if (goldAsteroid->getRadius() > 0 ) // If asteroid exists.
        return checkSpheresIntersection( x - 5 * sin( (PI / 180.0) * a), 0.0,
                z - 5 * cos( (PI / 180.0) * a), 7.072,
                goldAsteroid->getCenterX(), goldAsteroid->getCenterY(),
                goldAsteroid->getCenterZ(), goldAsteroid->getRadius() );

    return false;
}

// Drawing routine.
void drawScene(void)
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Begin right viewport.
    glViewport(width / 2.0, 0, width / 2.0, height);
    glLoadIdentity();

    // Write text in isolated (i.e., before gluLookAt) translate block.
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glRasterPos3f(-28.0, 25.0, -30.0);
    if (isCollision && !isGolden)
        writeBitmapString((void*)font, string("Cannot - will crash!").c_str());
    else if (isGolden)
        writeBitmapString((void*)font, string("You flew into the sun. Way to go!").c_str());
    glPopMatrix();

    // Fixed camera.
    gluLookAt(goldAsteroid->getCenterX(), goldAsteroid->getRadius() * 1.3, goldAsteroid->getCenterZ(),
              xVal, 0, zVal,
              0, 1, 0);

    // Draw all the asteroids in arrayAsteroids.
    for (int i = 0; i < COLUMNS * ROWS; i++)
        arrayAsteroids[i]->draw();

    // Draw spacecraft.
    glPushMatrix();
    glTranslatef(xVal, 0.0, zVal);
    glRotatef(angle, 0.0, 1.0, 0.0);
    glCallList(spacecraft);
    glPopMatrix();
    // End right viewport.

    // Begin left viewport.
    glViewport (0, 0, width / 2.0,  height);
    glLoadIdentity();

    // Write text in isolated (i.e., before gluLookAt) translate block.
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glRasterPos3f(-28.0, 25.0, -30.0);
    if (isCollision && !isGolden)
        writeBitmapString((void*)font, string("Cannot - will crash!").c_str());
    else if (isGolden)
        writeBitmapString((void*)font, string("You flew into the sun. Way to go!").c_str());
    glPopMatrix();

    // Draw a vertical line on the left of the viewport to separate the two viewports
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(2.0);
    glBegin(GL_LINES);
        glVertex3f(-5.0, -5.0, -5.0);
        glVertex3f(-5.0, 5.0, -5.0);
    glEnd();
    glLineWidth(1.0);

    // Locate the camera at the tip of the cone and pointing in the direction of the cone.
    gluLookAt(xVal - 10 * sin( (PI / 180.0) * angle),
	          0.0,
			  zVal - 10 * cos( (PI / 180.0) * angle),
	          xVal - 11 * sin( (PI / 180.0) * angle),
			  0.0,
              zVal - 11 * cos( (PI / 180.0) * angle),
              0.0,
			  1.0,
			  0.0);

    // Draw all the asteroids in arrayAsteroids.
    for (int i = 0; i < COLUMNS * ROWS; i++)
        arrayAsteroids[i]->draw();
    // End left viewport.

    glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport (0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 250.0);
    glMatrixMode(GL_MODELVIEW);

    // Pass the size of the OpenGL window.
    width = w;
    height = h;
}

// Closes the program
void close()
{
    for (int i = 0; i < COLUMNS * ROWS; ++i)
        delete arrayAsteroids[i];

    exit(0);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27: // Esc
        close();
        break;

    default: break;
    }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
    float tempxVal = xVal,
          tempzVal = zVal,
          tempAngle = angle;

    // Compute next position.
    switch (key)
    {
    case GLUT_KEY_LEFT:
        tempAngle += 5.;
        break;

    case GLUT_KEY_RIGHT:
        tempAngle -= 5.;
        break;

    case GLUT_KEY_UP:
        tempxVal = xVal - sin(angle * PI / 180.);
        tempzVal = zVal - cos(angle * PI / 180.);
        break;

    case GLUT_KEY_DOWN:
        tempxVal = xVal + sin(angle * PI / 180.);
        tempzVal = zVal + cos(angle * PI / 180.);
        break;

    default: break;
    }

    // Angle correction.
    tempAngle = fmod(tempAngle, 360.);

    // Move spacecraft to next position only if there will not be collision with an asteroid.
    isCollision = asteroidCraftCollision(tempxVal, tempzVal, tempAngle);

    isGolden = asteroidGoldFound(tempxVal, tempzVal, tempAngle);

    if (!isCollision)
    {
        xVal = tempxVal;
        zVal = tempzVal;
        angle = tempAngle;
    }

    glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
    cout << "Interaction:" << endl;
    cout << "Press the left/right arrow keys to turn the craft." << endl
         << "Press the up/down arrow keys to move the craft." << endl;
}

// Main routine.
int main(int argc, char *argv[])
{
    printInteraction();
    glutInit(&argc, argv);

    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Space Travel");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(specialKeyInput);

    glewExperimental = GL_TRUE;
    glewInit();

    setup();

    glutMainLoop();
}

