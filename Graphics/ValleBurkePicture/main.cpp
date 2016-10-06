// File: main.cpp
// Authors: Cody Valle, Andrew Burke
// Description: Draws a face of a bear using geometric primitives.

#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib")
#endif

// Set up some constants
const unsigned bear_xoffset = 0;
const unsigned bear_yoffset = 0;
const int bear_xscale = 2 * 100; // 100 is the width of half the drawing screen
const int bear_yscale = 2 * 100; // 100 is the height of half the drawing screen

// Helper function for drawScene
void drawMirroredLines(const float xs[], const float ys[], const int len)
{
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < len; i++) // Traverse forward
        glVertex3f(xs[i] * bear_xscale + bear_xoffset,
                   ys[i] * bear_yscale + bear_yoffset, 0.1);
    for (int i = len - 1; i >= 0; i--) // Traverse backward, flipping x coords across y axis
        glVertex3f(xs[i] * bear_xscale * -1 + bear_xoffset,
                   ys[i] * bear_yscale + bear_yoffset, 0.1);
    glEnd();
}

// Drawing routine.
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Color in the background
    glBegin(GL_POLYGON);
        glColor3f(0, 0, 0.1);
        glVertex3f(-720, -720, -0.2);
        glVertex3f(720, -720, -0.2);
        glVertex3f(720, 720, -0.2);
        glVertex3f(-720, 720, -0.2);
    glEnd();

    // Color in the bear
    for (int i = 1; i > -3; i -=2)
    {
        // Background Points
        glPointSize(4);
        glBegin(GL_POINTS);
            glColor3f(1,1,1);
            glVertex3f(20 * i, -80, -0.1);
            glVertex3f(20 * i, 80, -0.1);
            glVertex3f(90 * i, 48, -0.1);
            glVertex3f(98 * i, 98, -0.1);
            glVertex3f(93 * i, 85, -0.1);
            glVertex3f(30 * i, 81, -0.1);
            glVertex3f(34 * i, 71, -0.1);
            glVertex3f(10 * i, 92, -0.1);
            glVertex3f(58 * i, 76, -0.1);
            glVertex3f(99 * i, -5, -0.1);
            glVertex3f(86 * i, -15, -0.1);
            glVertex3f(84 * i, -34, -0.1);
            glVertex3f(91 * i, -84, -0.1);
            glVertex3f(72 * i, -80, -0.1);
            glVertex3f(4 * i, -90, -0.1);
            glVertex3f(10 * i, -98, -0.1);
            glVertex3f(44 * i, -84, -0.1);
            glVertex3f(30 * i, -91, -0.1);
            glVertex3f(28 * i, -72, -0.1);
        glEnd();

        // Color in the jowels with a triangle fan
        glBegin(GL_TRIANGLE_FAN);
            glColor3f(0.255, 0.094, 0); // Dark brown
            glVertex3f(0.33 * bear_xscale * i + bear_xoffset, -0.25 * bear_yscale + bear_yoffset, 0); // Center(ish) of jowel area
            glVertex3f(0.28 * bear_xscale * i + bear_xoffset, -0.22 * bear_yscale + bear_yoffset, 0); // Ears[6]
            glColor3f(0.5, 0.2, 0.02); // Regular brown
            glVertex3f(0.117 * bear_xscale * i + bear_xoffset, -0.41 * bear_yscale + bear_yoffset, 0); // Outline[0]
            glVertex3f(0.35 * bear_xscale * i + bear_xoffset, -0.4 * bear_yscale + bear_yoffset, 0); // Outline[1]
            glVertex3f(0.47 * bear_xscale * i + bear_xoffset, -0.2 * bear_yscale + bear_yoffset, 0); // Outline[2]
            glVertex3f(0.4 * bear_xscale * i + bear_xoffset, 0.18 * bear_yscale + bear_yoffset, 0); // Outline[3]
            glColor3f(0.255, 0.094, 0); // Dark brown
            glVertex3f(0.28 * bear_xscale * i + bear_xoffset, -0.22 * bear_yscale + bear_yoffset, 0); // Ears[6]
        glEnd();

        // Color in the face with a polygon
        glBegin(GL_POLYGON);
            glColor3f(0.4, 0.15, 0);  // Medium dark brown
            glVertex3f(0 * bear_xscale * i + bear_xoffset, 0.25 * bear_yscale + bear_yoffset, 0);  // Ears[0]
            glVertex3f(0.27 * bear_xscale * i + bear_xoffset, 0.36 * bear_yscale + bear_yoffset, 0);  // Ears[1]
            glVertex3f(0.4 * bear_xscale * i + bear_xoffset, 0.18 * bear_yscale + bear_yoffset, 0); // Ears[5]
            glColor3f(0.2, 0.04, 0);  // Dark brown
            glVertex3f(0.28 * bear_xscale * i + bear_xoffset, -0.22 * bear_yscale + bear_yoffset, 0); // Ears[6]
            glColor3f(0.4, 0.15, 0);  // Medium dark brown
            glVertex3f(0.1 * bear_xscale * i + bear_xoffset, -0.43 * bear_yscale + bear_yoffset, 0);  // Ears[7]
            glVertex3f(0 * bear_xscale * i + bear_xoffset, -0.43 * bear_yscale + bear_yoffset, 0);  // Bottom center of face
        glEnd();

        // Color in the forehead with a polygon
        glBegin(GL_POLYGON);
            glColor3f(0.2, 0.04, 0);  // Dark brown
            glVertex3f(0 * bear_xscale * i + bear_xoffset, 0.25 * bear_yscale + bear_yoffset, 0);  // Bottom of forehead
            glColor3f(0.5, 0.2, 0.02); // Regular brown
            glVertex3f(0.27 * bear_xscale * i + bear_xoffset, 0.36 * bear_yscale + bear_yoffset, 0);  // Side of forehead
            glVertex3f(0 * bear_xscale * i + bear_xoffset, 0.38 * bear_yscale + bear_yoffset, 0);  // Top of forehead
        glEnd();

        // Color in the ears with a triangle fan
        glBegin(GL_TRIANGLE_FAN);
            glColor3f(0.71, 0.369, 0.529); // Pink
            glVertex3f(0.41 * bear_xscale * i + bear_xoffset, 0.37 * bear_yscale + bear_yoffset, 0); // Center(ish) of ears
            glVertex3f(0.3325 * bear_xscale * i + bear_xoffset, 0.27 * bear_yscale + bear_yoffset, 0); // Midpoint of Ears[1] and Ears[5]
            glColor3f(0.5, 0.2, 0.02); // Brown
            glVertex3f(0.4 * bear_xscale * i + bear_xoffset, 0.18 * bear_yscale + bear_yoffset, 0); // Ears[5]
            glVertex3f(0.48 * bear_xscale * i + bear_xoffset, 0.39 * bear_yscale + bear_yoffset, 0); // Ears[4]
            glVertex3f(0.43 * bear_xscale * i + bear_xoffset, 0.44 * bear_yscale + bear_yoffset, 0); // Ears[3]
            glVertex3f(0.37 * bear_xscale * i + bear_xoffset, 0.44 * bear_yscale + bear_yoffset, 0); // Ears[2]
            glVertex3f(0.27 * bear_xscale * i + bear_xoffset, 0.36 * bear_yscale + bear_yoffset, 0); // Ears[1]
            glColor3f(0.71, 0.369, 0.529); // Pink
            glVertex3f(0.3325 * bear_xscale * i + bear_xoffset, 0.27 * bear_yscale + bear_yoffset, 0); // Midpoint of Ears[1] and Ears[5]4
        glEnd();

        // Color the eyes with a square
        glBegin(GL_POLYGON);
            glColor3f(0, 0, 0); // Black
            glVertex3f(0.2 * bear_xscale * i + bear_xoffset, 0.07 * bear_yscale + bear_yoffset, 0); // Eyes[0]
            glVertex3f(0.235 * bear_xscale * i + bear_xoffset, 0.035 * bear_yscale + bear_yoffset, 0); // Eyes[1]
            glVertex3f(0.27 * bear_xscale * i + bear_xoffset, 0.07 * bear_yscale + bear_yoffset, 0); // Eyes[2]
            glColor3f(0.6, 0.6, 0.6); // Light gray
            glVertex3f(0.235 * bear_xscale * i + bear_xoffset, 0.105 * bear_yscale + bear_yoffset, 0); // Eyes[3]
        glEnd();

        // Color in the nose with a polygon
        glBegin(GL_POLYGON);
            glColor3f(0, 0, 0);   // Black
            glVertex3f(0 * bear_xscale * i + bear_xoffset, -0.19 * bear_yscale + bear_yoffset, 0); // Nose[0]
            glVertex3f(0.1 * bear_xscale * i + bear_xoffset, -0.12 * bear_yscale + bear_yoffset, 0); // Nose[1]
            glColor3f(0.2,0.2,0.2);  // Dark gray
            glVertex3f(0.1 * bear_xscale * i + bear_xoffset, -0.09 * bear_yscale + bear_yoffset, 0); // Nose[2]
            glVertex3f(0 * bear_xscale * i + bear_xoffset, -0.09 * bear_yscale + bear_yoffset, 0); // Top center of nose
        glEnd();

        // Color in the mouth with a triangle
        glBegin(GL_TRIANGLES);
            glColor3f(0.4, 0, 0.1);  // Dark red
            glVertex3f(0.1 * bear_xscale * i + bear_xoffset, -0.27 * bear_yscale + bear_yoffset, 0);  // Mouth[0]
            glVertex3f(0 * bear_xscale * i + bear_xoffset, -0.33 * bear_yscale + bear_yoffset, 0);  // Mouth[1]
            glColor3f(0.8, 0, 0);  // Lighter red
            glVertex3f(0 * bear_xscale * i + bear_xoffset, -0.27 * bear_yscale + bear_yoffset, 0);  // Top center of mouth
        glEnd();
    }

    // Draw the outline of the bear
    glColor3f(0, 0, 0);
    glLineWidth(4);

    // Draw the outline of the bear   [0]   [1]    [2] ...
    const float bear_outline_xs[] = { 0.117, 0.35, 0.47, 0.4,  0.27, 0};
    const float bear_outline_ys[] = {-0.41, -0.4, -0.2,  0.18, 0.36, 0.38};
    const int bear_outline_arrlen = 6;

    drawMirroredLines(bear_outline_xs, bear_outline_ys, bear_outline_arrlen);

    // Draw the eyes of the bear  [0]   [1]    [2] ...
    const float bear_eyes_xs[] = {0.2,  0.235, 0.27, 0.235, 0.165, 0.04};
    const float bear_eyes_ys[] = {0.07, 0.035, 0.07, 0.105, 0.035, 0.14};
    const int bear_eyes_arrlen = 6;

    drawMirroredLines(bear_eyes_xs, bear_eyes_ys, bear_eyes_arrlen);

    // Draw the ears of the bear [0]    [1]   [2] ...
    const float bear_ear_xs[] =  {0,    0.27, 0.37, 0.43, 0.48, 0.4,   0.28,  0.1};
    const float bear_ear_ys[] =  {0.25, 0.36, 0.44, 0.44, 0.39, 0.18, -0.22, -0.43};
    const int bear_ear_arrlen = 8;

    drawMirroredLines(bear_ear_xs, bear_ear_ys, bear_ear_arrlen);

    // Draw the face of the bear  [0]   [1]    [2] ...
    const float bear_face_xs[] = {0.11, 0.21, 0.18};
    const float bear_face_ys[] = {0,   -0.1, -0.27};
    const int bear_face_arrlen = 3;

    drawMirroredLines(bear_face_xs, bear_face_ys, bear_face_arrlen);

    // Draw the mouth of the bear   [0]   [1]
    const float bear_mouth_xs[] = { 0.1,   0};
    const float bear_mouth_ys[] = {-0.27, -0.33};
    const int bear_mouth_arrlen = 2;

    drawMirroredLines(bear_mouth_xs, bear_mouth_ys, bear_mouth_arrlen);

    // Draw the nose of the bear  [0]     [1]    [2] ...
    const float bear_nose_xs[] = { 0,     0.1,   0.1};
    const float bear_nose_ys[] = {-0.19, -0.12, -0.09};
    const int bear_nose_arrlen = 3;

    drawMirroredLines(bear_nose_xs, bear_nose_ys, bear_nose_arrlen);

    glFlush();
}

// Initialization routine.
void setup(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-100.0, 100.0, -100.0, 100.0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key)
   {
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

// Main routine.
int main(int argc, char **argv)
{
   glutInit(&argc, argv);

   glutInitContextVersion(4, 3);
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
   glutInitWindowSize(720, 720);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("Bearly Geometric");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE;
   glewInit();

   setup();

   glutMainLoop();
}
