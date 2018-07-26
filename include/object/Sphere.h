#ifndef __SPHERE__
#define __SPHERE__

// This file contains the declaration of the class Sphere

//class ShadeRec;
//class GeometricObject;
#include "GeometricObject.h"
#include "ShadeRec.h"
#include "Sampler.h"

//-------------------------------------------------------------------------------- class Sphere

class Sphere: public GeometricObject {

	public:

		Sphere(void);   									// Default constructor

		Sphere(Point3D center, double r);					// Constructor

		Sphere(const Sphere& sphere); 						// Copy constructor

		virtual Sphere* 									// Virtual copy constructor
		clone(void) const;

		virtual												// Destructor
		~Sphere(void);

		Sphere& 											// assignment operator
		operator= (const Sphere& sphere);

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
Sphere::set_center(const Point3D& c) {
	center = c;
}

inline void
Sphere::set_center(const double x, const double y, const double z) {
	center.x = x;
	center.y = y;
	center.z = z;
}

inline void
Sphere::set_radius(const double r) {
	radius = r;
}

#endif
