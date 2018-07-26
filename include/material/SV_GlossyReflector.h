#ifndef SV_GLOSSYREFLECTOR_H
#define SV_GLOSSYREFLECTOR_H

// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "SV_Phong.h"
#include "SV_GlossySpecular.h"

class SV_GlossyReflector: public SV_Phong {
	public:

		SV_GlossyReflector(void);

		SV_GlossyReflector(const SV_GlossyReflector& gr);

		SV_GlossyReflector&
		operator= (const SV_GlossyReflector& rhs);

		virtual SV_GlossyReflector*
		clone(void) const;

		~SV_GlossyReflector(void);

		void
		set_kr(const float k);

		void
		set_cr(Texture* t_ptr);

		void
		set_exponent(const float e);

		void
		set_samples(const int num_samples, const float exp);

		virtual RGBColor
		shade(ShadeRec& s);

        virtual RGBColor
        area_light_shade(ShadeRec& sr);

	private:

		SV_GlossySpecular* glossy_specular_brdf;
};


// ---------------------------------------------------------------- set_kr

inline void
SV_GlossyReflector::set_kr(const float k) {
	glossy_specular_brdf->set_ks(k);
}


// ---------------------------------------------------------------- set_cr

inline void
SV_GlossyReflector::set_cr(Texture* t_ptr) {
	glossy_specular_brdf->set_cs(t_ptr);

}


// -------------------------------------------------------------- set_exp

inline void
SV_GlossyReflector::set_exponent(const float e) {
	glossy_specular_brdf->set_exp(e);
}

// -------------------------------------------------------------- set_samples

inline void
SV_GlossyReflector::set_samples(const int num_samples, const float exp) {
	glossy_specular_brdf->set_samples(num_samples, exp);
}

#endif // SV_GLOSSYREFLECTOR_H
