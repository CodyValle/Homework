//@file Vector3D.cpp
//@date June 21, 2012
//@authors a merger suggested by Dr.Jenny Orr of three classes originally designed and coded by Kevin Suffern
//         with documentation and some changes by Dr. Kathie Yerion

// ADT Vector3D:
//    data object Vector3D is a point or vector in xyz coordinate system. Just like in mathematics,
//       whether the 3 coordinate values describe a point or describe a vector will depend on the context.
//    data structure; 3 doubles for the x, y, z coordinates
//    operations: creates, copy, destroys, assigns,
//                vector space operations of unary minus; binary addition, compound addition, subtraction, scalar multiplication (on the right),
//                Also, dot product, cross product, division by a scalar (on the right),
//                Distance between two points and the square of such distance,
//                normalize a vector and return the normalization of a vector,
//                find the length of a vector

#include <cmath>
using namespace std;
#include "Vector3D.h"

//@ creates the origin
//@ post object is the origin (0, 0, 0)
//@ usage Vector3D v;
Vector3D::Vector3D()
	 : x(0.0), y(0.0), z(0.0)
{}

//@ creates a vector with all coordinates the same
//@ post object is (a, a, a)
//@ usage Vector3D v(1.0);
Vector3D::Vector3D(double a)
	 : x(a), y(a), z(a)
{}

//@ creates a vector to have specified coordinates
//@ post object is (_x, _y, _z);
//@ usage Vector3D v(1,0, 2.0, 3.0);
Vector3D::Vector3D(double _x, double _y, double _z)
	: x(_x), y(_y), z(_z)
{}

//@ creates a opy of a vector
//@ pre rhsv exists
//@ post object exists and is a copy of rhsv
//@ usage  Vector3D v(w);
Vector3D::Vector3D(const Vector3D& rhsv)
	: x(rhsv.x), y(rhsv.y), z(rhsv.z)
{}


//@ destroys a vector
//@ pre object exists
//@ post object does not exist
Vector3D::~Vector3D ()
{}

//@ in all member functions below, pre is that the object exists

//@ copies a vector to another vector
//@ pre rhsv exists
//@ post object is a copy of rhsv
//@ usage    v.operator=(w);    or      v = w;
Vector3D& Vector3D::operator= (const Vector3D& rhs) {
	if (this == &rhs)
		return (*this);

	x = rhs.x; y = rhs.y; z = rhs.z;

	return (*this);
}

//@ finds the length of a vector
//@ post returns the length of object
//@ usage len = v.length()
double Vector3D::length() const {
	return (sqrt(x * x + y * y + z * z));
}

//@ converts a vector to its normalized self
//@ post  object is now a unit vector pointing in same direction as the object
//@ usage v.normalize();
void Vector3D::normalize() {
	double length = sqrt(x * x + y * y + z * z);
	x /= length; y /= length; z /= length;
}


//@ returns the normalized vector                            may not have doc correct
//@ post returns the normalized object
//@ usage w = v.hat();
Vector3D& Vector3D::hat() {
	double length = sqrt(x * x + y * y + z * z);
	x /= length; y /= length; z /= length;
	return (*this);
}

//@ finds the distance between two points
//@ pre p is assigned
//@ post returns the distance between the object and p
//@ usage d = s.distance(p);
double Vector3D::distance(const Vector3D& p) const{
   return sqrt((x - p.x) * (x - p.x)
			+ 	(y - p.y) * (y - p.y)
			+	(z - p.z) * (z - p.z));
}


