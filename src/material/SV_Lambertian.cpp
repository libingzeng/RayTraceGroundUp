#include "SV_Lambertian.h"
#include "Constants.h"

// ---------------------------------------------------------------------- default constructor

SV_Lambertian::SV_Lambertian(void)
	:   BRDF(),
		kd(0.0),
		cd(new ConstantColor)
{}


// ---------------------------------------------------------------------- copy constructor

SV_Lambertian::SV_Lambertian(const SV_Lambertian& lamb)
	:   BRDF(lamb),
		kd(lamb.kd),
		cd(lamb.cd)
{}


// ---------------------------------------------------------------------- clone

SV_Lambertian*
SV_Lambertian::clone(void) const {
	return (new SV_Lambertian(*this));
}


// ---------------------------------------------------------------------- destructor

SV_Lambertian::~SV_Lambertian(void) {}


// ---------------------------------------------------------------------- assignment operator

SV_Lambertian&
SV_Lambertian::operator= (const SV_Lambertian& rhs) {
	if (this == &rhs)
		return (*this);

	BRDF::operator= (rhs);

	kd = rhs.kd;
	cd = rhs.cd;

	return (*this);
}


// ---------------------------------------------------------------------- set_sampler

void
SV_Lambertian::set_sampler(Sampler* s_ptr) {
	sampler_ptr = s_ptr;
	sampler_ptr->map_samples_to_hemisphere(1);
}

// ---------------------------------------------------------------------- f

RGBColor
SV_Lambertian::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const {
	return (kd * (cd->get_color(sr)) * invPI);
}



// ---------------------------------------------------------------------- sample_f

// this generates a direction by sampling the hemisphere with a cosine distribution
// this is called in path_shade for any material with a diffuse shading component
// the samples have to be stored with a cosine distribution

RGBColor
SV_Lambertian::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const {

	Vector3D w = sr.normal;
	Vector3D v = Vector3D(0.0034, 1, 0.0071) ^ w;
	v.normalize();
	Vector3D u = v ^ w;

	Point3D sp = sampler_ptr->sample_hemisphere();
	wi = sp.x * u + sp.y * v + sp.z * w;
	wi.normalize();

	pdf = sr.normal * wi * invPI;

	return (kd * (cd->get_color(sr)) * invPI);
}


// ---------------------------------------------------------------------- rho

RGBColor
SV_Lambertian::rho(const ShadeRec& sr, const Vector3D& wo) const {
	return (kd * (cd->get_color(sr)));
}

