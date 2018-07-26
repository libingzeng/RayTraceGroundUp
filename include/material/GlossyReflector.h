#ifndef GLOSSYREFLECTOR_H
#define GLOSSYREFLECTOR_H

// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "Phong.h"
#include "GlossySpecular.h"

class GlossyReflector: public Phong {
	public:

		GlossyReflector(void);

		GlossyReflector(const GlossyReflector& gr);

		GlossyReflector&
		operator= (const GlossyReflector& rhs);

		virtual GlossyReflector*
		clone(void) const;

		~GlossyReflector(void);

		void
		set_kr(const float k);

		void
		set_cr(const RGBColor& c);

		void
		set_cr(const float r, const float g, const float b);

		void
		set_cr(const float c);

		void
		set_exponent(const float e);

		void
		set_samples(const int num_samples, const float exp);

		virtual RGBColor
		shade(ShadeRec& s);

        virtual RGBColor
        area_light_shade(ShadeRec& sr);

	private:

		GlossySpecular* glossy_specular_brdf;
};


// ---------------------------------------------------------------- set_kr

inline void
GlossyReflector::set_kr(const float k) {
	glossy_specular_brdf->set_ks(k);
}


// ---------------------------------------------------------------- set_cr

inline void
GlossyReflector::set_cr(const RGBColor& c) {
	glossy_specular_brdf->set_cs(c);

}


// ---------------------------------------------------------------- set_cr

inline void
GlossyReflector::set_cr(const float r, const float g, const float b) {
	glossy_specular_brdf->set_cs(r, g, b);
}


// ---------------------------------------------------------------- set_cr

inline void
GlossyReflector::set_cr(const float c) {
	glossy_specular_brdf->set_cs(c);
}

// -------------------------------------------------------------- set_exp

inline void
GlossyReflector::set_exponent(const float e) {
	glossy_specular_brdf->set_exp(e);
}

// -------------------------------------------------------------- set_samples

inline void
GlossyReflector::set_samples(const int num_samples, const float exp) {
	glossy_specular_brdf->set_samples(num_samples, exp);
}

#endif // GLOSSYREFLECTOR_H
