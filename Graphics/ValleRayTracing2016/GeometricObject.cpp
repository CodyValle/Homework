//author   not I but Dr Y added documentation
//date  many moons ago
//file GeometricObject.h

//ADT Geometric Object: data object is a generic geometric object that synthetically exists in the world
//                      operations: create, copy, destroy, does a ray hit the object and where, others

#include "Constants.h"
#include "GeometricObject.h"


// ---------------------------------------------------------------------- default constructor

GeometricObject::GeometricObject(void)
	: 	color(black)
{}


// ---------------------------------------------------------------------- copy constructor

GeometricObject::GeometricObject (const GeometricObject& object)
	: 	color(object.color)
{}	


// ---------------------------------------------------------------------- assignment operator

GeometricObject&														
GeometricObject::operator= (const GeometricObject& rhs) {
	
	if (this == &rhs)
		return (*this);
		
	color = rhs.color;

	return (*this);
}


// ---------------------------------------------------------------------- destructor

GeometricObject::~GeometricObject (void) {}
