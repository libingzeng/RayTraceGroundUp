#include "Material.h"

// ---------------------------------------------------------------- default constructor

Material::Material(void) {}


// ---------------------------------------------------------------- copy constructor

Material::Material(const Material& m) {}



// ---------------------------------------------------------------- assignment operator

Material&
Material::operator= (const Material& rhs) {
	if (this == &rhs)
		return (*this);

	return (*this);
}


// ---------------------------------------------------------------- destructor

Material::~Material(void)
{}


// ---------------------------------------------------------------------- set_sampler

void
Material::set_sampler(Sampler* s_ptr) {}


// ---------------------------------------------------------------- get_Le

RGBColor
Material::get_Le(ShadeRec& sr) const {return RGBColor(0);}


// ---------------------------------------------------------------- shade

RGBColor
Material::shade(ShadeRec& sr) {
	return (black);
}


// ---------------------------------------------------------------- shade

RGBColor
Material::area_light_shade(ShadeRec& sr) {
	return (black);
}


// ---------------------------------------------------------------- path_shade

RGBColor
Material::path_shade(ShadeRec& sr) {
	return (black);
}


// ---------------------------------------------------------------- global_shade

RGBColor
Material::global_shade(ShadeRec& sr) {
	return (black);
}


