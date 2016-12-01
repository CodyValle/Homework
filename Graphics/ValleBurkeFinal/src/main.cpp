////////////////////////////////////////////////////////////////
// main.cpp
//
// Forward-compatible core GL 4.3 version
//
// This creates a Solar System, complete with Halley's Comet and an Asteroid Belt.
//
// Interaction:
// Press space to toggle between animation on and off.
// Press the up/down arrow keys to speed up/slow down animation.
// Press the x, X, y, Y, z, Z keys to rotate the scene.
//
// Cody Valle
////////////////////////////////////////////////////////////////

#include <cmath>
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "drawableFactory.h"
#include "node.h"
#include "ellipticalAnimator.h"

using namespace std;
using namespace glm;

// Globals.
static float Xangle = 90.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate scene.
static bool isAnimate = true; // Animated?
static unsigned animationPeriod = 10; // Time interval between frames.

// Identity Matrix View
mat4 identity = mat4(1.0);

// Root node
static Node* root = '\0';

// Timer function.
void animate(int value)
{
    if (isAnimate)
    {
        if (root)
            root->animate(); // Update all animations

        glutPostRedisplay();
        glutTimerFunc(animationPeriod, animate, value);
    }
}

// Initialization routine.
int setup(void)
{
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glEnable(GL_DEPTH_TEST);

    // Create a DrawableFactory
    DrawableFactory df;

    // Set up the root node
    root = new Node();

    /// Set up the sun
    // Create the sun
    Drawable* sun = df.makeSphere(5., 15., 10.);
    float sunColor[] = {1., 1., 0.3, 1.};
    sun->setColor(sunColor);
    root->addDrawable(sun);

    /// Set up the planet
    // Create the planet
    Node* planetNode = new Node();
    // Its Transform is controlled by an elliptical animation
    Animator* anim = new EllipticalAnimator(planetNode->getTransform(), 27, 17, PI / 128, PI / 3);
    // Planet is child of sun
    root->addChild(planetNode, anim);

    // Add the planet Sphere to the Node
    Drawable* planet = df.makeSphere(2., 10, 5);
    float planetColor[] = {0.2, 0.2, 0.9, 1.};
    planet->setColor(planetColor);
    planetNode->addDrawable(planet);

    /// Set up the close moon
    // Create the first moon
    Node* moon1Node = new Node();
    // Create its animation
    anim = new EllipticalAnimator(moon1Node->getTransform(), 11, 9.5, PI / 60);
    // Moon is child of planet
    planetNode->addChild(moon1Node, anim);

    // Add the moon Sphere to the Node
    Drawable* moon1 = df.makeSphere(1., 8, 4);
    float moon1Color[] = {0.3, 0.8, 0.3, 1.0};
    moon1->setColor(moon1Color);
    moon1Node->addDrawable(moon1);

    /// Set up the far moon
    // Create the second moon
    Node* moon2Node = new Node();
    // Create its animation
    anim = new EllipticalAnimator(moon2Node->getTransform(), 6, 7, -PI / 80);
    // Moon is child of planet
    planetNode->addChild(moon2Node, anim);

    // Add the moon Sphere to the Node
    Drawable* moon2 = df.makeSphere(1., 8, 4);
    float moon2Color[] = {0.4, 0.9, 0.7, 1.0};
    moon2->setColor(moon2Color);
    moon2Node->addDrawable(moon2);

    /// Add Halley's Comet
    Node* cometNode = new Node();
    anim = new EllipticalAnimator(cometNode->getTransform(), 70, 30, PI / 300);
    anim->setTranslation(vec3(60, 0, 0));
    root->addChild(cometNode, anim);

    Drawable* comet = df.makeSphere(0.7, 5, 5);
    float cometColor[] = {0.8, 0.3, 0.3, 1.0};
    comet->setColor(cometColor);
    cometNode->addDrawable(comet);

    /// Add asteroids
    float asteroidColor[] = {0.5, 0.5, 0.5, 1.0};
    const int numAsteroids = 100;

    for (float j = 0; j < numAsteroids / 10.; ++j)
        for (int i = 0; i < numAsteroids; ++i)
        {
            Node* asteroidNode = new Node();
            float radius = j / numAsteroids * 100.0 + 40.0;
            anim = new EllipticalAnimator(asteroidNode->getTransform(), radius, radius, PI / 500, PI * 2 * float(i) / numAsteroids);
            root->addChild(asteroidNode, anim);

            Drawable* asteroid = df.makeSphere(0.2, 3, 3);
            asteroid->setColor(asteroidColor);
            asteroidNode->addDrawable(asteroid);
        }

    // Animation on by default
    if (isAnimate)
        animate(1);

    return 0;
}

// Converts degrees to radians
float radians(float in)
{
    return in / 180. * PI;
}

// Drawing routine.
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Create base model view matrix
    mat4 modelViewMat = translate(identity, vec3(0., 0., -45.));
    modelViewMat = rotate(modelViewMat, radians(Xangle), vec3(1., 0., 0.));
    modelViewMat = rotate(modelViewMat, radians(Yangle), vec3(0., 1., 0.));
    modelViewMat = rotate(modelViewMat, radians(Zangle), vec3(0., 0., 1.));
    //modelViewMat = scale(modelViewMat, vec3(0.8, 0.8, 0.8));

    // Draw the scene
    if (root)
        root->draw(modelViewMat);

    glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport(0, 0, w, h);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    const float angleMod = 3;

    switch(key)
    {
    case 27:
        exit(0);

    case ' ':
        if (isAnimate)
            isAnimate = false;
        else
        {
            isAnimate = true;
            animate(1);
        }
        return;

    case 'x':
        Xangle = fmod(Xangle + angleMod, 360.0);
        break;

    case 'X':
        Xangle = fmod(Xangle - angleMod, 360.0);
        break;

    case 'y':
        Yangle = fmod(Yangle + angleMod, 360.0);
        break;

    case 'Y':
        Yangle = fmod(Yangle - angleMod, 360.0);
        break;

    case 'z':
        Zangle = fmod(Zangle + angleMod, 360.0);
        break;

    case 'Z':
        Zangle = fmod(Zangle - angleMod, 360.0);
        break;

    default:
        break;
    }
    glutPostRedisplay();
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
    if (key == GLUT_KEY_DOWN && animationPeriod < 500) animationPeriod += 1;
    if (key == GLUT_KEY_UP && animationPeriod >= 2) animationPeriod -= 1;
    glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
    cout << "Interaction:" << endl
         << "Press space to toggle between animation on and off." << endl
         << "Press the up/down arrow keys to speed up/slow down animation." << endl
         << "Press the x, X, y, Y, z, Z keys to rotate the scene." << endl;
}

// Main routine.
int main(int argc, char **argv)
{
    printInteraction();
    glutInit(&argc, argv);

    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("SpaceNox");
    //glutFullScreen();
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(specialKeyInput);

    glewExperimental = GL_TRUE;
    glewInit();

    int ret = setup();
    if (ret) return ret;

    glutMainLoop();
}

