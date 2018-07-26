#include "Annulus.h"


const double Annulus::kEpsilon = 0.001;

// ----------------------------------------------------------------------  default constructor

Annulus::Annulus(void)
	: 	GeometricObject(),
		center(0.0),
		normal(0, 1, 0),
		inner_radius(0.5),
		outer_radius(1.0)
{}


// ----------------------------------------------------------------------  constructor

Annulus::Annulus(const Point3D& c, const Normal& n, const double& i_r, const double& o_r)
	:	GeometricObject(),
		center(c),
		normal(n),
		inner_radius(i_r),
		outer_radius(o_r)
{
		normal.normalize();
}


// ---------------------------------------------------------------- copy constructor

Annulus::Annulus(const Annulus& annulus)
	:	GeometricObject(annulus),
		center(annulus.center),
		normal(annulus.normal),
		inner_radius(annulus.inner_radius),
		outer_radius(annulus.outer_radius)
{}


// ---------------------------------------------------------------- clone

Annulus*
Annulus::clone(void) const {
	return (new Annulus(*this));
}


// ---------------------------------------------------------------- assignment operator

Annulus&
Annulus::operator= (const Annulus& rhs)	{

	if (this == &rhs)
		return (*this);

	GeometricObject::operator= (rhs);

	center       = rhs.center;
	normal       = rhs.normal;
	inner_radius = rhs.inner_radius;
	outer_radius = rhs.outer_radius;

	return (*this);
}


// ---------------------------------------------------------------- destructor

Annulus::~Annulus(void)
{}


// ---------------------------------------------------------------- get_bounding_box

BBox
Annulus::get_bounding_box(void) {
	double delta = 0.000001;
	double cos_x = normal * Vector3D(1, 0, 0);
	double cos_y = normal * Vector3D(0, 1, 0);
	double cos_z = normal * Vector3D(0, 0, 1);

	return (BBox(center.x - outer_radius*fabs(cos_x) - delta, center.x + outer_radius*fabs(cos_x) + delta,
				 center.y - outer_radius*fabs(cos_y) - delta, center.y + outer_radius*fabs(cos_y) + delta,
				 center.z - outer_radius*fabs(cos_z) - delta, center.z + outer_radius*fabs(cos_z) + delta));
}


// ----------------------------------------------------------------- hit

bool
Annulus::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {

	double t = (center - ray.o) * normal / (ray.d * normal);

	if (t <= kEpsilon)
		return (false);

	Point3D p = ray.o + t * ray.d;

	if ((center.d_squared(p) < (outer_radius*outer_radius))
      && (center.d_squared(p) > (inner_radius*inner_radius))) {
		tmin 				= t;
		sr.normal 			= normal;
		sr.local_hit_point	= p;
		return (true);
	}
	else
		return (false);
}


// ----------------------------------------------------------------- shadow_hit

bool
Annulus::shadow_hit(const Ray& ray, double& tmin) const {

	double t = (center - ray.o) * normal / (ray.d * normal);

	if (t <= kEpsilon)
		return (false);

	Point3D p = ray.o + t * ray.d;

	if ((center.d_squared(p) < (outer_radius*outer_radius))
      && (center.d_squared(p) > (inner_radius*inner_radius))) {
		tmin 				= t;
		return (true);
	}
	else
		return (false);
}
