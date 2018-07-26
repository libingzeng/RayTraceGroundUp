#include "Matte.h"

// ---------------------------------------------------------------- default constructor

Matte::Matte (void)
	:	Material(),
		ambient_brdf(new Lambertian),
		diffuse_brdf(new Lambertian)
{}



// ---------------------------------------------------------------- copy constructor

Matte::Matte(const Matte& m)
	: 	Material(m)
{
	if(m.ambient_brdf)
		ambient_brdf = m.ambient_brdf->clone();
	else  ambient_brdf = NULL;

	if(m.diffuse_brdf)
		diffuse_brdf = m.diffuse_brdf->clone();
	else  diffuse_brdf = NULL;
}


// ---------------------------------------------------------------- clone

Material*
Matte::clone(void) const {
	return (new Matte(*this));
}


// ---------------------------------------------------------------- assignment operator

Matte&
Matte::operator= (const Matte& rhs) {
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

	return (*this);
}


// ---------------------------------------------------------------- destructor

Matte::~Matte(void) {

	if (ambient_brdf) {
		delete ambient_brdf;
		ambient_brdf = NULL;
	}

	if (diffuse_brdf) {
		delete diffuse_brdf;
		diffuse_brdf = NULL;
	}
}


// ---------------------------------------------------------------------- set_sampler

void
Matte::set_sampler(Sampler* s_ptr) {
    diffuse_brdf->set_sampler(s_ptr);
}


// ---------------------------------------------------------------- shade

RGBColor
Matte::shade(ShadeRec& sr) {
	Vector3D 	wo 			= -sr.ray.d;
	RGBColor 	L 			= ambient_brdf->rho(sr, wo) * sr.w.ambient_ptr->L(sr);
	int 		num_lights	= sr.w.lights.size();

	for (int j = 0; j < num_lights; j++) {
		Vector3D wi = sr.w.lights[j]->get_direction(sr);
		float ndotwi = sr.normal * wi;

		if (ndotwi > 0.0) {
			bool in_shadow = false;

			if (sr.w.lights[j]->get_cast_shadow()) {
				Ray shadowRay(sr.hit_point, wi);
				in_shadow = sr.w.lights[j]->in_shadow(shadowRay, sr);
			}

			if (!in_shadow)
                L += diffuse_brdf->f(sr, wo, wi) * sr.w.lights[j]->L(sr) * ndotwi;
		}
	}

	return (L);
}


// ---------------------------------------------------------------- area_light_shade

RGBColor
Matte::area_light_shade(ShadeRec& sr) {
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
				L += diffuse_brdf->f(sr, wo, wi)
                    * sr.w.lights[j]->L(sr) * sr.w.lights[j]->G(sr) * ndotwi / sr.w.lights[j]->pdf(sr);
		}
	}

	return (L);
}


// ---------------------------------------------------------------- path_shade

RGBColor
Matte::path_shade(ShadeRec& sr) {
	Vector3D 	wo = -sr.ray.d;
	Vector3D 	wi;
	float 		pdf;
	RGBColor 	f = diffuse_brdf->sample_f(sr, wo, wi, pdf);
	float 		ndotwi = sr.normal * wi;
	Ray 		reflected_ray(sr.hit_point, wi);

	return (f * sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1) * ndotwi / pdf);
}


// ---------------------------------------------------------------- global_shade

RGBColor
Matte::global_shade(ShadeRec& sr) {
    RGBColor L;

    if (sr.depth == 0) {
        L = area_light_shade(sr);
    }

	Vector3D 	wo = -sr.ray.d;
	Vector3D 	wi;
	float 		pdf;
	RGBColor 	f = diffuse_brdf->sample_f(sr, wo, wi, pdf);
	float 		ndotwi = sr.normal * wi;
	Ray 		reflected_ray(sr.hit_point, wi);

	L += f * sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1) * ndotwi / pdf;

	return (L);
}
