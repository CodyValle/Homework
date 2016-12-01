#include "Ray.h"

//creates a ray starting at the origin of the world coordinate system with
//    direction of the positive z-axis
//post the ray with origin (0,0,0) and direction (0,0, 1) exists
//usage  Ray r;
Ray::Ray (void)
	: 	o(0.0),
		d(0.0, 0.0, 1.0)
{}

//creates a ray starting at vector origin with direction dir
//pre origin and dir are assigned
//post the ray from origin in direction dir exists
//usage  Ray r(Vector(1.0, 2.0, 3.0), Vector(0.0, 0.0, -1.0));
Ray::Ray (const Vector3D& origin, const Vector3D& dir)
	: 	o(origin),
		d(dir)
{}

//copies ray
//pre ray is an assigned Ray object
//post a new ray object with the same origin and direction as ray exists
//usage Ray r(anotherRay);
Ray::Ray (const Ray& ray)
	: 	o(ray.o),
		d(ray.d)
{}

//copies rhsray
//pre rhsray is an assigned Ray object. Ray object exists
//post origin and direction of rhsray are copies to an existing Ray object
//usage ray1 = ray2;
Ray&
Ray::operator= (const Ray& rhs) {

	if (this == &rhs)
		return (*this);

	o = rhs.o;
	d = rhs.d;

	return (*this);
}

//releases memory for the ray
//pre Ray object exists
//post Ray object does not exist
Ray::~Ray () {}



