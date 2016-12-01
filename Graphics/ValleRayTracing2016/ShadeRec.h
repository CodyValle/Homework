//author   not I but Dr Y added documentation
//date    ??
//file ShadeRec.h

//ADT Shade Record: data object: Information on the hit point of a ray 
//                                   (did a ray hit, where did it hit in world coordinates, 
//                                     a normal at the hit point, what color should the pixel be)

#ifndef __SHADE_REC__
#define __SHADE_REC__

// this file contains the declaration of the class ShadeRec

class RenderEngine;	// only need a forward class declaration as the World data member is a reference

#include "Vector3D.h"
#include "RGBColor.h"

class ShadeRec {
	public:

		bool				hit_an_object;		// did the ray hit an object?
		Vector3D			local_hit_point;	// world coordinates of hit point
		Vector3D			normal;				// normal at hit point
		RGBColor			color;				

		ShadeRec();				            // constructor

		ShadeRec(const ShadeRec& sr);		// copy constructor
};

#endif
