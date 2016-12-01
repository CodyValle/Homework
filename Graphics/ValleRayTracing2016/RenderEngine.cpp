#include "RenderEngine.h"
#include "Ray.h"

//@ assigns a world to the render engine and creates it
//@pre w_ptr points to an existing world of objects
//@post world_ptr also points to that world
//@usage  engine_ptr = new RenderEngine(the_world_ptr);
RenderEngine:: RenderEngine(World* w_ptr):
   world_ptr(w_ptr)
{
}

//@ destroys the engine
RenderEngine:: ~RenderEngine()
{
}

//@ renders the world of objects
//@pre world has been built
//@post world has been rendered to the image array of pixels
//@usage engine_ptr -> render();
void RenderEngine::render()
{
    RGBColor	pixel_color;
	Ray			ray;
	int 		hres 	= world_ptr -> horizontal_res;
	int 		vres 	= world_ptr -> vertical_res;
	float		s		= world_ptr -> pixel_size;
	float		zw		= 100.0;			// hardwired in

	ray.d = Vector3D(0, 0, -1);
	ray.o = Vector3D(0,0,0);

	for (int row = 0; row < vres; row++)	{		// up
		for (int col = 0; col < hres; col++) {	// across

			ray.o = Vector3D(s * (-hres / 2.0 + col + .5)    , s * (-vres / 2.0 + row + .5)        , zw);
			pixel_color = trace_ray(ray);
 			display_pixel(row, col, pixel_color);

		}
	}
}

//     helper functions for rendering

RGBColor RenderEngine::trace_ray(const Ray& ray) {

	hit_bare_bones_objects(ray);

	if (sr.hit_an_object)
		return (sr.color);
	else
		return (world_ptr->background_color);
}

void RenderEngine::hit_bare_bones_objects(const Ray& ray) {
	double		t;
	float		tmin 			= kHugeValue;
	int 		num_objects 	= world_ptr -> objects.size();
    sr.hit_an_object = false;
	for (int j = 0; j < num_objects; j++) {
		if (world_ptr -> objects[j] -> hit(ray, t, sr) && (t < tmin)) {
			sr.hit_an_object	= true;
			tmin 				= t;
			sr.color			= world_ptr -> objects[j] -> get_color();
		}
	}
}

void RenderEngine::display_pixel(const int row, const int column, const RGBColor& raw_color) {
	RGBColor mapped_color;

	if (world_ptr -> show_out_of_gamut)  // I do not know where this gets set
		mapped_color = clamp_to_color(raw_color);
	else
		mapped_color = max_to_one(raw_color);

	if (world_ptr -> gamma != 1.0)
		mapped_color = mapped_color.powc(world_ptr -> inv_gamma);

   image[row][column][0] = mapped_color.r;
   image[row][column][1] = mapped_color.g;
   image[row][column][2] = mapped_color.b;

}

RGBColor RenderEngine::max_to_one(const RGBColor& c) const  {
	float max_value = max(c.r, max(c.g, c.b));

	if (max_value > 1.0)
		return (c / max_value);
	else
		return (c);
}

RGBColor RenderEngine::clamp_to_color(const RGBColor& raw_color) const {
	RGBColor c(raw_color);

	if (raw_color.r > 1.0 || raw_color.g > 1.0 || raw_color.b > 1.0) {
		c.r = 1.0; c.g = 0.0; c.b = 0.0;
	}
	return (c);
}





