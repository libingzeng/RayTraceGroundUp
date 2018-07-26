#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Light.h"
#include "Vector3D.h"
#include "RGBColor.h"

#include "World.h"			// you will need this later on for shadows
#include "ShadeRec.h"

class PointLight : public Light
{
    public:
		PointLight(void);

		PointLight(const PointLight& pl);

		virtual Light*
		clone(void) const;

		PointLight&
		operator= (const PointLight& rhs);

		virtual
		~PointLight(void);

		void
		scale_radiance(const float b);

		void
		set_color(const float c);

		void
		set_color(const RGBColor& c);

		void
		set_color(const float r, const float g, const float b);

		void
		set_location(Point3D l);

		void
		set_location(float lx, float ly, float lz);

		virtual Vector3D
		get_direction(ShadeRec& sr);

		virtual RGBColor
		L(ShadeRec& sr);

        virtual bool
        in_shadow(const Ray& ray, const ShadeRec& sr) const;

		virtual void
		set_cast_shadow(bool cs);

		virtual bool
		get_cast_shadow(void);

//    public:
//        bool cast_shadow;

	private:

		float		ls;
		RGBColor	color;
		Point3D	    location;
};

// inlined access functions


// ------------------------------------------------------------------------------- scale_radiance

inline void
PointLight::scale_radiance(const float b) {
	ls = b;
}

// ------------------------------------------------------------------------------- set_color

inline void
PointLight::set_color(const float c) {
	color.r = c; color.g = c; color.b = c;
}


// ------------------------------------------------------------------------------- set_color

inline void
PointLight::set_color(const RGBColor& c) {
	color = c;
}


// ------------------------------------------------------------------------------- set_color

inline void
PointLight::set_color(const float r, const float g, const float b) {
	color.r = r; color.g = g; color.b = b;
}


// ---------------------------------------------------------------------- set_direction

inline void
PointLight::set_location(Point3D l) {
	location = l;
}


// ---------------------------------------------------------------------- set_direction

inline void
PointLight::set_location(float lx, float ly, float lz) {
	location.x = lx; location.y = ly; location.z = lz;
}

// ---------------------------------------------------------------------- set_cast_shadow

inline void
PointLight::set_cast_shadow(bool cs) {
    cast_shadow = cs;
}

#endif // POINTLIGHT_H
