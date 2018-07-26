#ifndef FRESNELREFLECTOR_H
#define FRESNELREFLECTOR_H


// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// this implements perfect specular reflection for indirect illumination
// with reflective materials

#include "BRDF.h"
#include "Normal.h"

class FresnelReflector: public BRDF
{
	public:

		FresnelReflector(void);

		~FresnelReflector(void);

		virtual FresnelReflector*
		clone(void) const;

		void
		set_kr(const float k);

		void
		set_eta_in(const float ei);

		void
		set_eta_out(const float eo);

		void
		set_cr(const RGBColor& c);

		void
		set_cr(const float r, const float g, const float b);

		void
		set_cr(const float c);

		virtual RGBColor
		f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;

		virtual RGBColor
		sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wr) const;

		virtual RGBColor
		rho(const ShadeRec& sr, const Vector3D& wo) const;

        virtual float
        fresnel(const ShadeRec& sr) const;
	private:

		float		kr;			// reflection coefficient
		RGBColor 	cr;			// the reflection colour
		float		eta_in;
		float		eta_out;
};


// -------------------------------------------------------------- set_kr

inline void
FresnelReflector::set_kr(const float k) {
	kr = k;
}


// -------------------------------------------------------------- set_cr

inline void
FresnelReflector::set_cr(const RGBColor& c) {
	cr = c;
}


// ---------------------------------------------------------------- set_cr

inline void
FresnelReflector::set_cr(const float r, const float g, const float b) {
	cr.r = r; cr.g = g; cr.b = b;
}


// ---------------------------------------------------------------- set_cr

inline void
FresnelReflector::set_cr(const float c) {
	cr.r = c; cr.g = c; cr.b = c;
}


// -------------------------------------------------------------- set_eta_in

inline void
FresnelReflector::set_eta_in(const float ei) {
	eta_in = ei;
}


// -------------------------------------------------------------- set_eta_out

inline void
FresnelReflector::set_eta_out(const float eo) {
	eta_out = eo;
}
#endif // FRESNELREFLECTOR_H
