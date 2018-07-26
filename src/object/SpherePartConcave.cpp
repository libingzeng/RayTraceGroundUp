// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "Constants.h"
#include "SpherePartConcave.h"

// ----------------------------------------------------------------  default constructor
// a default SpherePartConcave is a whole sphere

SpherePartConcave::SpherePartConcave (void)
	: 	GeometricObject(),
		center(0.0),
		radius(1.0),
		phi_min(0.0),			// in radians
		phi_max(TWO_PI),		// in radians
		theta_min(0.0),			// in radians measured from top
		theta_max(PI),			// in radians measured from top
		phi_interval(TWO_PI),
		theta_interval(PI),
		cos_theta_min(1.0),
		cos_theta_max(-1.0)
{}


// ---------------------------------------------------------------- donstructor

SpherePartConcave::SpherePartConcave (const Point3D 	c,
									const double 	r,
									const double 	azimuth_min,	// in degrees
									const double 	azimuth_max,	// in degrees
									const double 	polar_min,		// in degrees measured from top
									const double 	polar_max)		// in degrees measured from top
	: 	GeometricObject(),
		center(c),
		radius(r),
		phi_min(azimuth_min * PI_ON_180),			// in radians
		phi_max(azimuth_max * PI_ON_180),			// in radians
		theta_min(polar_min * PI_ON_180),			// in radians measured from top
		theta_max(polar_max * PI_ON_180),			// in radians measured from top
		phi_interval(phi_max - phi_min),
		theta_interval(theta_max - theta_min),
		cos_theta_min(cos(theta_min)),
		cos_theta_max(cos(theta_max))
{}


// ---------------------------------------------------------------- constructor

SpherePartConcave::SpherePartConcave (const Point3D 	c,
									const double 	r)
	: 	GeometricObject(),
		center(c),
		radius(r),
		phi_min(0.0),
		phi_max(TWO_PI),
		theta_min(0.0),
		theta_max(PI),
		phi_interval(TWO_PI),
		theta_interval(PI),
		cos_theta_min(1.0),
		cos_theta_max(-1.0)
{}


// ---------------------------------------------------------------- clone

SpherePartConcave*
SpherePartConcave::clone(void) const {
	return (new SpherePartConcave(*this));
}


// ---------------------------------------------------------------- copy constructor

SpherePartConcave::SpherePartConcave (const SpherePartConcave& ps)
	: 	GeometricObject(ps),
		center(ps.center),
		radius(ps.radius),
		phi_min(ps.phi_min),
		phi_max(ps.phi_max),
		theta_min(ps.theta_min),
		theta_max(ps.theta_max),
		phi_interval(ps.phi_interval),
		theta_interval(ps.theta_interval),
		cos_theta_min(ps.cos_theta_min),
		cos_theta_max(ps.cos_theta_max)
{}



// ---------------------------------------------------------------- assignment operator

SpherePartConcave&
SpherePartConcave::operator= (const SpherePartConcave& rhs)
{
	if (this == &rhs)
		return (*this);

	GeometricObject::operator=(rhs);

	center 			= rhs.center;
	radius			= rhs.radius;
	phi_min			= rhs.phi_min;
	phi_max			= rhs.phi_max;
	theta_min		= rhs.theta_min;
	theta_max		= rhs.theta_max;
	phi_interval	= rhs.phi_interval;
	theta_interval	= rhs.theta_interval;
	cos_theta_min	= rhs.cos_theta_min;
	cos_theta_max	= rhs.cos_theta_max;

	return (*this);
}


// ---------------------------------------------------------------- destructor

SpherePartConcave::~SpherePartConcave(void) {}


// ------------------------------------------------------------------------------ hit

bool
SpherePartConcave::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {
	double 		t;
	Vector3D  	temp 	= ray.o - center;
	double 		a 		= ray.d * ray.d;
	double 		b 		= 2.0 * temp * ray.d;
	double 		c 		= temp * temp - radius * radius;
	double 		disc 	= b * b - 4.0 * a * c;

	if (disc < 0.0)
		return(false);
	else {
		double e = sqrt(disc);
		double denom = 2.0 * a;
		t = (-b - e) / denom;    // smaller root

		if (t > kEpsilon) {
			Vector3D hit = ray.o + t * ray.d - center;

			double phi = atan2(hit.x, hit.z);
			if (phi < 0.0)
				phi += TWO_PI;

			if (hit.y <= radius * cos_theta_min &&
				hit.y >= radius * cos_theta_max &&
				phi >= phi_min && phi <= phi_max) {

				tmin = t;
				sr.normal = (temp + t * ray.d) / radius;   // points outwards
#if 1 // reverse normal for Concave
				sr.normal = -sr.normal;
#endif // 0
				sr.local_hit_point = ray.o + tmin * ray.d;


                // texture parameters. first, compute theta_t and phi_t

                double theta_t = acos(sr.local_hit_point.y);
                double phi_t   = atan2(sr.local_hit_point.x, sr.local_hit_point.z);
                if (phi_t < 0.0)
                    phi_t += TWO_PI;

                //  texture parameters. next, map theta_t and phi_t to (u, v) in [0, 1] X [0, 1]

                sr.u = phi_t / phi_interval;
                sr.v = 1.0 - theta_t / theta_interval;


				return (true);
			}
		}

		t = (-b + e) / denom;    // larger root

		if (t > kEpsilon) {
			Vector3D hit = ray.o + t * ray.d - center;

			double phi = atan2(hit.x, hit.z);
			if (phi < 0.0)
				phi += TWO_PI;

			if (hit.y <= radius * cos_theta_min &&
				hit.y >= radius * cos_theta_max &&
				phi >= phi_min && phi <= phi_max) {

				tmin = t;
				sr.normal = (temp + t * ray.d) / radius;   // points outwards
#if 1 // reverse normal for Concave
				sr.normal = -sr.normal;
#endif // 0
				sr.local_hit_point = ray.o + tmin * ray.d;


                // texture parameters. first, compute theta_t and phi_t

                double theta_t = acos(sr.local_hit_point.y);
                double phi_t   = atan2(sr.local_hit_point.x, sr.local_hit_point.z);
                if (phi_t < 0.0)
                    phi_t += TWO_PI;

                //  texture parameters. next, map theta_t and phi_t to (u, v) in [0, 1] X [0, 1]

                sr.u = phi_t / phi_interval;
                sr.v = 1.0 - theta_t / theta_interval;


				return (true);
			}
		}
	}

	return (false);
}


// ------------------------------------------------------------------------------ shadow_hit

bool
SpherePartConcave::shadow_hit(const Ray& ray, double& tmin) const {
	double 		t;
	Vector3D  	temp 	= ray.o - center;
	double 		a 		= ray.d * ray.d;
	double 		b 		= 2.0 * temp * ray.d;
	double 		c 		= temp * temp - radius * radius;
	double 		disc 	= b * b - 4.0 * a * c;

	if (disc < 0.0)
		return(false);
	else {
		double e = sqrt(disc);
		double denom = 2.0 * a;
		t = (-b - e) / denom;    // smaller root

		if (t > kEpsilon) {
			Vector3D hit = ray.o + t * ray.d - center;

			double phi = atan2(hit.x, hit.z);
			if (phi < 0.0)
				phi += TWO_PI;

			if (hit.y <= radius * cos_theta_min &&
				hit.y >= radius * cos_theta_max &&
				phi >= phi_min && phi <= phi_max) {

				tmin = t;

				return (true);
			}
		}

		t = (-b + e) / denom;    // larger root

		if (t > kEpsilon) {
			Vector3D hit = ray.o + t * ray.d - center;

			double phi = atan2(hit.x, hit.z);
			if (phi < 0.0)
				phi += TWO_PI;

			if (hit.y <= radius * cos_theta_min &&
				hit.y >= radius * cos_theta_max &&
				phi >= phi_min && phi <= phi_max) {

				tmin = t;

				return (true);
			}
		}
	}

	return (false);
}



