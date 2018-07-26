#ifndef SV_REFLECTIVE_H
#define SV_REFLECTIVE_H

// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "SV_Phong.h"
#include "SV_PerfectSpecular.h"

class SV_Reflective: public SV_Phong {
	public:

		SV_Reflective(void);

		SV_Reflective(const SV_Reflective& rm);

		SV_Reflective&
		operator= (const SV_Reflective& rhs);

		virtual SV_Reflective*
		clone(void) const;

		~SV_Reflective(void);

		void
		set_kr(const float k);

		void
		set_cr(Texture* t_ptr);

		virtual RGBColor
		shade(ShadeRec& s);

        virtual RGBColor
        path_shade(ShadeRec& sr);

        virtual RGBColor
        global_shade(ShadeRec& sr);
	private:

		SV_PerfectSpecular* reflective_brdf;
};


// ---------------------------------------------------------------- set_kr

inline void
SV_Reflective::set_kr(const float k) {
	reflective_brdf->set_kr(k);
}


// ---------------------------------------------------------------- set_cr

inline void
SV_Reflective::set_cr(Texture* t_ptr) {
	reflective_brdf->set_cr(t_ptr);

}


#endif // SV_REFLECTIVE_H
