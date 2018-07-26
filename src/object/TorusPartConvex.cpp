// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "Maths.h"	// for SolveQuartic
#include "TorusPartConvex.h"


// ---------------------------------------------------------------- default constructor

TorusPartConvex::TorusPartConvex (void)
	: 	GeometricObject(),
		a(2.0),
		b(0.5),
		phi1(90.0 * PI / 180.0),
		phi2(270.0 * PI / 180.0),
		theta1(90.0 * PI / 180.0),
		theta2(270.0 * PI / 180.0),
		bbox(-a - b, a + b, -b, b, -a - b, a + b)
{}

// ---------------------------------------------------------------- constructor

TorusPartConvex::TorusPartConvex (const double _a,      const double _b,
                                    const double _phi1,   const double _phi2,
                                    const double _theta1, const double _theta2)
	: 	GeometricObject(),
		a(_a),
		b(_b),
		phi1(_phi1 * PI / 180.0),
		phi2(_phi2 * PI / 180.0),
		theta1(_theta1 * PI / 180.0),
		theta2(_theta2 * PI / 180.0),
		bbox(-a - b, a + b, -b, b, -a - b, a + b)
{}

// ---------------------------------------------------------------- clone

TorusPartConvex*
TorusPartConvex::clone(void) const {
	return (new TorusPartConvex(*this));
}


// ---------------------------------------------------------------- copy constructor

TorusPartConvex::TorusPartConvex (const TorusPartConvex& torus)
	: 	GeometricObject(torus),
		a(torus.a),
		b(torus.b),
		phi1(torus.phi1),
		phi2(torus.phi2),
		theta1(torus.theta1),
		theta2(torus.theta2),
		bbox(torus.bbox)
{}


// ---------------------------------------------------------------- assignment operator

TorusPartConvex&
TorusPartConvex::operator = (TorusPartConvex& rhs) {
	if (this == &rhs)
		return (*this);

	GeometricObject::operator=(rhs);

	a		= rhs.a;
	b 		= rhs.b;
	phi1	= rhs.phi1;
	phi2	= rhs.phi2;
	theta1	= rhs.theta1;
	theta2 	= rhs.theta2;
	bbox	= rhs.bbox;

	return (*this);
}


//------------------------------------------------------------------- destructor

TorusPartConvex::~TorusPartConvex(void) {}


//------------------------------------------------------------------ FindNormal
// Find the normal vector at the specified position
// This works because the torus is defined by a single implicit equation

Normal
TorusPartConvex::compute_normal(const Point3D& p) const {
	Normal normal;
	double param_squared = a * a + b * b;

	double x = p.x;
	double y = p.y;
	double z = p.z;
	double sum_squared = x * x + y * y + z * z;

	normal.x = 4.0 * x * (sum_squared - param_squared);
	normal.y = 4.0 * y * (sum_squared - param_squared + 2.0 * a * a);
	normal.z = 4.0 * z * (sum_squared - param_squared);
	normal.normalize();

	return (normal);
}


//---------------------------------------------------------------- hit

bool
TorusPartConvex::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {
	if (!bbox.hit(ray))
		return (false);

	double x1 = ray.o.x; double y1 = ray.o.y; double z1 = ray.o.z;
	double d1 = ray.d.x; double d2 = ray.d.y; double d3 = ray.d.z;

	double coeffs[5];	// coefficient array for the quartic equation
	double roots[4];	// solution array for the quartic equation

	// define the coefficients of the quartic equation

	double sum_d_sqrd 	= d1 * d1 + d2 * d2 + d3 * d3;
	double e			= x1 * x1 + y1 * y1 + z1 * z1 - a * a - b * b;
	double f			= x1 * d1 + y1 * d2 + z1 * d3;
	double four_a_sqrd	= 4.0 * a * a;

	coeffs[0] = e * e - four_a_sqrd * (b * b - y1 * y1); 	// constant term
	coeffs[1] = 4.0 * f * e + 2.0 * four_a_sqrd * y1 * d2;
	coeffs[2] = 2.0 * sum_d_sqrd * e + 4.0 * f * f + four_a_sqrd * d2 * d2;
	coeffs[3] = 4.0 * sum_d_sqrd * f;
	coeffs[4] = sum_d_sqrd * sum_d_sqrd;  					// coefficient of t^4

	// find roots of the quartic equation

	int num_real_roots = SolveQuartic(coeffs, roots);

	bool	intersected = false;
	double 	t 		 	= kHugeValue;
	double   t_t;
    Point3D  p_t;
    Vector3D v_t;
    Vector3D v_t_xz;
    double   phi;
    double   theta;

	if (num_real_roots == 0)  // ray misses the torus
		return(false);

	// find the smallest root greater than kEpsilon, if any
	// the roots array is not sorted
	for (int j = 0; j < num_real_roots; j++)
		if (roots[j] > kEpsilon) {
            t_t = roots[j];
            p_t = ray.o + ray.d * t_t;
            v_t = Vector3D(p_t.x, p_t.y, p_t.z);
            v_t_xz = Vector3D(p_t.x, 0.0, p_t.z);

            v_t_xz.normalize();
            phi = acos((v_t_xz * Vector3D(1.0, 0.0, 0.0)));
            if (v_t_xz.z > 0) {
                phi = TWO_PI - phi;
            }

            theta = acos((v_t.length()*v_t.length() - a*a - b*b) / (2*a*b));
            if (v_t.y < 0) {
                theta = TWO_PI - theta;
            }

			if ((roots[j] < t) &&
                ((theta > theta1) && (theta < theta2)) &&
                (((phi > phi1) && (phi < phi2)) || ((phi + TWO_PI) < phi2))) {
				t = roots[j];
                intersected = true;
            }
		}

	if(!intersected)
		return (false);

	tmin 			 	= t;
	sr.local_hit_point 	= ray.o + t * ray.d;
	sr.normal 		 	= compute_normal(sr.local_hit_point);
#if 0 // reverse normal for Concave
	sr.normal = -sr.normal;
#endif // 0

	return (true);
}


//---------------------------------------------------------------- shadow_hit

bool
TorusPartConvex::shadow_hit(const Ray& ray, double& tmin) const {
	if (!bbox.hit(ray))
		return (false);

	double x1 = ray.o.x; double y1 = ray.o.y; double z1 = ray.o.z;
	double d1 = ray.d.x; double d2 = ray.d.y; double d3 = ray.d.z;

	double coeffs[5];	// coefficient array for the quartic equation
	double roots[4];	// solution array for the quartic equation

	// define the coefficients of the quartic equation

	double sum_d_sqrd 	= d1 * d1 + d2 * d2 + d3 * d3;
	double e			= x1 * x1 + y1 * y1 + z1 * z1 - a * a - b * b;
	double f			= x1 * d1 + y1 * d2 + z1 * d3;
	double four_a_sqrd	= 4.0 * a * a;

	coeffs[0] = e * e - four_a_sqrd * (b * b - y1 * y1); 	// constant term
	coeffs[1] = 4.0 * f * e + 2.0 * four_a_sqrd * y1 * d2;
	coeffs[2] = 2.0 * sum_d_sqrd * e + 4.0 * f * f + four_a_sqrd * d2 * d2;
	coeffs[3] = 4.0 * sum_d_sqrd * f;
	coeffs[4] = sum_d_sqrd * sum_d_sqrd;  					// coefficient of t^4

	// find roots of the quartic equation

	int num_real_roots = SolveQuartic(coeffs, roots);

	bool	intersected = false;
	double 	t 		 	= kHugeValue;
	double   t_t;
    Point3D  p_t;
    Vector3D v_t;
    Vector3D v_t_xz;
    double   phi;
    double   theta;

	if (num_real_roots == 0)  // ray misses the torus
		return(false);

	// find the smallest root greater than kEpsilon, if any
	// the roots array is not sorted
	for (int j = 0; j < num_real_roots; j++)
		if (roots[j] > kEpsilon) {
            t_t = roots[j];
            p_t = ray.o + ray.d * t_t;
            v_t = Vector3D(p_t.x, p_t.y, p_t.z);
            v_t_xz = Vector3D(p_t.x, 0.0, p_t.z);

            v_t_xz.normalize();
            phi = acos((v_t_xz * Vector3D(1.0, 0.0, 0.0)));
            if (v_t_xz.z > 0) {
                phi = TWO_PI - phi;
            }

            theta = acos((v_t.length()*v_t.length() - a*a - b*b) / (2*a*b));
            if (v_t.y < 0) {
                theta = TWO_PI - theta;
            }

			if ((roots[j] < t) &&
                ((theta > theta1) && (theta < theta2)) &&
                (((phi > phi1) && (phi < phi2)) || ((phi + 2*M_PI) < phi2))) {
				t = roots[j];
                intersected = true;
            }
		}

	if(!intersected)
		return (false);

	tmin 			 	= t;

	return (true);
}
