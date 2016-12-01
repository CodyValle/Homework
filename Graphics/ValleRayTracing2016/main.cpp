//@file main.cpp
//@authors Dr Y and Dr. O
//@date June 2012

//@This program opens a window and renders a scene of a yellow sphere.

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cstdlib>

#include "World.h"
#include "RenderEngine.h"

World* world_ptr;
RenderEngine* engine_ptr;


/* GLUT callback Handlers */

static void resize(int width, int height)
{
    glViewport(0, 0, width, height);// usually set to window size. Can be used to draw just part of the window if you want.
    glMatrixMode(GL_PROJECTION); // matrix used to set camera projection. Initialized to the identity.
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height); // function in the glu library that determines the camera projection matrix.  Since this is 2d orthographic, I assume the projection tranformation just scales to the window size (width, height).  We will talk about this more when we start openGL
           //Orthographic projection (or orthogonal projection) is a means of representing a three-dimensional object in two dimensions. It is a form of parallel projection, where all the projection lines are orthogonal to the projection plane,[1]
           //resulting in every plane of the scene appearing in affine transformation on the viewing surface.

    glMatrixMode(GL_MODELVIEW); // matrix tranformation applied to the geometry.  We set it to the identity since we aren't doing any transformations.
    glLoadIdentity() ;
}

static void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    engine_ptr -> render();

    glDrawPixels(engine_ptr -> world_ptr -> horizontal_res, engine_ptr -> world_ptr -> vertical_res, GL_RGB, GL_FLOAT, engine_ptr -> image);

    glFlush();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    // defines the world of geometric objects and passes a pointer to that world to the engine which renders the scene of objects
    world_ptr = new World;
    world_ptr -> build();
    
    engine_ptr = new RenderEngine(world_ptr);

    glutInitWindowSize(world_ptr -> vertical_res, world_ptr -> horizontal_res);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB);

    glutCreateWindow("A Scene");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glClearColor(.5,.5,1,1);

    glutMainLoop();

    delete engine_ptr; engine_ptr = NULL;  // is it ok that these appear after glutMainLoop?
    delete world_ptr; world_ptr = NULL;

    return EXIT_SUCCESS;  // where is this constant set?
}
