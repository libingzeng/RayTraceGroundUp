#ifndef __LIGHT__
#define __LIGHT__

#include "Vector3D.h"
#include "RGBColor.h"
#include "Ray.h"

//#include "ShadeRec.h"
class ShadeRec;


//-------------------------------------------------------------------- class Light

class Light {
	public:

		Light(void);

		Light(const Light& ls);

		Light&
		operator= (const Light& rhs);

		virtual Light*
		clone(void) const = 0;

		virtual
		~Light(void);

		virtual Vector3D
		get_direction(ShadeRec& sr) = 0;

		virtual RGBColor
		L(ShadeRec& sr);

        virtual bool
        in_shadow(const Ray& ray, const ShadeRec& sr) const;

		virtual void
		set_cast_shadow(bool cs);

		virtual bool
		get_cast_shadow(void);
		virtual float		G(const ShadeRec& sr) const;		virtual float		pdf(ShadeRec& sr);
    public:
        bool cast_shadow;
};

#endif
