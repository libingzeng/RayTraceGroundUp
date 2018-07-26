#ifndef SV_PLANECHECKER_H
#define SV_PLANECHECKER_H

// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "Texture.h"
#include "RGBColor.h"
#include "ConstantColor.h"

// For checkers that are approximately square at the "equator" (polar angle theta = 90 degrees), there should be
// twice as many horizontal_checkers as vertical_checkers

//-----------------------------------------------------------------------------------class SV_PlaneChecker

class SV_PlaneChecker: public Texture {
	public:

		SV_PlaneChecker(void);

		SV_PlaneChecker(const SV_PlaneChecker& pc);

		SV_PlaneChecker&
		operator= (const SV_PlaneChecker& rhs);

		virtual SV_PlaneChecker*
		clone(void) const;

		~SV_PlaneChecker(void);

		virtual RGBColor
		get_color(const ShadeRec& sr) const;

		void
		set_size(const float s);

		void
		set_line_width(const float width);

		void
		set_color1(Texture* t_ptr);

		void
		set_color2(Texture* t_ptr);

		void
		set_line_color(Texture* t_ptr);

	private:

		float		size;		                // width of a single checker
		float		line_width;		            // width of the lines as a fraction of the checker width
		Texture*	color1;						// checker color 1
		Texture*	color2;						// checker color 2
		Texture*	line_color;					// the line color
};


// ---------------------------------------------------------------------------------------------------

// inlined access functions


inline void
SV_PlaneChecker::set_size(const float s) {
	size = s;
}

inline void
SV_PlaneChecker::set_line_width(const float width) {
	line_width = width;
}

inline void
SV_PlaneChecker::set_color1(Texture* t_ptr) {
	color1 = t_ptr;
}

inline void
SV_PlaneChecker::set_color2(Texture* t_ptr) {
	color2 = t_ptr;
}

inline void
SV_PlaneChecker::set_line_color(Texture* t_ptr) {
	line_color = t_ptr;
}


#endif // SV_PLANECHECKER_H
