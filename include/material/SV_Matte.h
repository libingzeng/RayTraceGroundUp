#ifndef SV_MATTE_H
#define SV_MATTE_H

#include "Material.h"
#include "SV_Lambertian.h"

//----------------------------------------------------------------------------- class SV_Matte

class SV_Matte: public Material {
	public:

		SV_Matte(void);

		SV_Matte(const SV_Matte& m);

		virtual Material*
		clone(void) const;

		SV_Matte&
		operator= (const SV_Matte& rhs);

		~SV_Matte(void);

		void
		set_ka(const float k);

		void
		set_kd(const float k);

		void
		set_cd(Texture* t_ptr);

		void
		set_sampler(Sampler* sp);   			// any type of sampling

		virtual RGBColor
		shade(ShadeRec& sr);

        virtual RGBColor
        area_light_shade(ShadeRec& sr);

        virtual RGBColor
        path_shade(ShadeRec& sr);

        virtual RGBColor
        global_shade(ShadeRec& sr);
	private:

		SV_Lambertian*		ambient_brdf;
		SV_Lambertian*		diffuse_brdf;
};


// ---------------------------------------------------------------- set_ka
// this sets Lambertian::kd
// there is no Lambertian::ka data member because ambient reflection
// is diffuse reflection

inline void
SV_Matte::set_ka(const float ka) {
	ambient_brdf->set_kd(ka);
}


// ---------------------------------------------------------------- set_kd
// this also sets Lambertian::kd, but for a different Lambertian object

inline void
SV_Matte::set_kd (const float kd) {
	diffuse_brdf->set_kd(kd);
}


// ---------------------------------------------------------------- set_cd

inline void
SV_Matte::set_cd(Texture* t_ptr) {
	ambient_brdf->set_cd(t_ptr);
	diffuse_brdf->set_cd(t_ptr);
}

#endif // SV_MATTE_H
