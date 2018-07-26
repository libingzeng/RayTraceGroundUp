#ifndef SPHERECONCAVE_H
#define SPHERECONCAVE_H


// This file contains the declaration of the class SphereConcave

//class ShadeRec;
//class GeometricObject;
#include "GeometricObject.h"
#include "ShadeRec.h"
#include "Sampler.h"

//-------------------------------------------------------------------------------- class SphereConcave

class SphereConcave: public GeometricObject {

	public:

		SphereConcave(void);   									// Default constructor

		SphereConcave(Point3D center, double r);					// Constructor

		SphereConcave(const SphereConcave& sphere); 						// Copy constructor

		virtual SphereConcave* 									// Virtual copy constructor
		clone(void) const;

		virtual												// Destructor
		~SphereConcave(void);

		SphereConcave& 											// assignment operator
		operator= (const SphereConcave& sphere);

		void
		set_center(const Point3D& c);

		void
		set_center(const double x, const double y, const double z);

		void
		set_radius(const double r);

		virtual bool
		hit(const Ray& ray, double& t, ShadeRec& s) const;

        virtual bool
        shadow_hit(const Ray& ray, double& tmin) const;

		// the following functions are used when the sphere is a light source

		virtual void
		set_sampler(Sampler* sampler);

		virtual Point3D
		sample(void);

		virtual Normal
		get_normal(const Point3D& p);

		virtual float
		pdf(ShadeRec& sr);

		virtual BBox
		get_bounding_box(void);
	private:

		Point3D 	center;   			// center coordinates as a point
		double 		radius;				// the radius

		float			area;			// for spherical lights
		float			inv_area;		// for spherical lights
		Sampler*		sampler_ptr;	// for spherical lights

		static const double kEpsilon;   // for shadows and secondary rays
};



inline void
SphereConcave::set_center(const Point3D& c) {
	center = c;
}

inline void
SphereConcave::set_center(const double x, const double y, const double z) {
	center.x = x;
	center.y = y;
	center.z = z;
}

inline void
SphereConcave::set_radius(const double r) {
	radius = r;
}

#endif // SPHERECONCAVE_H
