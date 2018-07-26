#ifndef FBMTEXTUREWRAPPED_H
#define FBMTEXTUREWRAPPED_H

#include "Texture.h"
#include "LatticeNoise.h"

class FBmTextureWrapped : public Texture
{
	public:

		FBmTextureWrapped(void);

		FBmTextureWrapped(LatticeNoise* ln_ptr);

		FBmTextureWrapped(const FBmTextureWrapped& ft);

		virtual FBmTextureWrapped*
		clone(void) const;

		virtual
		~FBmTextureWrapped(void);

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

		void
		set_expansion_number(const float en);

		virtual RGBColor
		get_color(const ShadeRec& sr) const;

	protected:

		FBmTextureWrapped&
		operator= (const FBmTextureWrapped& rhs);

    private:

		LatticeNoise*		noise_ptr;		RGBColor 	  		color;		float		  		min_value, max_value;	// scaling factors
		float               expansion_number;       // expand the amplitude of the noise function};


// ---------------------------------------------------------------- set_color

inline void
FBmTextureWrapped::set_noise(LatticeNoise* ln_ptr) {
	noise_ptr = ln_ptr;
}


// ---------------------------------------------------------------- set_color

inline void
FBmTextureWrapped::set_color(const RGBColor& c) {
	color = c;
}


// ---------------------------------------------------------------- set_color

inline void
FBmTextureWrapped::set_color(const float r, const float g, const float b) {
	color.r = r; color.g = g; color.b = b;
}


// ---------------------------------------------------------------- set_color

inline void
FBmTextureWrapped::set_color(const float c) {
	color.r = c; color.g = c; color.b = c;
}


// ---------------------------------------------------------------- set_min_value

inline void
FBmTextureWrapped::set_min_value(const float mi) {
	min_value = mi;
}


// ---------------------------------------------------------------- set_max_value

inline void
FBmTextureWrapped::set_max_value(const float ma) {
	max_value = ma;
}


// ---------------------------------------------------------------- set_expansion_number

inline void
FBmTextureWrapped::set_expansion_number(const float en) {
	expansion_number = en;
}
#endif // FBMTEXTUREWRAPPED_H
