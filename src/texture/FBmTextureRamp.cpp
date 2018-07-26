#include "FBmTextureRamp.h"

// ---------------------------------------------------------------- default constructor

FBmTextureRamp::FBmTextureRamp(void)
	:	Texture(),
        noise_ptr(NULL),
        ramp_ptr(NULL),
	    perturbation(1.0)
{}

// ----------------------------------------------------------------constructor

FBmTextureRamp::FBmTextureRamp(LatticeNoise* ln_ptr)
	:	Texture(),
        noise_ptr(ln_ptr),
        ramp_ptr(NULL),
	    perturbation(1.0)
{}

// ----------------------------------------------------------------constructor

FBmTextureRamp::FBmTextureRamp(Image* im_ptr)
	:	Texture(),
        noise_ptr(NULL),
        ramp_ptr(im_ptr),
	    perturbation(1.0)
{}

// ---------------------------------------------------------------- copy constructor

FBmTextureRamp::FBmTextureRamp(const FBmTextureRamp& ft)
	:   Texture(ft),
        noise_ptr(ft.noise_ptr),
        ramp_ptr(ft.ramp_ptr),
	    perturbation(ft.perturbation)
{}


// ---------------------------------------------------------------------- clone

FBmTextureRamp*
FBmTextureRamp::clone(void) const {
	return (new FBmTextureRamp(*this));
}


// ---------------------------------------------------------------------- destructor

FBmTextureRamp::~FBmTextureRamp(void) {}


// ---------------------------------------------------------------------- assignment operator

FBmTextureRamp&
FBmTextureRamp::operator= (const FBmTextureRamp& rhs) {
	if (this == &rhs)
		return (*this);

	Texture::operator= (rhs);

	noise_ptr        = rhs.noise_ptr;
	ramp_ptr         = rhs.ramp_ptr;
	perturbation     = rhs.perturbation;

	return (*this);
}


// --------------------------------------------------- get_color
RGBColorFBmTextureRamp::get_color(const ShadeRec& sr) const {	float 	noise 	= noise_ptr->value_fbm(sr.local_hit_point);	float 	y 		= sr.local_hit_point.y + perturbation * noise;	float 	u 		= (1.0 + sin(y)) / 2.0;	int 	row 	= 0;    // use top row of ramp image	int 	column 	= (int) (u * (ramp_ptr->get_hres() - 1));	return (ramp_ptr->get_color(row, column));}