// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// <math.h> is # included in Texture.h

#include "Maths.h"
#include "SphereChecker.h"


// ---------------------------------------------------------------- default constructor

SphereChecker::SphereChecker(void)
	:	Texture(),
		num_horizontal_checkers(20),
		num_vertical_checkers(10),
		horizontal_line_width(0.0),
		vertical_line_width(0.0),
		color1(white),
		color2(0.5),
		line_color(black)
{}


// ---------------------------------------------------------------- copy constructor

SphereChecker::SphereChecker(const SphereChecker& sc)
	: 	Texture(sc),
		num_horizontal_checkers(sc.num_horizontal_checkers),
		num_vertical_checkers(sc.num_vertical_checkers),
		horizontal_line_width(sc.horizontal_line_width),
		vertical_line_width(sc.vertical_line_width),
		color1(sc.color1),
		color2(sc.color2),
		line_color(sc.line_color)
{}


// ---------------------------------------------------------------- assignment operator

SphereChecker&
SphereChecker::operator= (const SphereChecker& rhs)
{
	if (this == &rhs)
		return (*this);

	Texture::operator=(rhs);

	num_horizontal_checkers = rhs.num_horizontal_checkers;
	num_vertical_checkers 	= rhs.num_vertical_checkers;
	horizontal_line_width 	= rhs.horizontal_line_width;
	vertical_line_width 	= rhs.vertical_line_width;
	color1					= rhs.color1;
	color2 					= rhs.color2;
	line_color 				= rhs.line_color;

	return (*this);
}


// ---------------------------------------------------------------- clone

SphereChecker*
SphereChecker::clone(void) const {
	return (new SphereChecker(*this));
}


// ---------------------------------------------------------------- destructor

SphereChecker::~SphereChecker(void) {}



// ---------------------------------------------------------------- get_color

RGBColor
SphereChecker::get_color(const ShadeRec& sr) const {
	double x = sr.local_hit_point.x;
	double y = sr.local_hit_point.y;
	double z = sr.local_hit_point.z;

	double theta = acos(y);
	double phi   = atan2(x, z);
	if (phi < 0.0)
		phi += 2.0 * PI;

	double phi_size = 2 * PI / num_horizontal_checkers;   	// in radians - azimuth angle
	double theta_size = PI / num_vertical_checkers;   		// in radians - polar angle

	int i_phi 	= floor(phi / phi_size);
	int i_theta = floor(theta / theta_size);

	double f_phi 	= phi / phi_size - i_phi;
	double f_theta 	= theta / theta_size - i_theta;

	double phi_line_width = 0.5 * vertical_line_width / phi_size;
	double theta_line_width = 0.5 * horizontal_line_width / theta_size;

	bool in_outline = (	f_phi < phi_line_width || f_phi > 1.0 - phi_line_width) ||
						(f_theta < theta_line_width || f_theta > 1.0 - theta_line_width);

	if ((i_phi + i_theta) % 2 == 0) {
		if (!in_outline)
			return (color1);
	}
	else {
		if (!in_outline)
			return (color2);
	}

	return (line_color);
}





