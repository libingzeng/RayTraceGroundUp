#ifndef TORUSPARTCONCAVE_H
#define TORUSPARTCONCAVE_H


// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "GeometricObject.h"
#include "ShadeRec.h"

class TorusPartConcave: public GeometricObject {
	public:

		TorusPartConcave(void);

		TorusPartConcave(const double _a,      const double _b,
                         const double _phi1,   const double _phi2,
                         const double _theta1, const double _theta2);

		virtual TorusPartConcave*
		clone(void) const;

		TorusPartConcave(const TorusPartConcave& torus);

		virtual
		~TorusPartConcave(void);

		TorusPartConcave&
		operator= (TorusPartConcave& rhs);

		Normal
		compute_normal(const Point3D& p) const;

		virtual bool
		hit(const Ray& ray, double& tmin, ShadeRec& sr) const;

		virtual bool
		shadow_hit(const Ray& ray, double& tmin) const;

	private:

		double 		a;	 	// swept radius
		double		b;	 	// tube radius
		double      phi1;   // azimuth angle
		double      phi2;
		double      theta1; // polar angle
		double      theta2;
		BBox		bbox;	// the bounding box
};


#endif // TORUSPARTCONCAVE_H
