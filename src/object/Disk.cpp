#include "Disk.h"


const double Disk::kEpsilon = 0.001;

// ----------------------------------------------------------------------  default constructor

Disk::Disk(void)
	: 	GeometricObject(),
		center(0.0),
		radius(1.0),
		normal(0, 1, 0),
		area(PI),
		inv_area(1.0 / area),
		sampler_ptr(NULL)
{}


// ----------------------------------------------------------------------  constructor

Disk::Disk(const Point3D& c, const double& r, const Normal& n)
	:	GeometricObject(),
		center(c),
		radius(r),
		normal(n),
		area(r*r*PI),
		inv_area(1.0 / area),
		sampler_ptr(NULL)
{
		normal.normalize();
}


// ---------------------------------------------------------------- clone

Disk*
Disk::clone(void) const {
	return (new Disk(*this));
}


// ---------------------------------------------------------------- copy constructor

Disk::Disk(const Disk& disk)
	:	GeometricObject(disk),
		center(disk.center),
		radius(disk.radius),
		normal(disk.normal),
		area(disk.area),
		inv_area(disk.inv_area)
{
	if(disk.sampler_ptr)
		sampler_ptr	= disk.sampler_ptr->clone();
	else  sampler_ptr = NULL;
}


// ---------------------------------------------------------------- assignment operator

Disk&
Disk::operator= (const Disk& rhs)	{

	if (this == &rhs)
		return (*this);

	GeometricObject::operator= (rhs);

	center      = rhs.center;
	radius      = rhs.radius;
	normal      = rhs.normal;
	area        = rhs.area;
	inv_area    = rhs.inv_area;

	if (sampler_ptr) {
		delete sampler_ptr;
		sampler_ptr = NULL;
	}

	if (rhs.sampler_ptr)
		sampler_ptr= rhs.sampler_ptr->clone();

	return (*this);
}


// ---------------------------------------------------------------- destructor

Disk::~Disk(void)
{
	if (sampler_ptr) {
		delete sampler_ptr;
		sampler_ptr = NULL;
	}
}


// ---------------------------------------------------------------- get_bounding_box

BBox
Disk::get_bounding_box(void) {
	double delta = 0.000001;
	double cos_x = normal * Vector3D(1, 0, 0);
	double cos_y = normal * Vector3D(0, 1, 0);
	double cos_z = normal * Vector3D(0, 0, 1);

	return (BBox(center.x - radius*fabs(cos_x) - delta, center.x + radius*fabs(cos_x) + delta,
				 center.y - radius*fabs(cos_y) - delta, center.y + radius*fabs(cos_y) + delta,
				 center.z - radius*fabs(cos_z) - delta, center.z + radius*fabs(cos_z) + delta));
}


// ----------------------------------------------------------------- hit

bool
Disk::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {

	double t = (center - ray.o) * normal / (ray.d * normal);

	if (t <= kEpsilon)
		return (false);

	Point3D p = ray.o + t * ray.d;

	if (center.d_squared(p) < (radius*radius)) {
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
Disk::shadow_hit(const Ray& ray, double& tmin) const {

	double t = (center - ray.o) * normal / (ray.d * normal);

	if (t <= kEpsilon)
		return (false);

	Point3D p = ray.o + t * ray.d;

	if (center.d_squared(p) < (radius*radius)) {
		tmin 				= t;
		return (true);
	}
	else
		return (false);
}


// ---------------------------------------------------------------- setSampler

void
Disk::set_sampler(Sampler* sampler) {
	sampler_ptr = sampler;
	sampler_ptr->map_samples_to_unit_disk();
}


// ---------------------------------------------------------------- sample
// returns a sample point on the disk
// just for disks with normal(0, 1, 0) or normal(0, -1, 0)

Point3D
Disk::sample(void) {
	Point2D sample_point = sampler_ptr->sample_unit_disk();
	return (center + Vector3D(sample_point.x * radius, 0, sample_point.y *radius));
}


//------------------------------------------------------------------ get_normal

Normal
Disk::get_normal(const Point3D& p) {
	return (normal);
}


// ---------------------------------------------------------------- pdf

float
Disk::pdf(ShadeRec& sr) {
	return (inv_area);
}

