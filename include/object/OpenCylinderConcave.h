#ifndef OPENCYLINDERCONCAVE_H
#define OPENCYLINDERCONCAVE_H


// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// An open cylinder is defined by its extent in the y direction and its radius.
// It is centered on the y axis.

#include "GeometricObject.h"
#include "ShadeRec.h"

class OpenCylinderConcave: public GeometricObject {

	public:

		OpenCylinderConcave(void);

		OpenCylinderConcave(const double bottom, const double top, const double radius);

		OpenCylinderConcave(const OpenCylinderConcave& c);

		virtual OpenCylinderConcave*
		clone (void) const;

		OpenCylinderConcave&
		operator= (const OpenCylinderConcave& rhs);

		virtual
		~OpenCylinderConcave(void);

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
};

#endif // OPENCYLINDERCONCAVE_H
