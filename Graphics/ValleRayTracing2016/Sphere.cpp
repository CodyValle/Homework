//author   not I but Dr Y added documentation
//date  many moons ago
//file GeometricObject.h

//ADT Sphere is-a Geometric Object:
//   data object is a sphere that synthetically exists in the world
//   operations: create, copy, destroy, copies, sets center and/or radius of a sphere, and
//               hit (does a ray hit the object and where)

#include "Sphere.h"
#include "math.h"

const double Sphere::kEpsilon = 0.001;


Sphere::Sphere(void)
	: 	GeometricObject(),
		center(0.0),
		radius(1.0)
{}


Sphere::Sphere(const Vector3D& c, double r)
	: 	GeometricObject(),
		center(c),
		radius(r)
{}

Sphere*
Sphere::clone(void) const {
	return (new Sphere(*this));
}


Sphere::Sphere (const Sphere& sphere)
	: 	GeometricObject(sphere),
		center(sphere.center),
		radius(sphere.radius)
{}


Sphere&
Sphere::operator= (const Sphere& rhs)
{
	if (this == &rhs)
		return (*this);

	GeometricObject::operator= (rhs);

	center 	= rhs.center;
	radius	= rhs.radius;

	return (*this);
}

Sphere::~Sphere(void) {}


bool
Sphere::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {
	double 		t;
	Vector3D    L                = ray.o - center;
	double 		a 		         = ray.d * ray.d;
	double 		b 		         = 2 * L * ray.d;
	double 		c 		         = L * L - radius * radius;
	double 		discriminant	= b * b - 4 * a * c;

	if (discriminant< 0.0)
		return false;
	else {
		double squareRootPart = sqrtf(discriminant);
		double denom          = 2 * a;
		t =  (-b - squareRootPart ) / denom;                           // smaller root

		if (t > kEpsilon) {
			tmin = t;
			sr.normal 	       =  (ray.o + ray.d * tmin - center ) / radius;                        // changed to mult by t on the right
			sr.local_hit_point =  ray.o + ray.d * tmin;//changed to multiplication on the right
			return true;               ;
		}

		t =  (-b + squareRootPart ) / denom;                      ;    // larger root

		if (t > kEpsilon) {
			tmin = t;
            sr.normal 	       =   (ray.o + ray.d * tmin - center) / radius;                        // changed to mult by t on the right
			sr.local_hit_point =  ray.o + ray.d * tmin;//changed to multiplication on the right
			return true;               ;
		}
	}

	return (false);
}







