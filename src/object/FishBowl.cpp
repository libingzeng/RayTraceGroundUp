// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "Constants.h"
#include "Disk.h"
#include "SpherePartConvex.h"
#include "SpherePartConcave.h"
#include "TorusPartConvex.h"
#include "TorusPartConcave.h"
#include "Instance.h"
#include "FishBowl.h"


// ------------------------------------------------------------------------------ default constructor

FishBowl::FishBowl(void)
	: 	Compound(),
		inner_radius(1.0),
		glass_thickness(0.1),
		water_depth(1.25),
		meniscus_radius(0.05),
		opening_angle(90)
{
	build_components();
}


// ------------------------------------------------------------------------------ constructor

FishBowl::FishBowl(	const double _inner_radius,
					const double _glass_thickness,
					const double _water_depth,
					const double _meniscus_radius,
					const double _opening_angle)
	: 	Compound(),
		inner_radius(_inner_radius),
		glass_thickness(_glass_thickness),
		water_depth(_water_depth),
		meniscus_radius(_meniscus_radius),
		opening_angle(_opening_angle)
{
	build_components();
}




// ------------------------------------------------------------------------------ copy constructor

FishBowl::FishBowl(const FishBowl& fb)
	: 	Compound(fb),
		inner_radius(fb.inner_radius),
		glass_thickness(fb.glass_thickness),
		water_depth(fb.water_depth),
		meniscus_radius(fb.meniscus_radius),
		opening_angle(fb.opening_angle)
{}


// ------------------------------------------------------------------------------ clone

FishBowl*
FishBowl::clone(void) const {
	return(new FishBowl(*this));
}


// ------------------------------------------------------------------------------ assignment operator

FishBowl&
FishBowl::operator= (const FishBowl& rhs) {
	if (this == &rhs)
		return (*this);

	Compound::operator=(rhs);

	inner_radius 	= rhs.inner_radius;
	glass_thickness = rhs.glass_thickness;
	water_depth 	= rhs.water_depth;
	meniscus_radius = rhs.meniscus_radius;
	opening_angle 	= rhs.opening_angle;

	return (*this) ;
}


// ------------------------------------------------------------------------------ destructor

FishBowl::~FishBowl(void) {}


// ------------------------------------------------------------------------------ build_components

void
FishBowl::build_components(void) {
	double angle_radians = (opening_angle / 2.0) * PI_ON_180; // half the opening angle in radians

	// meniscus calculations - required here because they affect the inner surface of the glass-air boundary

	// torus tube center coordinates

	double h = water_depth - inner_radius;
	double yc = h + meniscus_radius;
	double xc = sqrt(inner_radius * (inner_radius - 2.0 * meniscus_radius) - h * (h + 2.0 * meniscus_radius));
	double beta = atan2(yc, xc) * 180.0 / PI;   // in degrees


	// outer glass-air boundary

	objects.push_back(new SpherePartConvex(	Point3D(0.0),
											inner_radius + glass_thickness,
											0, 360,  				// azimuth angle range - full circle
											opening_angle / 2.0,  	// minimum polar angle measured from top
											180));                 	// maximum polar angle measured from top



	// inner glass-air boundary
	// the inner surface of the glass only goes down to the top of the meniscus

	objects.push_back(new SpherePartConcave(Point3D(0.0),
											inner_radius,
											0, 360, 				// azimuth angle - full circle
											opening_angle / 2.0,    // mimimum polar angle measured from top
											90 - beta));   			// maximum polar angle measured from top



	// round rim - need an instance for this as it's a half torus

	double theta_min = opening_angle / 2.0;  	// measured counter-clockwise from (x, z) plane
	double theta_max = theta_min + 180;			// measured counter-clockwise from (x, z) plane

	Instance* rim_ptr = new Instance (new TorusPartConvex(
												(inner_radius + glass_thickness / 2.0) * sin(angle_radians), // a
												glass_thickness / 2.0, 										 // b
												0, 360,
												theta_min,
												theta_max));

	rim_ptr->translate(0, (inner_radius + glass_thickness / 2.0) * cos(angle_radians), 0);
	objects.push_back(rim_ptr);


	// meniscus - if water_depth > 1, we need two part tori

	Instance* torus_ptr1 = new Instance (new TorusPartConcave(	xc,
																meniscus_radius,
																0, 360,
																270, 360));
	torus_ptr1->translate(0, yc, 0);
	objects.push_back(torus_ptr1);


	Instance* torus_ptr2 = new Instance (new TorusPartConcave(	xc,
																meniscus_radius,
																0, 360,
																0, beta));
	torus_ptr2->translate(0, yc, 0);
	objects.push_back(torus_ptr2);


	// water-air boundary

	objects.push_back(new Disk(	Point3D(0, h, 0),
								xc,
								Normal(0, 1, 0)));				// the disk just touches the bottom of the meniscus


	// water-glass boundary

	objects.push_back(new SpherePartConvex(	Point3D(0),
											inner_radius,
											0, 360,
											90 - beta,		// mimimum polar angle measured from top
											180));			// maximum polar angle measured from top
}



// ------------------------------------------------------------------------------ set_glass_air_material
// [0]: outer glass-air boundary
// [1]: inner glass-air boundary
// [2]: rim

void
FishBowl::set_glass_air_material(Material* m_ptr) {
	for (int j = 0; j < 3; j++)
		objects[j]->set_material(m_ptr);
}

// ------------------------------------------------------------------------------ set_water_air_material
// [3]: meniscus torus 1
// [4]: meniscus torus 2
// [5]: water-air boundary

void
FishBowl::set_water_air_material(Material* m_ptr) {
	objects[3]->set_material(m_ptr);
	objects[4]->set_material(m_ptr);
	objects[5]->set_material(m_ptr);
}


// ------------------------------------------------------------------------------ set_water_glass_material
// [6]: water-glass boundary

void
FishBowl::set_water_glass_material(Material* m_ptr) {
	objects[6]->set_material(m_ptr);
}





