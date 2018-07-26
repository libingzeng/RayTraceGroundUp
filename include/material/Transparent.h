#ifndef TRANSPARENT_H
#define TRANSPARENT_H

#include "Phong.h"
#include "PerfectSpecular.h"
#include "PerfectTransmitter.h"


class Transparent : public Phong
{
	public:

		Transparent(void);

		Transparent(const Transparent& tm);

		Transparent&
		operator= (const Transparent& rhs);

		virtual Transparent*
		clone(void) const;

		~Transparent(void);

		void
		set_kr(const float k);

		void
		set_cr(const RGBColor& c);

		void
		set_cr(const float r, const float g, const float b);

		void
		set_cr(const float c);

		void
		set_kt(const float k);

        void
        set_ior(const float i);

		virtual RGBColor
		shade(ShadeRec& s);

	private:

		PerfectSpecular* reflective_brdf;
		PerfectTransmitter* specular_btdf;
};

// ---------------------------------------------------------------- set_kr

inline void
Transparent::set_kr(const float k) {
	reflective_brdf->set_kr(k);
}


// ---------------------------------------------------------------- set_cr

inline void
Transparent::set_cr(const RGBColor& c) {
	reflective_brdf->set_cr(c);

}


// ---------------------------------------------------------------- set_cr

inline void
Transparent::set_cr(const float r, const float g, const float b) {
	reflective_brdf->set_cr(r, g, b);
}


// ---------------------------------------------------------------- set_cr

inline void
Transparent::set_cr(const float c) {
	reflective_brdf->set_cr(c);
}

// ---------------------------------------------------------------- set_kt

inline void
Transparent::set_kt(const float k) {
	specular_btdf->set_kt(k);
}

// ---------------------------------------------------------------- set_ior

inline void
Transparent::set_ior(const float i) {
	specular_btdf->set_ior(i);
}

#endif
