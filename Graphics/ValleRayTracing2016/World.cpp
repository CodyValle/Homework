//@file   World.cpp
//@ authors Original classes by ? then redesigned by Dr. Jenny Orr and Dr. Kathie Yerion
//@ June 22, 2012

// ADT World
//    data object: a world of objects
//    data members: a viewplane, background color, list of geometric objects
//    operations: create, destroy, add an object to a list, build the world


#include "World.h"
#include "Constants.h"

//#include "Plane.h"
#include "Sphere.h"

#include "Vector3D.h"
#include "Maths.h"

//@ creates an empty world
World::World()
	:  	background_color(black),
		horizontal_res(400),
		vertical_res(400),
		pixel_size(1.0),
		gamma(1.0),
		inv_gamma(1.0),
		show_out_of_gamut(false)
{
}

//@ creates an empty world
World::~World(void) {

    delete_objects();
 }

//@ builds the world
//@pre world is empty
//@post world has objects in it
//@usage world_ptr -> build();
void World::build() {

    // viewplane settings
    horizontal_res = 600;
	vertical_res   = 600;
	pixel_size     = 1.0;
    gamma          = 1.0;

	background_color = black;

    //geometric objects in the world

   // These do the exact same sphere
   /*
    Sphere* sphere_ptr = new Sphere;
    sphere_ptr -> set_center(0, 30, 0);
    sphere_ptr -> set_radius(60);
    sphere_ptr -> set_color(1.0, 1.0, 0.0);
    add_object(sphere_ptr);
    */
    Sphere* sphere_ptr;
    sphere_ptr = new Sphere(Vector3D(0, 30, 0), 60);
    sphere_ptr -> set_color(1.0, 1.0, 0);
    add_object(sphere_ptr);

}

void World::delete_objects(void) {
	int num_objects = objects.size();

	for (int j = 0; j < num_objects; j++) {
		delete objects[j];
		objects[j] = NULL;
	}

	objects.erase (objects.begin(), objects.end());
}


