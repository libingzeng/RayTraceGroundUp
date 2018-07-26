#ifndef EMISSIVE_H
#define EMISSIVE_H

#include "Material.h"

class Emissive: public Material {
	private:

		float		ls;		// radiance scaling factor
		RGBColor 	ce;		// color

	public:

		// constructors, set functions, etc ...

		Emissive(void);

		Emissive(const Emissive& em);

		virtual Material*
		clone(void) const;

		Emissive&
		operator= (const Emissive& rhs);

		~Emissive(void);

		void
		scale_radiance(const float _ls);

		void
		set_ce(const float c);

		void
		set_ce(const RGBColor& c);

		void
		set_ce(const float r, const float g, const float b);

		virtual RGBColor
		get_Le(ShadeRec& sr) const;

		virtual RGBColor
		area_light_shade(ShadeRec& sr);

		virtual RGBColor
		path_shade(ShadeRec& sr);

		virtual RGBColor
		global_shade(ShadeRec& sr);
};


// ------------------------------------------------------------------------------- scale_radiance

inline void
Emissive::scale_radiance(const float l) {
	ls = l;
}

// ------------------------------------------------------------------------------- set_color

inline void
Emissive::set_ce(const float c) {
	ce.r = c; ce.g = c; ce.b = c;
}


// ------------------------------------------------------------------------------- set_color

inline void
Emissive::set_ce(const RGBColor& c) {
	ce = c;
}


// ------------------------------------------------------------------------------- set_color

inline void
Emissive::set_ce(const float r, const float g, const float b) {
	ce.r = r; ce.g = g; ce.b = b;
}

#endif // EMISSIVE_H
