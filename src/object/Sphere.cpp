// This file contains the definition of the class sphere

#include "Sphere.h"
#include "math.h"

const double Sphere::kEpsilon = 0.001;

// ---------------------------------------------------------------- default constructor

Sphere::Sphere(void)
	: 	GeometricObject(),
		center(0.0),
		radius(1.0),
		area(4*PI),
		inv_area(1.0 / area)
{}


// ---------------------------------------------------------------- constructor

Sphere::Sphere(Point3D c, double r)
	: 	GeometricObject(),
		center(c),
		radius(r),
		area(4*PI*r*r),
		inv_area(1.0 / area)
{}


// ---------------------------------------------------------------- clone

Sphere*
Sphere::clone(void) const {
	return (new Sphere(*this));
}


// ---------------------------------------------------------------- copy constructor

Sphere::Sphere (const Sphere& sphere)
	: 	GeometricObject(sphere),
		center(sphere.center),
		radius(sphere.radius)
{}



// ---------------------------------------------------------------- assignment operator

Sphere&
Sphere::operator= (const Sphere& rhs)
{
	if (this == &rhs)
		return (*this);

	GeometricObject::operator= (rhs);

	center 	= rhs.center;
	radius	= rhs.radius;

	return (*this);
}


// ---------------------------------------------------------------- destructor

Sphere::~Sphere(void) {}


// ---------------------------------------------------------------- get_bounding_box

BBox
Sphere::get_bounding_box(void) {
	double delta = 0.000001;

	return (BBox(center.x - radius - delta, center.x + radius + delta,
				 center.y - radius - delta, center.y + radius + delta,
				 center.z - radius - delta, center.z + radius + delta));
}


//---------------------------------------------------------------- hit

bool
Sphere::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {
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
			sr.local_hit_point = ray.o + t * ray.d;
			return (true);
		}

		t = (-b + e) / denom;    // larger root

		if (t > kEpsilon) {
			tmin = t;
			sr.normal   = (temp + t * ray.d) / radius;
			sr.local_hit_point = ray.o + t * ray.d;
			return (true);
		}
	}

	return (false);
}


//---------------------------------------------------------------- shadow_hit

bool
Sphere::shadow_hit(const Ray& ray, double& tmin) const {
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
Sphere::set_sampler(Sampler* sampler) {
	sampler_ptr = sampler;
	sampler_ptr->map_samples_to_sphere();
}


// ---------------------------------------------------------------- sample
// returns a sample point on the rectangle

Point3D
Sphere::sample(void) {
	Point3D sample_point = sampler_ptr->sample_sphere();

	return (center + sample_point*radius);
}


//------------------------------------------------------------------ get_normal

Normal
Sphere::get_normal(const Point3D& p) {
    Normal nor = sample() - center;
    nor.normalize();
	return (nor);
}


// ---------------------------------------------------------------- pdf

float
Sphere::pdf(ShadeRec& sr) {
	return (inv_area);
}

