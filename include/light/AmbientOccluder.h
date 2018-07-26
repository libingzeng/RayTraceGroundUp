#ifndef AMBIENTOCCLUDER_H
#define AMBIENTOCCLUDER_H

#include "Light.h"
#include "Sampler.h"

class AmbientOccluder: public Light {
	public:

		AmbientOccluder(void);

		AmbientOccluder(const AmbientOccluder& a);

		virtual Light*
		clone(void) const;

		AmbientOccluder&
		operator= (const AmbientOccluder& rhs);

		virtual
		~AmbientOccluder(void);

		void
		scale_radiance(const float b);

		void
		set_color(const float c);

		void
		set_color(const RGBColor& c);

		void
		set_color(const float r, const float g, const float b);

        void
        set_min_amount(const float m);

		void
		set_sampler(Sampler* s_ptr);

		virtual Vector3D
		get_direction(ShadeRec& sr);

		virtual bool
		in_shadow(const Ray& ray, const ShadeRec& sr) const;

		virtual RGBColor
		L(ShadeRec& sr);

	private:

		float		ls;
		RGBColor	color;
		float 		min_amount;
		Vector3D 	u, v, w;
		Sampler*	sampler_ptr;
};


// ------------------------------------------------------------------------------- scale_radiance

inline void
AmbientOccluder::scale_radiance(const float b) {
	ls = b;
}

// ------------------------------------------------------------------------------- set_color

inline void
AmbientOccluder::set_color(const float c) {
	color.r = c; color.g = c; color.b = c;
}


// ------------------------------------------------------------------------------- set_color

inline void
AmbientOccluder::set_color(const RGBColor& c) {
	color = c;
}


// ------------------------------------------------------------------------------- set_color

inline void
AmbientOccluder::set_color(const float r, const float g, const float b) {
	color.r = r; color.g = g; color.b = b;
}


// ------------------------------------------------------------------------------- set_min_amount

inline void
AmbientOccluder::set_min_amount(const float m) {
	min_amount = m;
}
#endif // AMBIENTOCCLUDER_H
