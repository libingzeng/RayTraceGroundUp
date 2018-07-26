// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// This file contains definition of the class GlassOfWater

// Although the code below uses the classes ConvexPartCylinder and ConcavePartCylinder,
// you could use whole convex and concave open cylinders. This is because the
// whole angular region (0 <= azimuth angle < 2 * pi) is used.
// This would be more efficient


#include "Disk.h"
#include "OpenCylinderConvex.h"
#include "OpenCylinderConcave.h"
#include "Annulus.h"
#include "Instance.h"
#include "TorusPartConcave.h"
#include "GlassOfWater.h"


// ------------------------------------------------------------------------------ default constructor

GlassOfWater::GlassOfWater (void)
			: 	Compound(),
				height(2.0),
				inner_radius(0.9),
				wall_thickness(0.1),
				base_thickness(0.3),
				water_height(1.5),
				meniscus_radius(0.1)
{
	build_components();
}


// ------------------------------------------------------------------------------ constructor

GlassOfWater::GlassOfWater (	const double _height,
								const double _inner_radius,
								const double _wall_thickness,
								const double _base_thickness,
								const double _water_height,
								const double _meniscus_radius)
			: 	Compound(),
				height(_height),
				inner_radius(_inner_radius),
				wall_thickness(_wall_thickness),
				base_thickness(_base_thickness),
				water_height(_water_height),
				meniscus_radius(_meniscus_radius)
{
	build_components();
}


// ------------------------------------------------------------------------------ copy constructor

GlassOfWater::GlassOfWater (const GlassOfWater& gw)
			: 	Compound(gw),
				height(gw.height),
				inner_radius(gw.inner_radius),
				wall_thickness(gw.wall_thickness),
				base_thickness(gw.base_thickness),
				water_height(gw.water_height),
				meniscus_radius(gw.meniscus_radius)
{}


// ------------------------------------------------------------------------------ clone

GlassOfWater*
GlassOfWater::clone(void) const {
	return(new GlassOfWater(*this));
}


// ------------------------------------------------------------------------------ assignment operator

GlassOfWater&
GlassOfWater::operator= (const GlassOfWater& rhs) {
	if (this == &rhs)
		return (*this);

	Compound::operator=(rhs);

	height 			= rhs.height;
	inner_radius 	= rhs.inner_radius;
	wall_thickness 	= rhs.wall_thickness;
	base_thickness 	= rhs.base_thickness;
	water_height 	= rhs.water_height;
	meniscus_radius = rhs.meniscus_radius;

	return (*this) ;
}


// ------------------------------------------------------------------------------ destructor

GlassOfWater::~GlassOfWater(void) {}


// ------------------------------------------------------------------------------ build_components

// Using named components would be better
// It would make the material functions below a lot clearer and maintainable

void
GlassOfWater::build_components(void) {
	// build the glass parts

	objects.push_back(new Annulus(	Point3D(0, height, 0), 						// rim at top
									Normal(0, 1, 0),
									inner_radius,
									inner_radius + wall_thickness));

	objects.push_back(new Disk(	Point3D(0), 									// bottom of glass
								inner_radius + wall_thickness,
								Normal(0, -1, 0)));

	objects.push_back(new OpenCylinderConcave(	water_height + meniscus_radius, // inner curved surface of glass
												height,
												inner_radius ));

	objects.push_back(new OpenCylinderConvex(	0, 								// outer curved surface of glass
												height,
												inner_radius + wall_thickness));


	// build the water parts

	objects.push_back(new Disk(	Point3D(0, water_height, 0), 					// top of water
								inner_radius - meniscus_radius,
								Normal(0, 1, 0)));


	objects.push_back(new Disk(	Point3D(0, base_thickness, 0), 					// bottom of water
								inner_radius,
								Normal(0, -1, 0)));


	objects.push_back(new OpenCylinderConvex(	base_thickness,					// curved surface of water
												water_height + meniscus_radius,
												inner_radius ));


	// build the meniscus: we need an instance for this

	Instance* meniscus_ptr = new Instance (new TorusPartConcave(inner_radius - meniscus_radius,
																meniscus_radius,
																0, 360,			// azimuth angle - phi - range
																270, 360));		// polar angle - theta - range
	meniscus_ptr->translate(0, water_height + meniscus_radius, 0);
	objects.push_back(meniscus_ptr);
}


// ------------------------------------------------------------------------------ set_glass_air_material

void
GlassOfWater::set_glass_air_material(Material* m_ptr) {
	for (int j = 0; j < 4; j++)
		objects[j]->set_material(m_ptr);
}

// ------------------------------------------------------------------------------ set_water_air_material

void
GlassOfWater::set_water_air_material(Material* m_ptr) {
	objects[4]->set_material(m_ptr);	// top of water
	objects[7]->set_material(m_ptr);   	// meniscus
}


// ------------------------------------------------------------------------------ set_water_glass_material

void
GlassOfWater::set_water_glass_material(Material* m_ptr) {
	for (int j = 5; j < 7; j++)
		objects[j]->set_material(m_ptr);
}





