//
// Addition of a light source to the car navigation program

#include "Angel.h"
#include <iostream>
using std::cout;

#include "MatrixStack.h"
#include "Shapes.h"
#include "Car.h"

//********  These are available as extern variables in Globals.h **************
GLuint  projection; // projection matrix uniform shader variable location
GLuint  model_view;  // model-view matrix uniform shader variable location
GLuint  model_color;  // model-view matrix uniform shader variable location

GLuint program;

MatrixStack mvMatrixStack;  // stores the movel view matrix stack

Shapes shapes;
//********  End extern variables in Globals.h **************

Car car;

vec4 light_position(0, 10, 10, 1);

// Camera projection transformation parameters
GLfloat  fovy = 45.0;  // Field-of-view in Y direction angle (in degrees)
GLfloat  aspect;       // Viewport aspect ratio
GLfloat  zNear = 0.5, zFar = 100.0;

// Camera location and orientation parameters
vec4 eyeStart = vec4( 0.0 ,20.0, 30.0 ,1.0); // initial camera location
vec4 eye = eyeStart; // camera location
vec4 VPN(0,.5,1,0);  // used as starting value for setting uvn
vec4 VUP(0,1,0,1);  // used as starting value for setting uvn
mat4 viewRotation;  // rotational part of matrix that transforms between World and Camera coordinates


float alpha = 1.0;  // used to control the amount of a turn during the flythrough
float animateAngle = 0;  // used for animation

// For controlling Mouse Actions:
enum  actions { TUMBLE, TRACK, DOLLY };
GLint   action;
int     xStart = 0.0, yStart = 0.0;
int t = 0;    // toggles the tumble point between the origin and fixed distance (d) from eye. Starts out at origin
float d = 30; // fixed distance of tumble point in front of camera

void lightSetUp();

//---------------------------------------------------------------------------- printControls

void printControls()
{
    cout << "\n\n************  Controls for Lights **************" << "\n";
    cout << "q or Q ............ quit" << "\n";
    cout << "f ................. move car forward" << "\n";
    cout << "b ................. move car backward" << "\n";
    cout << "r ................. reset parameters" << "\n";
    cout << "Up arrow .......... turn up" << "\n";
    cout << "Down arrow ........ turn down" << "\n";
    cout << "Right arrow ....... turn right" << "\n";
    cout << "Insert key ........ roll left" << "\n";
    cout << "Page Up ........... roll right" << "\n";
    cout << "Home key .......... move camera forward" << "\n";
    cout << "End key ........... move camera backward" << "\n";
    cout << "Right Mouse drag .. pan up/down/left/right" << "\n";
    cout << "Middle Mouse drag . zoom in/out" << "\n";
    cout << "Left Mouse drag ... tumble about origin or white cube" << "\n";
    cout << "t ................. toggles the tumble point between origin and fixed distance"  << "\n";
    cout << "                    in front of camera. Starts at origin." << "\n";
}

// calculate orthogonal vectors for the camera coordinate system: u, v, n from VPN and VUP
void calcUVN(vec4 VPN, vec4 VUP)
{
    vec4 n = normalize(VPN);
    vec4 u = vec4(cross(VUP,n),0);
    u = normalize(u);
    vec4 v = vec4(cross(n,u),0);
    viewRotation = mat4(u,v,n,vec4(0,0,0,1));
}

//---------------------------------------------------------------------------- init
// OpenGL initialization
void
init()
{
    calcUVN(VPN, VUP);

    program = InitShader( "vShader53.glsl", "fShader53.glsl" );  // gouraud shading
    glUseProgram( program );
    // Uniform variables: color and viewing parameters

    model_color = glGetUniformLocation( program, "model_color" );
    model_view = glGetUniformLocation( program, "model_view" );
    projection = glGetUniformLocation( program, "projection" );

    lightSetUp();

    shapes.createVAO(program);

    glUniform4fv( model_color, 1,vec4(1,1,1,1) ); // set color to white initially

    glLineWidth(1);
    glEnable( GL_DEPTH_TEST );
    glClearColor( 0.5, 0.5, 0.5, 1.0 );
    printControls();
}

void lightSetUp()
{
    // Initialize shader lighting parameters

    vec4 light_ambient( 1.0, 1.0, 1.0, 1.0 );
    vec4 light_diffuse( 1.0, 1.0, 1.0, 1.0 );
    vec4 light_specular( 1.0, 1.0, 1.0, 1.0 );

//  These are set to gray because the colors are set elsewhere.  One can use these as a
//  way of specifying ka, kd, and ks
    vec4 material_ambient( .4, .4, .4, 1.0 );
    vec4 material_diffuse( 1, 1, 1, 1.0 );
    vec4 material_specular( 0.8, 0.8, 0.8, 1.0 );
    float  material_shininess = 50.0;

    vec4 ambient_product = light_ambient * material_ambient;
    vec4 diffuse_product = light_diffuse * material_diffuse;
    vec4 specular_product = light_specular * material_specular;

    glUniform4fv( glGetUniformLocation(program, "AmbientProduct"),
                  1, ambient_product );
    glUniform4fv( glGetUniformLocation(program, "DiffuseProduct"),
                  1, diffuse_product );
    glUniform4fv( glGetUniformLocation(program, "SpecularProduct"),
                  1, specular_product );

    glUniform4fv( glGetUniformLocation(program, "LightPosition"),
                  1, light_position );

    glUniform1f( glGetUniformLocation(program, "Shininess"),
                 material_shininess );
}

//---------------------------------------------------------------------------- drawAxes
// Draw coordinate axes - x is red, y is green, and z is blue
void drawAxes(mat4 &mv)
{
    mvMatrixStack.pushMatrix(mv);

    mv = mv * Scale(.1,30,.1);
    mv = mv * Translate(0,.5,0);
    glUniformMatrix4fv( model_view, 1, GL_TRUE, mv );
    glUniform4fv( model_color, 1,vec4(0,1,0,1) );
    shapes.myCylinder.draw();               // y axis
    mv = mvMatrixStack.popMatrix();

    mvMatrixStack.pushMatrix(mv);
    mv = mv * RotateX(-90);
    mv = mv * Scale(.1,30,.1);
    mv = mv * Translate(0,.5,0);
    glUniformMatrix4fv( model_view, 1, GL_TRUE, mv );
    glUniform4fv( model_color, 1,vec4(0,0,1,1) );
    shapes.myCylinder.draw();   // z axis
    mv = mvMatrixStack.popMatrix();

    mvMatrixStack.pushMatrix(mv);
    mv = mv * RotateZ(-90);
    mv = mv * Scale(.1,30,.1);
    mv = mv * Translate(0,.5,0);
    glUniformMatrix4fv( model_view, 1, GL_TRUE, mv );
    glUniform4fv( model_color, 1,vec4(1,0,0,1) );
    shapes.myCylinder.draw();   // x axis
    mv = mvMatrixStack.popMatrix();
}

void
display( void )
{
    // clear the color and depth buffers
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // calculate and set camera projection parameters:
    mat4  p  = Perspective( fovy, aspect, zNear, zFar );
    glUniformMatrix4fv( projection, 1, GL_TRUE, p );

    mat4 mv;  // modelview matrix

    // Set the View matrix which controls the camera orientation and location
    mvMatrixStack.loadIdentity();

    mv =  viewRotation * Translate( -eye );

    mvMatrixStack.pushMatrix(mv);
    mv = mv * Translate(   RotateY(animateAngle) * light_position       );

    glUniformMatrix4fv( model_view, 1, GL_TRUE, mv );
    shapes.drawCube(vec4(1,1,0,1));   // draw a cube at the light position
    mv = mvMatrixStack.popMatrix();

    vec4 l_position = mv * light_position;  // translate to eye position for shader calculation

    glUniform4fv( glGetUniformLocation(program, "LightPosition"),
                  1, l_position );

    drawAxes(mv);
    car.drawCar(mv);

    // Draw the ground
    mv = mv * Translate(0,-.1,0);
    mv = mv * Scale(20,.2,20);
    glUniformMatrix4fv( model_view, 1, GL_TRUE, mv );
    shapes.drawCube(vec4(.6,.4,.3,1));

    glBindVertexArray( 0 );
    glutSwapBuffers();
}

//---------------------------------------------------------------------------- keyboard

void
keyboard( unsigned char key, int x, int y )
{
    switch( key )
    {
    case 033: // Escape Key
    case 'q':
    case 'Q':
        exit( EXIT_SUCCESS );
        break;
    case 'j':     //
        light_position[0] += 5;
        break;
    case 'k':     //
        light_position[0] -= 5;
        break;
    case 'u':     //
        light_position[2] += 5;
        break;
    case 'i':     //
        light_position[2] -= 5;
        break;
    case 'f':     // drive car forward
        car.wheelAngle += 5;
        car.xLoc -= 2 * M_PI * 2. * 5 / 360.;
        break;
    case 'b':     // drive car forward
        car.wheelAngle -= 5;
        car.xLoc += 2 * M_PI * 2. * 5 / 360.;
        break;
    case 't':     // toggle tumblepoint
        t = (t + 1) % 2;
        break;
    case 'r':     // reset
        t = 0;  // tumble point at origin
        car.wheelAngle = 0;
        car.xLoc = 0;
        eye = eyeStart; // camera location
        calcUVN(VPN,VUP);
        xStart = 0.0;
        yStart = 0.0;
        break;
    }

    glutPostRedisplay();
}

//---------------------------------------------------------------------------- keySpecial
void
keySpecial( int key, int x, int y )
{
    switch( key )
    {
    case GLUT_KEY_UP:
        viewRotation = RotateX(alpha) * viewRotation;
        break;
    case GLUT_KEY_DOWN:
        viewRotation = RotateX(-alpha) * viewRotation;
        break;
    case GLUT_KEY_LEFT:
        viewRotation = RotateY(-alpha) * viewRotation;
        break;
    case GLUT_KEY_RIGHT:
        viewRotation = RotateY(alpha) * viewRotation;
        break;
    case GLUT_KEY_HOME:
        eye = eye - alpha * viewRotation[2];
        break;
    case GLUT_KEY_END:
        eye = eye + alpha * viewRotation[2];
        break;
    case GLUT_KEY_INSERT:
        viewRotation = RotateZ(alpha) * viewRotation;
        break;
    case  GLUT_KEY_PAGE_UP :
        viewRotation = RotateZ(-alpha) * viewRotation;
        break;
    }

    glutPostRedisplay();
}

//---------------------------------------------------------------------------- mouse
void
mouse( GLint button, GLint state, GLint x, GLint y )
{
    //static GLint buttons_down = 0;

    if (state == GLUT_DOWN)
    {
        // cout << "mouse: button = " << button << "  state = " << state << "  x,y = " << x << "," << y << "\n";

        switch (button)
        {
        case GLUT_LEFT_BUTTON:
            // cout << "     mouse: GLUT_LEFT_BUTTON - TUMBLE\n";
            action = TUMBLE;
            xStart = x;
            yStart = y;
            break;
        case GLUT_MIDDLE_BUTTON:
            //  cout << "     mouse: GLUT_MIDDLE_BUTTON - DOLLY\n";
            action = DOLLY;
            xStart = x;
            yStart = y;
            break;
        case GLUT_RIGHT_BUTTON:
            //  cout << "     mouse: GLUT_RIGHT_BUTTON - TRACK\n";
            action = TRACK;
            xStart = x;
            yStart = y;
            break;
        }
    }
    if (state == GLUT_UP)
    {
        // printControls();
    }
}

//---------------------------------------------------------------------------- tumble
// tumble about to location tumblePoint in World Coordinate System.
void tumble(mat4 rx, mat4 ry, vec4 tumblePoint)
{
   // not here for obvious reasons
}

//---------------------------------------------------------------------------- motion
void
motion( GLint x, GLint y )
{
    float dx, dy;
    mat4 ry, rx;
    vec4 tumblePoint;

    switch (action)
    {
    case TUMBLE:
        dx = 0.05 * (x - xStart);
        dy = 0.05 * (y - yStart);

        ry = RotateY(10 * dx);
        rx = RotateX(10 * dy);

        // tumble about a point tumblePoint in WCS. Two options currently.
        if (t==0)   // tumble about origin in  WCS
        {
            tumblePoint =  vec4(0,0,0,1);
        }
        else        // tumble about fixed distance in front of camera in  WCS
        {
            tumblePoint =   eye - d * viewRotation[2];
            tumblePoint[3] = 1;
        }
        tumble(rx, ry, tumblePoint);   //  <----  NEED TO IMPLEMENT this method!!!

        xStart = x;
        yStart = y;
        break;
    case TRACK:  // PAN
        dx = -0.05 * (x - xStart);
        dy = 0.05 * (y - yStart);

        eye = eye + dx * viewRotation[0] + dy * viewRotation[1];
      //missing code
        xStart = x;
        yStart = y;
        break;
    case DOLLY:
        dy = 0.05*(y - yStart);
        dx = 0.05*(x - xStart);
       // missing code
        xStart = x;
        yStart = y;
        break;
    default:
        printf("unknown action %d\n", action);
    }

    glutPostRedisplay();
}


//---------------------------------------------------------------------------- reshape

void
reshape( int width, int height )
{
    glViewport( 0, 0, width, height );

    aspect = GLfloat(width)/height;
}

void idle()
{
    animateAngle += .01;
    if (animateAngle > 360) animateAngle = animateAngle - 360;

    glutPostRedisplay();
}

//---------------------------------------------------------------------------- main

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
    glutInitContextVersion( 3, 3 );
    glutInitContextProfile( GLUT_FORWARD_COMPATIBLE );
    glutCreateWindow( "Camera Navigation with Light Source" );

    glewInit();

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutSpecialFunc(keySpecial);
    glutMouseFunc( mouse );
    glutMotionFunc( motion );
    glutReshapeFunc( reshape );
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}
