//@file Vector3D.h
//@date June 21, 2012
//@authors a merger suggested by Dr. Jenny Orr of three classes  originally designed and coded by Kevin Suffern
//         with documentation and some changes by Dr. Kathie Yerion

// ADT Vector3D:
//    data object Vector3D is a point or vector in xyz coordinate system. Just like in mathematics,
//       whether the 3 coordinate values describe a point or describe a vector will depend on the context.
//    operations: creates, copy, destroys, assigns,
//                vector space operations of unary minus; binary addition, compound addition, subtraction, scalar multiplication (on the right),
//                Also, dot product, cross product, division by a scalar (on the right),
//                Distance between two points and the square of such distance,
//                normalize a vector and return the normalization of a vector,
//                find the length of a vector

#ifndef __VECTOR_3D__
#define __VECTOR_3D__

class Vector3D {
   public:

      double x, y, z;

   public:
      //@ creates the origin
      //@ post object is the origin (0, 0, 0)
      //@ usage Vector3D v;
      Vector3D();

      //@ creates a vector with all coordinates the same
      //@ post object is (a, a, a)
      //@ usage Vector3D v(1.0);
      Vector3D(double a);

      //@ creates a vector to have specified coordinates
      //@ post object is (_x, _y, _z);
      //@ usage Vector3D v(1,0, 2.0, 3.0);
      Vector3D(double _x, double _y, double _z);

      //@ creates a copy of a vector
      //@ pre rhsv exists
      //@ post object exists and is a copy of rhsv
      //@ usage  Vector3D v(w);
      Vector3D(const Vector3D& rhsv);

      //@ destroys a vector
      //@ pre object exists
      //@ post object does not exist
      ~Vector3D ();

      //@ in all member functions below, pre is that the object exists

      //@ copies a vector to another vector
      //@ pre rhsv exists
      //@ post object is a copy of rhsv
      //@ usage    v.operator=(w);    or      v = w;
      Vector3D& operator= (const Vector3D& rhsv);

      //@ returns the unary minus of a vector
      //@ pre object exists as some (x,y,z)
      //@ post returns object  (-x, -y, -z)
      //@ usage v = -w;
      Vector3D	operator- () const;

      //@ finds the length of a vector
      //@ post returns the length of object
      //@ usage len = v.length()
      double length() const;

      //@ finds the square of the length of a vector
      //@ post returns the square of the length of a vector
      //@ usage  len2 = v.len_squared();
      double len_squared() const;

      //@ returns the scalar multiple of a vector
      //@ pre scalar a is assigned
      //@ post returns the vector that is the object multiplied on the right by a
      //@ usage v = w * a;
      Vector3D	operator* (const double a) const;

      //@ returns the vector that is the scalar multiple of a reciprocal of a value
      //@ pre scalar a is assigned
      //@ post returns the vector that is the object mutliplied on the right by 1/a
      //@ usage v = w / a;
      Vector3D	operator/ (const double a) const;

      //@ returns the addition of the object with v
      //@ pre v exists
      //@ post returns object + v
      //@ usage w = u + v;
      Vector3D	operator+ (const Vector3D& v) const;

      //@ allows for compound addition operations
      //@ pre v exists
      //@ post returns the address of a vector
      //@ usage w += v += u;
      Vector3D& operator+= (const Vector3D& v);

      //@ returns the subtraction of the object with v
      //@ pre v exists
      //@ post returns object - v
      //@ usage w = u - v;
      Vector3D operator- (const Vector3D& v) const;

      //@ finds the dot product of two vectors
      //@ pre v exists
      //@ post returns the dot product object dot v
      //@ usage dotProduct = w * v;
      double operator* (const Vector3D& v) const;

      //@ finds the cross product of two vectors
      //@ pre v exists
      //@ post returns the vector which is the cross product of object with v
      //@ usage u = w ^ v;
      Vector3D operator^ (const Vector3D& v) const;

      //@ converts a vector to its normalized self
      //@ post  object is now a unit vector pointing in same direction as the object
      //@ usage v.normalize();
      void normalize();

      //@ returns the normalized vector
      //@ post returns the normalized object
      //@ usage w = v.hat();
      Vector3D& hat();

      //@ finds the square of the distance between two points
      //@ pre p is assigned
      //@ post returns the square of the distance between and object and p
      //@ usage d2 = p1.d_squared(p0);
      double d_squared(const Vector3D& p) const;

      //@ finds the distance between two points
      //@ pre p is assigned
      //@ post returns the distance between the object and p
      //@ usage d = s.distance(p);
      double distance(const Vector3D& p) const;

      //@
      //@ pre
      //@ post
      //@ usage
};


// inlined member functions for better efficiency

// ------------------------------------------------------------------------ unary minus
// this does not change the current vector
// this allows ShadeRec objects to be declared as constant arguments in many shading
// functions that reverse the direction of a ray that's stored in the ShadeRec object
inline Vector3D Vector3D::operator- () const {
	return (Vector3D(-x, -y, -z));
}

// ---------------------------------------------------------------------  len_squared
inline double Vector3D::len_squared() const {
	return (x * x + y * y + z * z);
}

// ----------------------------------------------------------------------- operator*
// multiplication by a scalar on the right
inline Vector3D Vector3D::operator* (const double a) const {
	return (Vector3D(x * a, y * a, z * a));
}

// ----------------------------------------------------------------------- operator/
// division by a scalar
inline Vector3D Vector3D::operator/ (const double a) const {
	return (Vector3D(x / a, y / a, z / a));
}

// ----------------------------------------------------------------------- operator+
// binary addition
inline Vector3D Vector3D::operator+ (const Vector3D& v) const {
	return (Vector3D(x + v.x, y + v.y, z + v.z));
}

// ----------------------------------------------------------------------- operator-
// binary subtraction
inline Vector3D Vector3D::operator- (const Vector3D& v) const {
	return (Vector3D(x - v.x, y - v.y, z - v.z));
}

// ----------------------------------------------------------------------- operator*
// dot product
inline double Vector3D::operator* (const Vector3D& v) const {
	return (x * v.x + y * v.y + z * v.z);
}

// ----------------------------------------------------------------------- operator^
// cross product
inline Vector3D Vector3D::operator^ (const Vector3D& v) const {
	return (Vector3D(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x));
}

// ---------------------------------------------------------------------  operator+=
// compound addition

inline Vector3D& Vector3D::operator+= (const Vector3D& v) {
	x += v.x; y += v.y; z += v.z;
	return (*this);
}

// -------------------------------------------------------------------- d_squared
// distance squared between two points
inline double Vector3D::d_squared(const Vector3D& p) const {
	return (	(x - p.x) * (x - p.x)
			+ 	(y - p.y) * (y - p.y)
			+	(z - p.z) * (z - p.z) );
}

// inlined non-member function
// ----------------------------------------------------------------------- operator*
// multiplication by a double on the left
// usage w = operator* (a, v);
Vector3D operator* (const double a, const Vector3D& v);    // does this need to be declared a friend

inline Vector3D operator* (const double a, const Vector3D& v) {
	return (Vector3D(a * v.x, a * v.y, a * v.z));
}


#endif


