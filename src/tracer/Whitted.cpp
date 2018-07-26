// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "Whitted.h"
#include "World.h"
#include "ShadeRec.h"
#include "Material.h"

// -------------------------------------------------------------------- default constructor

Whitted::Whitted(void)
	: Tracer()
{}


// -------------------------------------------------------------------- constructor

Whitted::Whitted(World* _worldPtr)
	: Tracer(_worldPtr)
{}


// -------------------------------------------------------------------- destructor

Whitted::~Whitted(void) {}


// -------------------------------------------------------------------- trace_ray

RGBColor
Whitted::trace_ray(const Ray ray, const int depth) const {
	if (depth > world_ptr->vp.max_depth)
		return(black);
	else {
		ShadeRec sr(world_ptr->hit_objects(ray));

		if (sr.hit_an_object) {
			sr.depth = depth;
			sr.ray = ray;
			return (sr.material_ptr->shade(sr));
		}
		else
			return (world_ptr->background_color);
	}
}


// -------------------------------------------------------------------- trace_ray

// Because this function is called polymorphically in Dielectric::shade, you should
// declare it as a virtual function in Tracer.h and give it a dummy definition in Tracer.cpp.
// See Section 1.10.6

RGBColor
Whitted::trace_ray(const Ray ray, double& tmin, const int depth) const {
	if (depth > world_ptr->vp.max_depth)
		return (black);
	else {
		ShadeRec sr(world_ptr->hit_objects(ray));

		if (sr.hit_an_object) {
			sr.depth 	= depth;
			sr.ray 		= ray;
			tmin		= sr.t;     // required for colored transparency

			return (sr.material_ptr->shade(sr));
		}
		else {
			tmin = kHugeValue;

			return (world_ptr->background_color);
		}
	}
}
