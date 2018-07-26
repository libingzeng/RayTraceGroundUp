#ifndef SOLIDCYLINDER_H
#define SOLIDCYLINDER_H

#include "Compound.h"
#include "ShadeRec.h"


class SolidCylinder : public Compound
{
    public:

		SolidCylinder(void);

		SolidCylinder(const double bottom, const double top, const double radius);

		SolidCylinder(const SolidCylinder& c);

		virtual SolidCylinder*
		clone (void) const;

		SolidCylinder&
		operator= (const SolidCylinder& rhs);

		virtual
		~SolidCylinder(void);

		virtual bool
		hit(const Ray& ray, double& t, ShadeRec& sr) const;

		virtual bool
		shadow_hit(const Ray& ray, double& t) const;

		virtual BBox
		get_bounding_box(void);

	protected:

		double		y0;				// bottom y value
		double		y1;				// top y value
		double		radius;			// radius
		double		inv_radius;  	// one over the radius
		BBox        bbox;
};

#endif // SOLIDCYLINDER_H
