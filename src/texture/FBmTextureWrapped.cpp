#include "FBmTextureWrapped.h"

// ---------------------------------------------------------------- default constructor

FBmTextureWrapped::FBmTextureWrapped(void)
	:	Texture(),
        noise_ptr(NULL),
	    color(0.0),
	    min_value(-0.1),
	    max_value(1.1),
	    expansion_number(1.0)
{}

// ----------------------------------------------------------------constructor

FBmTextureWrapped::FBmTextureWrapped(LatticeNoise* ln_ptr)
	:	Texture(),
        noise_ptr(ln_ptr),
	    color(0.0),
	    min_value(-0.1),
	    max_value(1.1),
	    expansion_number(1.0)
{}

// ---------------------------------------------------------------- copy constructor

FBmTextureWrapped::FBmTextureWrapped(const FBmTextureWrapped& ft)
	:   Texture(ft),
        noise_ptr(ft.noise_ptr),
	    color(ft.color),
	    min_value(ft.min_value),
	    max_value(ft.max_value),
	    expansion_number(ft.expansion_number)
{}


// ---------------------------------------------------------------------- clone

FBmTextureWrapped*
FBmTextureWrapped::clone(void) const {
	return (new FBmTextureWrapped(*this));
}


// ---------------------------------------------------------------------- destructor

FBmTextureWrapped::~FBmTextureWrapped(void) {}


// ---------------------------------------------------------------------- assignment operator

FBmTextureWrapped&
FBmTextureWrapped::operator= (const FBmTextureWrapped& rhs) {
	if (this == &rhs)
		return (*this);

	Texture::operator= (rhs);

	noise_ptr        = rhs.noise_ptr;
	color            = rhs.color;
	min_value        = rhs.min_value;
	max_value        = rhs.max_value;
	expansion_number = rhs.expansion_number;

	return (*this);
}


// --------------------------------------------------- get_color
RGBColorFBmTextureWrapped::get_color(const ShadeRec& sr) const {	float noise = expansion_number * noise_ptr ->value_fbm(sr.local_hit_point);	float value = noise - floor(noise);	value = min_value + (max_value - min_value) * value;	return (value * color);}