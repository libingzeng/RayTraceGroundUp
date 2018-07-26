#ifndef __WOOD__
#define __WOOD__

// 	Copyright (C) Kevin Suffern 2000-2008.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "RGBColor.h"
#include "LatticeNoise.h"
#include "Texture.h"

// This class defines a procedural texture based on the Larry Gritz Renderman wood texture described in Apodaca and Gritz (2000).
// The rings here are centered on the y axis, instead of the z axis as in Apodaca and Gritz.
// This is not a complete implementation because it does not do the intrinsic antialiasing that the Renderman version does. 

// The following description of the parameters is largely based on code comments in Apodaca and Gritz.

// 		light_color: 				The light background color of the wood.
// 		dark_color: 				The darker color of the rings and grain.
//		ring_frequency:				The mean radial distance berween the rings.	
//		ring_uneveness:				If this is zero, the rings are equally spaced and have equal thickness.
//									If this is greater than zero, the rings are unequally spaced and have different thicknesses.	
	
//		ring_noise:					These two parameters just perturb the hit point. The default value of ring_noise is 0.02, and if you		
//		ring_noise_frequency:		make ring_noise = 1.0, the wood is destroyed. The wood still looks good with these parameters set to zero.

//		trunk_wobble:				These two parameters perturb the trunk with a noise function so that it's not exactly along the y axis.
//		trunk_wobble_frequency:		trunk_wobble determines the noise amplitude, and trunk_wobble_frequency determines its spatial frequency.

//		angular_wobble:				These two parameters add noise to the rings so that they are not exactly round; angular_wobble specifies
//		angular_wobble_frequency:	the magnitude of the noise, and	angular_wobble_frequency specifies how quckly it varies around the rings.

//		grain_frequency:			This seems to control the size of the grain.

//		grainy:						This is grain-nee, not grain "y". It should be in the range [0, 1]. It determines how the grain is weighted
//									in the shading. With grainy = 0 there is no grain; with grainy = 1, the grain is fully shaded.

//		ringy:						This is ring-ee, not ring "y". It should be in the range [0, 1]. It determines how the rings are weighted
//									in the shading. With ringy = 0 there are no rings; with ringy = 1, the rings are fully shaded.

// All these parameters have default values. The easiest way to experiment with different values is to use the first or second constructor, and
// the access functions, which are supplied for each parameter.



class Wood: public Texture {			
			
	public:
	
		Wood(void);
	
		Wood(const RGBColor light, const RGBColor dark);
					
		Wood(		  LatticeNoise*	noise_ptr, 
				const RGBColor		light_color,
				const RGBColor		dark_color,
				const double 		ring_frequency,
				const double 		ring_uneveness,
				const double 		ring_noise,
				const double 		ring_noise_frequency,
				const double 		trunk_wobble,
				const double 		trunk_wobble_frequency,
				const double 		angular_wobble,
				const double 		angular_wobble_frequency,
				const double 		grain_frequency,
				const double 		grainy,
				const double 		ringy);

		Wood(const Wood& wood);								

		Wood& 													
		operator= (const Wood& rhs);		

		virtual Wood*											
		clone(void) const;				

		~Wood(void);	
		
		virtual RGBColor														
		get_color(const ShadeRec& sr) const;
		
		
		// access functions:

		void
		set_noise(LatticeNoise* noise_ptr);
		
		
		// noise parameters:
		
		void
		set_num_octaves(const int _num_octaves);
		
		void
		set_lacunarity(const float _lacunarity);
		
		void
		set_gain(const float _gain);
		
		
		// wood parameters:
		
		void
		set_light_color(const RGBColor& c);
		
		void
		set_light_color(const float r,  const float g, const float b);
		
		void
		set_light_color(const float c);
		
		void
		set_dark_color(const RGBColor& c);
		
		void
		set_dark_color(const float r,  const float g, const float b);
		
		void
		set_dark_color(const float c);
		
		void
		set_ring_frequency(const double _ring_frequency);
		
		void
		set_ring_uneveness(const double _ring_uneveness);
		
		void
		set_ring_noise(const double _ring_noise);
		
		void
		set_ring_noise_frequency(const double _ring_noise_frequency);
		
		void
		set_trunk_wobble(const double _trunk_wobble);
		
		void
		set_trunk_wobble_frequency(const double _trunk_wobble_frequency);
		
		void
		set_angular_wobble(const double _angular_wobble);
		
		void
		set_angular_wobble_frequency(const double _angular_wobble_frequency);
		
		void
		set_grain_frequency(const double _grain_frequency);
		
		void
		set_grainy(const double _grainy);
		
		void
		set_ringy(const double _ringy);
		
	private:
	
		LatticeNoise*		noise_ptr;
		RGBColor			light_color;						
		RGBColor			dark_color;							
		double				ring_frequency;						
		double				ring_uneveness;						
		double				ring_noise;							
		double 				ring_noise_frequency;				
		double 				trunk_wobble;							
		double 				trunk_wobble_frequency;
		double 				angular_wobble;
		double 				angular_wobble_frequency;
		double 				grain_frequency;
		double 				grainy;								
		double 				ringy;	
};							


// inlined access functions

// The noise access functions don't need to check if the noise pointer is not NULL because the only way we can
// construct a Wood object with a NULL noise pointer is to use a NULL pointer in the third constructor.


// -------------------------------------------------------------------- set_noise

inline void
Wood::set_noise(LatticeNoise* _noise_ptr) {
	noise_ptr = _noise_ptr;
}


// -------------------------------------------------------------------- set_num_octaves
		
inline void
Wood::set_num_octaves(const int _num_octaves) {
	noise_ptr->set_num_octaves(_num_octaves);
}


// -------------------------------------------------------------------- set_lacunarity

inline void
Wood::set_lacunarity(const float _lacunarity) {
	noise_ptr->set_lacunarity(_lacunarity);
}


// -------------------------------------------------------------------- set_gain

inline void
Wood::set_gain(const float _gain)  {
	noise_ptr->set_gain(_gain);
}


// -------------------------------------------------------------------- set_light_color

inline void
Wood::set_light_color(const RGBColor& c) {
	light_color = c;
}


// -------------------------------------------------------------------- set_light_color

inline void
Wood::set_light_color(const float r,  const float g, const float b) {
	light_color.r = r;
	light_color.b = b;
	light_color.g = g;
}


// -------------------------------------------------------------------- set_light_color

inline void
Wood::set_light_color(const float c) {
	light_color.r = light_color.b = light_color.g = c;
}


// -------------------------------------------------------------------- set_dark_color

inline void
Wood::set_dark_color(const RGBColor& c) {
	dark_color = c;
}


// -------------------------------------------------------------------- set_dark_color

inline void
Wood::set_dark_color(const float r,  const float g, const float b) {
	dark_color.r = r;
	dark_color.b = b;
	dark_color.g = g;
}


// -------------------------------------------------------------------- set_dark_color

inline void
Wood::set_dark_color(const float c) {
	dark_color.r = dark_color.b = dark_color.g = c;
}


// -------------------------------------------------------------------- set_ring_frequency

inline void
Wood::set_ring_frequency(const double _ring_frequency) {
	ring_frequency = _ring_frequency;
}


// -------------------------------------------------------------------- set_ring_uneveness

inline void
Wood::set_ring_uneveness(const double _ring_uneveness) {
	ring_uneveness = _ring_uneveness;
}


// -------------------------------------------------------------------- set_ring_noise

inline void
Wood::set_ring_noise(const double _ring_noise) {
	ring_noise = _ring_noise;
}


// -------------------------------------------------------------------- set_ring_noise_frequency

inline void
Wood::set_ring_noise_frequency(const double _ring_noise_frequency) {
	ring_noise_frequency = _ring_noise_frequency;
}


// -------------------------------------------------------------------- set_trunk_wobble

inline void
Wood::set_trunk_wobble(const double _trunk_wobble) {
	trunk_wobble = _trunk_wobble;
}


// -------------------------------------------------------------------- set_trunk_wobble_frequency

inline void
Wood::set_trunk_wobble_frequency(const double _trunk_wobble_frequency) {
	trunk_wobble_frequency = _trunk_wobble_frequency;
}


// -------------------------------------------------------------------- set_angular_wobble

inline void
Wood::set_angular_wobble(const double _angular_wobble) {
	angular_wobble = _angular_wobble;
}


// -------------------------------------------------------------------- set_angular_wobble_frequency

inline void
Wood::set_angular_wobble_frequency(const double _angular_wobble_frequency) {
	angular_wobble_frequency = _angular_wobble_frequency;
}


// -------------------------------------------------------------------- set_grain_frequency

inline void
Wood::set_grain_frequency(const double _grain_frequency) {
	grain_frequency = _grain_frequency;
}


// -------------------------------------------------------------------- set_grainy

inline void
Wood::set_grainy(const double _grainy) {
	grainy = _grainy;
}


// -------------------------------------------------------------------- set_ringy

inline void
Wood::set_ringy(const double _ringy) {
	ringy = _ringy;
}

#endif
		