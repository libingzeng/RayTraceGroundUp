
#include "Globe.h"
#include "Grid.h"
#include "OpenCylinder.h"
#include "TorusPartConvex.h"
#include "Sphere.h"
#include "Instance.h"
#include "Wood.h"
#include "InstanceTexture.h"
#include "SV_Phong.h"
#include "Reflective.h"
#include "Image.h"
#include "SphericalMap.h"
#include "ImageTexture.h"


// ------------------------------------------------------------------------------ default constructor

Globe::Globe (void)
    : 	Compound()
{
	build_components();


//--------------------------default material for the base--------------------------

	// wood texture

	RGBColor light_color(0.5, 0.2, 0.065);
	RGBColor dark_color(0.05);

	Wood* wood_ptr = new Wood(light_color, dark_color);
	wood_ptr->set_grainy(1.0);
	wood_ptr->set_ringy(1.0);

	InstanceTexture* transformed_wood_ptr = new InstanceTexture(wood_ptr);
	transformed_wood_ptr->scale(0.4);
	transformed_wood_ptr->rotate_z(110);


	// material:

	SV_Phong* sv_phong_base_ptr = new SV_Phong;
	sv_phong_base_ptr->set_ka(0.25);
	sv_phong_base_ptr->set_kd(0.75);
	sv_phong_base_ptr->set_cd(transformed_wood_ptr);
	sv_phong_base_ptr->set_ks(0.1);
	sv_phong_base_ptr->set_exp(20.0);

	set_base_material(sv_phong_base_ptr);


//--------------------------default material for the holder--------------------------

	Reflective* reflective_holder_ptr = new Reflective;
	reflective_holder_ptr->set_ka(0.0);
	reflective_holder_ptr->set_kd(0.0);
	reflective_holder_ptr->set_cd(black);
	reflective_holder_ptr->set_ks(0.5);
	reflective_holder_ptr->set_exp(50.0);
	reflective_holder_ptr->set_kr(0.75);
	reflective_holder_ptr->set_cr(white);

	set_holder_material(reflective_holder_ptr);


//--------------------------default material for the globe sphere--------------------------

	// image:

	Image* image_ptr = new Image;
	image_ptr->read_ppm_file(".\\TextureFiles\\ppm\\EarthLowRes.ppm");


	// mapping:

	SphericalMap* map_ptr = new SphericalMap;


	// image based texture:

	ImageTexture* texture_image_ptr = new ImageTexture;
	texture_image_ptr->set_image(image_ptr);
	texture_image_ptr->set_mapping(map_ptr);


	// material:

	SV_Phong* sv_phong_globe_sphere_ptr = new SV_Phong;
	sv_phong_globe_sphere_ptr->set_ka(0.25);
	sv_phong_globe_sphere_ptr->set_kd(0.75);
	sv_phong_globe_sphere_ptr->set_cd(texture_image_ptr);
	sv_phong_globe_sphere_ptr->set_ks(0.1);
	sv_phong_globe_sphere_ptr->set_exp(20.0);

    set_sphere_material(sv_phong_globe_sphere_ptr);
}


// ------------------------------------------------------------------------------ copy constructor

Globe::Globe (const Globe& g)
    : 	Compound(g)
{}


// ------------------------------------------------------------------------------ clone

Globe*
Globe::clone(void) const {
	return(new Globe(*this));
}


// ------------------------------------------------------------------------------ assignment operator

Globe&
Globe::operator= (const Globe& rhs) {
	if (this == &rhs)
		return (*this);

	Compound::operator=(rhs);

	return (*this) ;
}


// ------------------------------------------------------------------------------ destructor

Globe::~Globe(void) {}


// ------------------------------------------------------------------------------ build_components

// Using named components would be better
// It would make the material functions below a lot clearer and maintainable

void
Globe::build_components(void) {

	// build the base parts

    Compound* base_compound_ptr = new Compound;

	Grid* grid_revolution_ptr = new Grid;
	Point2D ctrl_points1[4] = {Point2D( 2.0,  1.0), Point2D(-0.5,  1.0),
                               Point2D( 1.5, -3.0), Point2D( 3.0,  0.0)};
    grid_revolution_ptr->tessellate_flat_rotational_sweeping(200, 50, ctrl_points1, 4, false);
	grid_revolution_ptr->setup_cells();
	base_compound_ptr->add_object(grid_revolution_ptr);

	OpenCylinder* oc_base_ptr = new OpenCylinder(-2, (-11.0/6.0), (8.5/6.0));
	base_compound_ptr->add_object(oc_base_ptr);

    Instance* base_instance_ptr = new Instance(base_compound_ptr);

	objects.push_back(base_instance_ptr);

    // build the holder parts

    Compound* holder_compound_ptr = new Compound;

    OpenCylinder* cylinder_ptr = new OpenCylinder(-3.1, 3.1, 0.125);
    holder_compound_ptr->add_object(cylinder_ptr);

    TorusPartConvex* tori_ptr = new TorusPartConvex(2.875, 0.125, 90, 270, 0, 360);
    Instance* instance_tori_ptr = new Instance(tori_ptr);
    instance_tori_ptr->rotate_x(90);
    holder_compound_ptr->add_object(instance_tori_ptr);

    Sphere* sphere_small_ptr1 = new Sphere(Point3D(0, 3.25, 0), 0.25);
    holder_compound_ptr->add_object(sphere_small_ptr1);

    Sphere* sphere_small_ptr2 = new Sphere(Point3D(0, -3.25, 0), 0.25);
    holder_compound_ptr->add_object(sphere_small_ptr2);

    Instance* holder_instance_ptr = new Instance(holder_compound_ptr);
    holder_instance_ptr->rotate_z(23.26);
    holder_instance_ptr->translate(0, (2.0+0.5+0.5+0.125), 0);

	objects.push_back(holder_instance_ptr);

	// build the globe sphere

	Sphere* globe_sphere_ptr = new Sphere;

    Instance* globe_sphere_instance_ptr = new Instance(globe_sphere_ptr);
    globe_sphere_instance_ptr->scale(2.625);
    globe_sphere_instance_ptr->rotate_y(60);
    globe_sphere_instance_ptr->rotate_z(23.26);
    globe_sphere_instance_ptr->translate(0, (2.0+0.5+0.5+0.125), 0);

	objects.push_back(globe_sphere_instance_ptr);
}


// ------------------------------------------------------------------------------ set_base_material

void
Globe::set_base_material(Material* m_ptr) {
	objects[0]->set_material(m_ptr);
}

// ------------------------------------------------------------------------------ set_holder_material

void
Globe::set_holder_material(Material* m_ptr) {
	objects[1]->set_material(m_ptr);
}


// ------------------------------------------------------------------------------ set_sphere_material

void
Globe::set_sphere_material(Material* m_ptr) {
	objects[2]->set_material(m_ptr);
}



