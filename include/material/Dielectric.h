#ifndef DIELECTRIC_H
#define DIELECTRIC_H


#include "Phong.h"
#include "FresnelReflector.h"
#include "FresnelTransmitter.h"


class Dielectric : public Phong
{
	public:

		Dielectric(void);

		Dielectric(const Dielectric& dm);

		Dielectric&
		operator= (const Dielectric& rhs);

		virtual Dielectric*
		clone(void) const;

		~Dielectric(void);


		void
		set_eta_in(const float ei);

		void
		set_eta_out(const float eo);

		void
		set_cf_in(const RGBColor& ci);

		void
		set_cf_in(const float ri, const float gi, const float bi);

		void
		set_cf_in(const float ci);

		void
		set_cf_out(const RGBColor& co);

		void
		set_cf_out(const float ro, const float go, const float bo);

		void
		set_cf_out(const float co);

		virtual RGBColor
		shade(ShadeRec& s);

	private:

		RGBColor 			cf_in;			// interior filter color
		RGBColor 			cf_out;			// exterior filter color

		FresnelReflector* fresnel_brdf;
		FresnelTransmitter* fresnel_btdf;
};



// -------------------------------------------------------------- set_eta_in

inline void
Dielectric::set_eta_in(const float ei) {
	fresnel_brdf->set_eta_in(ei);
	fresnel_btdf->set_eta_in(ei);
}


// -------------------------------------------------------------- set_eta_out

inline void
Dielectric::set_eta_out(const float eo) {
	fresnel_brdf->set_eta_out(eo);
	fresnel_btdf->set_eta_out(eo);
}


// -------------------------------------------------------------- set_cf_in

inline void
Dielectric::set_cf_in(const RGBColor& ci) {
	cf_in = ci;
}


// -------------------------------------------------------------- set_cf_in

inline void
Dielectric::set_cf_in(const float ri, const float gi, const float bi) {
	cf_in.r = ri; cf_in.g = gi; cf_in.b = bi;
}


// -------------------------------------------------------------- set_cf_in

inline void
Dielectric::set_cf_in(const float ci) {
	cf_in.r = ci; cf_in.g = ci; cf_in.b = ci;
}


// -------------------------------------------------------------- set_cf_out

inline void
Dielectric::set_cf_out(const RGBColor& co) {
	cf_out = co;
}


// -------------------------------------------------------------- set_cf_out

inline void
Dielectric::set_cf_out(const float ro, const float go, const float bo) {
	cf_out.r = ro; cf_out.g = go; cf_out.b = bo;
}


// -------------------------------------------------------------- set_cf_out

inline void
Dielectric::set_cf_out(const float co) {
	cf_out.r = co; cf_out.g = co; cf_out.b = co;
}

#endif // DIELECTRIC_H
