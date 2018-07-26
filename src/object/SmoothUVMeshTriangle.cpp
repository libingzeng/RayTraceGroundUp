// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "Constants.h"
#include "SmoothUVMeshTriangle.h"
#include "ShadeRec.h"


// ----------------------------------------------------------------  default constructor

SmoothUVMeshTriangle::SmoothUVMeshTriangle(void)
	: 	SmoothMeshTriangle()
{}


// ---------------------------------------------------------------- constructor

SmoothUVMeshTriangle::SmoothUVMeshTriangle (Mesh* _mesh_ptr, const int i0, const int i1, const int i2)
	: 	SmoothMeshTriangle(_mesh_ptr, i0, i1, i2)
{}


// ---------------------------------------------------------------- clone

SmoothUVMeshTriangle*
SmoothUVMeshTriangle::clone (void) const {
	return (new SmoothUVMeshTriangle (*this));
}


// ---------------------------------------------------------------- copy constructor

SmoothUVMeshTriangle::SmoothUVMeshTriangle (const SmoothUVMeshTriangle& fmt)
	:	SmoothMeshTriangle(fmt)
{}


// ---------------------------------------------------------------- assignment operator

SmoothUVMeshTriangle&
SmoothUVMeshTriangle::operator= (const SmoothUVMeshTriangle& rhs) {
	if (this == &rhs)
		return (*this);

	SmoothMeshTriangle::operator= (rhs);

	return (*this);
}


// ---------------------------------------------------------------- destructor

SmoothUVMeshTriangle::~SmoothUVMeshTriangle(void) {}


// ---------------------------------------------------------------- interpolate_normal

Normal
SmoothUVMeshTriangle::interpolate_normal(const float beta, const float gamma) const {
	Normal normal((1 - beta - gamma) * mesh_ptr->normals[index0]
						+ beta * mesh_ptr->normals[index1]
								+ gamma * mesh_ptr->normals[index2]);
	normal.normalize();

	return(normal);
}

// ---------------------------------------------------------------- hit

bool
SmoothUVMeshTriangle::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {
	Point3D v0(mesh_ptr->vertices[index0]);
	Point3D v1(mesh_ptr->vertices[index1]);
	Point3D v2(mesh_ptr->vertices[index2]);

	double a = v0.x - v1.x, b = v0.x - v2.x, c = ray.d.x, d = v0.x - ray.o.x;
	double e = v0.y - v1.y, f = v0.y - v2.y, g = ray.d.y, h = v0.y - ray.o.y;
	double i = v0.z - v1.z, j = v0.z - v2.z, k = ray.d.z, l = v0.z - ray.o.z;

	double m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
	double q = g * i - e * k, s = e * j - f * i;

	double inv_denom  = 1.0 / (a * m + b * q + c * s);

	double e1 = d * m - b * n - c * p;
	double beta = e1 * inv_denom;

	if (beta < 0.0)
	 	return (false);

	double r = e * l - h * i;
	double e2 = a * n + d * q + c * r;
	double gamma = e2 * inv_denom;

	if (gamma < 0.0)
	 	return (false);

	if (beta + gamma > 1.0)
		return (false);

	double e3 = a * p - b * r + d * s;
	double t = e3 * inv_denom;

	if (t < kEpsilon)
		return (false);

	tmin 				= t;
	sr.normal 			= interpolate_normal(beta, gamma); // for smooth shading
	sr.local_hit_point 	= ray.o + t * ray.d;

	sr.u = interpolate_u(beta, gamma);
	sr.v = interpolate_v(beta, gamma);

	return (true);
}


// ---------------------------------------------------------------- shadow_hit

bool
SmoothUVMeshTriangle::shadow_hit(const Ray& ray, double& tmin) const {
	Point3D v0(mesh_ptr->vertices[index0]);
	Point3D v1(mesh_ptr->vertices[index1]);
	Point3D v2(mesh_ptr->vertices[index2]);

	double a = v0.x - v1.x, b = v0.x - v2.x, c = ray.d.x, d = v0.x - ray.o.x;
	double e = v0.y - v1.y, f = v0.y - v2.y, g = ray.d.y, h = v0.y - ray.o.y;
	double i = v0.z - v1.z, j = v0.z - v2.z, k = ray.d.z, l = v0.z - ray.o.z;

	double m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
	double q = g * i - e * k, s = e * j - f * i;

	double inv_denom  = 1.0 / (a * m + b * q + c * s);

	double e1 = d * m - b * n - c * p;
	double beta = e1 * inv_denom;

	if (beta < 0.0)
	 	return (false);

	double r = e * l - h * i;
	double e2 = a * n + d * q + c * r;
	double gamma = e2 * inv_denom;

	if (gamma < 0.0)
	 	return (false);

	if (beta + gamma > 1.0)
		return (false);

	double e3 = a * p - b * r + d * s;
	double t = e3 * inv_denom;

	if (t < kEpsilon)
		return (false);

	tmin 				= t;

	return (true);
}

