#include "PointLight.h"

// ---------------------------------------------------------------------- default constructor

PointLight::PointLight(void)
	: 	Light(),
		ls(1.0),
		color(1.0),
		location(0, 1, 0)
{}


// ---------------------------------------------------------------------- dopy constructor

PointLight::PointLight(const PointLight& pl)
	: 	Light(pl),
		ls(pl.ls),
		color(pl.color),
		location(pl.location)
{}


// ---------------------------------------------------------------------- clone

Light*
PointLight::clone(void) const {
	return (new PointLight(*this));
}


// ---------------------------------------------------------------------- assignment operator

PointLight&
PointLight::operator= (const PointLight& rhs)
{
	if (this == &rhs)
		return (*this);

	Light::operator= (rhs);

	ls		    = rhs.ls;
	color 	    = rhs.color;
	location 	= rhs.location;

	return (*this);
}


// ---------------------------------------------------------------------- destructor

PointLight::~PointLight(void) {}


// ---------------------------------------------------------------------- get_direction
// as this function is virtual, it shouldn't be inlined


Vector3D
PointLight::get_direction(ShadeRec& sr) {
	return ((location - sr.hit_point).hat());
}

RGBColor
PointLight::L(ShadeRec& sr) {
    Vector3D distance(0,1,0);
//    distance = location - sr.hit_point;
	return (ls * color) / (distance.length()*distance.length());
}

bool
PointLight::in_shadow(const Ray& ray, const ShadeRec& sr) const {
	double t;
	int num_objects = sr.w.objects.size();
	double d = location.distance(ray.o);

	for (int j = 0; j < num_objects; j++)
		if (sr.w.objects[j]->shadow_hit(ray, t) && t < d)
//		if (sr.w.objects[j]->shadow_hit(ray, t))
			return (true);

	return (false);
}

bool
PointLight::get_cast_shadow(void) {
    return cast_shadow;
}
