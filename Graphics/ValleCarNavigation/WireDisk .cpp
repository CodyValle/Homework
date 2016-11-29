// creates a wireframe disk of radius r at the origin with normal facing along y+

#include "WireDisk.h"
#include <iostream>
using std::cout;

WireDisk::WireDisk(int slices, float radius)
{
    //cout << "******************  Creating WireDisk" << "\n";
    //numVertices = 2*slices;  // no spokes
    numVertices = 4*slices;

    points = new vec4[numVertices];
    colors = new vec4[numVertices];
    vec4 black(0,0,0,1);
    vec4 center(0,0,0,1);
    float dAngle = 2*M_PI/slices;


    vec4 vertices[slices];
    for (int i=0 ; i < slices; i++) {
       vertices[i]= vec4(radius*cos(i*dAngle)+ .001,  0,radius*sin((i*dAngle))+.001,1) ;
       //cout << "wiredisk vertex " << vertices[i] <<"\n";
    }
    // outside ring
    int index = 0;
    for (int i = 0; i < slices; i++)
    {
        points[index]= vertices[i];
        colors[index]= black;
        index++;
        points[index]= vertices[(i+1)%slices];
        colors[index]= black;
        index++;
    }

    // spokes
    for (int i = 0; i < slices; i++)
    {
        points[index]= vertices[i];
        colors[index]= black;
        index++;
        points[index]= center;
        colors[index]= black;
        index++;
    }
    /*
    cout << "WireDisk: number of vertices = " << index << "\n";
    for (int i=0; i < index; i++)
    {
        cout << "pts " << i << ": " << points[i] << "\n";
    }
    */
}

WireDisk::~WireDisk()
{
    delete[] points;
    delete[] colors;
}

WireDisk::WireDisk(const WireDisk& other)
{
    for (int i=0; i < numVertices; i++)
    {
        points[i]=other.points[i];
        colors[i]=other.colors[i];
    }
    numVertices = other.numVertices;
}

WireDisk& WireDisk::operator=(const WireDisk& rhs)
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
WireDisk::createVAO(GLint _vao, GLint program )
{
    vao = _vao;
    glBindVertexArray( vao );
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );

    int csize = sizeof(*points) * numVertices;
    glBufferData( GL_ARRAY_BUFFER, 2*csize,
                  NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, csize, *points );
    glBufferSubData( GL_ARRAY_BUFFER, csize, csize, *colors );
    //glBufferSubData( GL_ARRAY_BUFFER, 2*csize, csize, *normals );
    // set up shader variables
    GLint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );
    GLint vColor = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(csize) );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
}



void WireDisk::draw() {
    glBindVertexArray( vao );
    glDrawArrays( GL_LINES, 0, numVertices );
    glBindVertexArray( 0 );
}

