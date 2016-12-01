// creates a disk of radius 1 at the origin with normal facing along y+

#include "Disk.h"
#include <iostream>
using std::cout;

Disk::Disk(int slices, float radius)
{
    numVertices = slices + 2;

    points = new vec4[numVertices];
    colors = new vec4[numVertices];
    normals = new vec4[numVertices];
    vec4 normal(0,1,0,0);
    vec4 color[3] = {vec4(1,0,1,1),vec4(.5,0,1,1)};
    vec4 center(0,0,0,1);
    float dAngle = 2*M_PI/slices;

    int index = 0;

    // center vertex
    points[index]= center;
    colors[index]= color[index];
    normals[index] = normal;

    for (int i = 0; i < slices; i++)
    {
        index++;
        points[index]= vec4(radius*cos(i*dAngle),0,radius*sin((i*dAngle)),1);
        colors[index]= color[i%2];
        normals[index] = normal;
    }

    index++;
    points[index]= points[1];
    colors[index]= color[1];
    normals[index] = normal;

/*
    cout << "Disk: number of vertices = " << index << "\n";
    for (int i=0; i < index; i++)
    {
        cout << "pts " << i << ": " << points[i] << "\n";
    }
    */
}

Disk::~Disk()
{
    delete[] points;
    delete[] colors;
    delete[] normals;
}

Disk::Disk(const Disk& other)
{
    for (int i=0; i < numVertices; i++)
    {
        points[i]=other.points[i];
        colors[i]=other.colors[i];
        normals[i]=other.normals[i];
    }
    numVertices = other.numVertices;
}

Disk& Disk::operator=(const Disk& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;

    for (int i=0; i < numVertices; i++)
    {
        points[i]=rhs.points[i];
        colors[i]=rhs.colors[i];
        normals[i]=rhs.normals[i];
    }
    numVertices = rhs.numVertices;
}

void
Disk::createVAO(GLint _vao, GLint program )
{
    vao = _vao;
    glBindVertexArray( vao );
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );

    int csize = sizeof(*points) * numVertices;
    glBufferData( GL_ARRAY_BUFFER, 3*csize,
                  NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, csize, *points );
    glBufferSubData( GL_ARRAY_BUFFER, csize, csize, *colors );
    glBufferSubData( GL_ARRAY_BUFFER, 2*csize, csize, *normals );
    // set up shader variables
    GLint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );
    GLint vColor = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(csize) );

    GLint vNormal = glGetAttribLocation( program, "vNormal" );
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(2*csize) );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
}

void Disk::draw() {
    glBindVertexArray( vao );
    glDrawArrays( GL_TRIANGLE_FAN, 0, numVertices );
    glBindVertexArray( 0 );
}

