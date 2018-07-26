#ifndef PLANECHECKER_H
#define PLANECHECKER_H

// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "Texture.h"
#include "RGBColor.h"

// For checkers that are approximately square at the "equator" (polar angle theta = 90 degrees), there should be
// twice as many horizontal_checkers as vertical_checkers

//-----------------------------------------------------------------------------------class PlaneChecker

class PlaneChecker: public Texture {
	public:

		PlaneChecker(void);

		PlaneChecker(const PlaneChecker& pc);

		PlaneChecker&
		operator= (const PlaneChecker& rhs);

		virtual PlaneChecker*
		clone(void) const;

		~PlaneChecker(void);

		virtual RGBColor
		get_color(const ShadeRec& sr) const;

		void
		set_size(const float s);

		void
		set_line_width(const float width);

		void
		set_color1(const float r, const float g, const float b);

		void
		set_color1(const float c);

		void
		set_color1(const RGBColor& c);

		void
		set_color2(const float r, const float g, const float b);

		void
		set_color2(const float c);

		void
		set_color2(const RGBColor& c);

		void
		set_line_color(const float r, const float g, const float b);

		void
		set_line_color(const float c);

		void
		set_line_color(const RGBColor& c);

	private:

		float		size;		                // width of a single checker
		float		line_width;		            // width of the lines as a fraction of the checker width
		RGBColor	color1;						// checker color 1
		RGBColor	color2;						// checker color 2
		RGBColor	line_color;					// the line color
};


// ---------------------------------------------------------------------------------------------------

// inlined access functions


inline void
PlaneChecker::set_size(const float s) {
	size = s;
}

inline void
PlaneChecker::set_line_width(const float width) {
	line_width = width;
}

inline void
PlaneChecker::set_color1(const float r, const float g, const float b) {
	color1.r = r; color1.g = g; color1.b = b;
}

inline void
PlaneChecker::set_color1(const float c) {
	color1.r = c; color1.g = c; color1.b = c;
}

inline void
PlaneChecker::set_color1(const RGBColor& c) {
color1.r = c.r; color1.g = c.g; color1.b = c.b;
}

inline void
PlaneChecker::set_color2(const float r, const float g, const float b) {
	color2.r = r; color2.g = g; color2.b = b;
}

inline void
PlaneChecker::set_color2(const float c) {
	color2.r = c; color2.g = c; color2.b = c;
}

inline void
PlaneChecker::set_color2(const RGBColor& c) {
color2.r = c.r; color2.g = c.g; color2.b = c.b;
}

inline void
PlaneChecker::set_line_color(const float r, const float g, const float b) {
	line_color.r = r; line_color.g = g; line_color.b = b;
}

inline void
PlaneChecker::set_line_color(const float c) {
	line_color.r = c; line_color.g = c; line_color.b = c;
}

inline void
PlaneChecker::set_line_color(const RGBColor& c) {
	line_color.r = c.r; line_color.g = c.g; line_color.b = c.b;
}

#endif // PLANECHECKER_H
