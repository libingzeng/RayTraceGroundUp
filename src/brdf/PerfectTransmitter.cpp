// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "PerfectTransmitter.h"

// ------------------------------------------------------------------- default constructor

PerfectTransmitter::PerfectTransmitter(void)
	: 	BTDF(),
		kt(0.0), 
		ior(1.0)
{}


// ------------------------------------------------------------------- copy constructor

PerfectTransmitter::PerfectTransmitter(const PerfectTransmitter& pt)
	: 	BTDF(pt),
		kt(pt.kt), 
		ior(pt.ior)
{}


// ------------------------------------------------------------------- clone

PerfectTransmitter* 
PerfectTransmitter::clone(void) {
	return (new PerfectTransmitter(*this));
}


// ------------------------------------------------------------------- destructor

PerfectTransmitter::~PerfectTransmitter(void) {}



// ------------------------------------------------------------------- assignment operator
		
PerfectTransmitter&							
PerfectTransmitter::operator= (const PerfectTransmitter& rhs) {
	if (this == &rhs)
		return (*this);
		
	kt = rhs.kt;
	ior = rhs.ior;

	return (*this);
}


// ------------------------------------------------------------------- tir
// tests for total internal reflection

bool													
PerfectTransmitter::tir(const ShadeRec& sr) const {
	Vector3D wo(-sr.ray.d); 
	float cos_thetai = sr.normal * wo;  
	float eta = ior;
	
	if (cos_thetai < 0.0) 
		eta = 1.0 / eta; 
		
	return (1.0 - (1.0 - cos_thetai * cos_thetai) / (eta * eta) < 0.0);
}	


// ------------------------------------------------------------------- f

RGBColor
PerfectTransmitter::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const {
	return (black);
}


// ------------------------------------------------------------------- sample_f
// this computes the direction wt for perfect transmission
// and returns the transmission coefficient
// this is only called when there is no total internal reflection

RGBColor
PerfectTransmitter::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wt) const {
	
	Normal n(sr.normal);
	float cos_thetai = n * wo;
	float eta = ior;	
		
	if (cos_thetai < 0.0) {			// transmitted ray is outside     
		cos_thetai = -cos_thetai;
		n = -n;  					// reverse direction of normal
		eta = 1.0 / eta; 			// invert ior 
	}

	float temp = 1.0 - (1.0 - cos_thetai * cos_thetai) / (eta * eta);
	float cos_theta2 = sqrt(temp);
	wt = -wo / eta - (cos_theta2 - cos_thetai / eta) * n;   
	
	return (kt / (eta * eta) * white / fabs(sr.normal * wt));
}


// ------------------------------------------------------------------- rho

RGBColor
PerfectTransmitter::rho(const ShadeRec& sr, const Vector3D& wo) const {
	return (black);
}


