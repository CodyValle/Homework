//@file RenderEngine.h
//@ authors Original classes by ?? then redesigned by Drs. Jenny Orr and Dr. Kathie Yerion
//@ June 22, 2012

// ADT RenderEngine
//    data object: the engine that renders a world of objects
//    operations: create, destroy, render and its helpers

#ifndef _RENDER_ENGINE_H
#define _RENDER_ENGINE_H
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "RGBColor.h"
#include "Ray.h"
#include "World.h"
#include "Constants.h"
#include "RGBColor.h"
#include "Vector3D.h"

class RenderEngine
{
    public:
        RGBColor	background_color;

        GLfloat     image[600][600][3];

        World*      world_ptr;

        ShadeRec sr;

    public:
       //@ assigns a world to the render engine and creates it
       //@pre w_ptr points to an existing world of objects
       //@post world_ptr also points to that world
       //@usage  engine_ptr = new RenderEngine(the_world_ptr);
       RenderEngine(World* w_ptr);

       //@ destroys the engine
       ~RenderEngine();

       //@ renders the world of objects
       //@pre world has been built
       //@post world has been rendered to the image array of pixels
       //@usage engine_ptr -> render();
       void render() ;

    private:
       RGBColor	trace_ray(const Ray& ray);
       RGBColor max_to_one(const RGBColor& c) const;
       RGBColor clamp_to_color(const RGBColor& raw_color) const;
       void display_pixel(const int row, const int column, const RGBColor& raw_color);
       void hit_bare_bones_objects(const Ray& ray);
};
#endif
