////////////////////////////////////////////////////////////////
// main.cpp
//
// Forward-compatible core GL 4.3 version
//
// This creates an awesome game.
//
// Cody Valle, Andrew Burke
////////////////////////////////////////////////////////////////

#include <cmath>
#include <iostream>
#include <fstream>
#include <ctime>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "drawableFactory.h"
#include "rootNode.h"
#include "ellipticalAnimator.h"
#include "shipController.h"

#include "collisionDetection.h"
#include "enemyLauncher.h"

using namespace std;
using namespace glm;

// Globals.
static bool isAnimate = true; // Animated?
static unsigned animationPeriod = 10; // Time interval between frames in milliseconds.

// Drawable Factory
static DrawableFactory* drawableFactory = 0;

// Root node
static RootNode* root = 0;

// If a button is pressed
static KeyInput* keys = new KeyInput();

// For checking collisions
static CollisionDetector* collisionDetector = 0;

// Launches enemy ships
EnemyLauncher* motherShip = 0;

// For tracking deltaTime
static float old_t;

// Drawing routine.
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw the scene
    if (root)
        root->draw(mat4(1.0));

    glutSwapBuffers();
}

// Timer function.
void animate(int value)
{
    if (isAnimate)
    {
        int t = glutGet(GLUT_ELAPSED_TIME);

        if (root)
            root->animate((t - old_t) / 10.0); // Update all animations

        old_t = t;
        glutPostRedisplay();
        glutTimerFunc(animationPeriod, animate, value);
    }
}

// Initialization routine.
int setup(void)
{
    // Create a DrawableFactory
    drawableFactory = new DrawableFactory();

    /// Set up the main ship
    Node* shipParentNode = new Node();
    shipParentNode->getTransform().setTranslate(vec3(0., 0., -30.));
    // Set up the shooting capabilities
    BulletController* bulletController = new BulletController(drawableFactory, root, -130.);
    // Create an animator
    Animator* shipController = new ShipController(shipParentNode->getTransform(), keys, bulletController);
    root->addChild(shipParentNode, shipController);

    // Create the wings
    Node* wingsNode = new Node();
    shipParentNode->addChild(wingsNode);
    // Add the wings
    Drawable* wingsObj = drawableFactory->makeSphere(1., 4., 4.);
    wingsNode->addDrawable(wingsObj);
    // Change the color
    float wingsColor[] = {.5, 0.8, 0.5, 1.};
    wingsObj->setColor(wingsColor);
    // Change the shape
    wingsNode->getTransform().setScale(vec3(6., 0.2, .6));

    // Create the body
    Node* bodyNode = new Node();
    shipParentNode->addChild(bodyNode);
    // Add the body
    Drawable* bodyObj = drawableFactory->makeSphere(2., 4., 4.);
    bodyNode->addDrawable(bodyObj);
    // Change the color
    float bodyColor[] = {.5, 0.3, 0.5, 1.};
    bodyObj->setColor(bodyColor);
    // Change the shape
    bodyNode->getTransform().setScale(vec3(1., 1., 2.));

    /// Set up the enemy ships
    EnemyDistributor* enemies = new EnemyDistributor(drawableFactory, root, 0);
    motherShip = new EnemyLauncher(enemies, 500);
    Node* motherShipNode = new Node();
    root->addChild(motherShipNode, motherShip);

    // Animation on by default
    if (isAnimate)
        animate(1);

    return 0;
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport(0, 0, w, h);
}

// Keyboard input processing routine.
void keyUp(unsigned char key, int x, int y)
{
    keys->keyUp(key);
}

// Keyboard input processing routine.
void keyDown(unsigned char key, int x, int y)
{
    keys->keyDown(key);
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
    cout << "Interaction:" << endl
         << "Press space to shoot." << endl
         << "Press the WASD keys to move." << endl;
}

// Main routine.
int main(int argc, char **argv)
{
    printInteraction();

    glutInit(&argc, argv);

    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("SpaceNox");
    glutFullScreen();
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);

    glewExperimental = GL_TRUE;
    glewInit();

    glClearColor(0.2, 0.2, 0.2, 1.0);
    glEnable(GL_DEPTH_TEST);

    root = RootNode::getInstance();
    collisionDetector = CollisionDetector::getInstance();
    std::srand(std::time(0)); // use current time as seed for random generator

    int ret = setup();
    if (ret) return ret;

    old_t = glutGet(GLUT_ELAPSED_TIME);
    glutMainLoop();
}

