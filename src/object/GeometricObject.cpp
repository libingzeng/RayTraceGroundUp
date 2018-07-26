// this file contains the definition of the class GeometricObject

#include "Constants.h"
#include "Material.h"
#include "GeometricObject.h"


// ---------------------------------------------------------------------- default constructor


GeometricObject::GeometricObject(void)
	: material_ptr(NULL)
{}


// ---------------------------------------------------------------------- copy constructor

GeometricObject::GeometricObject (const GeometricObject& object) {
	if(object.material_ptr)
		material_ptr = object.material_ptr->clone();
	else  material_ptr = NULL;
}


// ---------------------------------------------------------------------- assignment operator

GeometricObject&
GeometricObject::operator= (const GeometricObject& rhs) {
	if (this == &rhs)
		return (*this);

	if (material_ptr) {
		delete material_ptr;
		material_ptr = NULL;
	}

	if (rhs.material_ptr)
		material_ptr = rhs.material_ptr->clone();

	return (*this);
}


// ---------------------------------------------------------------------- destructor

GeometricObject::~GeometricObject (void) {
	if (material_ptr) {
		delete material_ptr;
		material_ptr = NULL;
	}
}


// ---------------------------------------------------------------- set_material

void
GeometricObject::set_material(Material* mPtr) {
	material_ptr = mPtr;
}




// ---------------------------------------------------------------- shadow_hit

bool
GeometricObject::shadow_hit(const Ray& ray, double& tmin) const {
		return (false);
}


// ---------------------------------------------------------------- sample

Point3D
GeometricObject::sample(void) {return Point3D(0);}


// ---------------------------------------------------------------- get_normal

Normal
GeometricObject::get_normal(const Point3D& p) {return Normal(0);}


// ---------------------------------------------------------------- get_normal

Normal
GeometricObject::get_normal(void) const {return Normal(0);}


// ---------------------------------------------------------------- pdf

float
GeometricObject::pdf(ShadeRec& sr) {return 1.0;}



// ----------------------------------------------------------------------- set_bounding_box

void
GeometricObject::set_bounding_box (void) {}


// ----------------------------------------------------------------------- set_bounding_box

BBox
GeometricObject::get_bounding_box (void) {
	return BBox();
}


// ----------------------------------------------------------------------- add_object

void
GeometricObject::add_object (GeometricObject* object_ptr) {}

