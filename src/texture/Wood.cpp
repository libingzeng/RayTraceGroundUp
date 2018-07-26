// 	Copyright (C) Kevin Suffern 2000-2008.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// This file contains the definition of the class Wood

// This class defines a procedural texture based on the Larry Gritz Renderman wood texture described in Apodaca and Gritz (2000).
// The rings here are centered on the y axis, instead of the z axis as in Apodaca and Gritz.
// This is not a complete implementation because it does not do the intrinsic antialiasing that the Renderman version does.


// The first two constructors use a CubicNoise object which uses tri-cubic interpolation for the vector_noise and vector_fbm
// functions called in get_color. These are very slow, but seem to give better results than tri-linear interpolation
// which is much quicker.
// You can experiment with linear interpolation by either using the third constructor with a pointer to a LInearNoise object
// for the first argument, or by using the set_noise access function. This will replace the CubicNoise object set up in the
// first two constructors.
// It's not only the vector interpolation that slows things down - the get_color fuction also calls value_noise up to four times.

// The get_color function requires the following utility functions:
//
//			smooth_step			called from smooth_pulse_train
//			smooth_pulse_train
//			mix_double
//			mix_color
//			clamp
//			mod					called from smooth_pulse_train
//			max

// These are defined in Maths.h and Maths.cpp.
// A templated version of mix would allow us to use a single mix function

// The code below also uses sqrt and pow from the ANSI C library and white and black from Constants.h.
// #include <math.h> and #include "Constants.h" are in Texture.h, and therefore do not have to be included here.

#include "Maths.h"
#include "CubicNoise.h"
#include "Wood.h"


// ------------------------------------------------------------------------------------------ default constructor
// Uses default values for everything

Wood::Wood (void)
	:	Texture(),
		noise_ptr(new CubicNoise(2, 4.0, 0.5)), // this specifies num_octaves, lacunarity, and gain
		light_color(white),
		dark_color(black),
		ring_frequency(4.0),
		ring_uneveness(0.25),
		ring_noise(0.02),
		ring_noise_frequency(1.0),
		trunk_wobble(0.15),
		trunk_wobble_frequency(0.025),
		angular_wobble(0.5),
		angular_wobble_frequency(1.0),
		grain_frequency(25.0),
		grainy(0.5),
		ringy(0.5)
{}


// ------------------------------------------------------------------------------------------ constructor
// Specifies the two colors and uses default values for everything else

Wood::Wood(const RGBColor light, const RGBColor dark)
	:	Texture(),
		noise_ptr(new CubicNoise(2, 4.0, 0.5)), // this specifies num_octaves, lacunarity, and gain
		light_color(light),
		dark_color(dark),
		ring_frequency(4.0),
		ring_uneveness(0.25),
		ring_noise(0.02),
		ring_noise_frequency(1.0),
		trunk_wobble(0.15),
		trunk_wobble_frequency(0.025),
		angular_wobble(0.5),
		angular_wobble_frequency(1.0),
		grain_frequency(25.0),
		grainy(0.5),
		ringy(0.5)
{}



// ------------------------------------------------------------------------------------------ constructor
// Allows you to specify everything
// To use this constructor you will have to define a noise object in the build function or use a call
// to a noise constructor as the first argument.

Wood::Wood(		  LatticeNoise*	_noise_ptr,
			const RGBColor		_light_color,
			const RGBColor		_dark_color,
			const double 		_ring_frequency,
			const double 		_ring_uneveness,
			const double 		_ring_noise,
			const double 		_ring_noise_frequency,
			const double 		_trunk_wobble,
			const double 		_trunk_wobble_frequency,
			const double 		_angular_wobble,
			const double 		_angular_wobble_frequency,
			const double 		_grain_frequency,
			const double 		_grainy,
			const double 		_ringy)

	:	Texture(),
		noise_ptr(_noise_ptr),
		light_color(_light_color),
		dark_color(_dark_color),
		ring_frequency(_ring_frequency),
		ring_uneveness(_ring_uneveness),
		ring_noise(_ring_noise),
		ring_noise_frequency(_ring_noise_frequency),
		trunk_wobble(_trunk_wobble),
		trunk_wobble_frequency(_trunk_wobble_frequency),
		angular_wobble(_angular_wobble),
		angular_wobble_frequency(_angular_wobble_frequency),
		grain_frequency(_grain_frequency),
		grainy(_grainy),
		ringy(_ringy)
{}


// ------------------------------------------------------------------------------------------ copy constructor

Wood::Wood (const Wood& wood)
	: 	Texture(wood),
		light_color(wood.light_color),
		dark_color(wood.dark_color),
		ring_frequency(wood.ring_frequency),
		ring_uneveness(wood.ring_uneveness),
		ring_noise(wood.ring_noise),
		ring_noise_frequency(wood.ring_noise_frequency),
		trunk_wobble(wood.trunk_wobble),
		trunk_wobble_frequency(wood.trunk_wobble_frequency),
		angular_wobble(wood.angular_wobble),
		angular_wobble_frequency(wood.angular_wobble_frequency),
		grain_frequency(wood.grain_frequency),
		grainy(wood.grainy),
		ringy(wood.ringy)
{
	if(wood.noise_ptr)
		noise_ptr = wood.noise_ptr->clone();
	else  noise_ptr = NULL;
}


// ------------------------------------------------------------------------------------------ assignment operator

Wood&
Wood::operator= (const Wood& rhs) {
	if (this == &rhs)
		return (*this);

	Texture::operator=(rhs);

	if (noise_ptr) {
		delete noise_ptr;
		noise_ptr = NULL;
	}

	if (rhs.noise_ptr)
		noise_ptr = rhs.noise_ptr->clone();

	light_color 				= rhs.light_color;
	dark_color					= rhs.dark_color;
	ring_frequency				= rhs.ring_frequency;
	ring_uneveness				= rhs.ring_uneveness;
	ring_noise					= rhs.ring_noise;
	ring_noise_frequency 		= rhs.ring_noise_frequency;
	trunk_wobble				= rhs.trunk_wobble;
	trunk_wobble_frequency		= rhs.trunk_wobble_frequency;
	angular_wobble				= rhs.angular_wobble;
	angular_wobble_frequency	= rhs.angular_wobble_frequency;
	grain_frequency				= rhs.grain_frequency;
	grainy						= rhs.grainy;
	ringy						= rhs.ringy;

	return (*this);
}


// ------------------------------------------------------------------------------------------ clone

Wood*
Wood::clone(void) const {
	return (new Wood(*this));
}


// ------------------------------------------------------------------------------------------ destructor

Wood::~Wood(void) {
	if (noise_ptr) {
		delete noise_ptr;
		noise_ptr = NULL;
	}
}


// ------------------------------------------------------------------------------------------ get_color
#if 0
RGBColor
Wood::get_color(const ShadeRec& sr) const {
	Point3D hit_point = sr.local_hit_point;

	// perturb the hit point

	Vector3D offset = noise_ptr->vector_fbm(hit_point * ring_noise_frequency);

	Point3D ring_point = hit_point + ring_noise * offset;

	// perturb the trunk so that it's quite along the y axis

	Vector3D temp_vec = trunk_wobble * noise_ptr->vector_noise(Point3D(0, 0, hit_point.y * trunk_wobble_frequency));
	ring_point.x += temp_vec.x;
	ring_point.z += temp_vec.z;


	// distance from the y axis

	double r = sqrt(ring_point.x * ring_point.x + ring_point.z * ring_point.z) * ring_frequency;


	// perturb r so that the rings aren't quite round

	Point3D temp_vec2;
	temp_vec2.x = angular_wobble_frequency * ring_point.x;
	temp_vec2.y = angular_wobble_frequency * ring_point.y * 0.1;
	temp_vec2.z = angular_wobble_frequency * ring_point.z;

	double delta_r = angular_wobble * smooth_step(0.0, 5.0, r) * noise_ptr->value_noise(temp_vec2);
	r += delta_r;


	// add some noise so that the rings are not equally spaced and have different thicknesses

	r += ring_uneveness * noise_ptr->value_noise(Point3D(r));

	double temp = r;
	double in_ring = smooth_pulse_train(0.1, 0.55, 0.7, 0.95, 1.0, r);


	// the grain

	Point3D grain_point;
	grain_point.x = hit_point.x * grain_frequency;
	grain_point.y = hit_point.y * grain_frequency * 0.05;
	grain_point.z = hit_point.z * grain_frequency;

	double dpgrain 		= 0.2;
	double grain 		= 0.0;
	double amplitude 	= 1.0;

	for (int i = 0; i < 2; i++) {
		float grain_valid = 1.0 - smooth_step(0.2, 0.6, dpgrain);
		if (grain_valid > 0.0) {
			double g = grain_valid * noise_ptr->value_noise(grain_point);
			g *= (0.3 + 0.7 * in_ring);
			g = pow(clamp(0.8 - g, 0.0, 1.0), 2.0);
			g = grainy * smooth_step (0.5, 1.0, g);
			if (i == 0)
				in_ring *= (1.0 - 0.4 * grain_valid);
			grain = amplitude * max(grain, g);
		}
		grain_point = 	2.0 * grain_point;
		dpgrain 	*= 	2.0;
		amplitude 	*= 	0.5;
	}

	double final_value = mix_double(in_ring * ringy, 1.0, grain);

	return (mix_color(light_color, dark_color, final_value));
}
#endif // 0

RGBColor
Wood::get_color(const ShadeRec& sr) const {
	Point3D hit_point = sr.local_hit_point;

	// perturb the hit point
	Vector3D offset = noise_ptr->vector_fbm(hit_point * ring_noise_frequency);
	Point3D ring_point = hit_point + ring_noise * offset;

	// perturb the trunk so that it's quite along the y axis
	Vector3D temp_vec = trunk_wobble * noise_ptr->vector_noise(
                            Point3D(0, 0, hit_point.y * trunk_wobble_frequency));
	ring_point.x += temp_vec.x;
	ring_point.z += temp_vec.z;

	double r = sqrt(ring_point.x * ring_point.x + ring_point.z * ring_point.z)
                * ring_frequency;

	// perturb r so that the rings aren't quite round
	Point3D temp_vec2;
	temp_vec2.x = angular_wobble_frequency * ring_point.x;
	temp_vec2.y = angular_wobble_frequency * ring_point.y * 0.1;
	temp_vec2.z = angular_wobble_frequency * ring_point.z;
	double delta_r = angular_wobble * smooth_step(0.0, 5.0, r)
                        * noise_ptr->value_noise(temp_vec2);
	r += delta_r;

	// add some noise so that the rings are not equally spaced and have different thicknesses
	r += ring_uneveness * noise_ptr->value_noise(Point3D(r));

	double in_ring = smooth_pulse_train(0.1, 0.55, 0.7, 0.95, 1.0, r);

	// the grain

	Point3D grain_point;
	grain_point.x = hit_point.x * grain_frequency;
	grain_point.y = hit_point.y * grain_frequency * 0.05;
	grain_point.z = hit_point.z * grain_frequency;

	double dpgrain 		= 0.2;
	double grain 		= 0.0;
	double amplitude 	= 1.0;

	for (int i = 0; i < 2; i++) {
		float grain_valid = 1.0 - smooth_step(0.2, 0.6, dpgrain);
		if (grain_valid > 0.0) {
			double g = grain_valid * noise_ptr->value_noise(grain_point);
			g *= (0.3 + 0.7 * in_ring);
			g = pow(clamp(0.8 - g, 0.0, 1.0), 2.0);
			g = grainy * smooth_step (0.5, 1.0, g);
			if (i == 0)
				in_ring *= (1.0 - 0.4 * grain_valid);
			grain = amplitude * max(grain, g);
		}
		grain_point = 	2.0 * grain_point;
		dpgrain 	*= 	2.0;
		amplitude 	*= 	0.5;
	}

	double final_value = mix_double(in_ring * ringy, 1.0, grain);

	return (mix_color(light_color, dark_color, final_value));
}





