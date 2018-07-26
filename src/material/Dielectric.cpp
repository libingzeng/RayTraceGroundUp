#include "Dielectric.h"

// ---------------------------------------------------------------- default constructor

Dielectric::Dielectric (void)
	:	Phong(),
		fresnel_brdf(new FresnelReflector),
		fresnel_btdf(new FresnelTransmitter)
{}


// ---------------------------------------------------------------- copy constructor

Dielectric::Dielectric(const Dielectric& dm)
	: 	Phong(dm) {

	if(dm.fresnel_brdf)
		fresnel_brdf = dm.fresnel_brdf->clone();
	else
		fresnel_brdf = NULL;

	if(dm.fresnel_btdf)
		fresnel_btdf = dm.fresnel_btdf->clone();
	else
		fresnel_btdf = NULL;
}


// ---------------------------------------------------------------- assignment operator

Dielectric&
Dielectric::operator= (const Dielectric& rhs) {
	if (this == &rhs)
		return (*this);

	Phong::operator=(rhs);

	if (fresnel_brdf) {
		delete fresnel_brdf;
		fresnel_brdf = NULL;
	}

	if (rhs.fresnel_brdf)
		fresnel_brdf = rhs.fresnel_brdf->clone();

	if (fresnel_btdf) {
		delete fresnel_btdf;
		fresnel_btdf = NULL;
	}

	if (rhs.fresnel_btdf)
		fresnel_btdf = rhs.fresnel_btdf->clone();

	return (*this);
}


// ---------------------------------------------------------------- clone

Dielectric*
Dielectric::clone(void) const {
	return (new Dielectric(*this));
}


// ---------------------------------------------------------------- destructor

Dielectric::~Dielectric(void) {
	if (fresnel_brdf) {
		delete fresnel_brdf;
		fresnel_brdf = NULL;
	}
	if (fresnel_btdf) {
		delete fresnel_btdf;
		fresnel_btdf = NULL;
	}
}


// ------------------------------------------------------------------------------------ shade

RGBColor
Dielectric::shade(ShadeRec& sr) {
	RGBColor L(Phong::shade(sr));

	Vector3D 	wi;
	Vector3D 	wo(-sr.ray.d);
	RGBColor 	fr = fresnel_brdf->sample_f(sr, wo, wi);  	// computes wi
	Ray 		reflected_ray(sr.hit_point, wi);
	double 		t;
	RGBColor 	Lr, Lt;
	float 		ndotwi =  sr.normal * wi;

	if(fresnel_btdf->tir(sr)) {								// total internal reflection
		if (ndotwi < 0.0) {
			// reflected ray is inside

			Lr = sr.w.tracer_ptr->trace_ray(reflected_ray, t, sr.depth + 1);
			L += cf_in.powc(t) * Lr;   						// inside filter color
		}
		else {
			// reflected ray is outside

			Lr = sr.w.tracer_ptr->trace_ray(reflected_ray, t, sr.depth + 1);   // kr = 1
			L += cf_out.powc(t) * Lr;   					// outside filter color
		}
	}
	else { 													// no total internal reflection
		Vector3D wt;
		RGBColor ft = fresnel_btdf->sample_f(sr, wo, wt);  	// computes wt
		Ray transmitted_ray(sr.hit_point, wt);
		float ndotwt = sr.normal * wt;

		if (ndotwi < 0.0) {
			// reflected ray is inside

			Lr = fr * sr.w.tracer_ptr->trace_ray(reflected_ray, t, sr.depth + 1) * fabs(ndotwi);
			L += cf_in.powc(t) * Lr;     					// inside filter color

			// transmitted ray is outside

			Lt = ft * sr.w.tracer_ptr->trace_ray(transmitted_ray, t, sr.depth + 1) * fabs(ndotwt);
			L += cf_out.powc(t) * Lt;   					// outside filter color
		}
		else {
			// reflected ray is outside

			Lr = fr * sr.w.tracer_ptr->trace_ray(reflected_ray, t, sr.depth + 1) * fabs(ndotwi);
			L += cf_out.powc(t) * Lr;						// outside filter color

			// transmitted ray is inside

			Lt = ft * sr.w.tracer_ptr->trace_ray(transmitted_ray, t, sr.depth + 1) * fabs(ndotwt);
			L += cf_in.powc(t) * Lt; 						// inside filter color
		}
	}

	return (L);
}
