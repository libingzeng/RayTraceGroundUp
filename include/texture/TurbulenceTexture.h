#ifndef TURBULENCETEXTURE_H
#define TURBULENCETEXTURE_H

#include "Texture.h"
#include "LatticeNoise.h"

class TurbulenceTexture : public Texture
{
	public:

		TurbulenceTexture(void);

		TurbulenceTexture(LatticeNoise* ln_ptr);

		TurbulenceTexture(const TurbulenceTexture& tt);

		virtual TurbulenceTexture*
		clone(void) const;

		virtual
		~TurbulenceTexture(void);

		void
		set_noise(LatticeNoise* ln_ptr);

		void
		set_color(const RGBColor& c);

		void
		set_color(const float r, const float g, const float b);

		void
		set_color(const float c);

		void
		set_min_value(const float mi);

		void
		set_max_value(const float ma);

		virtual RGBColor
		get_color(const ShadeRec& sr) const;

	protected:

		TurbulenceTexture&
		operator= (const TurbulenceTexture& rhs);

    private:

		LatticeNoise*		noise_ptr;		RGBColor 	  		color;		float		  		min_value, max_value;	// scaling factors};


// ---------------------------------------------------------------- set_color

inline void
TurbulenceTexture::set_noise(LatticeNoise* ln_ptr) {
	noise_ptr = ln_ptr;
}


// ---------------------------------------------------------------- set_color

inline void
TurbulenceTexture::set_color(const RGBColor& c) {
	color = c;
}


// ---------------------------------------------------------------- set_color

inline void
TurbulenceTexture::set_color(const float r, const float g, const float b) {
	color.r = r; color.g = g; color.b = b;
}


// ---------------------------------------------------------------- set_color

inline void
TurbulenceTexture::set_color(const float c) {
	color.r = c; color.g = c; color.b = c;
}


// ---------------------------------------------------------------- set_min_value

inline void
TurbulenceTexture::set_min_value(const float mi) {
	min_value = mi;
}


// ---------------------------------------------------------------- set_max_value

inline void
TurbulenceTexture::set_max_value(const float ma) {
	max_value = ma;
}

#endif // TURBULENCETEXTURE_H
