//author   not I but Dr Y added documentation
//date  many moons ago
//file GeometricObject.h

//ADT Geometric Object: data object is a generic geometric object that synthetically exists in the world
//                      operations: create, copy, destroy, does a ray hit the object and where, others

#ifndef __GEOMETRIC_OBJECT__
#define __GEOMETRIC_OBJECT__

#include "RGBColor.h"
#include "Ray.h"
#include "ShadeRec.h"
#include "Constants.h"

class GeometricObject {

	public:

		GeometricObject(void);									// default constructor

		GeometricObject(const GeometricObject& object);			// copy constructor

		virtual GeometricObject*								// virtual copy constructor
		clone(void) const = 0;

		virtual 												// destructor
		~GeometricObject (void);

		//determines if a ray hits the geometric object and where
		//pre ray has already been assigned
		//post if ray hits the object then t is the value of the ray where it hits the object
		//     and sr is shading information and returns true  else returns false
		//usage if (obj.hit(ray, t, sr))
		//note: the code for this function depends on what type the geometric object is
		//      THUS, it can only be defined in the derived class for a specific geometric object
		virtual bool
		hit(const Ray& ray, double& t, ShadeRec& sr) const = 0; //


		// the following three functions are only required for Chapter 3

		void
		set_color(const RGBColor& c);

		void
		set_color(const float r, const float g, const float b);

		RGBColor
		get_color(void);


	protected:

		RGBColor   color;						// only used for Bare Bones ray tracing

		GeometricObject&						// assignment operator
		operator= (const GeometricObject& rhs);
};


// --------------------------------------------------------------------  set_colour

inline void
GeometricObject::set_color(const RGBColor& c) {
	color = c;
}

// --------------------------------------------------------------------  set_colour

inline void
GeometricObject::set_color(const float r, const float g, const float b) {
	color.r = r;
	color.b = b;
	color.g = g;
}

// --------------------------------------------------------------------  get_colour

inline RGBColor
GeometricObject::get_color(void) {
	return (color);
}

#endif
