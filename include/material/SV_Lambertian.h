#ifndef SV_LAMBERTIAN_H
#define SV_LAMBERTIAN_H

#include "BRDF.h"
#include "ConstantColor.h"

class SV_Lambertian: public BRDF {
	public:

		SV_Lambertian(void);

		SV_Lambertian(const SV_Lambertian& lamb);

		virtual SV_Lambertian*
		clone(void) const;

		~SV_Lambertian(void);

		SV_Lambertian&
		operator= (const SV_Lambertian& rhs);

		virtual RGBColor
		f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;

		virtual RGBColor
		sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const;

		virtual RGBColor
		rho(const ShadeRec& sr, const Vector3D& wo) const;

		void
		set_ka(const float ka);

		void
		set_kd(const float kd);

		void
		set_cd(Texture* t_ptr);

		void
		set_sampler(Sampler* sp);   			// any type of sampling

	private:

		float		kd;
		Texture* 	cd;
		Sampler*	sampler_ptr;    // for use in sample_f
};




// -------------------------------------------------------------- set_ka

inline void
SV_Lambertian::set_ka(const float k) {
	kd = k;
}



// -------------------------------------------------------------- set_kd

inline void
SV_Lambertian::set_kd(const float k) {
	kd = k;
}


// -------------------------------------------------------------- set_cd

inline void
SV_Lambertian::set_cd(Texture* t_ptr) {
	cd = t_ptr;
}

#endif // SV_LAMBERTIAN_H
