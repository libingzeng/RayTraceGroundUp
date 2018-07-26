#ifndef FBMTEXTURERAMP_H
#define FBMTEXTURERAMP_H

#include "Texture.h"
#include "LatticeNoise.h"
#include "Image.h"

class FBmTextureRamp : public Texture
{
	public:

		FBmTextureRamp(void);

		FBmTextureRamp(LatticeNoise* ln_ptr);

		FBmTextureRamp(Image* im_ptr);

		FBmTextureRamp(const FBmTextureRamp& ft);

		virtual FBmTextureRamp*
		clone(void) const;

		virtual
		~FBmTextureRamp(void);

		void
		set_noise(LatticeNoise* ln_ptr);

		void
		set_image(Image* im_ptr);

		void
		set_perturbation(const float p);

		virtual RGBColor
		get_color(const ShadeRec& sr) const;

	protected:

		FBmTextureRamp&
		operator= (const FBmTextureRamp& rhs);

    private:

		LatticeNoise*		noise_ptr;		Image* 	  		    ramp_ptr;		float               perturbation;                      // the amount of fbm};


// ---------------------------------------------------------------- set_noise

inline void
FBmTextureRamp::set_noise(LatticeNoise* ln_ptr) {
	noise_ptr = ln_ptr;
}


// ---------------------------------------------------------------- set_noise

inline void
FBmTextureRamp::set_image(Image* im_ptr) {
	ramp_ptr = im_ptr;
}

// ---------------------------------------------------------------- set_expansion_number

inline void
FBmTextureRamp::set_perturbation(const float p) {
	perturbation = p;
}

#endif // FBMTEXTURERAMP_H
