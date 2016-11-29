// creates a Wireframe version of a Steiner Suface - http://www.ipfw.edu/math/Coffman/steinersurface.html

#include "WireSteiner.h"
#include <iostream>
using std::cout;

WireSteiner::WireSteiner(int rDivisions, int tDivisions)
{
    //cout << "******************  Creating WireSteiner" << "\n";

    numVertices = rDivisions*tDivisions * 8;

    points = new vec4[numVertices];
    colors = new vec4[numVertices];
    vec4 black(0,0,0,1);



    GLfloat dr = 1./rDivisions;
    GLfloat dt = 2*M_PI/tDivisions;
    int index = 0;
    for (int ri=0 ; ri < rDivisions; ri++) {
        for (int ti=0 ; ti < tDivisions; ti++) {
            GLfloat r = ri*dr;
            GLfloat t = ti*dt;
            vec4 v0(r*r*sin(t)*cos(t), r*sin(t)*pow(1-r*r,.5), r*cos(t)*pow(1-r*r,.5),1) ;
            r = (ri+1)*dr;
            vec4 v1(r*r*sin(t)*cos(t), r*sin(t)*pow(1-r*r,.5), r*cos(t)*pow(1-r*r,.5),1) ;
            t = (ti+1)*dt;
            vec4 v2(r*r*sin(t)*cos(t), r*sin(t)*pow(1-r*r,.5), r*cos(t)*pow(1-r*r,.5),1) ;
            r = ri*dr;
            vec4 v3(r*r*sin(t)*cos(t), r*sin(t)*pow(1-r*r,.5), r*cos(t)*pow(1-r*r,.5),1) ;


            points[index]= v0;
            colors[index]= black;
            index++;
            points[index]= v1;
            colors[index]= black;

            points[index]= v1;
            colors[index]= black;
            index++;

            points[index]= v2;
            colors[index]= black;
            index++;
            points[index]= v2;
            colors[index]= black;
            index++;
            points[index]= v3;
            colors[index]= black;
            index++;

            points[index]= v3;
            colors[index]= black;
            index++;
            points[index]= v3;
            colors[index]= black;
            index++;
            points[index]= v0;
            colors[index]= black;
            index++;

        }
    }

//    cout << "WireSteiner: number of vertices = " << index << "\n";
//    for (int i=0; i < index; i++)
//    {
//        cout << "pts " << i << ": " << points[i] << "\n";
//    }
}

WireSteiner::~WireSteiner()
{
    delete[] points;
    delete[] colors;
}

WireSteiner::WireSteiner(const WireSteiner& other)
{
    for (int i=0; i < numVertices; i++)
    {
        points[i]=other.points[i];
        colors[i]=other.colors[i];
    }
    numVertices = other.numVertices;
}

WireSteiner& WireSteiner::operator=(const WireSteiner& rhs)
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
WireSteiner::createVAO(GLint _vao, GLint program )
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

void WireSteiner::draw() {
    glBindVertexArray( vao );
    glDrawArrays( GL_LINES, 0, numVertices );
    glBindVertexArray( 0 );
}

