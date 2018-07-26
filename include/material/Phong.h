#ifndef __PHONG__
#define __PHONG__

#include "Material.h"
#include "Lambertian.h"
#include "GlossySpecular.h"

//----------------------------------------------------------------------------- class Phong

class Phong: public Material {
	public:

		Phong(void);

		Phong(const Phong& m);

		virtual Material*
		clone(void) const;

		Phong&
		operator= (const Phong& rhs);

		~Phong(void);

		void
		set_ka(const float k);

		void
		set_kd(const float k);

		void
		set_ks(const float k);

		void
		set_exp (const float exp);

		void
		set_cd(const RGBColor c);

		void
		set_cd(const float r, const float g, const float b);

		void
		set_cd(const float c);

		virtual RGBColor
		shade(ShadeRec& sr);

        virtual RGBColor
        area_light_shade(ShadeRec& sr);

	private:

		Lambertian*		ambient_brdf;
		Lambertian*		diffuse_brdf;
		GlossySpecular*	specular_brdf;
};


// ---------------------------------------------------------------- set_ka
// this sets Lambertian::kd
// there is no Lambertian::ka data member because ambient reflection
// is diffuse reflection

inline void
Phong::set_ka(const float ka) {
	ambient_brdf->set_kd(ka);
}


// ---------------------------------------------------------------- set_kd
// this also sets Lambertian::kd, but for a different Lambertian object

inline void
Phong::set_kd (const float kd) {
	diffuse_brdf->set_kd(kd);
}


// ---------------------------------------------------------------- set_ks
// this sets GlossySpecular::ks

inline void
Phong::set_ks (const float ks) {
	specular_brdf->set_ks(ks);
}

// ---------------------------------------------------------------- set_exp
// this sets GlossySpecular::ks

inline void
Phong::set_exp (const float exp) {
	specular_brdf->set_exp(exp);
}


// ---------------------------------------------------------------- set_cd

inline void
Phong::set_cd(const RGBColor c) {
	ambient_brdf->set_cd(c);
	diffuse_brdf->set_cd(c);
}


// ---------------------------------------------------------------- set_cd

inline void
Phong::set_cd(const float r, const float g, const float b) {
	ambient_brdf->set_cd(r, g, b);
	diffuse_brdf->set_cd(r, g, b);
}

// ---------------------------------------------------------------- set_cd

inline void
Phong::set_cd(const float c) {
	ambient_brdf->set_cd(c);
	diffuse_brdf->set_cd(c);
}

#endif
