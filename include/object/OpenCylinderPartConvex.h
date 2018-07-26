#ifndef OPENCYLINDERPARTCONVEX_H
#define OPENCYLINDERPARTCONVEX_H


// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// An open cylinder is defined by its extent in the y direction and its radius.
// It is centered on the y axis.

#include "GeometricObject.h"
#include "ShadeRec.h"

class OpenCylinderPartConvex: public GeometricObject {

	public:

		OpenCylinderPartConvex(void);

		OpenCylinderPartConvex(const double bottom, const double top, const double radius,
                               const double _phi_min, const double _phi_max);

		OpenCylinderPartConvex(const OpenCylinderPartConvex& c);

		virtual OpenCylinderPartConvex*
		clone (void) const;

		OpenCylinderPartConvex&
		operator= (const OpenCylinderPartConvex& rhs);

		virtual
		~OpenCylinderPartConvex(void);

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
		double      phi_min;        // minimum phi value
		double      phi_max;        // maximum phi value
};

#endif // OPENCYLINDERPARTCONVEX_H
