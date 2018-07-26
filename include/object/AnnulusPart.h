#ifndef ANNULUSPART_H
#define ANNULUSPART_H

#include "GeometricObject.h"
#include "ShadeRec.h"

//-------------------------------------------------------------------- class AnnulusPart

class AnnulusPart: public GeometricObject {

	public:

		AnnulusPart(void);   												// default constructor

		AnnulusPart(const Point3D& point, const Normal& normal,
                const double& inner_radius, const double& outer_radius,
                const double& _phi_min, const double& _phi_max);			// constructor

		AnnulusPart(const AnnulusPart& AnnulusPart); 									// copy constructor

		virtual AnnulusPart* 												// virtual copy constructor
		clone(void) const;

		AnnulusPart& 														// assignment operator
		operator= (const AnnulusPart& rhs);

		virtual														// destructor
		~AnnulusPart(void);

		virtual bool
		hit(const Ray& ray, double& tmin, ShadeRec& sr) const;

        virtual bool
        shadow_hit(const Ray& ray, double& tmin) const;

		virtual BBox
		get_bounding_box(void);
	private:

		Point3D 	center;   				// point through which plane passes
		Normal 		normal;					// normal to the plane
		double 		inner_radius;			// the inner radius
		double 		outer_radius;			// the outer radius
		double      phi_min;                // minimum phi value
		double      phi_max;                // maximum phi value

		static const double kEpsilon;       // for shadows and secondary rays
};

#endif // ANNULUSPART_H
