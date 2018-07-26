#include "Light.h"

#include "Constants.h"

// ---------------------------------------------------------------------- default constructor

Light::Light(void) {}

// ---------------------------------------------------------------------- dopy constructor

Light::Light(const Light& ls) {}


// ---------------------------------------------------------------------- assignment operator

Light&
Light::operator= (const Light& rhs) {
	if (this == &rhs)
		return (*this);

	return (*this);
}


// ---------------------------------------------------------------------- destructor

Light::~Light(void) {}



// ---------------------------------------------------------------------- L
// returns the radiance

RGBColor
Light::L(ShadeRec& s) {
	return (black);
}

bool
Light::in_shadow(const Ray& ray, const ShadeRec& sr) const {return false;}

void
Light::set_cast_shadow(bool cs) {}

bool
Light::get_cast_shadow(void) {return false;}



// ---------------------------------------------------------------- G
// G is part of the geometric factor

float
Light::G(const ShadeRec& sr) const {return 1.0;}


// ---------------------------------------------------------------- pdf

float
Light::pdf(ShadeRec& sr) {return 1.0;}
