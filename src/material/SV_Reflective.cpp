// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "SV_Reflective.h"

// ---------------------------------------------------------------- default constructor

SV_Reflective::SV_Reflective (void)
	:	SV_Phong(),
		reflective_brdf(new SV_PerfectSpecular)
{}


// ---------------------------------------------------------------- copy constructor

SV_Reflective::SV_Reflective(const SV_Reflective& rm)
	: 	SV_Phong(rm) {

	if(rm.reflective_brdf)
		reflective_brdf = rm.reflective_brdf->clone();
	else
		reflective_brdf = NULL;
}


// ---------------------------------------------------------------- assignment operator

SV_Reflective&
SV_Reflective::operator= (const SV_Reflective& rhs) {
	if (this == &rhs)
		return (*this);

	SV_Phong::operator=(rhs);

	if (reflective_brdf) {
		delete reflective_brdf;
		reflective_brdf = NULL;
	}

	if (rhs.reflective_brdf)
		reflective_brdf = rhs.reflective_brdf->clone();

	return (*this);
}


// ---------------------------------------------------------------- clone

SV_Reflective*
SV_Reflective::clone(void) const {
	return (new SV_Reflective(*this));
}


// ---------------------------------------------------------------- destructor

SV_Reflective::~SV_Reflective(void) {
	if (reflective_brdf) {
		delete reflective_brdf;
		reflective_brdf = NULL;
	}
}


// ------------------------------------------------------------------------------------ shade

RGBColor
SV_Reflective::shade(ShadeRec& sr) {
	RGBColor L(SV_Phong::shade(sr));  // direct illumination

	Vector3D wo = -sr.ray.d;
	Vector3D wi;
	RGBColor fr = reflective_brdf->sample_f(sr, wo, wi);
	Ray reflected_ray(sr.hit_point, wi);
	reflected_ray.depth = sr.depth + 1;

	L += fr * sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1) * (sr.normal * wi);

	return (L);
}


// ------------------------------------------------------------------------------------ path_shade

RGBColor
SV_Reflective::path_shade(ShadeRec& sr) {
	Vector3D 	wo = -sr.ray.d;
	Vector3D 	wi;
	float 		pdf;
	RGBColor 	fr = reflective_brdf->sample_f(sr, wo, wi, pdf);
	Ray 		reflected_ray(sr.hit_point, wi);

	return (fr * sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1) * (sr.normal * wi) / pdf);
}


// ------------------------------------------------------------------------------------ global_shade

RGBColor
SV_Reflective::global_shade(ShadeRec& sr) {
	Vector3D 	wo = -sr.ray.d;
	Vector3D 	wi;
	float 		pdf;
	RGBColor 	fr = reflective_brdf->sample_f(sr, wo, wi, pdf);
	Ray 		reflected_ray(sr.hit_point, wi);

	if (sr.depth == 0) {
        return (fr * sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 2) * (sr.normal * wi) / pdf);
	}
	else {
        return (fr * sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1) * (sr.normal * wi) / pdf);
	}
}

