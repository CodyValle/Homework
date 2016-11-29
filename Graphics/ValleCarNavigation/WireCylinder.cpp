// creates a Wireframe Cylinder of radius r and height h  along the y+ axis and centered at the origin

#include "WireCylinder.h"
#include <iostream>
using std::cout;

WireCylinder::WireCylinder(int slices, float radius, float height)
{
    numVertices = 6*slices;  // using a GL_TRIANGLE_STRIP

    points = new vec4[numVertices];
    colors = new vec4[numVertices];
    vec4 black(0,0,0,1);
    float dAngle = 2*M_PI/slices;

    vec4 verticesTop[slices];
    vec4 verticesBot[slices];
    for (int i=0 ; i < slices; i++) {
       verticesTop[i]= vec4(radius*cos(i*dAngle)+ .001,  height/2,radius*sin((i*dAngle))+.001,1) ;
       verticesBot[i]= vec4(radius*cos(i*dAngle)+ .001,  -height/2,radius*sin((i*dAngle))+.001,1) ;
    }

    int index = 0;

    for (int i = 0; i < slices ; i++)
    {
        // top
        points[index]= verticesTop[i];
        colors[index]= black;
        index++;
        points[index]= verticesTop[(i+1)%slices];
        colors[index]= black;
        index++;

        // bottom
        points[index]= verticesBot[i];
        colors[index]= black;
        index++;
        points[index]= verticesBot[(i+1)%slices];
        colors[index]= black;
        index++;

        //sides
        points[index]= verticesTop[i];
        colors[index]= black;
        index++;
        points[index]= verticesBot[i];
        colors[index]= black;
        index++;
    }
    /*
    cout << "WireCylinder: number of vertices = " << index << "\n";
    for (int i=0; i < numVertices; i++)
    {
        cout << "pts " << i << ": " << points[i] << "\n";
    }
    */
}

WireCylinder::~WireCylinder()
{
    delete[] points;
    delete[] colors;
}

WireCylinder::WireCylinder(const WireCylinder& other)
{
    for (int i=0; i < numVertices; i++)
    {
        points[i]=other.points[i];
        colors[i]=other.colors[i];
    }
    numVertices = other.numVertices;
}

WireCylinder& WireCylinder::operator=(const WireCylinder& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;

    for (int i=0; i < numVertices; i++)
    {
        points[i]=rhs.points[i];
        colors[i]=rhs.colors[i];
    }
    numVertices = rhs.numVertices;
}

void
WireCylinder::createVAO(GLint _vao, GLint program )
{
    vao = _vao;
    glBindVertexArray( vao );
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );

     int csize = sizeof((*points))* numVertices;
    glBufferData( GL_ARRAY_BUFFER, 2*csize,
                  NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, csize, *points );
    glBufferSubData( GL_ARRAY_BUFFER, csize, csize, *colors );
    // set up shader variables
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );
    GLuint vColor = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(csize) );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );

}

void WireCylinder::draw() {
    glBindVertexArray( vao );
    glDrawArrays( GL_LINES, 0, numVertices );
    glBindVertexArray( 0 );
}
