#ifndef FRESNELTRANSMITTER_H
#define FRESNELTRANSMITTER_H

#include "BTDF.h"

class FresnelTransmitter: public BTDF {
	public:

		FresnelTransmitter(void);

		FresnelTransmitter(const FresnelTransmitter& ft);

		virtual FresnelTransmitter*
		clone(void);

		~FresnelTransmitter(void);

		FresnelTransmitter&
		operator= (const FresnelTransmitter& rhs);

		void
		set_eta_in(const float ei);

		void
		set_eta_out(const float eo);

		bool
		tir(const ShadeRec& sr) const;

		virtual RGBColor
		f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;

		virtual RGBColor
		sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wt) const;

		virtual RGBColor
		rho(const ShadeRec& sr, const Vector3D& wo) const;

        virtual float
        fresnel(const ShadeRec& sr) const;

	private:

		float	eta_in;
		float	eta_out;
};



// -------------------------------------------------------------- set_eta_in

inline void
FresnelTransmitter::set_eta_in(const float ei) {
	eta_in = ei;
}


// -------------------------------------------------------------- set_eta_out

inline void
FresnelTransmitter::set_eta_out(const float eo) {
	eta_out = eo;
}
#endif // FRESNELTRANSMITTER_H
