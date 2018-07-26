// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "SV_GlossyReflector.h"

// ---------------------------------------------------------------- default constructor

SV_GlossyReflector::SV_GlossyReflector (void)
	:	SV_Phong(),
		glossy_specular_brdf(new SV_GlossySpecular)
{}


// ---------------------------------------------------------------- copy constructor

SV_GlossyReflector::SV_GlossyReflector(const SV_GlossyReflector& gr)
	: 	SV_Phong(gr) {

	if(gr.glossy_specular_brdf)
		glossy_specular_brdf = gr.glossy_specular_brdf->clone();
	else
		glossy_specular_brdf = NULL;
}


// ---------------------------------------------------------------- assignment operator

SV_GlossyReflector&
SV_GlossyReflector::operator= (const SV_GlossyReflector& rhs) {
	if (this == &rhs)
		return (*this);

	SV_Phong::operator=(rhs);

	if (glossy_specular_brdf) {
		delete glossy_specular_brdf;
		glossy_specular_brdf = NULL;
	}

	if (rhs.glossy_specular_brdf)
		glossy_specular_brdf = rhs.glossy_specular_brdf->clone();

	return (*this);
}


// ---------------------------------------------------------------- clone

SV_GlossyReflector*
SV_GlossyReflector::clone(void) const {
	return (new SV_GlossyReflector(*this));
}


// ---------------------------------------------------------------- destructor

SV_GlossyReflector::~SV_GlossyReflector(void) {
	if (glossy_specular_brdf) {
		delete glossy_specular_brdf;
		glossy_specular_brdf = NULL;
	}
}


// ------------------------------------------------------------------------------------ shade

RGBColor
SV_GlossyReflector::shade(ShadeRec& sr) {
	RGBColor L(SV_Phong::shade(sr));  // direct illumination

	Vector3D wo = -sr.ray.d;
	Vector3D wi;
	float    pdf;
	RGBColor fr = glossy_specular_brdf->sample_f(sr, wo, wi, pdf);
	Ray reflected_ray(sr.hit_point, wi);

	L += fr * sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1) * (sr.normal * wi) / pdf;

	return (L);
}


// ------------------------------------------------------------------------------------ shade

RGBColor
SV_GlossyReflector::area_light_shade(ShadeRec& sr) {
	RGBColor L(SV_Phong::area_light_shade(sr));  // direct illumination

	Vector3D wo = -sr.ray.d;
	Vector3D wi;
	float    pdf;
	RGBColor fr = glossy_specular_brdf->sample_f(sr, wo, wi, pdf);
	Ray reflected_ray(sr.hit_point, wi);

	L += fr * sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1) * (sr.normal * wi) / pdf;

	return (L);
}

