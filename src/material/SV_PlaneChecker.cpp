// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// <math.h> is # included in Texture.h

#include "Maths.h"
#include "SV_PlaneChecker.h"


// ---------------------------------------------------------------- default constructor

SV_PlaneChecker::SV_PlaneChecker(void)
	:	Texture(),
		size(0.2),
		line_width(0.0),
		color1(new ConstantColor(white)),
		color2(new ConstantColor(white)),
		line_color(new ConstantColor(black))
{}


// ---------------------------------------------------------------- copy constructor

SV_PlaneChecker::SV_PlaneChecker(const SV_PlaneChecker& pc)
	: 	Texture(pc),
		size(pc.size),
		line_width(pc.line_width),
		color1(pc.color1),
		color2(pc.color2),
		line_color(pc.line_color)
{}


// ---------------------------------------------------------------- assignment operator

SV_PlaneChecker&
SV_PlaneChecker::operator= (const SV_PlaneChecker& rhs)
{
	if (this == &rhs)
		return (*this);

	Texture::operator=(rhs);

	size 	                = rhs.size;
	line_width 	            = rhs.line_width;
	color1					= rhs.color1;
	color2 					= rhs.color2;
	line_color 				= rhs.line_color;

	return (*this);
}


// ---------------------------------------------------------------- clone

SV_PlaneChecker*
SV_PlaneChecker::clone(void) const {
	return (new SV_PlaneChecker(*this));
}


// ---------------------------------------------------------------- destructor

SV_PlaneChecker::~SV_PlaneChecker(void) {}



// ---------------------------------------------------------------- get_color

RGBColor
SV_PlaneChecker::get_color(const ShadeRec& sr) const {

	float x = sr.local_hit_point.x;
	float z = sr.local_hit_point.z;
	int ix = floor(x / size);
	int iz = floor(z / size);
	float fx = x / size - ix;
	float fz = z / size - iz;
	float width = 0.5 * line_width / size;
	bool in_outline = (fx < width || fx > 1.0 - width) || (fz < width || fz > 1.0 - width);

	if ((ix + iz) % 2 == 0) {
		if (!in_outline)
			return (color1->get_color(sr));
	}
	else {
		if (!in_outline)
			return (color2->get_color(sr));
	}

	return (line_color->get_color(sr));
}
