#ifndef DISK_H
#define DISK_H

#include "GeometricObject.h"
#include "ShadeRec.h"

//-------------------------------------------------------------------- class Disk

class Disk: public GeometricObject {

	public:

		Disk(void);   												// default constructor

		Disk(const Point3D& point, const double& radius, const Normal& normal);			// constructor

		Disk(const Disk& disk); 									// copy constructor

		virtual Disk* 												// virtual copy constructor
		clone(void) const;

		Disk& 														// assignment operator
		operator= (const Disk& rhs);

		virtual														// destructor
		~Disk(void);

		virtual bool
		hit(const Ray& ray, double& tmin, ShadeRec& sr) const;

        virtual bool
        shadow_hit(const Ray& ray, double& tmin) const;


		// the following functions are used when the disk is a light source

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

		Point3D 	center;   				// point through which plane passes
		double 		radius;				    // the radius
		Normal 		normal;					// normal to the plane

		float			area;			    // for disk lights
		float			inv_area;		    // for disk lights
		Sampler*		sampler_ptr;	    // for disk lights

		static const double kEpsilon;       // for shadows and secondary rays
};
#endif // DISK_H
