// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "GlossyReflector.h"

// ---------------------------------------------------------------- default constructor

GlossyReflector::GlossyReflector (void)
	:	Phong(),
		glossy_specular_brdf(new GlossySpecular)
{}


// ---------------------------------------------------------------- copy constructor

GlossyReflector::GlossyReflector(const GlossyReflector& gr)
	: 	Phong(gr) {

	if(gr.glossy_specular_brdf)
		glossy_specular_brdf = gr.glossy_specular_brdf->clone();
	else
		glossy_specular_brdf = NULL;
}


// ---------------------------------------------------------------- assignment operator

GlossyReflector&
GlossyReflector::operator= (const GlossyReflector& rhs) {
	if (this == &rhs)
		return (*this);

	Phong::operator=(rhs);

	if (glossy_specular_brdf) {
		delete glossy_specular_brdf;
		glossy_specular_brdf = NULL;
	}

	if (rhs.glossy_specular_brdf)
		glossy_specular_brdf = rhs.glossy_specular_brdf->clone();

	return (*this);
}


// ---------------------------------------------------------------- clone

GlossyReflector*
GlossyReflector::clone(void) const {
	return (new GlossyReflector(*this));
}


// ---------------------------------------------------------------- destructor

GlossyReflector::~GlossyReflector(void) {
	if (glossy_specular_brdf) {
		delete glossy_specular_brdf;
		glossy_specular_brdf = NULL;
	}
}


// ------------------------------------------------------------------------------------ shade

RGBColor
GlossyReflector::shade(ShadeRec& sr) {
	RGBColor L(Phong::shade(sr));  // direct illumination

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
GlossyReflector::area_light_shade(ShadeRec& sr) {
	RGBColor L(Phong::area_light_shade(sr));  // direct illumination

	Vector3D wo = -sr.ray.d;
	Vector3D wi;
	float    pdf;
	RGBColor fr = glossy_specular_brdf->sample_f(sr, wo, wi, pdf);
	Ray reflected_ray(sr.hit_point, wi);

	L += fr * sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1) * (sr.normal * wi) / pdf;

	return (L);
}

