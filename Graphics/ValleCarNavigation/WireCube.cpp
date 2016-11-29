//  Generates the vertices, colors, and normals of a wireframe unit cube
// centered at origin, sides aligned with axes.
//
//    v5----- v6
//   /|      /|
//  v1------v2|              ^ y
//  | |     | |              |
//  | |v4---|-|v7            -->x
//  |/      |/              /
//  v0------v3              z


#include "WireCube.h"
#include <iostream>
using std::cout;

vec4 black = vec4(0,0,0,1);  // black

WireCube::WireCube()
{
    Index = 0;

    float v = .50001;
    vertices[0] = vec4( -v, -v,  v, 1.0 );   // v0
    vertices[1] = vec4( -v,  v, v, 1.0 );   // v1
    vertices[2] = vec4(  v,  v,  v, 1.0 );   // v2
    vertices[3] = vec4(  v, -v,  v, 1.0 );   // v3
    vertices[4] = vec4( -v, -v, -v, 1.0 );   // v4
    vertices[5] = vec4( -v,  v, -v, 1.0 );   // v5
    vertices[6] = vec4(  v,  v, -v, 1.0 );   // v6
    vertices[7] = vec4(  v, -v, -v, 1.0 );    // v7



    quad( 1, 0, 3, 2 );   // front (+z) face
    quad( 2, 3, 7, 6 );   // +x
    quad( 3, 0, 4, 7 );   // -y
    quad( 6, 5, 1, 2 );   // +y
    quad( 4, 5, 6, 7 );   // -z
    quad( 5, 4, 0, 1 );   // -x

    numVertices = Index;
}

void
WireCube::quad( int a, int b, int c, int d)
{

    colors[Index] = black;  // black
    points[Index] = vertices[a];
    Index++;
    colors[Index] = black;  // black
    points[Index] = vertices[b];
    Index++;
    colors[Index] = black;  // black
    points[Index] = vertices[b];
    Index++;
    colors[Index] = black;  // black
    points[Index] = vertices[c];
    Index++;
    colors[Index] = black;  // black
    points[Index] = vertices[c];
    Index++;
    colors[Index] = black;  // black
    points[Index] = vertices[d];
    Index++;
    colors[Index] = black;  // black
    points[Index] = vertices[d];
    Index++;
    colors[Index] = black;  // black
    points[Index] = vertices[a];
    Index++;
}

// Assume drawn with GL_LINES
WireCube::WireCube(const WireCube& other)
{
    for (int i=0; i < 48; i++)
    {
        points[i]=other.points[i];
        colors[i]=other.colors[i];
    }
    numVertices = other.numVertices;
}

WireCube&
WireCube::operator=(const WireCube& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
       for (int i=0; i < 48; i++)
    {
        points[i]=rhs.points[i];
        colors[i]=rhs.colors[i];
    }
    numVertices = rhs.numVertices;
}

WireCube::~WireCube()
{}


void
WireCube::createVAO(GLint _vao, GLint program )
{
    vao = _vao;
    glBindVertexArray( vao );
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );

    int csize = sizeof(points);
    //cout << "csize " << csize << "\n";
    glBufferData( GL_ARRAY_BUFFER, 2*csize,
                  NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, csize, points );
    glBufferSubData( GL_ARRAY_BUFFER, csize, csize, colors );
    //glBufferSubData( GL_ARRAY_BUFFER, 2*csize, csize, normals );
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

void WireCube::draw() {
    glBindVertexArray( vao );
    glDrawArrays( GL_LINES, 0, numVertices );
    glBindVertexArray( 0 );
}




