// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "FresnelTransmitter.h"

// ------------------------------------------------------------------- default constructor

FresnelTransmitter::FresnelTransmitter(void)
	: 	BTDF(),
		eta_in(1.0),
		eta_out(1.0)
{}


// ------------------------------------------------------------------- copy constructor

FresnelTransmitter::FresnelTransmitter(const FresnelTransmitter& ft)
	: 	BTDF(ft),
		eta_in(ft.eta_in),
		eta_out(ft.eta_out)
{}


// ------------------------------------------------------------------- clone

FresnelTransmitter*
FresnelTransmitter::clone(void) {
	return (new FresnelTransmitter(*this));
}


// ------------------------------------------------------------------- destructor

FresnelTransmitter::~FresnelTransmitter(void) {}



// ------------------------------------------------------------------- assignment operator

FresnelTransmitter&
FresnelTransmitter::operator= (const FresnelTransmitter& rhs) {
	if (this == &rhs)
		return (*this);

	eta_in = rhs.eta_in;
	eta_out = rhs.eta_out;

	return (*this);
}


// ----------------------------------------------------------------------------------------- fresnel

float
FresnelTransmitter::fresnel(const ShadeRec& sr) const {
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

	return (1.0 - kr);
}


// ------------------------------------------------------------------- tir
// tests for total internal reflection

bool
FresnelTransmitter::tir(const ShadeRec& sr) const {
	Vector3D wo(-sr.ray.d);
	float cos_thetai = sr.normal * wo;
	float eta = eta_in / eta_out;

	if (cos_thetai < 0.0)
		eta = 1.0 / eta;

	return (1.0 - (1.0 - cos_thetai * cos_thetai) / (eta * eta) < 0.0);
}


// ------------------------------------------------------------------- f

RGBColor
FresnelTransmitter::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const {
	return (black);
}


// ------------------------------------------------------------------- sample_f
// this computes the direction wt for perfect transmission
// and returns the transmission coefficient
// this is only called when there is no total internal reflection

RGBColor
FresnelTransmitter::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wt) const {

	Normal n(sr.normal);
	float cos_thetai = n * wo;
	float eta = eta_in / eta_out;

	if (cos_thetai < 0.0) {			// transmitted ray is outside
		cos_thetai = -cos_thetai;
		n = -n;  					// reverse direction of normal
		eta = 1.0 / eta; 			// invert ior
	}

	float temp = 1.0 - (1.0 - cos_thetai * cos_thetai) / (eta * eta);
	float cos_theta2 = sqrt(temp);
	wt = -wo / eta - (cos_theta2 - cos_thetai / eta) * n;

	return (fresnel(sr) / (eta * eta) * white / fabs(sr.normal * wt));
}


// ------------------------------------------------------------------- rho

RGBColor
FresnelTransmitter::rho(const ShadeRec& sr, const Vector3D& wo) const {
	return (black);
}


