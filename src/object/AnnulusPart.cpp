#include "AnnulusPart.h"


const double AnnulusPart::kEpsilon = 0.001;

// ----------------------------------------------------------------------  default constructor

AnnulusPart::AnnulusPart(void)
	: 	GeometricObject(),
		center(0.0),
		normal(0, 1, 0),
		inner_radius(0.5),
		outer_radius(1.0),
		phi_min(0.0),
		phi_max(TWO_PI)
{}


// ----------------------------------------------------------------------  constructor

AnnulusPart::AnnulusPart(const Point3D& c, const Normal& n, const double& i_r, const double& o_r,
                         const double& _phi_min, const double& _phi_max)
	:	GeometricObject(),
		center(c),
		normal(n),
		inner_radius(i_r),
		outer_radius(o_r),
		phi_min(_phi_min * PI_ON_180),
		phi_max(_phi_max * PI_ON_180)
{
		normal.normalize();
}


// ---------------------------------------------------------------- copy constructor

AnnulusPart::AnnulusPart(const AnnulusPart& annulus)
	:	GeometricObject(annulus),
		center(annulus.center),
		normal(annulus.normal),
		inner_radius(annulus.inner_radius),
		outer_radius(annulus.outer_radius),
		phi_min(annulus.phi_min),
		phi_max(annulus.phi_max)
{}


// ---------------------------------------------------------------- clone

AnnulusPart*
AnnulusPart::clone(void) const {
	return (new AnnulusPart(*this));
}


// ---------------------------------------------------------------- assignment operator

AnnulusPart&
AnnulusPart::operator= (const AnnulusPart& rhs)	{

	if (this == &rhs)
		return (*this);

	GeometricObject::operator= (rhs);

	center       = rhs.center;
	normal       = rhs.normal;
	inner_radius = rhs.inner_radius;
	outer_radius = rhs.outer_radius;
	phi_min = rhs.phi_min;
	phi_max = rhs.phi_max;

	return (*this);
}


// ---------------------------------------------------------------- destructor

AnnulusPart::~AnnulusPart(void)
{}


// ---------------------------------------------------------------- get_bounding_box

BBox
AnnulusPart::get_bounding_box(void) {
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
AnnulusPart::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {

	double t = (center - ray.o) * normal / (ray.d * normal);

	if (t <= kEpsilon)
		return (false);

	Point3D p = ray.o + t * ray.d;

    double phi = atan2(p.x, p.z);
    if (phi < 0.0)
        phi += TWO_PI;

	if ((center.d_squared(p) < (outer_radius*outer_radius))
      && (center.d_squared(p) > (inner_radius*inner_radius))
      && (phi >= phi_min && phi <= phi_max)) {
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
AnnulusPart::shadow_hit(const Ray& ray, double& tmin) const {

	double t = (center - ray.o) * normal / (ray.d * normal);

	if (t <= kEpsilon)
		return (false);

	Point3D p = ray.o + t * ray.d;

    double phi = atan2(p.x, p.z);
    if (phi < 0.0)
        phi += TWO_PI;

	if ((center.d_squared(p) < (outer_radius*outer_radius))
      && (center.d_squared(p) > (inner_radius*inner_radius))
      && (phi >= phi_min && phi <= phi_max)) {
		tmin 				= t;
		return (true);
	}
	else
		return (false);
}
