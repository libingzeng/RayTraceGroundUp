// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "FresnelReflector.h"

// ---------------------------------------------------------- default constructor

FresnelReflector::FresnelReflector(void)
	: 	BRDF(),
		kr(0.0),
		cr(1.0)
{}

// ---------------------------------------------------------- destructor

FresnelReflector::~FresnelReflector(void) {}


// ---------------------------------------------------------------------- clone

FresnelReflector*
FresnelReflector::clone(void) const {
	return (new FresnelReflector(*this));
}


// ----------------------------------------------------------------------------------------- fresnel

float
FresnelReflector::fresnel(const ShadeRec& sr) const {
	Normal normal(sr.normal);
	float ndotd = -normal * sr.ray.d;
	float eta;

	if (ndotd < 0.0) {
		normal = -normal;
		eta = eta_out / eta_in;
	}
	else
		eta = eta_in / eta_out;

	float cos_theta_i 		= -normal * sr.ray.d;
	float temp 				= 1.0 - (1.0 - cos_theta_i * cos_theta_i) / (eta * eta);
	float cos_theta_t 		= sqrt (1.0 - (1.0 - cos_theta_i * cos_theta_i) / (eta * eta));
	float r_parallel 		= (eta * cos_theta_i - cos_theta_t) / (eta * cos_theta_i + cos_theta_t);
	float r_perpendicular 	= (cos_theta_i - eta * cos_theta_t) / (cos_theta_i + eta * cos_theta_t);
	float kr 				= 0.5 * (r_parallel * r_parallel + r_perpendicular * r_perpendicular);

	return (kr);
}


// ---------------------------------------------------------- f

RGBColor
FresnelReflector::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const {
	return (black);
}


// ----------------------------------------------------------------------------------------- sample_f

RGBColor
FresnelReflector::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wr) const {
	float ndotwo = sr.normal * wo;
	wr = -wo + 2.0 * sr.normal * ndotwo;
	return (fresnel(sr) * white / fabs(sr.normal * wr));
}

// ---------------------------------------------------------- rho

RGBColor
FresnelReflector::rho(const ShadeRec& sr, const Vector3D& wo) const {
	return (black);
}
