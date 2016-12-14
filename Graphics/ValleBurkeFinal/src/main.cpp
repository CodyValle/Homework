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
#include <ctime>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "solidDrawableFactory.h"
#include "texturedDrawableFactory.h"
#include "rootNode.h"
#include "ellipticalAnimator.h"
#include "shipController.h"

#include "collisionDetection.h"
#include "enemyLauncher.h"

using namespace std;
using namespace glm;

// Drawable Factory
static DrawableFactory* drawableFactory = 0;
static TexturedDrawableFactory* textureDrawableFactory = 0;

// Root node
static RootNode* root = 0;

// If a button is pressed
static KeyInput* keys = 0;

// For checking collisions
static CollisionDetector* collisionDetector = 0;

// Launches enemy ships
static EnemyLauncher* motherShip = 0;

// For tracking deltaTime
static float old_t = 0.0;

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
    int t = glutGet(GLUT_ELAPSED_TIME);

    if (root)
        root->animate((t - old_t) / 10.0); // Update all animations

    old_t = t;
    glutPostRedisplay();
    glutTimerFunc(0, animate, value);
}

// Initialization routine.
int setup(void)
{
    // Create a DrawableFactory
    drawableFactory = new SolidDrawableFactory();
    textureDrawableFactory = new TexturedDrawableFactory();
    // Set up key input
    keys = new KeyInput();

    /// Set up the main ship
    Node* shipParentNode = new Node();
    shipParentNode->getTransform().setTranslate(vec3(0., 0., -30.));
    // Set up the shooting capabilities
    BulletController* bulletController = new BulletController(drawableFactory, root, -330.);
    // Create an animator
    Animator* shipController = new ShipController(shipParentNode->getTransform(), keys, bulletController);
    root->addChild(shipParentNode, shipController);

    // Create the wings
    Node* wingsNode = new Node();
    shipParentNode->addChild(wingsNode);
    // Add the wings
    Drawable* wingsObj = drawableFactory->makeSphere(20., 20.);
    wingsNode->addDrawable(wingsObj);
    // Change the color
    wingsObj->setAmbient(.5, 0.8, 0.5);
    wingsObj->setDiffuse(.5, 0.8, 0.5);
    // Change the shape
    wingsNode->getTransform().translate(vec3(0, -0.3, -.5));
    wingsNode->getTransform().setScale(vec3(6., 0.2, .6));

    // Create the body
    Node* bodyNode = new Node();
    shipParentNode->addChild(bodyNode);
    // Add the body
    Drawable* bodyObj = drawableFactory->makeSphere(50., 50.);
    bodyNode->addDrawable(bodyObj);
    // Change the color
    bodyObj->setAmbient(.5, 0.3, 0.5);
    bodyObj->setDiffuse(.5, 0.3, 0.5);
    bodyObj->setSpecular(1.0, 1.0, 1.0);
    bodyObj->setShininess(100.0);
    bodyNode->getTransform().translate(vec3(0., 0., 3.));
    bodyNode->getTransform().setScale(vec3(1., 1., 6.));

    /// Add the background stars
    Node* starsNode = new Node();
    root->addChild(starsNode);
    Drawable* starPlane = textureDrawableFactory->makePlane(string("stars.bmp"));
    starsNode->addDrawable(starPlane);

    starsNode->getTransform().translate(vec3(-10., 0., -250.));
    starsNode->getTransform().setScale(vec3(500., 500., 500.));

    /// Set up the enemy ships
    EnemyDistributor* enemies = new EnemyDistributor(drawableFactory, root, 0);
    YerionDistributor* yerion = new YerionDistributor(textureDrawableFactory, root, 0);
    motherShip = new EnemyLauncher(enemies, 500, yerion, 1500);
    Node* motherShipNode = new Node();
    root->addChild(motherShipNode, motherShip);

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
    if (key == 27)
    {
        // Destroy everything
        glutLeaveMainLoop();
    }

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

    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1000, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("SpaceNox");
    //glutFullScreen();
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);

    glewExperimental = GL_TRUE;
    glewInit();

    glClearColor(0., 0., 0., 1.0);
    glEnable(GL_DEPTH_TEST);

    root = RootNode::getInstance();
    collisionDetector = CollisionDetector::getInstance();
    std::srand(std::time(0)); // Use current time as seed for random generator

    int ret = setup();
    if (ret) return ret;

    // Animate
    animate(1);

    glutMainLoop();
}

