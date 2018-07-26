#include "Emissive.h"


// ---------------------------------------------------------------- default constructor

Emissive::Emissive (void)
	:	Material()
{}



// ---------------------------------------------------------------- copy constructor

Emissive::Emissive(const Emissive& em)
	: 	Material(em)
{}


// ---------------------------------------------------------------- clone

Material*
Emissive::clone(void) const {
	return (new Emissive(*this));
}


// ---------------------------------------------------------------- assignment operator

Emissive&
Emissive::operator= (const Emissive& rhs) {
	if (this == &rhs)
		return (*this);

	Material::operator=(rhs);

	return (*this);
}


// ---------------------------------------------------------------- destructor

Emissive::~Emissive(void) {
}


// ---------------------------------------------------------------- get_Le

RGBColor
Emissive::get_Le(ShadeRec& sr) const {
    return (ls * ce);
}


// ---------------------------------------------------------------- area_light_shade

RGBColor
Emissive::area_light_shade(ShadeRec& sr) {
	if (-sr.normal * sr.ray.d > 0.0)
		return (ls * ce);
	else
		return (black);
}


// ---------------------------------------------------------------- path_shade

RGBColor
Emissive::path_shade(ShadeRec& sr) {
	if (-sr.normal * sr.ray.d > 0.0)
		return (ls * ce);
	else
		return (black);
}


// ---------------------------------------------------------------- global_shade

RGBColor
Emissive::global_shade(ShadeRec& sr) {
    if (sr.depth == 1) {
        return (black);
    }
	if (-sr.normal * sr.ray.d > 0.0)
		return (ls * ce);
	else
		return (black);
}

