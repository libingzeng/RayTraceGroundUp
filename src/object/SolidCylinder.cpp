// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "SolidCylinder.h"
#include "OpenCylinder.h"
#include "Disk.h"

// This uses the value of kEpsilon defined in Constants.h

// ---------------------------------------------------------------- default constructor

SolidCylinder::SolidCylinder(void)
	: 	Compound(),
		y0(-1.0),
		y1(1.0),
		radius(1.0),
		inv_radius(1.0) {

	objects.push_back(new Disk(	Point3D(0, -1, 0), 			// bottom
								1,
								Normal(0, -1, 0)));

	objects.push_back(new Disk(	Point3D(0, 1, 0), 			// top
								1,
								Normal(0, 1, 0)));

	objects.push_back(new OpenCylinder(-1, 1, 1));	// wall
}

// ---------------------------------------------------------------- constructor

SolidCylinder::SolidCylinder(const double bottom, const double top, const double r)
	:  	Compound(),
		y0(bottom),
		y1(top),
		radius(r),
		inv_radius(1.0 / radius) {

	objects.push_back(new Disk(	Point3D(0, bottom, 0), 			// bottom
								radius,
								Normal(0, -1, 0)));

	objects.push_back(new Disk(	Point3D(0, top, 0), 			// top
								radius,
								Normal(0, 1, 0)));

	objects.push_back(new OpenCylinder(bottom, top, radius));	// wall
}

// ---------------------------------------------------------------- copy constructor

SolidCylinder::SolidCylinder(const SolidCylinder& c)
	: 	Compound(c),
		y0(c.y0),
		y1(c.y1),
		radius(c.radius),
		inv_radius(c.inv_radius) {

	objects.push_back(new Disk(	Point3D(0, c.y0, 0), 			// bottom
								c.radius,
								Normal(0, -1, 0)));

	objects.push_back(new Disk(	Point3D(0, c.y1, 0), 			// top
								c.radius,
								Normal(0, 1, 0)));

	objects.push_back(new OpenCylinder(c.y0, c.y1, c.radius));	// wall
}



// ---------------------------------------------------------------- clone

SolidCylinder*
SolidCylinder::clone(void) const {
	return (new SolidCylinder (*this));
}


// ---------------------------------------------------------------- assignment operator

SolidCylinder&
SolidCylinder::operator= (const SolidCylinder& rhs)
{
	if (this == &rhs)
		return (*this);

	GeometricObject::operator= (rhs);

	y0 			= rhs.y0;
	y1 			= rhs.y1;
	radius 		= rhs.radius;
	inv_radius 	= rhs.inv_radius;

	return (*this) ;
}


// ---------------------------------------------------------------- destructor

SolidCylinder::~SolidCylinder(void) {}


// ---------------------------------------------------------------- get_bounding_box

BBox
SolidCylinder::get_bounding_box(void) {
	double delta = 0.000001;

    bbox = BBox(0 - radius - delta, 0 + radius + delta,
                         y0 - delta,         y1 + delta,
				 0 - radius - delta, 0 + radius + delta);

	return (bbox);
}


// ----------------------------------------------------------------------------- hit
// The code reverses the normal when the ray hits the inside surface, allows both
// sides to be shaded, but completely messes up transparency.

bool
SolidCylinder::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {
	if (bbox.hit(ray))
		return (Compound::hit(ray, tmin, sr));
	else
		return (false);
}


// ----------------------------------------------------------------------------- shadow_hit

bool
SolidCylinder::shadow_hit(const Ray& ray, double& tmin) const {
	if (bbox.hit(ray))
		return (Compound::shadow_hit(ray, tmin));
	else
		return (false);
}
