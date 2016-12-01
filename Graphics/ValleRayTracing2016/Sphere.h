//author   not I but Dr Y added documentation
//date  many moons ago
//file GeometricObject.h

//ADT Sphere is-a Geometric Object: 
//   data object is a sphere that synthetically exists in the world
//   operations: create, copy, destroy, copies, sets center and/or radius of a sphere, and 
//               hit (does a ray hit the object and where)

#ifndef __SPHERE__
#define __SPHERE__

#include "GeometricObject.h"
#include "Vector3D.h"

class Sphere: public GeometricObject {

	public:

		Sphere(void);   	

		Sphere(const Vector3D& center, double r);	

		Sphere(const Sphere& sphere); 

		virtual Sphere* 			
		clone(void) const;

		virtual											
		~Sphere(void);

		Sphere& 											
		operator= (const Sphere& sphere);

		void
		set_center(const Vector3D& c);

		void
		set_center(const double x, const double y, const double z);

		void
		set_radius(const double r);

		virtual bool
		hit(const Ray& ray, double& tmin, ShadeRec& sr) const;	

	private:

		Vector3D 	center;   			// sphere's center given as a point
		double 		radius;				// the radius of a sphere

		static const double kEpsilon;   // for shadows and secondary rays
};



inline void
Sphere::set_center(const Vector3D& c) {
	center = c;
}

inline void
Sphere::set_center(const double x, const double y, const double z) {
	center.x = x;
	center.y = y;
	center.z = z;
}

inline void
Sphere::set_radius(const double r) {
	radius = r;
}

#endif
