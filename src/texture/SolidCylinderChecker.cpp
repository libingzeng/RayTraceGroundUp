// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// <math.h> is # included in Texture.h // for floor()

#include "Maths.h"
#include "SolidCylinderChecker.h"


// ---------------------------------------------------------------- default constructor

SolidCylinderChecker::SolidCylinderChecker(void)
	:	Texture(),
		num_horizontal_checkers(20),
		num_vertical_checkers(10),
		num_radius_checkers_disk(3),
		horizontal_line_width(0.0),
		vertical_line_width(0.0),
		color1(white),
		color2(0.5),
		line_color(black)
{}


// ---------------------------------------------------------------- copy constructor

SolidCylinderChecker::SolidCylinderChecker(const SolidCylinderChecker& sc)
	: 	Texture(sc),
		num_horizontal_checkers(sc.num_horizontal_checkers),
		num_vertical_checkers(sc.num_vertical_checkers),
		num_radius_checkers_disk(sc.num_radius_checkers_disk),
		horizontal_line_width(sc.horizontal_line_width),
		vertical_line_width(sc.vertical_line_width),
		color1(sc.color1),
		color2(sc.color2),
		line_color(sc.line_color)
{}


// ---------------------------------------------------------------- assignment operator

SolidCylinderChecker&
SolidCylinderChecker::operator= (const SolidCylinderChecker& rhs)
{
	if (this == &rhs)
		return (*this);

	Texture::operator=(rhs);

	num_horizontal_checkers = rhs.num_horizontal_checkers;
	num_vertical_checkers 	= rhs.num_vertical_checkers;
	num_radius_checkers_disk= rhs.num_radius_checkers_disk;
	horizontal_line_width 	= rhs.horizontal_line_width;
	vertical_line_width 	= rhs.vertical_line_width;
	color1					= rhs.color1;
	color2 					= rhs.color2;
	line_color 				= rhs.line_color;

	return (*this);
}


// ---------------------------------------------------------------- clone

SolidCylinderChecker*
SolidCylinderChecker::clone(void) const {
	return (new SolidCylinderChecker(*this));
}


// ---------------------------------------------------------------- destructor

SolidCylinderChecker::~SolidCylinderChecker(void) {}



// ---------------------------------------------------------------- get_color

RGBColor
SolidCylinderChecker::get_color(const ShadeRec& sr) const {
	double x = sr.local_hit_point.x;
	double y = sr.local_hit_point.y;
	double z = sr.local_hit_point.z;

	double phi   = atan2(x, z);
	if (phi < 0.0)
		phi += 2.0 * PI;
	double phi_size = 2 * PI / num_horizontal_checkers; // in radians - azimuth angle
	int i_phi 	= floor(phi / phi_size);
	double f_phi 	= phi / phi_size - i_phi;
	double phi_line_width = 0.5 * vertical_line_width / phi_size;

	bool in_outline;

	if ((x*x + z*z) < (1 - 1e-5)) {// radius is 1
        // '1e-5' is very important.
        // if without it, some points on the 'wall' may meet this condition.
        // then, this 'if' returns wrong color for 'wall-point'.
        double rrr = sqrt(x*x + z*z);
        double rrr_size = 1.0 / num_radius_checkers_disk;
        int i_rrr = floor(rrr / rrr_size);
        double f_rrr = rrr / rrr_size - i_rrr;
        double rrr_line_width = 0.5 * horizontal_line_width / rrr_size;

        in_outline = (	f_phi < phi_line_width || f_phi > 1.0 - phi_line_width) ||
                       (f_rrr < rrr_line_width || f_rrr > 1.0 - rrr_line_width);

        if ((i_phi + i_rrr) % 2 == 0) {
            if (!in_outline)
                return (color1);
        }
        else {
            if (!in_outline)
                return (color2);
        }
	}
	else {
        double yyy = y - (-1.0); // bottom y is -1.0, top y is 1.0
        double yyy_size = (1.0 - (-1.0)) / num_vertical_checkers;
        int i_yyy = floor(yyy / yyy_size);
        double f_yyy = yyy / yyy_size - i_yyy;
        double yyy_line_width = 0.5 * horizontal_line_width / yyy_size;

        in_outline = (	f_phi < phi_line_width || f_phi > 1.0 - phi_line_width) ||
                       (f_yyy < yyy_line_width || f_yyy > 1.0 - yyy_line_width);

        if ((i_phi + i_yyy) % 2 == 0) {
            if (!in_outline)
                return (color1);
        }
        else {
            if (!in_outline)
                return (color2);
        }
	}

	return (line_color);
}
