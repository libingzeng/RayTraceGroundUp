#include "ConstantColor.h"

// ---------------------------------------------------------------- default constructor

ConstantColor::ConstantColor(void)
	:	Texture(),
	    color(0.0)
{}

// ---------------------------------------------------------------- default constructor

ConstantColor::ConstantColor(const RGBColor& c)
	:	Texture(),
	    color(c)
{}


// ---------------------------------------------------------------- copy constructor

ConstantColor::ConstantColor(const ConstantColor& cc)
	:   Texture(cc),
		color(cc.color)
{}


// ---------------------------------------------------------------------- clone

ConstantColor*
ConstantColor::clone(void) const {
	return (new ConstantColor(*this));
}


// ---------------------------------------------------------------------- destructor

ConstantColor::~ConstantColor(void) {}


// ---------------------------------------------------------------------- assignment operator

ConstantColor&
ConstantColor::operator= (const ConstantColor& rhs) {
	if (this == &rhs)
		return (*this);

	Texture::operator= (rhs);

	color = rhs.color;

	return (*this);
}


// --------------------------------------------------- get_color

RGBColor
ConstantColor::get_color(const ShadeRec& sr) const {
    return (color);
}
