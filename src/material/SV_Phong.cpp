#include "SV_Phong.h"

// ---------------------------------------------------------------- default constructor

SV_Phong::SV_Phong (void)
	:	Material(),
		ambient_brdf(new SV_Lambertian),
		diffuse_brdf(new SV_Lambertian),
		specular_brdf(new SV_GlossySpecular)
{}



// ---------------------------------------------------------------- copy constructor

SV_Phong::SV_Phong(const SV_Phong& p)
	: 	Material(p)
{
	if(p.ambient_brdf)
		ambient_brdf = p.ambient_brdf->clone();
	else  ambient_brdf = NULL;

	if(p.diffuse_brdf)
		diffuse_brdf = p.diffuse_brdf->clone();
	else  diffuse_brdf = NULL;

	if(p.specular_brdf)
		specular_brdf = p.specular_brdf->clone();
	else  specular_brdf = NULL;
}


// ---------------------------------------------------------------- clone

Material*
SV_Phong::clone(void) const {
	return (new SV_Phong(*this));
}


// ---------------------------------------------------------------- assignment operator

SV_Phong&
SV_Phong::operator= (const SV_Phong& rhs) {
	if (this == &rhs)
		return (*this);

	Material::operator=(rhs);

	if (ambient_brdf) {
		delete ambient_brdf;
		ambient_brdf = NULL;
	}

	if (rhs.ambient_brdf)
		ambient_brdf = rhs.ambient_brdf->clone();

	if (diffuse_brdf) {
		delete diffuse_brdf;
		diffuse_brdf = NULL;
	}

	if (rhs.diffuse_brdf)
		diffuse_brdf = rhs.diffuse_brdf->clone();

	if (specular_brdf) {
		delete specular_brdf;
		specular_brdf = NULL;
	}

	if (rhs.specular_brdf)
		specular_brdf = rhs.specular_brdf->clone();

	return (*this);
}


// ---------------------------------------------------------------- destructor

SV_Phong::~SV_Phong(void) {

	if (ambient_brdf) {
		delete ambient_brdf;
		ambient_brdf = NULL;
	}

	if (diffuse_brdf) {
		delete diffuse_brdf;
		diffuse_brdf = NULL;
	}

	if (specular_brdf) {
		delete specular_brdf;
		specular_brdf = NULL;
	}
}


// ---------------------------------------------------------------- shade
RGBColor
SV_Phong::shade(ShadeRec& sr) {
	Vector3D 	wo 			= -sr.ray.d;
	RGBColor 	L 			= ambient_brdf->rho(sr, wo) * sr.w.ambient_ptr->L(sr);
	int 		num_lights	= sr.w.lights.size();

	for (int j = 0; j < num_lights; j++) {
		Vector3D 	wi 		= sr.w.lights[j]->get_direction(sr);
		float 		ndotwi 	= sr.normal * wi;

		if (ndotwi > 0.0) {
			bool in_shadow = false;

			if (sr.w.lights[j]->get_cast_shadow()) {
				Ray shadowRay(sr.hit_point, wi);
				in_shadow = sr.w.lights[j]->in_shadow(shadowRay, sr);
			}

			if (!in_shadow)
				L += (	diffuse_brdf->f(sr, wo, wi)
						  + specular_brdf->f(sr, wo, wi)) * sr.w.lights[j]->L(sr) * ndotwi;
		}
	}

	return (L);
}


// ---------------------------------------------------------------- area_light_shade

RGBColor
SV_Phong::area_light_shade(ShadeRec& sr) {
	Vector3D 	wo 			= -sr.ray.d;
	RGBColor 	L 			= ambient_brdf->rho(sr, wo) * sr.w.ambient_ptr->L(sr);
	int 		num_lights	= sr.w.lights.size();

	for (int j = 0; j < num_lights; j++) {
		Vector3D 	wi 		= sr.w.lights[j]->get_direction(sr);
		float 		ndotwi 	= sr.normal * wi;

		if (ndotwi > 0.0) {
			bool in_shadow = false;

			if (sr.w.lights[j]->get_cast_shadow()) {
				Ray shadow_ray(sr.hit_point, wi);
				in_shadow = sr.w.lights[j]->in_shadow(shadow_ray, sr);
			}

			if (!in_shadow)
				L += (	diffuse_brdf->f(sr, wo, wi)
						  + specular_brdf->f(sr, wo, wi)) * sr.w.lights[j]->L(sr) * sr.w.lights[j]->G(sr) * ndotwi / sr.w.lights[j]->pdf(sr);
		}
	}

	return (L);
}


