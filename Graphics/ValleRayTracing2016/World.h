//@file   World.h
//@ authors Original classes by Kevin Suffern then redesigned by Dr. Jenny Orr and Dr. Kathie Yerion
//@ June 22, 2012   Revised June 24 to include object and operations of the ViewPlane class

// ADT World
//    data object: a world of objects
//    operations: create, destroy, add an object to a list, build the world

#ifndef __WORLD__
#define __WORLD__

#include <vector>
//#include "ViewPlane.h"
#include "GeometricObject.h"

using namespace std;

class World {
	public:

        int 			horizontal_res;   					// horizontal image resolution on the viewplane
		int 			vertical_res;   					// vertical image resolution on the viewplane
		float			pixel_size;							// pixel size on the viewplane

		float			gamma;					        	// gamma correction factor
		float			inv_gamma;				        	// the inverse of the gamma correction factor
		bool			show_out_of_gamut;		        	// display red if RGBColor out of gamut

		RGBColor					background_color;
        vector<GeometricObject*>	objects;                // list of objects in the world

	public:
        //@ creates an empty world
		World();

        //@ destroys the world
		~World();

        //@ adds an object to a list
        //@pre object_ptr points to a geometric object
        //@post the geometric object has been added to the list
        //@usage  world_ptr -> add_object (sphere_ptr);
		void add_object(GeometricObject* object_ptr);

        //@ builds the world
        //@pre world is empty
        //@post world has objects in it
        //@usage world_ptr -> build();
		void build();

	private:

		void delete_objects(void);

};


// ------------------------------------------------------------------ add_object
inline void
World::add_object(GeometricObject* object_ptr) {
	objects.push_back(object_ptr);
}

#endif
