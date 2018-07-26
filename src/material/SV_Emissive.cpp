#include "SV_Emissive.h"


// ---------------------------------------------------------------- default constructor

SV_Emissive::SV_Emissive (void)
	:	Material()
{}



// ---------------------------------------------------------------- copy constructor

SV_Emissive::SV_Emissive(const SV_Emissive& em)
	: 	Material(em)
{}


// ---------------------------------------------------------------- clone

Material*
SV_Emissive::clone(void) const {
	return (new SV_Emissive(*this));
}


// ---------------------------------------------------------------- assignment operator

SV_Emissive&
SV_Emissive::operator= (const SV_Emissive& rhs) {
	if (this == &rhs)
		return (*this);

	Material::operator=(rhs);

	return (*this);
}


// ---------------------------------------------------------------- destructor

SV_Emissive::~SV_Emissive(void) {
}


// ---------------------------------------------------------------- get_Le

RGBColor
SV_Emissive::get_Le(ShadeRec& sr) const {
    return (ls * (ce->get_color(sr)));
}


// ---------------------------------------------------------------- area_light_shade

RGBColor
SV_Emissive::area_light_shade(ShadeRec& sr) {
	if (-sr.normal * sr.ray.d > 0.0)
		return (ls * (ce->get_color(sr)));
	else
		return (black);
}


// ---------------------------------------------------------------- path_shade

RGBColor
SV_Emissive::path_shade(ShadeRec& sr) {
	if (-sr.normal * sr.ray.d > 0.0)
		return (ls * (ce->get_color(sr)));
	else
		return (black);
}


// ---------------------------------------------------------------- global_shade

RGBColor
SV_Emissive::global_shade(ShadeRec& sr) {
    if (sr.depth == 1) {
        return (black);
    }
	if (-sr.normal * sr.ray.d > 0.0)
		return (ls * (ce->get_color(sr)));
	else
		return (black);
}

