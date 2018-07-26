#ifndef ANNULUS_H
#define ANNULUS_H

#include "GeometricObject.h"
#include "ShadeRec.h"

//-------------------------------------------------------------------- class Annulus

class Annulus: public GeometricObject {

	public:

		Annulus(void);   												// default constructor

		Annulus(const Point3D& point, const Normal& normal,
           const double& inner_radius, const double& outer_radius);			// constructor

		Annulus(const Annulus& Annulus); 									// copy constructor

		virtual Annulus* 												// virtual copy constructor
		clone(void) const;

		Annulus& 														// assignment operator
		operator= (const Annulus& rhs);

		virtual														// destructor
		~Annulus(void);

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

		static const double kEpsilon;       // for shadows and secondary rays
};

#endif // ANNULUS_H
