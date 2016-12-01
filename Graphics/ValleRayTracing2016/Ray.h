//author   not I   but documentation added by Dr Y
//date    before
//file Ray.h
//
//ADT Ray: data object: a ray with operations: 3 constructors, a copy, a destructor

#ifndef __RAY__
#define __RAY__

#include "Vector3D.h"

class Ray {
	public:

		Vector3D		o;  	// origin
		Vector3D		d; 	// direction

		//creates a ray starting at the origin of the world coordinate system with
		//    direction of the positive z-axis
		//post the ray with origin (0,0,0) and direction (0,0, 1) exists
		//usage  Ray r;
		Ray();

		//creates a ray starting at vector origin with direction dir
		//pre origin and dir are assigned
		//post the ray from origin in direction dir exists
		//usage  Ray r(Vector(1.0, 2.0, 3.0), Vector(0.0, 0.0, -1.0));
		Ray(const Vector3D& origin, const Vector3D& dir);

		//copies ray
		//pre ray is an assigned Ray object
		//post a new ray object with the same origin and direction as ray exists
		//usage Ray r(anotherRay);
		Ray(const Ray& ray);

		//copies rhsray
		//pre rhsray is an assigned Ray object. Ray object exists
		//post origin and direction of rhsray are copies to an existing Ray object
		//usage ray1 = ray2;
		Ray&
		operator= (const Ray& rhsray);

		//releases memory for the ray
		//pre Ray object exists
		//post Ray object does not exist
		~Ray();
};

#endif
