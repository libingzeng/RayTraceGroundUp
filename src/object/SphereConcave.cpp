// This file contains the definition of the class concave sphere

#include "SphereConcave.h"
#include "math.h"

const double SphereConcave::kEpsilon = 0.001;

// ---------------------------------------------------------------- default constructor

SphereConcave::SphereConcave(void)
	: 	GeometricObject(),
		center(0.0),
		radius(1.0),
		area(4*PI),
		inv_area(1.0 / area)
{}


// ---------------------------------------------------------------- constructor

SphereConcave::SphereConcave(Point3D c, double r)
	: 	GeometricObject(),
		center(c),
		radius(r),
		area(4*PI*r*r),
		inv_area(1.0 / area)
{}


// ---------------------------------------------------------------- clone

SphereConcave*
SphereConcave::clone(void) const {
	return (new SphereConcave(*this));
}


// ---------------------------------------------------------------- copy constructor

SphereConcave::SphereConcave (const SphereConcave& sphere)
	: 	GeometricObject(sphere),
		center(sphere.center),
		radius(sphere.radius)
{}



// ---------------------------------------------------------------- assignment operator

SphereConcave&
SphereConcave::operator= (const SphereConcave& rhs)
{
	if (this == &rhs)
		return (*this);

	GeometricObject::operator= (rhs);

	center 	= rhs.center;
	radius	= rhs.radius;

	return (*this);
}


// ---------------------------------------------------------------- destructor

SphereConcave::~SphereConcave(void) {}


// ---------------------------------------------------------------- get_bounding_box

BBox
SphereConcave::get_bounding_box(void) {
	double delta = 0.000001;

	return (BBox(center.x - radius - delta, center.x + radius + delta,
				 center.y - radius - delta, center.y + radius + delta,
				 center.z - radius - delta, center.z + radius + delta));
}


//---------------------------------------------------------------- hit

bool
SphereConcave::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {
	double 		t;
	Vector3D	temp 	= ray.o - center;
	double 		a 		= ray.d * ray.d;
	double 		b 		= 2.0 * temp * ray.d;
	double 		c 		= temp * temp - radius * radius;
	double 		disc	= b * b - 4.0 * a * c;

	if (disc < 0.0)
		return(false);
	else {
		double e = sqrt(disc);
		double denom = 2.0 * a;
		t = (-b - e) / denom;    // smaller root

		if (t > kEpsilon) {
			tmin = t;
			sr.normal 	 = (temp + t * ray.d) / radius;
#if 1 // reverse normal for Concave
			sr.normal = -sr.normal;
#endif // 0
			sr.local_hit_point = ray.o + t * ray.d;
			return (true);
		}

		t = (-b + e) / denom;    // larger root

		if (t > kEpsilon) {
			tmin = t;
			sr.normal   = (temp + t * ray.d) / radius;
#if 1 // reverse normal for Concave
			sr.normal = -sr.normal;
#endif // 0
			sr.local_hit_point = ray.o + t * ray.d;
			return (true);
		}
	}

	return (false);
}


//---------------------------------------------------------------- shadow_hit

bool
SphereConcave::shadow_hit(const Ray& ray, double& tmin) const {
	double 		t;
	Vector3D	temp 	= ray.o - center;
	double 		a 		= ray.d * ray.d;
	double 		b 		= 2.0 * temp * ray.d;
	double 		c 		= temp * temp - radius * radius;
	double 		disc	= b * b - 4.0 * a * c;

	if (disc < 0.0)
		return(false);
	else {
		double e = sqrt(disc);
		double denom = 2.0 * a;
		t = (-b - e) / denom;    // smaller root

		if (t > kEpsilon) {
			tmin = t;
			return (true);
		}

		t = (-b + e) / denom;    // larger root

		if (t > kEpsilon) {
			tmin = t;
			return (true);
		}
	}

	return (false);
}


// ---------------------------------------------------------------- setSampler

void
SphereConcave::set_sampler(Sampler* sampler) {
	sampler_ptr = sampler;
	sampler_ptr->map_samples_to_sphere();
}


// ---------------------------------------------------------------- sample
// returns a sample point on the rectangle

Point3D
SphereConcave::sample(void) {
	Point3D sample_point = sampler_ptr->sample_sphere();

	return (center + sample_point*radius);
}


//------------------------------------------------------------------ get_normal

Normal
SphereConcave::get_normal(const Point3D& p) {
    Normal nor = sample() - center;
    nor.normalize();
	return (nor);
}


// ---------------------------------------------------------------- pdf

float
SphereConcave::pdf(ShadeRec& sr) {
	return (inv_area);
}

