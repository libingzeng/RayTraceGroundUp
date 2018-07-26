
#define ChapterNum 324
//00: test
//22: Regular Grid, Triangle Mesh
//221: SV_Phong
//2211: SV_Phong, rotational sweeping, L766
//222: SV_Phong, rotational sweeping, glass wine cup, L909
//223: SV_Phong, rotational sweeping, globe, L1084
//224: SV_Phong, rotational sweeping, globe, 2, L1355
//23: PLY
//24: mirror reflection
//25: glossy reflection
//26: global illumination
//27: simple transparent
//28: realistic transparent
//282: realistic transparent: a glass of water
//283: realistic transparent: fish bowl
//284: realistic transparent: fish bowl flat bottom
//291: texture mapping: constant color, image texture for sphere and cylinder
//292: texture mapping: triangle meshes
//300: procedural textures, 3D checker
//301: procedural textures, 2D checker, sphere
//302: procedural textures, transformation
//31: noise-based textures: basic and wrapped
//312: noise-based textures: marble, sandstone
//313: noise-based textures: wood
//314: noise-based textures: test wood
//32: rotational sweeping, glass
//321: RayTracing Scenes, 2, PLYs, L5104
//322: RayTracing Scenes, 3, two horses, L5436
//323: RayTracing Scenes, 4, Seaside, L5792
//324: RayTracing Scenes, 5, dragon, L6410


#if ChapterNum == 0

#include "World.h"
#include "Ambient.h"
#include "Pinhole.h"
#include "Directional.h"
#include "PointLight.h"
#include "RayCast.h"
#include "Matte.h"
#include "Plane.h"
#include "Phong.h"
#include "MultiJittered.h"
#include "AmbientOccluder.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "Rectangle.h"
#include "AreaLighting.h"
#include "Instance.h"
#include "Disk.h"
#include "Annulus.h"
#include "OpenCylinderConvex.h"
#include "OpenCylinderConcave.h"
#include "Torus.h"
#include "TorusPart.h"
#include "TorusPartConcave.h"
#include "ConstantColor.h"
#include "SV_Matte.h"

#define TYPE 5
// TYPE = 0: disk
// TYPE = 1: annulus
// TYPE = 2: open cylinder convex
// TYPE = 3: open cylinder concave
// TYPE = 4: torus
// TYPE = 5: part torus
// TYPE = 6: part torus concave


void
World::build(void){
	int num_samples = 1;

	vp.set_hres(300);
	vp.set_vres(300);
	vp.set_samples(num_samples);

	tracer_ptr = new RayCast(this);

	Pinhole* pinhole_ptr = new Pinhole;

	pinhole_ptr->set_eye(0, 10, 20);
	pinhole_ptr->set_lookat(0, 0, 0);
	pinhole_ptr->set_view_distance(500);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

	Directional* light_ptr = new Directional;
	light_ptr->set_direction(-10, 20, 20);
	light_ptr->scale_radiance(3.0);
	light_ptr->set_cast_shadow(false);
	add_light(light_ptr);

	ConstantColor* cc_ptr = new ConstantColor;
	cc_ptr->set_color(0.0, 1.0, 0.0);

    SV_Matte* matte_ptr = new SV_Matte;
    matte_ptr->set_ka(0.25);
    matte_ptr->set_kd(0.75);
    matte_ptr->set_cd(cc_ptr);

#if TYPE == 0
	Disk* disk_ptr = new Disk(Point3D(0, 0, 0), 2, Normal(0, 0, 1));
    disk_ptr->set_material(matte_ptr);
	add_object(disk_ptr);
#endif // TYPE

#if TYPE == 1
	Annulus* annulus_ptr = new Annulus(Point3D(0, 0, 0), Normal(0, 0, 1), 1, 2);
    annulus_ptr->set_material(matte_ptr);
	add_object(annulus_ptr);
#endif // TYPE

#if TYPE == 2
	OpenCylinderConvex* oclinder_convex_ptr = new OpenCylinderConvex(-2, 2, 2);
    oclinder_convex_ptr->set_material(matte_ptr);
	add_object(oclinder_convex_ptr);
#endif // TYPE

#if TYPE == 3
	OpenCylinderConcave* oclinder_concave_ptr = new OpenCylinderConcave(-2, 2, 2);
    oclinder_concave_ptr->set_material(matte_ptr);
	add_object(oclinder_concave_ptr);
#endif // TYPE

#if TYPE == 4
	Torus* torus_ptr = new Torus(2, 0.5);
    torus_ptr->set_material(matte_ptr);
	add_object(torus_ptr);
#endif // TYPE

#if TYPE == 5
	TorusPart* tp_ptr = new TorusPart(2.0, 0.5, 0.0, 270.0, 90.0, 360.0);
    tp_ptr->set_material(matte_ptr);
	add_object(tp_ptr);
#endif // TYPE

#if TYPE == 6
	TorusPartConcave* tpc_ptr = new TorusPartConcave(2.0, 0.5, 0.0, 360.0, 270.0, 360.0);
    tpc_ptr->set_material(matte_ptr);
	add_object(tpc_ptr);
#endif // TYPE

}



#elif ChapterNum == 20

#include "World.h"
#include "Ambient.h"
#include "Pinhole.h"
#include "Directional.h"
#include "PointLight.h"
#include "RayCast.h"
#include "Matte.h"
#include "Plane.h"
#include "Phong.h"
#include "MultiJittered.h"
#include "AmbientOccluder.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "Rectangle.h"
#include "AreaLighting.h"
#include "Instance.h"
#include "Disk.h"
#include "Grid.h"

void
World::build(void) {
	int num_samples = 1;

	// view plane

	vp.set_hres(400);
	vp.set_vres(400);
	vp.set_pixel_size(0.5);
	vp.set_samples(num_samples);


//	tracer_ptr = new RayCast(this);
	tracer_ptr = new AreaLighting(this);
	// ambient occlusion

	MultiJittered* sampler_ptr = new MultiJittered(num_samples);

#if 1
	AmbientOccluder* occluder_ptr = new AmbientOccluder;
	occluder_ptr->scale_radiance(1.0);
	occluder_ptr->set_min_amount(0.0);
	occluder_ptr->set_sampler(sampler_ptr);
	set_ambient_light(occluder_ptr);
#endif // 0

	// camera

	Pinhole* pinhole_ptr = new Pinhole;
	pinhole_ptr->set_eye(0, 0, 1000);
	pinhole_ptr->set_lookat(0.0);
	pinhole_ptr->set_view_distance(600.0);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);


	// light
#if 0
	Directional* light_ptr1 = new Directional;
	light_ptr1->set_direction(100, 100, 100);
	light_ptr1->scale_radiance(3.0);
	add_light(light_ptr1);
#endif // 1
#if 0
	PointLight* light_ptr1 = new PointLight;
	light_ptr1->set_location(50, 0, 100);
	light_ptr1->scale_radiance(3.0);
    light_ptr1->set_cast_shadow(true);
	add_light(light_ptr1);
#endif // 1

#if 1 // rectangle light
	Emissive* emissive_ptr = new Emissive;
	emissive_ptr->scale_radiance(80.0);
	emissive_ptr->set_ce(white);


	// define a rectangle for the rectangular light

	float width = 50.0;				// for Figure 18.4(a) & (b)
	float height = 50.0;
//	float width = 2.0;				// for Figure 18.4(c)
//	float height = 2.0;
	Point3D center(0.0, -100.0, -40.0);	// center of each area light (rectangular, disk, and spherical)
	Point3D p0(-0.5 * width, center.y - 0.5 * height, center.z);
	Vector3D a(width, 0.0, 0.0);
	Vector3D b(0.0, height, 0.0);
	Normal normal(0, 0, 1);

	Rectangle* rectangle_ptr = new Rectangle(p0, a, b, normal);
	rectangle_ptr->set_material(emissive_ptr);
	rectangle_ptr->set_sampler(sampler_ptr);
//	rectangle_ptr->set_shadows(false);
	add_object(rectangle_ptr);


	AreaLight* area_light_ptr = new AreaLight;
	area_light_ptr->set_object(rectangle_ptr);
	area_light_ptr->set_cast_shadow(true);
	add_light(area_light_ptr);
#endif // 0

#if 0 // spherical light
	Emissive* emissive_ptr = new Emissive;
	emissive_ptr->scale_radiance(80.0);
	emissive_ptr->set_ce(white);


	// sphere for a spherical light
	// the center and radius are the same as for the disk

//	float width = 50.0;
//	float radius = 0.56 * width;
	float radius = 50;
	Point3D center(0.0, -100.0, -40.0);
	Sphere* sphere_ptr = new Sphere(center, radius);
	sphere_ptr->set_material(emissive_ptr);
	sphere_ptr->set_sampler(sampler_ptr);
//	sphere_ptr->set_shadows(false);
	add_object(sphere_ptr);

	AreaLight* area_light_ptr = new AreaLight;
	area_light_ptr->set_object(sphere_ptr);
	area_light_ptr->set_cast_shadow(true);
	add_light(area_light_ptr);
#endif // 0


	// colors

	RGBColor red(1, 0, 0);										    // red
	RGBColor green(0, 1, 0);										// green
	RGBColor blue(0, 0, 1);										    // blue
	RGBColor yellow(1, 1, 0);										// yellow
	RGBColor light_purple(0.65, 0.3, 1);							// light purple

		// Matt material reflection coefficients

	float ka = 0.25;
	float kd = 0.75;

#if 1

	// spheres

	Matte* matte_ptr1 = new Matte;
	matte_ptr1->set_ka(ka);
	matte_ptr1->set_kd(kd);
	matte_ptr1->set_cd(yellow);
	Sphere*	sphere_ptr1 = new Sphere(Point3D(0, 0, 0), 30);
	sphere_ptr1->set_material(matte_ptr1);	   							// yellow
	add_object(sphere_ptr1);
#endif // 0

#if 0
// rotation_x
	Matte* matte_ptr2 = new Matte;
	matte_ptr2->set_ka(ka);
	matte_ptr2->set_kd(kd);
	matte_ptr2->set_cd(yellow);
	Instance* ellipsoid_ptr1 = new Instance(new Sphere(Point3D(0, 0, 0), 30));
	ellipsoid_ptr1->set_material(matte_ptr2);

	ellipsoid_ptr1->scale(1, 2, 1);
	ellipsoid_ptr1->rotate_x(0);
	ellipsoid_ptr1->translate(-120, -60, 0);
	add_object(ellipsoid_ptr1);



	Matte* matte_ptr3 = new Matte;
	matte_ptr3->set_ka(ka);
	matte_ptr3->set_kd(kd);
	matte_ptr3->set_cd(red);
	Instance* ellipsoid_ptr2 = new Instance(new Sphere(Point3D(0, 0, 0), 30));

	ellipsoid_ptr2->set_material(matte_ptr3);
	ellipsoid_ptr2->scale(1, 2, 1);
	ellipsoid_ptr2->rotate_x(90);
	ellipsoid_ptr2->translate(120, -60, 0);
	add_object(ellipsoid_ptr2);
#endif // 1

#if 1
// shear
	Matte* matte_ptr2 = new Matte;
	matte_ptr2->set_ka(ka);
	matte_ptr2->set_kd(kd);
	matte_ptr2->set_cd(yellow);
	Instance* ellipsoid_ptr1 = new Instance(new Sphere(Point3D(0, 0, 0), 30));
	ellipsoid_ptr1->set_material(matte_ptr2);

	ellipsoid_ptr1->scale(1, 2, 1);
//	ellipsoid_ptr1->shear(shear_matrix1);
	ellipsoid_ptr1->translate(-120, -60, 0);
	add_object(ellipsoid_ptr1);



	Matte* matte_ptr3 = new Matte;
	matte_ptr3->set_ka(ka);
	matte_ptr3->set_kd(kd);
	matte_ptr3->set_cd(blue);
	Instance* ellipsoid_ptr2 = new Instance(new Disk(Point3D(0, 0, 0), 30, Normal(0, 0, 1)));
	ellipsoid_ptr2->set_material(matte_ptr3);

	Matrix shear_matrix;
	shear_matrix.m[0][1] = 0.5;
	shear_matrix.m[0][1] = 0.5;
	ellipsoid_ptr2->scale(1, 2, 1);
	ellipsoid_ptr2->shear(shear_matrix);
	ellipsoid_ptr2->translate(120, -60, 0);
	add_object(ellipsoid_ptr2);
#endif // 1
	// planes

//down plane
	Matte* matte_ptr9 = new Matte;
	matte_ptr9->set_ka(ka);
	matte_ptr9->set_kd(kd);
	matte_ptr9->set_cd(green);
	Plane* plane_ptr9 = new Plane(Point3D(0, 60, 0), Normal(0, -1, 0));
	plane_ptr9->set_material(matte_ptr9);
	add_object (plane_ptr9);
#if 0

	// planes

//down plane
	Matte* matte_ptr9 = new Matte;
	matte_ptr9->set_ka(ka);
	matte_ptr9->set_kd(kd);
	matte_ptr9->set_cd(green);
	Plane* plane_ptr9 = new Plane(Point3D(0, 60, 0), Normal(0, -1, 0));
	plane_ptr9->set_material(matte_ptr9);
	add_object (plane_ptr9);
#endif // 0

#if 0
	// Phong materials' reflection coefficients

	float ka 	= 0.1;//0.1
	float kd 	= 0.5;
	float ks 	= 0.25;
	float exp 	= 20;// 10

	// spheres

	Phong* phong_ptr1 = new Phong;
	phong_ptr1->set_ka(ka);
	phong_ptr1->set_kd(kd);
	phong_ptr1->set_ks(ks);
	phong_ptr1->set_exp(exp);
	phong_ptr1->set_cd(yellow);

	Sphere*	sphere_ptr1 = new Sphere(Point3D(0, 0, 0), 30);
	sphere_ptr1->set_material(phong_ptr1);	   							// yellow
	add_object(sphere_ptr1);


	// planes

//down plane
	Phong* phong_ptr9 = new Phong;
	phong_ptr9->set_ka(ka);
	phong_ptr9->set_kd(kd);
	phong_ptr9->set_ks(ks);
	phong_ptr9->set_exp(exp);
	phong_ptr9->set_cd(green);

	Plane* plane_ptr9 = new Plane(Point3D(0, 60, 0), Normal(0, -1, 0));
	plane_ptr9->set_material(phong_ptr9);
	add_object (plane_ptr9);

//back plane
	Phong* phong_ptr5 = new Phong;
	phong_ptr5->set_ka(ka);
	phong_ptr5->set_kd(kd);
	phong_ptr5->set_ks(ks);
	phong_ptr5->set_exp(exp);
	phong_ptr5->set_cd(red);

	Plane* plane_ptr5 = new Plane(Point3D(0, 0, -40), Normal(0, 0, 1));
	plane_ptr5->set_material(phong_ptr5);
	add_object (plane_ptr5);


//up plane
	Phong* phong_ptr6 = new Phong;
	phong_ptr6->set_ka(ka);
	phong_ptr6->set_kd(kd);
	phong_ptr6->set_ks(ks);
	phong_ptr6->set_exp(exp);
	phong_ptr6->set_cd(green);

	Plane* plane_ptr6 = new Plane(Point3D(0, -60, 0), Normal(0, 1, 0));
	plane_ptr6->set_material(phong_ptr6);
	add_object (plane_ptr6);

//left plane
	Phong* phong_ptr7 = new Phong;
	phong_ptr7->set_ka(ka);
	phong_ptr7->set_kd(kd);
	phong_ptr7->set_ks(ks);
	phong_ptr7->set_exp(exp);
	phong_ptr7->set_cd(blue);

	Plane* plane_ptr7 = new Plane(Point3D(-60, 0, 0), Normal(1, 0, 0));
	plane_ptr7->set_material(phong_ptr7);
	add_object (plane_ptr7);

//right plane
	Phong* phong_ptr8 = new Phong;
	phong_ptr8->set_ka(ka);
	phong_ptr8->set_kd(kd);
	phong_ptr8->set_ks(ks);
	phong_ptr8->set_exp(exp);
	phong_ptr8->set_cd(blue);

	Plane* plane_ptr8 = new Plane(Point3D(60, 0, 0), Normal(-1, 0, 0));
	plane_ptr8->set_material(phong_ptr8);
	add_object (plane_ptr8);
#endif // 0
}

#elif ChapterNum == 22

#include "World.h"
#include "Ambient.h"
#include "Pinhole.h"
#include "Directional.h"
#include "PointLight.h"
#include "RayCast.h"
#include "Matte.h"
#include "Plane.h"
#include "Phong.h"
#include "MultiJittered.h"
#include "AmbientOccluder.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "Rectangle.h"
#include "AreaLighting.h"
#include "Instance.h"
#include "Disk.h"
#include "Grid.h"
#include "CubicNoise.h"
#include "Image.h"
#include "FBmTextureRamp.h"
#include "InstanceTexture.h"
#include "SV_Matte.h"
#include "Wood.h"

#include <iostream>
#include <fstream>
using namespace std;

#define TYPE 2
// TYPE == 0: matte
// TYPE == 1: marble, sandstone
// TYPE == 2: wood

void
World::build(void){
	int num_samples = 16;

	vp.set_hres(300);
	vp.set_vres(300);
	vp.set_samples(num_samples);

	tracer_ptr = new RayCast(this);

	Pinhole* pinhole_ptr = new Pinhole;

	pinhole_ptr->set_eye(0, 0, 100);
	pinhole_ptr->set_lookat(0, 0, 0);
	pinhole_ptr->set_view_distance(3600);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

	Directional* light_ptr = new Directional;
	light_ptr->set_direction(10, -20, -20);
	light_ptr->scale_radiance(3.0);
	light_ptr->set_cast_shadow(false);
	add_light(light_ptr);

	Grid* grid_ptr = new Grid;

    int patches[32][16];
    float vertices[306][3];
    get_teapot_data(patches, vertices);
    grid_ptr->tessellate_flat_bezier_patches(40, 40, vertices, patches, 32);
	grid_ptr->setup_cells();

#if TYPE == 0
    Matte* matte_ptr = new Matte;
    matte_ptr->set_ka(0.25);
    matte_ptr->set_kd(0.75);
    matte_ptr->set_cd(0.0, 1.0, 0.0);
#endif // TYPE

#if TYPE == 1 // marble, sandstone

	// noise:

	CubicNoise* noise_ptr = new CubicNoise;
	noise_ptr->set_num_octaves(6);
	noise_ptr->set_gain(0.5);			// not relevant when num_octaves = 1
	noise_ptr->set_lacunarity(2.0);     // not relevant when num_octaves = 1

	// ramp image:

	Image* image_ptr = new Image;
//	image_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/BlueMarbleRamp.ppm");
	image_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/sandstone_ramp4.ppm");

	// marble texture:

	FBmTextureRamp* marble_ptr = new FBmTextureRamp(image_ptr);
	marble_ptr->set_noise(noise_ptr);
	marble_ptr->set_perturbation(0.1);

	InstanceTexture* it_ptr = new InstanceTexture(marble_ptr);
//	it_ptr->scale(0.1);
//	it_ptr->rotate_x(-60);
//	it_ptr->rotate_y(-60);
	it_ptr->rotate_z(110);
	it_ptr->translate(1.0, 4.0, 0.0);

	// material:

	SV_Matte* matte_ptr = new SV_Matte;
	matte_ptr->set_ka(0.25);
	matte_ptr->set_kd(0.85);
	matte_ptr->set_cd(it_ptr);
#endif // TYPE

#if TYPE == 2
	// wood texture

	// This Wood constructor just specifies the light and dark colors.
	// The other parameters are defaults.

	RGBColor light_color(0.5, 0.2, 0.065);
	RGBColor dark_color(0.05);

	Wood* wood_ptr = new Wood(light_color, dark_color);
	wood_ptr->set_grainy(1.0);
	wood_ptr->set_ringy(1.0);

	InstanceTexture* transformed_wood_ptr = new InstanceTexture(wood_ptr);
	transformed_wood_ptr->scale(0.4);
	transformed_wood_ptr->rotate_z(110);


	// material:

	SV_Matte* matte_ptr = new SV_Matte;
	matte_ptr->set_ka(0.25);
	matte_ptr->set_kd(0.85);
	matte_ptr->set_cd(transformed_wood_ptr);

#endif // TYPE

    Instance* instance_ptr = new Instance(grid_ptr);
    instance_ptr->rotate_x(-90);
    instance_ptr->translate(0, -2, 0);
    instance_ptr->set_material(matte_ptr);

	add_object(instance_ptr);

}

#elif ChapterNum == 221 // SV_Phong

#include "World.h"
#include "Ambient.h"
#include "Pinhole.h"
#include "Directional.h"
#include "PointLight.h"
#include "RayCast.h"
#include "Matte.h"
#include "Plane.h"
#include "Phong.h"
#include "MultiJittered.h"
#include "AmbientOccluder.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "Rectangle.h"
#include "AreaLighting.h"
#include "Instance.h"
#include "Disk.h"
#include "Grid.h"
#include "CubicNoise.h"
#include "Image.h"
#include "FBmTextureRamp.h"
#include "InstanceTexture.h"
#include "SV_Phong.h"
#include "Wood.h"

#include <iostream>
#include <fstream>
using namespace std;

#define TYPE 2
// TYPE == 1: marble, sandstone
// TYPE == 2: wood

void
World::build(void){
	int num_samples = 16;

	vp.set_hres(300);
	vp.set_vres(300);
	vp.set_samples(num_samples);

	tracer_ptr = new RayCast(this);

	Pinhole* pinhole_ptr = new Pinhole;

	pinhole_ptr->set_eye(0, 0, 80);
	pinhole_ptr->set_lookat(0, 0, 0);
	pinhole_ptr->set_view_distance(3600);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

	Directional* light_ptr = new Directional;
	light_ptr->set_direction(30, 10, 30);
	light_ptr->scale_radiance(3.0);
	light_ptr->set_cast_shadow(false);
	add_light(light_ptr);

	Grid* grid_ptr = new Grid;

    int patches[32][16];
    float vertices[306][3];
    get_teapot_data(patches, vertices);
    grid_ptr->tessellate_flat_bezier_patches(40, 40, vertices, patches, 32);
	grid_ptr->setup_cells();

#if TYPE == 1 // marble, sandstone

	// noise:

	CubicNoise* noise_ptr = new CubicNoise;
	noise_ptr->set_num_octaves(6);
	noise_ptr->set_gain(0.5);			// not relevant when num_octaves = 1
	noise_ptr->set_lacunarity(2.0);     // not relevant when num_octaves = 1

	// ramp image:

	Image* image_ptr = new Image;
//	image_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/BlueMarbleRamp.ppm");
	image_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/sandstone_ramp4.ppm");

	// marble texture:

	FBmTextureRamp* marble_ptr = new FBmTextureRamp(image_ptr);
	marble_ptr->set_noise(noise_ptr);
	marble_ptr->set_perturbation(0.1);

	InstanceTexture* it_ptr = new InstanceTexture(marble_ptr);
//	it_ptr->scale(0.1);
//	it_ptr->rotate_x(-60);
//	it_ptr->rotate_y(-60);
	it_ptr->rotate_z(110);
	it_ptr->translate(1.0, 4.0, 0.0);

	// material:

	SV_Phong* sv_phong_ptr = new SV_Phong;
	sv_phong_ptr->set_ka(0.25);
	sv_phong_ptr->set_kd(0.75);
	sv_phong_ptr->set_cd(it_ptr);
	sv_phong_ptr->set_ks(0.1);
	sv_phong_ptr->set_exp(20.0);
#endif // TYPE

#if TYPE == 2
	// wood texture

	// This Wood constructor just specifies the light and dark colors.
	// The other parameters are defaults.

	RGBColor light_color(0.5, 0.2, 0.065);
	RGBColor dark_color(0.05);

	Wood* wood_ptr = new Wood(light_color, dark_color);
	wood_ptr->set_grainy(1.0);
	wood_ptr->set_ringy(1.0);

	InstanceTexture* transformed_wood_ptr = new InstanceTexture(wood_ptr);
	transformed_wood_ptr->scale(0.4);
	transformed_wood_ptr->rotate_z(110);


	// material:

	SV_Phong* sv_phong_ptr = new SV_Phong;
	sv_phong_ptr->set_ka(0.25);
	sv_phong_ptr->set_kd(0.75);
	sv_phong_ptr->set_cd(transformed_wood_ptr);
	sv_phong_ptr->set_ks(0.1);
	sv_phong_ptr->set_exp(20.0);

#endif // TYPE

    Instance* instance_ptr = new Instance(grid_ptr);
    instance_ptr->rotate_x(-90);
    instance_ptr->translate(0, -2, 0);
    instance_ptr->set_material(sv_phong_ptr);

	add_object(instance_ptr);

	Sphere* sphere_ptr = new Sphere(Point3D(0,0,0), 2);
	sphere_ptr->set_material(sv_phong_ptr);
//	add_object(sphere_ptr);

}

#elif ChapterNum == 2211 // SV_Phong, rotational sweeping

#include "World.h"
#include "Ambient.h"
#include "Pinhole.h"
#include "Directional.h"
#include "PointLight.h"
#include "RayCast.h"
#include "Matte.h"
#include "Plane.h"
#include "Phong.h"
#include "MultiJittered.h"
#include "AmbientOccluder.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "Rectangle.h"
#include "AreaLighting.h"
#include "Instance.h"
#include "Disk.h"
#include "Grid.h"
#include "CubicNoise.h"
#include "Image.h"
#include "FBmTextureRamp.h"
#include "InstanceTexture.h"
#include "SV_Phong.h"
#include "Wood.h"

#include <iostream>
#include <fstream>
using namespace std;

#define TYPE 2
// TYPE == 1: marble, sandstone
// TYPE == 2: wood

void
World::build(void){
	int num_samples = 16;

	vp.set_hres(300);
	vp.set_vres(300);
	vp.set_samples(num_samples);

	tracer_ptr = new RayCast(this);

	Pinhole* pinhole_ptr = new Pinhole;

	pinhole_ptr->set_eye(0, 0, 80);
	pinhole_ptr->set_lookat(0, 0, 0);
	pinhole_ptr->set_view_distance(3600);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

	Directional* light_ptr = new Directional;
	light_ptr->set_direction(30, 10, 30);
	light_ptr->scale_radiance(3.0);
	light_ptr->set_cast_shadow(false);
	add_light(light_ptr);

	Grid* grid_ptr = new Grid;

	Point2D ctrl_points1[6] = {Point2D(-1.0,  5.0), Point2D( 2.0,  4.0),
                               Point2D( 2.0,  1.0), Point2D(-0.5,  1.0),
                               Point2D( 1.5, -3.0), Point2D( 3.0,  0.0)};
    grid_ptr->tessellate_flat_rotational_sweeping(200, 50, ctrl_points1, 6, false);
	grid_ptr->setup_cells();

#if TYPE == 1 // marble, sandstone

	// noise:

	CubicNoise* noise_ptr = new CubicNoise;
	noise_ptr->set_num_octaves(6);
	noise_ptr->set_gain(0.5);			// not relevant when num_octaves = 1
	noise_ptr->set_lacunarity(2.0);     // not relevant when num_octaves = 1

	// ramp image:

	Image* image_ptr = new Image;
//	image_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/BlueMarbleRamp.ppm");
	image_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/sandstone_ramp4.ppm");

	// marble texture:

	FBmTextureRamp* marble_ptr = new FBmTextureRamp(image_ptr);
	marble_ptr->set_noise(noise_ptr);
	marble_ptr->set_perturbation(0.1);

	InstanceTexture* it_ptr = new InstanceTexture(marble_ptr);
//	it_ptr->scale(0.1);
//	it_ptr->rotate_x(-60);
//	it_ptr->rotate_y(-60);
	it_ptr->rotate_z(110);
	it_ptr->translate(1.0, 4.0, 0.0);

	// material:

	SV_Phong* sv_phong_ptr = new SV_Phong;
	sv_phong_ptr->set_ka(0.25);
	sv_phong_ptr->set_kd(0.75);
	sv_phong_ptr->set_cd(it_ptr);
	sv_phong_ptr->set_ks(0.1);
	sv_phong_ptr->set_exp(20.0);
#endif // TYPE

#if TYPE == 2
	// wood texture

	// This Wood constructor just specifies the light and dark colors.
	// The other parameters are defaults.

	RGBColor light_color(0.5, 0.2, 0.065);
	RGBColor dark_color(0.05);

	Wood* wood_ptr = new Wood(light_color, dark_color);
	wood_ptr->set_grainy(1.0);
	wood_ptr->set_ringy(1.0);

	InstanceTexture* transformed_wood_ptr = new InstanceTexture(wood_ptr);
	transformed_wood_ptr->scale(0.4);
	transformed_wood_ptr->rotate_z(110);


	// material:

	SV_Phong* sv_phong_ptr = new SV_Phong;
	sv_phong_ptr->set_ka(0.25);
	sv_phong_ptr->set_kd(0.75);
	sv_phong_ptr->set_cd(transformed_wood_ptr);
	sv_phong_ptr->set_ks(0.1);
	sv_phong_ptr->set_exp(20.0);

#endif // TYPE

    Instance* instance_ptr = new Instance(grid_ptr);
    instance_ptr->scale(0.8);
    instance_ptr->translate(0, -1, 0);
    instance_ptr->set_material(sv_phong_ptr);

	add_object(instance_ptr);

}

#elif ChapterNum == 222 // SV_Phong, rotational sweeping, glass wine cup

#include "World.h"
#include "Ambient.h"
#include "Pinhole.h"
#include "Directional.h"
#include "PointLight.h"
#include "RayCast.h"
#include "Whitted.h"
#include "Matte.h"
#include "Plane.h"
#include "Phong.h"
#include "MultiJittered.h"
#include "AmbientOccluder.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "Rectangle.h"
#include "AreaLighting.h"
#include "Instance.h"
#include "Disk.h"
#include "Grid.h"
#include "CubicNoise.h"
#include "Image.h"
#include "FBmTextureRamp.h"
#include "InstanceTexture.h"
#include "SV_Matte.h"
#include "SV_Phong.h"
#include "Wood.h"
#include "Checker3D.h"
#include "Dielectric.h"
#include "Annulus.h"
#include "TorusPartConvex.h"

#include <iostream>
#include <fstream>
using namespace std;

#define TYPE 1
// TYPE == 1: marble, sandstone
// TYPE == 2: wood

void
World::build(void){
	int num_samples = 16;

	vp.set_hres(300);
	vp.set_vres(300);
	vp.set_samples(num_samples);
	vp.set_max_depth(4);

//	tracer_ptr = new RayCast(this);
	tracer_ptr = new Whitted(this);

	Pinhole* pinhole_ptr = new Pinhole;

	pinhole_ptr->set_eye(0, 20, 80);
	pinhole_ptr->set_lookat(0, 0, 0);
	pinhole_ptr->set_view_distance(3600);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

	PointLight* point_light_ptr = new PointLight;
	point_light_ptr->set_location(30, 10, 30);
	point_light_ptr->scale_radiance(3.0);
	point_light_ptr->set_cast_shadow(false);
	add_light(point_light_ptr);


	Grid* grid_ptr1 = new Grid;

	Point2D ctrl_points1[6] = {Point2D(-1.0,  5.0), Point2D( 2.0,  4.0),
                               Point2D( 2.0,  1.0), Point2D(-0.5,  1.0),
                               Point2D( 1.5, -3.0), Point2D( 3.0,  0.0)};
    grid_ptr1->tessellate_flat_rotational_sweeping(200, 50, ctrl_points1, 6, true);
	grid_ptr1->setup_cells();

	Grid* grid_ptr2 = new Grid;
	Point2D ctrl_points2[6] = {Point2D(-0.8,  5.0), Point2D( 2.2,  4.0),
                               Point2D( 2.2,  1.0), Point2D(-0.3,  1.0),
                               Point2D( 1.7, -3.0), Point2D( 3.2,  0.0)};
    grid_ptr2->tessellate_flat_rotational_sweeping(200, 50, ctrl_points2, 6, false);
	grid_ptr2->setup_cells();

	TorusPartConvex* tori_ptr = new TorusPartConvex(1.6, 0.1, 0, 360, 0, 90);
    Annulus* annulus_ptr2 = new Annulus(Point3D(0, -11.0/6.0, 0), Normal(0, -1, 0), 8.5/6.0, (8.5/6.0 + 0.2));
	// glass-air interface

	float c = 2;
	RGBColor glass_color(0.27*c, 0.49*c, 0.42*c);
	RGBColor water_color(0.75, 1, 0.75);

	Dielectric* glass_ptr = new Dielectric;
	glass_ptr->set_ks(0.5);
	glass_ptr->set_exp(2000.0);
	glass_ptr->set_eta_in(1.50);			// glass
	glass_ptr->set_eta_out(1.0);			// air
	glass_ptr->set_cf_in(glass_color);
	glass_ptr->set_cf_out(white);


    Instance* instance_ptr1 = new Instance(grid_ptr1);
    instance_ptr1->scale(0.8);
    instance_ptr1->translate(0, -1, 0);
    instance_ptr1->set_material(glass_ptr);

//	add_object(instance_ptr1);


    Instance* instance_ptr2 = new Instance(grid_ptr2);
    instance_ptr2->scale(0.8);
    instance_ptr2->translate(0, -1, 0);
    instance_ptr2->set_material(glass_ptr);

//	add_object(instance_ptr2);


    Instance* instance_ptr3 = new Instance(tori_ptr);
    instance_ptr3->translate(0, 22.0/6.0, 0);
    instance_ptr3->scale(0.8);
    instance_ptr3->translate(0, -1, 0);
    instance_ptr3->set_material(glass_ptr);

//	add_object(instance_ptr3);


    Instance* instance_ptr4 = new Instance(annulus_ptr2);
    instance_ptr4->scale(0.8);
    instance_ptr4->translate(0, -1, 0);
    instance_ptr4->set_material(glass_ptr);

	add_object(instance_ptr4);


    //------------------------------Checker3D

	Checker3D* texture_ptr1 = new Checker3D();
	texture_ptr1->set_size(1.0);
    texture_ptr1->set_color1(0.0, 1.0, 0.0);
    texture_ptr1->set_color2(1.0, 1.0, 1.0);


	// textured material:

    SV_Matte* matte_ptr1 = new SV_Matte;
    matte_ptr1->set_ka(0.25);
    matte_ptr1->set_kd(0.75);
    matte_ptr1->set_cd(texture_ptr1);

    Plane* plane_ptr1 = new Plane(Point3D(0, -3, 0), Normal(0, 1, 0));
    plane_ptr1->set_material(matte_ptr1);

	add_object(plane_ptr1);


    //------------------------------Checker3D

	Checker3D* texture_ptr2 = new Checker3D();
	texture_ptr2->set_size(1.0);
    texture_ptr2->set_color1(0.0, 1.0, 0.0);
    texture_ptr2->set_color2(1.0, 1.0, 1.0);


	// textured material:

    SV_Matte* matte_ptr2 = new SV_Matte;
    matte_ptr2->set_ka(0.25);
    matte_ptr2->set_kd(0.75);
    matte_ptr2->set_cd(texture_ptr2);

    Plane* plane_ptr2 = new Plane(Point3D(0, 0, -5), Normal(0, 0, 1));
    plane_ptr2->set_material(matte_ptr2);

	add_object(plane_ptr2);
}

#elif ChapterNum == 223 // SV_Phong, rotational sweeping, globe

#include "World.h"
#include "Ambient.h"
#include "Pinhole.h"
#include "Directional.h"
#include "PointLight.h"
#include "RayCast.h"
#include "Whitted.h"
#include "Matte.h"
#include "Plane.h"
#include "Phong.h"
#include "MultiJittered.h"
#include "AmbientOccluder.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "Rectangle.h"
#include "AreaLighting.h"
#include "Instance.h"
#include "Disk.h"
#include "Grid.h"
#include "CubicNoise.h"
#include "Image.h"
#include "FBmTextureRamp.h"
#include "InstanceTexture.h"
#include "SV_Matte.h"
#include "SV_Phong.h"
#include "Wood.h"
#include "Checker3D.h"
#include "Dielectric.h"
#include "Annulus.h"
#include "TorusPartConvex.h"
#include "SphericalMap.h"
#include "Image.h"
#include "ImageTexture.h"
#include "OpenCylinder.h"
#include "Reflective.h"
#include "SolidCylinder.h"
#include "SolidCylinderChecker.h"
#include "PlaneChecker.h"

#include <iostream>
#include <fstream>
using namespace std;


void
World::build(void){
	int num_samples = 16;

	vp.set_hres(600);
	vp.set_vres(500);
	vp.set_samples(num_samples);
	vp.set_max_depth(4);

//	tracer_ptr = new RayCast(this);
	tracer_ptr = new Whitted(this);

	Pinhole* pinhole_ptr = new Pinhole;

	pinhole_ptr->set_eye(0, 20, 80);
	pinhole_ptr->set_lookat(0, 1, 0);
	pinhole_ptr->set_view_distance(3600);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

	PointLight* point_light_ptr = new PointLight;
	point_light_ptr->set_location(30, 10, 30);
	point_light_ptr->scale_radiance(3.0);
	point_light_ptr->set_cast_shadow(true);
	add_light(point_light_ptr);

	Grid* grid_revolution_ptr = new Grid;

	Point2D ctrl_points1[4] = {Point2D( 2.0,  1.0), Point2D(-0.5,  1.0),
                               Point2D( 1.5, -3.0), Point2D( 3.0,  0.0)};
    grid_revolution_ptr->tessellate_flat_rotational_sweeping(200, 50, ctrl_points1, 4, false);
	grid_revolution_ptr->setup_cells();

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

	SV_Phong* sv_phong_revolution_ptr = new SV_Phong;
	sv_phong_revolution_ptr->set_ka(0.25);
	sv_phong_revolution_ptr->set_kd(0.75);
	sv_phong_revolution_ptr->set_cd(transformed_wood_ptr);
	sv_phong_revolution_ptr->set_ks(0.1);
	sv_phong_revolution_ptr->set_exp(20.0);

	Instance* instance_revolution_ptr = new Instance(grid_revolution_ptr);
	instance_revolution_ptr->set_material(sv_phong_revolution_ptr);
	add_object(instance_revolution_ptr);

	OpenCylinder* oc_base_ptr = new OpenCylinder(-2, (-11.0/6.0), (8.5/6.0));
	oc_base_ptr->set_material(sv_phong_revolution_ptr);
	add_object(oc_base_ptr);

///////////////////////////////////////////////////////////////////////////////////////////

	Sphere* sphere_ptr = new Sphere;

	// image:

	Image* image_ptr = new Image;
	image_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/EarthLowRes.ppm");


	// mapping:

	SphericalMap* map_ptr = new SphericalMap;


	// image based texture:

	ImageTexture* texture_image_ptr = new ImageTexture;
	texture_image_ptr->set_image(image_ptr);
	texture_image_ptr->set_mapping(map_ptr);


	// material:

	SV_Phong* sv_phong_sphere_ptr = new SV_Phong;
	sv_phong_sphere_ptr->set_ka(0.25);
	sv_phong_sphere_ptr->set_kd(0.75);
	sv_phong_sphere_ptr->set_cd(texture_image_ptr);
	sv_phong_sphere_ptr->set_ks(0.1);
	sv_phong_sphere_ptr->set_exp(20.0);


    Instance* instance_sphere_ptr = new Instance(sphere_ptr);
    instance_sphere_ptr->scale(2.625);
    instance_sphere_ptr->rotate_y(60);
    instance_sphere_ptr->rotate_z(23.26);
    instance_sphere_ptr->translate(0, (2.0+0.5+0.5+0.125), 0);
    instance_sphere_ptr->set_material(sv_phong_sphere_ptr);

	add_object(instance_sphere_ptr);


///////////////////////////////////////////////////////////////////////////////////////////

    OpenCylinder* cylinder_ptr = new OpenCylinder(-3.1, 3.1, 0.125);

	Reflective* reflective_ptr1 = new Reflective;
	reflective_ptr1->set_ka(0.0);               // ambient reflection, ka=0.4
	reflective_ptr1->set_kd(0.0);               // diffuse reflection, kd=0.8
	reflective_ptr1->set_cd(black);    	        // white
	reflective_ptr1->set_ks(0.5);               // specular reflection, ks=0.5
	reflective_ptr1->set_exp(50.0);             // 50.0
	reflective_ptr1->set_kr(0.75);              // indirect, mirror reflection, kr=0.75
	reflective_ptr1->set_cr(white); 	        // default color is (0.5, 0.5, 1.0)

    Instance* instance_cylinder_ptr = new Instance(cylinder_ptr);
    instance_cylinder_ptr->rotate_z(23.26);
    instance_cylinder_ptr->translate(0, (2.0+0.5+0.5+0.125), 0);
    instance_cylinder_ptr->set_material(reflective_ptr1);

	add_object(instance_cylinder_ptr);


///////////////////////////////////////////////////////////////////////////////////////////

    TorusPartConvex* tori_ptr = new TorusPartConvex(2.875, 0.125, 90, 270, 0, 360);

    Instance* instance_tori_ptr = new Instance(tori_ptr);
    instance_tori_ptr->rotate_x(90);
    instance_tori_ptr->rotate_z(23.26);
    instance_tori_ptr->translate(0, (2.0+0.5+0.5+0.125), 0);
    instance_tori_ptr->set_material(reflective_ptr1);

	add_object(instance_tori_ptr);


///////////////////////////////////////////////////////////////////////////////////////////

    Sphere* sphere_small_ptr1 = new Sphere(Point3D(0, 3.25, 0), 0.25);

    Instance* instance_sphere_small_ptr1 = new Instance(sphere_small_ptr1);
    instance_sphere_small_ptr1->rotate_z(23.26);
    instance_sphere_small_ptr1->translate(0, (2.0+0.5+0.5+0.125), 0);
    instance_sphere_small_ptr1->set_material(reflective_ptr1);

	add_object(instance_sphere_small_ptr1);


///////////////////////////////////////////////////////////////////////////////////////////

    Sphere* sphere_small_ptr2 = new Sphere(Point3D(0, -3.25, 0), 0.25);

    Instance* instance_sphere_small_ptr2 = new Instance(sphere_small_ptr2);
    instance_sphere_small_ptr2->rotate_z(23.26);
    instance_sphere_small_ptr2->translate(0, (2.0+0.5+0.5+0.125), 0);
    instance_sphere_small_ptr2->set_material(reflective_ptr1);

	add_object(instance_sphere_small_ptr2);

///////////////////////////////////////////////////////////////////////////////////////////

    SolidCylinder* sc_ptr = new SolidCylinder();

	// SolidCylinderChecker:
	SolidCylinderChecker* texture_ptr3 = new SolidCylinderChecker();
	texture_ptr3->set_num_horizontal_checkers(20);
	texture_ptr3->set_num_vertical_checkers(5);
	texture_ptr3->set_num_radius_checkers_disk(5);
	texture_ptr3->set_horizontal_line_width(0.05);
	texture_ptr3->set_vertical_line_width(0.02);
    texture_ptr3->set_color1(1.0, 0.0, 0.0);
    texture_ptr3->set_color2(0.0, 1.0, 0.0);
    texture_ptr3->set_line_color(1.0, 1.0, 1.0);

	// material:

	SV_Phong* sv_phong_sc_ptr = new SV_Phong;
	sv_phong_sc_ptr->set_ka(0.25);
	sv_phong_sc_ptr->set_kd(0.75);
	sv_phong_sc_ptr->set_cd(texture_ptr3);
	sv_phong_sc_ptr->set_ks(0.1);
	sv_phong_sc_ptr->set_exp(20.0);

	Instance* instance_sc_ptr = new Instance(sc_ptr);
	instance_sc_ptr->scale(3, (1.0/2.0), 3);
	instance_sc_ptr->translate(0, (-5.0/2.0), 0);
	instance_sc_ptr->set_material(sv_phong_sc_ptr);
    add_object(instance_sc_ptr);

    //------------------------------PlaneChecker

	PlaneChecker* plane_checker_ptr = new PlaneChecker;
	plane_checker_ptr->set_size(1.0);
	plane_checker_ptr->set_line_width(0.125);
    plane_checker_ptr->set_color1(white);
    plane_checker_ptr->set_color2(white);
    plane_checker_ptr->set_line_color(black);

	// textured material:

    SV_Matte* matte_ptr1 = new SV_Matte;
    matte_ptr1->set_ka(0.25);
    matte_ptr1->set_kd(0.75);
    matte_ptr1->set_cd(plane_checker_ptr);

    Plane* plane_ptr = new Plane();

    Instance* instance_plane_ptr1 = new Instance(plane_ptr);
    instance_plane_ptr1->translate(0, -3, 0);
    instance_plane_ptr1->set_material(matte_ptr1);
	add_object(instance_plane_ptr1);

    Instance* instance_plane_ptr2 = new Instance(plane_ptr);
    instance_plane_ptr2->rotate_x(90);
    instance_plane_ptr2->translate(0, 0, -5);
    instance_plane_ptr2->set_material(matte_ptr1);
	add_object(instance_plane_ptr2);

}


#elif ChapterNum == 224 // SV_Phong, rotational sweeping, globe, 2

#include "World.h"
#include "Ambient.h"
#include "Pinhole.h"
#include "Directional.h"
#include "PointLight.h"
#include "RayCast.h"
#include "Whitted.h"
#include "Matte.h"
#include "Plane.h"
#include "Phong.h"
#include "MultiJittered.h"
#include "AmbientOccluder.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "Rectangle.h"
#include "AreaLighting.h"
#include "Instance.h"
#include "Disk.h"
#include "Grid.h"
#include "CubicNoise.h"
#include "Image.h"
#include "FBmTextureRamp.h"
#include "InstanceTexture.h"
#include "SV_Matte.h"
#include "SV_Phong.h"
#include "Wood.h"
#include "Checker3D.h"
#include "Dielectric.h"
#include "Annulus.h"
#include "TorusPartConvex.h"
#include "SphericalMap.h"
#include "Image.h"
#include "ImageTexture.h"
#include "OpenCylinder.h"
#include "Reflective.h"
#include "SolidCylinder.h"
#include "SolidCylinderChecker.h"
#include "PlaneChecker.h"
#include "Globe.h"

#include <iostream>
#include <fstream>
using namespace std;


void
World::build(void){
	int num_samples = 16;

	vp.set_hres(600);
	vp.set_vres(500);
	vp.set_samples(num_samples);
	vp.set_max_depth(4);

//	tracer_ptr = new RayCast(this);
	tracer_ptr = new Whitted(this);

	Pinhole* pinhole_ptr = new Pinhole;

	pinhole_ptr->set_eye(0, 20, 80);
	pinhole_ptr->set_lookat(0, 1, 0);
	pinhole_ptr->set_view_distance(3600);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

	PointLight* point_light_ptr = new PointLight;
	point_light_ptr->set_location(30, 10, 30);
	point_light_ptr->scale_radiance(3.0);
	point_light_ptr->set_cast_shadow(true);
	add_light(point_light_ptr);

	Globe* globe_ptr = new Globe;
	add_object(globe_ptr);

///////////////////////////////////////////////////////////////////////////////////////////

    SolidCylinder* sc_ptr = new SolidCylinder();

	// SolidCylinderChecker:
	SolidCylinderChecker* texture_ptr3 = new SolidCylinderChecker();
	texture_ptr3->set_num_horizontal_checkers(20);
	texture_ptr3->set_num_vertical_checkers(5);
	texture_ptr3->set_num_radius_checkers_disk(5);
	texture_ptr3->set_horizontal_line_width(0.05);
	texture_ptr3->set_vertical_line_width(0.02);
    texture_ptr3->set_color1(1.0, 0.0, 0.0);
    texture_ptr3->set_color2(0.0, 1.0, 0.0);
    texture_ptr3->set_line_color(1.0, 1.0, 1.0);

	// material:

	SV_Phong* sv_phong_sc_ptr = new SV_Phong;
	sv_phong_sc_ptr->set_ka(0.25);
	sv_phong_sc_ptr->set_kd(0.75);
	sv_phong_sc_ptr->set_cd(texture_ptr3);
	sv_phong_sc_ptr->set_ks(0.1);
	sv_phong_sc_ptr->set_exp(20.0);

	Instance* instance_sc_ptr = new Instance(sc_ptr);
	instance_sc_ptr->scale(3, (1.0/2.0), 3);
	instance_sc_ptr->translate(0, (-5.0/2.0), 0);
	instance_sc_ptr->set_material(sv_phong_sc_ptr);
    add_object(instance_sc_ptr);

    //------------------------------PlaneChecker

	PlaneChecker* plane_checker_ptr = new PlaneChecker;
	plane_checker_ptr->set_size(1.0);
	plane_checker_ptr->set_line_width(0.125);
    plane_checker_ptr->set_color1(white);
    plane_checker_ptr->set_color2(white);
    plane_checker_ptr->set_line_color(black);

	// textured material:

    SV_Matte* matte_ptr1 = new SV_Matte;
    matte_ptr1->set_ka(0.25);
    matte_ptr1->set_kd(0.75);
    matte_ptr1->set_cd(plane_checker_ptr);

    Plane* plane_ptr = new Plane();

    Instance* instance_plane_ptr1 = new Instance(plane_ptr);
    instance_plane_ptr1->translate(0, -3, 0);
    instance_plane_ptr1->set_material(matte_ptr1);
	add_object(instance_plane_ptr1);

    Instance* instance_plane_ptr2 = new Instance(plane_ptr);
    instance_plane_ptr2->rotate_x(90);
    instance_plane_ptr2->translate(0, 0, -5);
    instance_plane_ptr2->set_material(matte_ptr1);
	add_object(instance_plane_ptr2);

}

#elif ChapterNum == 23

#include "World.h"
#include "Ambient.h"
#include "Pinhole.h"
#include "Directional.h"
#include "RayCast.h"
#include "Matte.h"
#include "Phong.h"
#include "Plane.h"
#include "MultiJittered.h"
#include "AmbientOccluder.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "AreaLighting.h"
#include "Instance.h"
#include "Grid.h"
#include "PointLight.h"

#if 0 // TwoTriangles
void
World::build(void) {
	int num_samples = 1;

	vp.set_hres(40);
	vp.set_vres(40);
	vp.set_samples(num_samples);

	tracer_ptr = new RayCast(this);

	background_color = black;

	Pinhole* pinhole_ptr = new Pinhole;
	pinhole_ptr->set_eye(100, 50, 90);
	pinhole_ptr->set_lookat(0, -0.5, 0);
	pinhole_ptr->set_view_distance(1600);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

	PointLight* light_ptr1 = new PointLight;
	light_ptr1->set_location(0, 50, -10);
	light_ptr1->scale_radiance(8.0);
    light_ptr1->set_cast_shadow(true);
	add_light(light_ptr1);

	Matte* matte_ptr1 = new Matte;
	matte_ptr1->set_ka(0.1);
	matte_ptr1->set_kd(0.75);
	matte_ptr1->set_cd(0.1, 0.5, 1.0);

	char* file_name = "/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/TwoTriangles.ply";
	Grid* grid_ptr = new Grid(new Mesh);
	grid_ptr->read_flat_triangles(file_name);		// for Figure 23.7(a)
//	grid_ptr->read_smooth_triangles(file_name);		// for Figure 23.7(b)
	grid_ptr->set_material(matte_ptr1);
	grid_ptr->setup_cells();
	add_object(grid_ptr);

	Matte* matte_ptr2 = new Matte;
	matte_ptr2->set_cd(1, 0.9, 0.6);
	matte_ptr2->set_ka(0.25);
	matte_ptr2->set_kd(0.4);

	Plane* plane_ptr1 = new Plane(Point3D(0, -2.0, 0), Normal(0, 1, 0));
	plane_ptr1->set_material(matte_ptr2);
	add_object(plane_ptr1);
}

#endif // 0

#if 0 // bunny, horse, dragon, happy, and so on
void
World::build(void) {
	int num_samples = 1;

	vp.set_hres(400);
	vp.set_vres(400);
	vp.set_samples(num_samples);

	tracer_ptr = new RayCast(this);

	background_color = black;

	Pinhole* pinhole_ptr = new Pinhole;
#if 0 // 300*300: happy, Bunny69K, dragon
	pinhole_ptr->set_eye(0, 0.1, 2.0);
	pinhole_ptr->set_lookat(0, 0.1, 0);
	pinhole_ptr->set_view_distance(1600);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

	PointLight* light_ptr1 = new PointLight;
	light_ptr1->set_location(13, 10, 10);
	light_ptr1->scale_radiance(3.0);
    light_ptr1->set_cast_shadow(true);
	add_light(light_ptr1);
#endif // 0
#if 0 // 300*300: Horse97K
	pinhole_ptr->set_eye(0, 0.1, 6.0);
	pinhole_ptr->set_lookat(0, 0.1, 0);
	pinhole_ptr->set_view_distance(1600);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

	PointLight* light_ptr1 = new PointLight;
	light_ptr1->set_location(13, 10, 10);
	light_ptr1->scale_radiance(3.0);
    light_ptr1->set_cast_shadow(true);
	add_light(light_ptr1);
#endif // 0
#if 1 // 400*400: hand
	pinhole_ptr->set_eye(25, 10, 25);
	pinhole_ptr->set_lookat(1, 1, 0);
	pinhole_ptr->set_view_distance(1600);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

	PointLight* light_ptr1 = new PointLight;
	light_ptr1->set_location(13, 10, 10);
	light_ptr1->scale_radiance(3.0);
    light_ptr1->set_cast_shadow(true);
	add_light(light_ptr1);
#endif // 0
#if 0 // 400*400: goldfish_high_res,
	pinhole_ptr->set_eye(75, 20, 80);
	pinhole_ptr->set_lookat(-0.05, -0.5, 0);
	pinhole_ptr->set_view_distance(1600);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

	PointLight* light_ptr1 = new PointLight;
	light_ptr1->set_location(13, 10, 10);
	light_ptr1->scale_radiance(3.0);
    light_ptr1->set_cast_shadow(true);
	add_light(light_ptr1);
#endif // 1

	Phong* phong_ptr1 = new Phong;
	phong_ptr1->set_ka(0.4);
	phong_ptr1->set_kd(0.8);
	phong_ptr1->set_cd(1.0, 0.2, 0.0);
	phong_ptr1->set_ks(0.5);
//	phong_ptr1->set_cs(1.0, 1.0, 0.0);
	phong_ptr1->set_exp(50.0);

//	char* file_name = "/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/goldfish_low_res.ply";
//	char* file_name = "/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/goldfish_high_res.ply";
//	char* file_name = "/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/Horse2K.ply";
//	char* file_name = "/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/Horse97K.ply";
//	char* file_name = "/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/Bunny16K.ply";
//	char* file_name = "/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/Bunny69K.ply";
//	char* file_name = "/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/dragon.ply";
//	char* file_name = "/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/happy.ply";
	char* file_name = "/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/hand.ply";
	Grid* grid_ptr = new Grid(new Mesh);
//	grid_ptr->read_flat_triangles(file_name);
	grid_ptr->read_smooth_triangles(file_name);
	grid_ptr->set_material(phong_ptr1);
	grid_ptr->setup_cells();
	add_object(grid_ptr);

	Phong* phong_ptr2 = new Phong;
	phong_ptr2->set_ka(0.4);
	phong_ptr2->set_kd(0.8);
	phong_ptr2->set_cd(0.5, 1.0, 0.5);
	phong_ptr2->set_ks(0.5);
//	phong_ptr2->set_cs(1.0, 1.0, 0.0);
	phong_ptr2->set_exp(50.0);

    Plane* plane_ptr1 = new Plane(Point3D(0, -2.0, 0), Normal(0, 1, 0));//Point3D(0, -2.0, 0) for goldfish
	plane_ptr1->set_material(phong_ptr2);
	add_object(plane_ptr1);

}
#endif // 0

#if 0 // multiple-object, penguin, flat
void
World::build(void) {
	int num_samples = 16;

	vp.set_hres(400);
	vp.set_vres(400);
	vp.set_max_depth(0);
	vp.set_samples(num_samples);
	vp.set_pixel_size(1.0);

	background_color = black;

	tracer_ptr = new RayCast(this);

	Pinhole* pinhole_ptr = new Pinhole;
	pinhole_ptr->set_eye(50, 40, 110);
	pinhole_ptr->set_lookat(1, 0, 0);
	pinhole_ptr->set_view_distance(1500);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

	PointLight* light_ptr1 = new PointLight;
	light_ptr1->set_location(0, 30, 10);
//	light_ptr1->set_location(13, 10, 10);
	light_ptr1->scale_radiance(3.0);
    light_ptr1->set_cast_shadow(true);
	add_light(light_ptr1);


	float ka = 0.25;
	float kd = 0.75;

	RGBColor body_color(0.5, 0.5, 1.0);
	RGBColor wing_color(0.5, 1.0, 0.4);
	RGBColor feet_color(1.0, 0.8, 0.34);
	RGBColor eyeball_color(1.0);
	RGBColor eyelid_color(1, 0, 0);

	// body

	Matte* matte_ptr1 = new Matte;
	matte_ptr1->set_ka(ka);
	matte_ptr1->set_kd(0.75);
	matte_ptr1->set_cd(body_color);

	Grid* body_ptr = new Grid;
	body_ptr->read_flat_triangles("/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/penguin_body.ply");
	body_ptr->set_material(matte_ptr1);
	body_ptr->setup_cells();
	add_object(body_ptr);

	// wings

	Matte* matte_ptr2 = new Matte;
	matte_ptr2->set_ka(ka);
	matte_ptr2->set_kd(0.5);
	matte_ptr2->set_cd(wing_color);

	Grid* left_wing_ptr = new Grid;
	left_wing_ptr->read_flat_triangles("/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/penguin_left_wing.ply");
	left_wing_ptr->set_material(matte_ptr2);
	left_wing_ptr->setup_cells();

	Grid* right_wing_ptr = new Grid;
	right_wing_ptr->read_flat_triangles("/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/penguin_right_wing.ply");
	right_wing_ptr->set_material(matte_ptr2);
	right_wing_ptr->setup_cells();


	// feet

	Matte* matte_ptr3 = new Matte;
	matte_ptr3->set_ka(ka);
	matte_ptr3->set_kd(0.5);
	matte_ptr3->set_cd(feet_color);

	Grid* left_foot_ptr = new Grid;
	left_foot_ptr->read_flat_triangles("/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/penguin_left_foot.ply");
	left_foot_ptr->set_material(matte_ptr3);
	left_foot_ptr->setup_cells();

	Grid* right_foot_ptr = new Grid;
	right_foot_ptr->read_flat_triangles("/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/penguin_right_foot.ply");
	right_foot_ptr->set_material(matte_ptr3);
	right_foot_ptr->setup_cells();


	// eyeballs

	Matte* matte_ptr4 = new Matte;
	matte_ptr4->set_ka(ka);
	matte_ptr4->set_kd(kd);
	matte_ptr4->set_cd(eyeball_color);

	Grid* left_eyeball_ptr = new Grid;
	left_eyeball_ptr->read_flat_triangles("/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/penguin_left_eyeball.ply");
	left_eyeball_ptr->set_material(matte_ptr4);
	left_eyeball_ptr->setup_cells();

	Grid* right_eyeball_ptr = new Grid;
	right_eyeball_ptr->read_flat_triangles("/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/penguin_right_eyeball.ply");
	right_eyeball_ptr->set_material(matte_ptr4);
	right_eyeball_ptr->setup_cells();


	// eyelids

	Matte* matte_ptr5 = new Matte;
	matte_ptr5->set_ka(ka);
	matte_ptr5->set_kd(kd);
	matte_ptr5->set_cd(eyelid_color);

	Grid* left_lower_eyelid_ptr = new Grid;
	left_lower_eyelid_ptr->read_flat_triangles("/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/penguin_left_lower_eyelid.ply");
	left_lower_eyelid_ptr->set_material(matte_ptr5);
	left_lower_eyelid_ptr->setup_cells();

	Grid* right_lower_eyelid_ptr = new Grid;
	right_lower_eyelid_ptr->read_flat_triangles("/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/penguin_right_lower_eyelid.ply");
	right_lower_eyelid_ptr->set_material(matte_ptr5);
	right_lower_eyelid_ptr->setup_cells();


	Grid* left_upper_eyelid_ptr = new Grid;
	left_upper_eyelid_ptr->read_flat_triangles("/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/penguin_left_upper_eyelid.ply");
	left_upper_eyelid_ptr->set_material(matte_ptr5);
	left_upper_eyelid_ptr->setup_cells();

	Grid* right_upper_eyelid_ptr = new Grid;
	right_upper_eyelid_ptr->read_flat_triangles("/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/penguin_right_upper_eyelid.ply");
	right_upper_eyelid_ptr->set_material(matte_ptr5);
	right_upper_eyelid_ptr->setup_cells();



	// complete penguin

	Grid* penguin_ptr = new Grid;
	penguin_ptr->add_object(body_ptr);
	penguin_ptr->add_object(left_wing_ptr);
	penguin_ptr->add_object(right_wing_ptr);
	penguin_ptr->add_object(left_foot_ptr);
	penguin_ptr->add_object(right_foot_ptr);
	penguin_ptr->add_object(left_eyeball_ptr);
	penguin_ptr->add_object(right_eyeball_ptr);
	penguin_ptr->add_object(left_lower_eyelid_ptr);
	penguin_ptr->add_object(right_lower_eyelid_ptr);
	penguin_ptr->add_object(left_upper_eyelid_ptr);
	penguin_ptr->add_object(right_upper_eyelid_ptr);

	penguin_ptr->setup_cells();
	add_object(penguin_ptr);

	Phong* phong_ptr2 = new Phong;
	phong_ptr2->set_ka(0.4);
	phong_ptr2->set_kd(0.8);
	phong_ptr2->set_cd(0.5, 1.0, 0.5);
	phong_ptr2->set_ks(0.5);
//	phong_ptr2->set_cs(1.0, 1.0, 0.0);
	phong_ptr2->set_exp(50.0);

    Plane* plane_ptr1 = new Plane(Point3D(0, -15.0, 0), Normal(0, 1, 0));
	plane_ptr1->set_material(phong_ptr2);
	add_object(plane_ptr1);
}
#endif // 0

#if 0 // multiple-object, penguin, smooth
void
World::build(void) {
	int num_samples = 16;

	vp.set_hres(400);
	vp.set_vres(400);
	vp.set_max_depth(0);
	vp.set_samples(num_samples);
	vp.set_pixel_size(1.0);

	background_color = black;

	tracer_ptr = new RayCast(this);

	Pinhole* pinhole_ptr = new Pinhole;
	pinhole_ptr->set_eye(50, 40, 110);
	pinhole_ptr->set_lookat(1, 0, 0);
	pinhole_ptr->set_view_distance(1500);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

	PointLight* light_ptr1 = new PointLight;
	light_ptr1->set_location(0, 30, 10);
//	light_ptr1->set_location(13, 10, 10);
	light_ptr1->scale_radiance(3.0);
    light_ptr1->set_cast_shadow(true);
	add_light(light_ptr1);


	float ka = 0.25;
	float kd = 0.75;

	RGBColor body_color(0.5, 0.5, 1.0);
	RGBColor wing_color(0.5, 1.0, 0.4);
	RGBColor feet_color(1.0, 0.8, 0.34);
	RGBColor eyeball_color(1.0);
	RGBColor eyelid_color(1, 0, 0);

	// body

	Matte* matte_ptr1 = new Matte;
	matte_ptr1->set_ka(ka);
	matte_ptr1->set_kd(0.75);
	matte_ptr1->set_cd(body_color);

	Grid* body_ptr = new Grid;
	body_ptr->read_smooth_triangles("/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/penguin_body.ply");
	body_ptr->set_material(matte_ptr1);
	body_ptr->setup_cells();
	add_object(body_ptr);

	// wings

	Matte* matte_ptr2 = new Matte;
	matte_ptr2->set_ka(ka);
	matte_ptr2->set_kd(0.5);
	matte_ptr2->set_cd(wing_color);

	Grid* left_wing_ptr = new Grid;
	left_wing_ptr->read_smooth_triangles("/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/penguin_left_wing.ply");
	left_wing_ptr->set_material(matte_ptr2);
	left_wing_ptr->setup_cells();

	Grid* right_wing_ptr = new Grid;
	right_wing_ptr->read_smooth_triangles("/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/penguin_right_wing.ply");
	right_wing_ptr->set_material(matte_ptr2);
	right_wing_ptr->setup_cells();


	// feet

	Matte* matte_ptr3 = new Matte;
	matte_ptr3->set_ka(ka);
	matte_ptr3->set_kd(0.5);
	matte_ptr3->set_cd(feet_color);

	Grid* left_foot_ptr = new Grid;
	left_foot_ptr->read_smooth_triangles("/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/penguin_left_foot.ply");
	left_foot_ptr->set_material(matte_ptr3);
	left_foot_ptr->setup_cells();

	Grid* right_foot_ptr = new Grid;
	right_foot_ptr->read_smooth_triangles("/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/penguin_right_foot.ply");
	right_foot_ptr->set_material(matte_ptr3);
	right_foot_ptr->setup_cells();


	// eyeballs

	Matte* matte_ptr4 = new Matte;
	matte_ptr4->set_ka(ka);
	matte_ptr4->set_kd(kd);
	matte_ptr4->set_cd(eyeball_color);

	Grid* left_eyeball_ptr = new Grid;
	left_eyeball_ptr->read_smooth_triangles("/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/penguin_left_eyeball.ply");
	left_eyeball_ptr->set_material(matte_ptr4);
	left_eyeball_ptr->setup_cells();

	Grid* right_eyeball_ptr = new Grid;
	right_eyeball_ptr->read_smooth_triangles("/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/penguin_right_eyeball.ply");
	right_eyeball_ptr->set_material(matte_ptr4);
	right_eyeball_ptr->setup_cells();


	// eyelids

	Matte* matte_ptr5 = new Matte;
	matte_ptr5->set_ka(ka);
	matte_ptr5->set_kd(kd);
	matte_ptr5->set_cd(eyelid_color);

	Grid* left_lower_eyelid_ptr = new Grid;
	left_lower_eyelid_ptr->read_smooth_triangles("/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/penguin_left_lower_eyelid.ply");
	left_lower_eyelid_ptr->set_material(matte_ptr5);
	left_lower_eyelid_ptr->setup_cells();

	Grid* right_lower_eyelid_ptr = new Grid;
	right_lower_eyelid_ptr->read_smooth_triangles("/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/penguin_right_lower_eyelid.ply");
	right_lower_eyelid_ptr->set_material(matte_ptr5);
	right_lower_eyelid_ptr->setup_cells();


	Grid* left_upper_eyelid_ptr = new Grid;
	left_upper_eyelid_ptr->read_smooth_triangles("/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/penguin_left_upper_eyelid.ply");
	left_upper_eyelid_ptr->set_material(matte_ptr5);
	left_upper_eyelid_ptr->setup_cells();

	Grid* right_upper_eyelid_ptr = new Grid;
	right_upper_eyelid_ptr->read_smooth_triangles("/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/penguin_right_upper_eyelid.ply");
	right_upper_eyelid_ptr->set_material(matte_ptr5);
	right_upper_eyelid_ptr->setup_cells();



	// complete penguin

	Grid* penguin_ptr = new Grid;
	penguin_ptr->add_object(body_ptr);
	penguin_ptr->add_object(left_wing_ptr);
	penguin_ptr->add_object(right_wing_ptr);
	penguin_ptr->add_object(left_foot_ptr);
	penguin_ptr->add_object(right_foot_ptr);
	penguin_ptr->add_object(left_eyeball_ptr);
	penguin_ptr->add_object(right_eyeball_ptr);
	penguin_ptr->add_object(left_lower_eyelid_ptr);
	penguin_ptr->add_object(right_lower_eyelid_ptr);
	penguin_ptr->add_object(left_upper_eyelid_ptr);
	penguin_ptr->add_object(right_upper_eyelid_ptr);

	penguin_ptr->setup_cells();
	add_object(penguin_ptr);

	Phong* phong_ptr2 = new Phong;
	phong_ptr2->set_ka(0.4);
	phong_ptr2->set_kd(0.8);
	phong_ptr2->set_cd(0.5, 1.0, 0.5);
	phong_ptr2->set_ks(0.5);
//	phong_ptr2->set_cs(1.0, 1.0, 0.0);
	phong_ptr2->set_exp(50.0);

    Plane* plane_ptr1 = new Plane(Point3D(0, -15.0, 0), Normal(0, 1, 0));
	plane_ptr1->set_material(phong_ptr2);
	add_object(plane_ptr1);
}
#endif // 0

#if 1 // hierarchical instance grids
void
World::build(void) {
	int num_samples = 1;

	vp.set_hres(500);
	vp.set_vres(400);
	vp.set_samples(num_samples);

	background_color = black;
	tracer_ptr = new RayCast(this);

	// the camera is defined at the bottom

	Directional* light_ptr1 = new Directional;
	light_ptr1->set_direction(20, 40, 20);
	light_ptr1->scale_radiance(3.0);
	add_light(light_ptr1);


	Phong* phong_ptr = new Phong;
	phong_ptr->set_ka(0.2);
	phong_ptr->set_kd(0.5);
	phong_ptr->set_cd(1.0);
	phong_ptr->set_ks(0.4);
	phong_ptr->set_exp(20);

//	char* fileName = "/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/Bunny16K.ply";
	char* fileName = "/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/dragon.ply";

	Grid* bunny_ptr = new Grid(new Mesh);
//	bunny_ptr->read_flat_triangles(fileName);		// read PLY file
	bunny_ptr->read_smooth_triangles(fileName);		// read PLY file
//	bunny_ptr->set_material(phong_ptr);				// only use this if you want white bunnies
	bunny_ptr->setup_cells();

	int 	num_levels 			= 6;		// number of levels
	int 	instances_grid_res 	= 10;
	// each level consists of instances_grid_res x instances_grid_res instances
	double 	delta 				= 0.1; 		// gap between instances.
	double 	gap 				= 0.16;
	// gap between bunnies in the level 0 grid. bunny 0.08, dragon 0.16
	double 	size 				= 0.1;    	// bunny size
	double 	mcx 				= 0.5;
	// camera coordinates as multiple mcx of grid size - allows us to keep the whole grid in view

	set_rand_seed(1000);

	Grid* current_grid_ptr = bunny_ptr;							// just the bunny

	for (int level = 0; level < num_levels; level++) {
		Grid* instance_grid_ptr = new Grid;						// temporary grid

		for (int i = 0; i < instances_grid_res; i++) {   		// xw direction
			for (int k = 0; k < instances_grid_res; k++) {   	// zw direction

				Phong* phong_ptr = new Phong;
				phong_ptr->set_ka(0.2);
				phong_ptr->set_kd(0.5);
				phong_ptr->set_cd(rand_float(), rand_float(), rand_float());
				phong_ptr->set_ks(0.4);
				phong_ptr->set_exp(20);

				Instance* instance_ptr = new Instance;
				instance_ptr->set_object(current_grid_ptr); 	// add whole grid up to this level
				instance_ptr->set_material(phong_ptr);
				instance_ptr->translate(i * (size + gap), 0.0, k * (size + gap));
				instance_ptr->compute_bounding_box();
				instance_grid_ptr->add_object(instance_ptr);
			}
		}

		size = instances_grid_res * size + (instances_grid_res - 1) * gap;
		gap = delta * size;
		instance_grid_ptr->setup_cells();
		current_grid_ptr = instance_grid_ptr;					// now the whole grid up to this level
	}

	add_object(current_grid_ptr); 								// the whole n-level grid


	Pinhole* pinhole_ptr = new Pinhole;
	pinhole_ptr->set_eye(7 * mcx * size, 4 * mcx * size, 5 * mcx * size);
//	pinhole_ptr->set_lookat(mcx * size, 0.0, mcx * size);     				// use this for the whole grid
	pinhole_ptr->set_lookat(mcx * size, 0.4 * mcx * size, mcx * size);   	// use this for the zooms
//	pinhole_ptr->set_view_distance(1400);   		// Figure 23.12(a) the whole grid
//	pinhole_ptr->set_view_distance(140000);  		// Figure 23.12(b)
//	pinhole_ptr->set_view_distance(14000000);  		// Figure 23.12(c)
//	pinhole_ptr->set_view_distance(300000000);   	// Figure 23.12(d)
	pinhole_ptr->set_view_distance(1200000000);   	// Figure 23.12(d) for dragon
//	pinhole_ptr->set_view_distance(100000000);   	// extra image
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);
}
#endif // 0


#elif ChapterNum == 24

#include "World.h"
#include "Ambient.h"
#include "Pinhole.h"
#include "Directional.h"
#include "RayCast.h"
#include "Matte.h"
#include "Phong.h"
#include "Plane.h"
#include "MultiJittered.h"
#include "AmbientOccluder.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "AreaLighting.h"
#include "Instance.h"
#include "Grid.h"
#include "PointLight.h"#include "Reflective.h"
#include "Whitted.h"

#if 1
void
World::build(void) {
	int num_samples = 1;

	vp.set_hres(600);
	vp.set_vres(600);
	vp.set_samples(num_samples);
//	vp.set_max_depth(0);
	vp.set_max_depth(1);

	tracer_ptr = new Whitted(this);

	background_color = black;

	Pinhole* pinhole_ptr = new Pinhole;

#if 1 // 400*400: hand
	pinhole_ptr->set_eye(25, 10, 25);
	pinhole_ptr->set_lookat(1, 0, 0);
	pinhole_ptr->set_view_distance(1600);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

	PointLight* light_ptr1 = new PointLight;
	light_ptr1->set_location(13, 10, 10);
	light_ptr1->scale_radiance(3.0);
    light_ptr1->set_cast_shadow(true);
	add_light(light_ptr1);
#endif //

	Phong* phong_ptr1 = new Phong;
	phong_ptr1->set_ka(0.4);
	phong_ptr1->set_kd(0.8);
	phong_ptr1->set_cd(1.0, 0.2, 0.0);
	phong_ptr1->set_ks(0.5);
	phong_ptr1->set_exp(50.0); // 50.0

	Reflective* reflective_ptr1 = new Reflective;
	reflective_ptr1->set_ka(0.4);               // ambient reflection, ka=0.4
	reflective_ptr1->set_kd(0.8);               // diffuse reflection, kd=0.8
	reflective_ptr1->set_cd(0.5, 1.0, 0.5);    	// light green
	reflective_ptr1->set_ks(0.5);               // specular reflection, ks=0.5
	reflective_ptr1->set_exp(50.0);             // 50.0
	reflective_ptr1->set_kr(0.75);              // indirect, mirror reflection, kr=0.75
	reflective_ptr1->set_cr(white); 	// default color is (0.5, 0.5, 1.0)

	char* file_name = "/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/hand.ply";
	Grid* grid_ptr = new Grid(new Mesh);
	grid_ptr->read_smooth_triangles(file_name);
	grid_ptr->set_material(phong_ptr1);
	grid_ptr->setup_cells();
	add_object(grid_ptr);

    Plane* plane_ptr1 = new Plane(Point3D(0, -0.5, 0), Normal(0, 1, 0));
	plane_ptr1->set_material(reflective_ptr1);
	add_object(plane_ptr1);

}
#endif // 0

#elif ChapterNum == 25

#include "World.h"
#include "Ambient.h"
#include "Pinhole.h"
#include "Directional.h"
#include "RayCast.h"
#include "Matte.h"
#include "Phong.h"
#include "Plane.h"
#include "OpenCylinder.h"
#include "MultiJittered.h"
#include "AmbientOccluder.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "AreaLighting.h"
#include "Instance.h"
#include "Grid.h"
#include "PointLight.h"#include "Reflective.h"
#include "GlossyReflector.h"
#include "Whitted.h"

#if 0
void
World::build(void) {
	int num_samples = 256;   // use 1 for testing!

	vp.set_hres(600);
	vp.set_vres(400);
	vp.set_samples(num_samples);
	vp.set_max_depth(10);

	tracer_ptr = new Whitted(this);

	background_color = black;

	AmbientOccluder* ambient_occluder_ptr = new AmbientOccluder;
	ambient_occluder_ptr->set_sampler(new MultiJittered(num_samples));
	ambient_occluder_ptr->set_min_amount(0.5);
	set_ambient_light(ambient_occluder_ptr);


	Pinhole* pinhole_ptr = new Pinhole;
	pinhole_ptr->set_eye(100, 45, 100);
	pinhole_ptr->set_lookat(-10, 35, 0);
	pinhole_ptr->set_view_distance(400);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);


	PointLight* light_ptr2 = new PointLight;
	light_ptr2->set_location(150, 250, -150);
	light_ptr2->scale_radiance(1.5);
	light_ptr2->set_cast_shadow(true);
	add_light(light_ptr2);


	// other objects

	// large sphere

	GlossyReflector* glossy_ptr1 = new GlossyReflector;
	glossy_ptr1->set_samples(num_samples, 100.0);
	glossy_ptr1->set_ka(0.0);
	glossy_ptr1->set_kd(0.0);
	glossy_ptr1->set_ks(0.3);
	glossy_ptr1->set_exp(100.0);
	glossy_ptr1->set_cd(1.0, 1.0, 0.3);	 // orange
	glossy_ptr1->set_kr(0.9);
	glossy_ptr1->set_exponent(100.0);
	glossy_ptr1->set_cr(1.0, 0.75, 0.5);  // orange

	Sphere* sphere_ptr1 = new Sphere(Point3D(38, 20, -24), 20);
	sphere_ptr1->set_material(glossy_ptr1);
	add_object(sphere_ptr1);


	// small sphere

	Matte* matte_ptr2 = new Matte;
	matte_ptr2->set_ka(0.4);
	matte_ptr2->set_kd(0.4);
	matte_ptr2->set_cd(0.75, 0, 0);     // red

	Sphere* sphere_ptr2 = new Sphere(Point3D(34, 12, 13), 12);
	sphere_ptr2->set_material(matte_ptr2);
	add_object(sphere_ptr2);


	// medium sphere

	Reflective* reflective_ptr = new Reflective;
	reflective_ptr->set_cd(0.75);
	reflective_ptr->set_ka(0.0);
	reflective_ptr->set_kd(0.0);
	reflective_ptr->set_ks(0.0);
	reflective_ptr->set_exp(20);
	reflective_ptr->set_kr(0.9);
	reflective_ptr->set_cr(1.0, 0.75, 0.5);   // orange

	Sphere* sphere_ptr3 = new Sphere(Point3D(-7, 15, 42), 16);
	sphere_ptr3->set_material(reflective_ptr);
	add_object(sphere_ptr3);


	// cylinder

	GlossyReflector* glossy_ptr2 = new GlossyReflector;
	glossy_ptr2->set_samples(num_samples, 10.0);
	glossy_ptr2->set_ka(0.0);
	glossy_ptr2->set_kd(0.0);
	glossy_ptr2->set_ks(0.75);
//	glossy_ptr2->set_cs(0.35, 0.75, 0.55);  // green
	glossy_ptr2->set_exp(10.0);
	glossy_ptr2->set_cd(1.0, 1.0, 0.3);
	glossy_ptr2->set_kr(0.9);
	glossy_ptr2->set_exponent(10.0);
	glossy_ptr2->set_cr(0.35, 0.75, 0.55);   // green

	double bottom 	= 0.0;
	double top 		= 85.0;
	double radius	= 22.0;
	OpenCylinder* cylinder_ptr = new OpenCylinder(bottom, top, radius);
	cylinder_ptr->set_material(glossy_ptr2);
	add_object(cylinder_ptr);


	// ground plane

	Reflective* reflective_ptr1 = new Reflective;
	reflective_ptr1->set_ka(0.4);               // ambient reflection, ka=0.4
	reflective_ptr1->set_kd(0.8);               // diffuse reflection, kd=0.8
	reflective_ptr1->set_cd(0.5, 1.0, 0.5);    	// light green
	reflective_ptr1->set_ks(0.5);               // specular reflection, ks=0.5
	reflective_ptr1->set_exp(50.0);             // 50.0
	reflective_ptr1->set_kr(0.75);              // indirect, mirror reflection, kr=0.75
	reflective_ptr1->set_cr(white); 	        // default color is (0.5, 0.5, 1.0)

	Plane* plane_ptr = new Plane(Point3D(0, 0.01, 0), Normal(0, 1, 0));
	plane_ptr->set_material(reflective_ptr1);
	add_object(plane_ptr);
}
#endif // 0

#if 1
void
World::build(void) {
	int num_samples = 100;

	vp.set_hres(400);
	vp.set_vres(600);
	vp.set_samples(num_samples);
//	vp.set_max_depth(0);
	vp.set_max_depth(1);

	tracer_ptr = new Whitted(this);

	background_color = black;

	Pinhole* pinhole_ptr = new Pinhole;

#if 1 // 400*400: hand
	pinhole_ptr->set_eye(25, 10, 25);
	pinhole_ptr->set_lookat(1, 0, 0);
	pinhole_ptr->set_view_distance(1600);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

	PointLight* light_ptr1 = new PointLight;
	light_ptr1->set_location(13, 10, 10);
	light_ptr1->scale_radiance(3.0);
    light_ptr1->set_cast_shadow(true);
	add_light(light_ptr1);
#endif //

	Phong* phong_ptr1 = new Phong;
	phong_ptr1->set_ka(0.4);
	phong_ptr1->set_kd(0.8);
	phong_ptr1->set_cd(1.0, 0.2, 0.0);
	phong_ptr1->set_ks(0.5);
	phong_ptr1->set_exp(50.0); // 50.0

//	float exp = 10.0;  		    // 10.0
//	float exp = 100.0;  		// 100.0
//	float exp = 1000.0;  		// 1000.0
//	float exp = 10000.0;  		// 10000.0
	float exp = 100000.0;  		// 100000.0
//	float exp = 1000000.0;  	// 1000000.0

	GlossyReflector* glossy_ptr2 = new GlossyReflector;
	glossy_ptr2->set_samples(num_samples, exp);
	glossy_ptr2->set_ka(0.0);                // 0.4
	glossy_ptr2->set_kd(0.0);                // 0.8
	glossy_ptr2->set_ks(0.0);                // 0.5
	glossy_ptr2->set_exp(exp);              // for phong
	glossy_ptr2->set_cd(0.5, 1.0, 0.5);
	glossy_ptr2->set_kr(0.75);
	glossy_ptr2->set_exponent(exp);         // for glossy.
	glossy_ptr2->set_cr(white);   // green

	char* file_name = "/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/hand.ply";
	Grid* grid_ptr = new Grid(new Mesh);
	grid_ptr->read_smooth_triangles(file_name);
	grid_ptr->set_material(phong_ptr1);
	grid_ptr->setup_cells();
	add_object(grid_ptr);

    Plane* plane_ptr1 = new Plane(Point3D(0, -0.5, 0), Normal(0, 1, 0));
	plane_ptr1->set_material(glossy_ptr2);
	add_object(plane_ptr1);

}

#endif // 1

#elif ChapterNum == 26

#include "World.h"
#include "Ambient.h"
#include "Pinhole.h"
#include "Directional.h"
#include "RayCast.h"
#include "Matte.h"
#include "Phong.h"
#include "Plane.h"
#include "OpenCylinder.h"
#include "MultiJittered.h"
#include "AmbientOccluder.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "AreaLighting.h"
#include "Instance.h"
#include "Grid.h"
#include "PointLight.h"#include "Reflective.h"
#include "GlossyReflector.h"
#include "PathTrace.h"
#include "GlobalTrace.h"
#include "Rectangle.h"

#if 0 // path trace. Conell Box
void
World::build(void) {
//	int num_samples = 1;		// for Figure 26.7(a)
//	int num_samples = 100;		// for Figure 26.7(b)
	int num_samples = 1024;		// for Figure 26.7(c)
//	int num_samples = 10000;	// for Figure 26.7(d)

	vp.set_hres(300);
	vp.set_vres(300);
	vp.set_samples(num_samples);
	vp.set_max_depth(10);

	background_color = black;

	tracer_ptr = new PathTrace(this);

	Pinhole* pinhole_ptr = new Pinhole;
	pinhole_ptr->set_eye(27.6, 27.4, -80.0);
	pinhole_ptr->set_lookat(27.6, 27.4, 0.0);
	pinhole_ptr->set_view_distance(400);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);


	Point3D p0;
	Vector3D a, b;
	Normal normal;

	// box dimensions

	double width 	= 55.28;   	// x direction
	double height 	= 54.88;  	// y direction
	double depth 	= 55.92;	// z direction


	// the ceiling light - doesn't need samples

	Emissive* emissive_ptr = new Emissive;
	emissive_ptr->set_ce(1.0, 0.73, 0.4);
	emissive_ptr->scale_radiance(100);

	p0 = Point3D(21.3, height - 0.001, 22.7);
	a = Vector3D(0.0, 0.0, 10.5);
	b = Vector3D(13.0, 0.0, 0.0);
	normal = Normal(0.0, -1.0, 0.0);
	Rectangle* light_ptr = new Rectangle(p0, a, b, normal);
	light_ptr->set_material(emissive_ptr);
	add_object(light_ptr);


	// left wall

	Matte* matte_ptr1 = new Matte;
	matte_ptr1->set_ka(0.0);
	matte_ptr1->set_kd(0.6);
	matte_ptr1->set_cd(0.57, 0.025, 0.025);	 // red
	matte_ptr1->set_sampler(new MultiJittered(num_samples));

	p0 = Point3D(width, 0.0, 0.0);
	a = Vector3D(0.0, 0.0, depth);
	b = Vector3D(0.0, height, 0.0);
	normal = Normal(-1.0, 0.0, 0.0);
	Rectangle* left_wall_ptr = new Rectangle(p0, a, b, normal);
	left_wall_ptr->set_material(matte_ptr1);
	add_object(left_wall_ptr);


	// right wall

	Matte* matte_ptr2 = new Matte;
	matte_ptr2->set_ka(0.0);
	matte_ptr2->set_kd(0.6);
	matte_ptr2->set_cd(0.37, 0.59, 0.2);	 // green   from Photoshop
	matte_ptr2->set_sampler(new MultiJittered(num_samples));

	p0 = Point3D(0.0, 0.0, 0.0);
	a = Vector3D(0.0, 0.0, depth);
	b = Vector3D(0.0, height, 0.0);
	normal = Normal(1.0, 0.0, 0.0);
	Rectangle* right_wall_ptr = new Rectangle(p0, a, b, normal);
	right_wall_ptr->set_material(matte_ptr2);
	add_object(right_wall_ptr);


	// back wall

	Matte* matte_ptr3 = new Matte;
	matte_ptr3->set_ka(0.0);
	matte_ptr3->set_kd(0.6);
	matte_ptr3->set_cd(1.0);	 // white
	matte_ptr3->set_sampler(new MultiJittered(num_samples));

	p0 = Point3D(0.0, 0.0, depth);
	a = Vector3D(width, 0.0, 0.0);
	b = Vector3D(0.0, height, 0.0);
	normal = Normal(0.0, 0.0, -1.0);
	Rectangle* back_wall_ptr = new Rectangle(p0, a, b, normal);
	back_wall_ptr->set_material(matte_ptr3);
	add_object(back_wall_ptr);


	// floor

	p0 = Point3D(0.0, 0.0, 0.0);
	a = Vector3D(0.0, 0.0, depth);
	b = Vector3D(width, 0.0, 0.0);
	normal = Normal(0.0, 1.0, 0.0);
	Rectangle* floor_ptr = new Rectangle(p0, a, b, normal);
	floor_ptr->set_material(matte_ptr3);
	add_object(floor_ptr);


	// ceiling

	p0 = Point3D(0.0, height, 0.0);
	a = Vector3D(0.0, 0.0, depth);
	b = Vector3D(width, 0.0, 0.0);
	normal = Normal(0.0, -1.0, 0.0);
	Rectangle* ceiling_ptr = new Rectangle(p0, a, b, normal);
	ceiling_ptr->set_material(matte_ptr3);
	add_object(ceiling_ptr);


	// the two boxes defined as 5 rectangles each

	// short box

	// top

	p0 = Point3D(13.0, 16.5, 6.5);
	a = Vector3D(-4.8, 0.0, 16.0);
	b = Vector3D(16.0, 0.0, 4.9);
	normal = Normal(0.0, 1.0, 0.0);
	Rectangle* short_top_ptr = new Rectangle(p0, a, b, normal);
	short_top_ptr->set_material(matte_ptr3);
	add_object(short_top_ptr);


	// side 1

	p0 = Point3D(13.0, 0.0, 6.5);
	a = Vector3D(-4.8, 0.0, 16.0);
	b = Vector3D(0.0, 16.5, 0.0);
	Rectangle* short_side_ptr1 = new Rectangle(p0, a, b);
	short_side_ptr1->set_material(matte_ptr3);
	add_object(short_side_ptr1);


	// side 2

	p0 = Point3D(8.2, 0.0, 22.5);
	a = Vector3D(15.8, 0.0, 4.7);
	Rectangle* short_side_ptr2 = new Rectangle(p0, a, b);
	short_side_ptr2->set_material(matte_ptr3);
	add_object(short_side_ptr2);


	// side 3

	p0 = Point3D(24.2, 0.0, 27.4);
	a = Vector3D(4.8, 0.0, -16.0);
	Rectangle* short_side_ptr3 = new Rectangle(p0, a, b);
	short_side_ptr3->set_material(matte_ptr3);
	add_object(short_side_ptr3);


	// side 4

	p0 = Point3D(29.0, 0.0, 11.4);
	a = Vector3D(-16.0, 0.0, -4.9);
	Rectangle* short_side_ptr4 = new Rectangle(p0, a, b);
	short_side_ptr4->set_material(matte_ptr3);
	add_object(short_side_ptr4);




	// tall box

	// top

	p0 = Point3D(42.3, 33.0, 24.7);
	a = Vector3D(-15.8, 0.0, 4.9);
	b = Vector3D(4.9, 0.0, 15.9);
	normal = Normal(0.0, 1.0, 0.0);
	Rectangle* tall_top_ptr = new Rectangle(p0, a, b, normal);
	tall_top_ptr->set_material(matte_ptr3);
	add_object(tall_top_ptr);


	// side 1

	p0 = Point3D(42.3, 0.0, 24.7);
	a = Vector3D(-15.8, 0.0, 4.9);
	b = Vector3D(0.0, 33.0, 0.0);
	Rectangle* tall_side_ptr1 = new Rectangle(p0, a, b);
	tall_side_ptr1->set_material(matte_ptr3);
	add_object(tall_side_ptr1);


	// side 2

	p0 = Point3D(26.5, 0.0, 29.6);
	a = Vector3D(4.9, 0.0, 15.9);
	Rectangle* tall_side_ptr2 = new Rectangle(p0, a, b);
	tall_side_ptr2->set_material(matte_ptr3);
	add_object(tall_side_ptr2);


	// side 3

	p0 = Point3D(31.4, 0.0, 45.5);
	a = Vector3D(15.8, 0.0, -4.9);
	Rectangle* tall_side_ptr3 = new Rectangle(p0, a, b);
	tall_side_ptr3->set_material(matte_ptr3);
	add_object(tall_side_ptr3);


	// side 4

	p0 = Point3D(47.2, 0.0, 40.6);
	a = Vector3D(-4.9, 0.0, -15.9);
	Rectangle* tall_side_ptr4 = new Rectangle(p0, a, b);
	tall_side_ptr4->set_material(matte_ptr3);
	add_object(tall_side_ptr4);
}

#endif // 0

#if 0 // path trace. caustic 1, reflective cylinder
void
World::build(void) {
	int num_samples = 100; // 5041

	vp.set_hres(400);
	vp.set_vres(400);
	vp.set_samples(num_samples);
	vp.set_max_depth(5);

	tracer_ptr = new PathTrace(this);

	Pinhole* pinhole_ptr = new Pinhole;
	pinhole_ptr->set_eye(5, 10, 20);
	pinhole_ptr->set_lookat(-1, 1, 0);
	pinhole_ptr->set_view_distance(500);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);


	// emissive sphere

	Emissive* emissive_ptr = new Emissive;
	emissive_ptr->scale_radiance(20.0);
	emissive_ptr->set_ce(0.75, 1, 0.75);      // light green

	Sphere* sphere_ptr = new Sphere(Point3D(-2, 7, 6), 2);
	sphere_ptr->set_material(emissive_ptr);
	sphere_ptr->set_sampler(new MultiJittered(num_samples));
	add_object(sphere_ptr);


	// reflective rectangle

	Reflective* reflective_ptr = new Reflective;
	reflective_ptr->set_ka(0.0);
	reflective_ptr->set_kd(0.0);
	reflective_ptr->set_ks(0.0);
	reflective_ptr->set_exp(1.0);
	reflective_ptr->set_kr(0.95);
	reflective_ptr->set_cr(1.0, 0.2, 0.2);

	double bottom 	= -1.0;
	double top 		=  1.0;
	double radius	=  7.0;
	OpenCylinder* cylinder_ptr = new OpenCylinder(bottom, top, radius);
	cylinder_ptr->set_material(reflective_ptr);
	add_object(cylinder_ptr);


	// plane

	Matte* matte_ptr = new Matte;
	matte_ptr->set_ka(0);
	matte_ptr->set_kd(0.75);
	matte_ptr->set_cd(white);
	matte_ptr->set_sampler(new MultiJittered(num_samples));

	Plane* plane_ptr = new Plane(Point3D(0, -1.0, 0), Normal(0, 1, 0));
	plane_ptr->set_material(matte_ptr);
	add_object(plane_ptr);
}

#endif // 1

#if 0 // global trace. Conell Box
void
World::build(void) {
//	int num_samples = 1;		// for Figure 26.7(a)
	int num_samples = 100;		// for Figure 26.7(b)
//	int num_samples = 1024;		// for Figure 26.7(c)
//	int num_samples = 10000;	// for Figure 26.7(d)

	vp.set_hres(300);
	vp.set_vres(300);
	vp.set_samples(num_samples);
	vp.set_max_depth(10);

	background_color = black;

	tracer_ptr = new GlobalTrace(this);

	Pinhole* pinhole_ptr = new Pinhole;
	pinhole_ptr->set_eye(27.6, 27.4, -80.0);
	pinhole_ptr->set_lookat(27.6, 27.4, 0.0);
	pinhole_ptr->set_view_distance(400);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);


	Point3D p0;
	Vector3D a, b;
	Normal normal;

	// box dimensions

	double width 	= 55.28;   	// x direction
	double height 	= 54.88;  	// y direction
	double depth 	= 55.92;	// z direction


	// the ceiling light - doesn't need samples

	Emissive* emissive_ptr = new Emissive;
	emissive_ptr->set_ce(1.0, 0.73, 0.4);
	emissive_ptr->scale_radiance(100);

	p0 = Point3D(21.3, height - 0.001, 22.7);
	a = Vector3D(0.0, 0.0, 10.5);
	b = Vector3D(13.0, 0.0, 0.0);
	normal = Normal(0.0, -1.0, 0.0);
	Rectangle* light_ptr = new Rectangle(p0, a, b, normal);
	light_ptr->set_material(emissive_ptr);
	light_ptr->set_sampler(new MultiJittered(num_samples));
	add_object(light_ptr);


	AreaLight* area_light_ptr = new AreaLight;
	area_light_ptr->set_object(light_ptr);
	area_light_ptr->set_cast_shadow(true);
	add_light(area_light_ptr);


	// left wall

	Matte* matte_ptr1 = new Matte;
	matte_ptr1->set_ka(0.0);
	matte_ptr1->set_kd(0.6);
	matte_ptr1->set_cd(0.57, 0.025, 0.025);	 // red
	matte_ptr1->set_sampler(new MultiJittered(num_samples));

	p0 = Point3D(width, 0.0, 0.0);
	a = Vector3D(0.0, 0.0, depth);
	b = Vector3D(0.0, height, 0.0);
	normal = Normal(-1.0, 0.0, 0.0);
	Rectangle* left_wall_ptr = new Rectangle(p0, a, b, normal);
	left_wall_ptr->set_material(matte_ptr1);
	add_object(left_wall_ptr);


	// right wall

	Matte* matte_ptr2 = new Matte;
	matte_ptr2->set_ka(0.0);
	matte_ptr2->set_kd(0.6);
	matte_ptr2->set_cd(0.37, 0.59, 0.2);	 // green   from Photoshop
	matte_ptr2->set_sampler(new MultiJittered(num_samples));

	p0 = Point3D(0.0, 0.0, 0.0);
	a = Vector3D(0.0, 0.0, depth);
	b = Vector3D(0.0, height, 0.0);
	normal = Normal(1.0, 0.0, 0.0);
	Rectangle* right_wall_ptr = new Rectangle(p0, a, b, normal);
	right_wall_ptr->set_material(matte_ptr2);
	add_object(right_wall_ptr);


	// back wall

	Matte* matte_ptr3 = new Matte;
	matte_ptr3->set_ka(0.0);
	matte_ptr3->set_kd(0.6);
	matte_ptr3->set_cd(1.0);	 // white
	matte_ptr3->set_sampler(new MultiJittered(num_samples));

	p0 = Point3D(0.0, 0.0, depth);
	a = Vector3D(width, 0.0, 0.0);
	b = Vector3D(0.0, height, 0.0);
	normal = Normal(0.0, 0.0, -1.0);
	Rectangle* back_wall_ptr = new Rectangle(p0, a, b, normal);
	back_wall_ptr->set_material(matte_ptr3);
	add_object(back_wall_ptr);


	// floor

	p0 = Point3D(0.0, 0.0, 0.0);
	a = Vector3D(0.0, 0.0, depth);
	b = Vector3D(width, 0.0, 0.0);
	normal = Normal(0.0, 1.0, 0.0);
	Rectangle* floor_ptr = new Rectangle(p0, a, b, normal);
	floor_ptr->set_material(matte_ptr3);
	add_object(floor_ptr);


	// ceiling

	p0 = Point3D(0.0, height, 0.0);
	a = Vector3D(0.0, 0.0, depth);
	b = Vector3D(width, 0.0, 0.0);
	normal = Normal(0.0, -1.0, 0.0);
	Rectangle* ceiling_ptr = new Rectangle(p0, a, b, normal);
	ceiling_ptr->set_material(matte_ptr3);
	add_object(ceiling_ptr);


	// the two boxes defined as 5 rectangles each

	// short box

	// top

	p0 = Point3D(13.0, 16.5, 6.5);
	a = Vector3D(-4.8, 0.0, 16.0);
	b = Vector3D(16.0, 0.0, 4.9);
	normal = Normal(0.0, 1.0, 0.0);
	Rectangle* short_top_ptr = new Rectangle(p0, a, b, normal);
	short_top_ptr->set_material(matte_ptr3);
	add_object(short_top_ptr);


	// side 1

	p0 = Point3D(13.0, 0.0, 6.5);
	a = Vector3D(-4.8, 0.0, 16.0);
	b = Vector3D(0.0, 16.5, 0.0);
	Rectangle* short_side_ptr1 = new Rectangle(p0, a, b);
	short_side_ptr1->set_material(matte_ptr3);
	add_object(short_side_ptr1);


	// side 2

	p0 = Point3D(8.2, 0.0, 22.5);
	a = Vector3D(15.8, 0.0, 4.7);
	Rectangle* short_side_ptr2 = new Rectangle(p0, a, b);
	short_side_ptr2->set_material(matte_ptr3);
	add_object(short_side_ptr2);


	// side 3

	p0 = Point3D(24.2, 0.0, 27.4);
	a = Vector3D(4.8, 0.0, -16.0);
	Rectangle* short_side_ptr3 = new Rectangle(p0, a, b);
	short_side_ptr3->set_material(matte_ptr3);
	add_object(short_side_ptr3);


	// side 4

	p0 = Point3D(29.0, 0.0, 11.4);
	a = Vector3D(-16.0, 0.0, -4.9);
	Rectangle* short_side_ptr4 = new Rectangle(p0, a, b);
	short_side_ptr4->set_material(matte_ptr3);
	add_object(short_side_ptr4);




	// tall box

	// top

	p0 = Point3D(42.3, 33.0, 24.7);
	a = Vector3D(-15.8, 0.0, 4.9);
	b = Vector3D(4.9, 0.0, 15.9);
	normal = Normal(0.0, 1.0, 0.0);
	Rectangle* tall_top_ptr = new Rectangle(p0, a, b, normal);
	tall_top_ptr->set_material(matte_ptr3);
	add_object(tall_top_ptr);


	// side 1

	p0 = Point3D(42.3, 0.0, 24.7);
	a = Vector3D(-15.8, 0.0, 4.9);
	b = Vector3D(0.0, 33.0, 0.0);
	Rectangle* tall_side_ptr1 = new Rectangle(p0, a, b);
	tall_side_ptr1->set_material(matte_ptr3);
	add_object(tall_side_ptr1);


	// side 2

	p0 = Point3D(26.5, 0.0, 29.6);
	a = Vector3D(4.9, 0.0, 15.9);
	Rectangle* tall_side_ptr2 = new Rectangle(p0, a, b);
	tall_side_ptr2->set_material(matte_ptr3);
	add_object(tall_side_ptr2);


	// side 3

	p0 = Point3D(31.4, 0.0, 45.5);
	a = Vector3D(15.8, 0.0, -4.9);
	Rectangle* tall_side_ptr3 = new Rectangle(p0, a, b);
	tall_side_ptr3->set_material(matte_ptr3);
	add_object(tall_side_ptr3);


	// side 4

	p0 = Point3D(47.2, 0.0, 40.6);
	a = Vector3D(-4.9, 0.0, -15.9);
	Rectangle* tall_side_ptr4 = new Rectangle(p0, a, b);
	tall_side_ptr4->set_material(matte_ptr3);
	add_object(tall_side_ptr4);
}

#endif // 0

#if 1 // global trace. caustic 1, reflective cylinder
void
World::build(void) {
	int num_samples = 100; // 5041

	vp.set_hres(400);
	vp.set_vres(400);
	vp.set_samples(num_samples);
	vp.set_max_depth(5);

//	tracer_ptr = new PathTrace(this);
	tracer_ptr = new GlobalTrace(this);

	Pinhole* pinhole_ptr = new Pinhole;
	pinhole_ptr->set_eye(5, 10, 20);
	pinhole_ptr->set_lookat(-1, 1, 0);
	pinhole_ptr->set_view_distance(500);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);


	// emissive sphere

	Emissive* emissive_ptr = new Emissive;
	emissive_ptr->scale_radiance(20.0); // 20
	emissive_ptr->set_ce(0.75, 1, 0.75);      // light green

	Sphere* sphere_ptr = new Sphere(Point3D(-2, 7, 6), 2);
	sphere_ptr->set_material(emissive_ptr);
	sphere_ptr->set_sampler(new MultiJittered(num_samples));
	add_object(sphere_ptr);


	AreaLight* area_light_ptr = new AreaLight;
	area_light_ptr->set_object(sphere_ptr);
	area_light_ptr->set_cast_shadow(true);
	add_light(area_light_ptr);


	// reflective rectangle

	Reflective* reflective_ptr = new Reflective;
	reflective_ptr->set_ka(0.0);
	reflective_ptr->set_kd(0.0);
	reflective_ptr->set_ks(0.0);
	reflective_ptr->set_exp(1.0);
	reflective_ptr->set_kr(0.95);
	reflective_ptr->set_cr(1.0, 0.2, 0.2);

	double bottom 	= -1.0;
	double top 		=  1.0;
	double radius	=  7.0;
	OpenCylinder* cylinder_ptr = new OpenCylinder(bottom, top, radius);
	cylinder_ptr->set_material(reflective_ptr);
	add_object(cylinder_ptr);


	// plane

	Matte* matte_ptr = new Matte;
	matte_ptr->set_ka(0);
	matte_ptr->set_kd(0.75);
	matte_ptr->set_cd(white);
	matte_ptr->set_sampler(new MultiJittered(num_samples));

	Plane* plane_ptr = new Plane(Point3D(0, -1.0, 0), Normal(0, 1, 0));
	plane_ptr->set_material(matte_ptr);
	add_object(plane_ptr);
}

#endif // 1


#elif ChapterNum == 27 // simple TRANSPARENT_H

#include "World.h"
#include "Ambient.h"
#include "Pinhole.h"
#include "Directional.h"
#include "RayCast.h"
#include "Matte.h"
#include "Phong.h"
#include "Plane.h"
#include "OpenCylinder.h"
#include "MultiJittered.h"
#include "AmbientOccluder.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "AreaLighting.h"
#include "Instance.h"
#include "Grid.h"
#include "PointLight.h"#include "Reflective.h"
#include "GlossyReflector.h"
#include "Whitted.h"
#include "PathTrace.h"
#include "GlobalTrace.h"
#include "Rectangle.h"
#include "Transparent.h"


void
World::build(void){
	int num_samples = 16;

	vp.set_hres(300);
	vp.set_vres(300);
	vp.set_samples(num_samples);
	vp.set_max_depth(4);			// for Figure 27.13(a)
//	vp.set_max_depth(4);			// for Figure 27.13(b)
//	vp.set_max_depth(6);			// for Figure 27.13(c)

	background_color = RGBColor(0.0, 0.3, 0.25);

	tracer_ptr = new Whitted(this);

	Ambient* ambient_ptr = new Ambient;
	ambient_ptr->scale_radiance(0.25);
	set_ambient_light(ambient_ptr);

	Pinhole* pinhole_ptr = new Pinhole;


	pinhole_ptr->set_eye(3.5, 5.5, 40);
	pinhole_ptr->set_lookat(3.5, 4, 0);
	pinhole_ptr->set_view_distance(2400.0);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);


	// point light

	PointLight* light_ptr1 = new PointLight;
	light_ptr1->set_location(30, 50, 10);
	light_ptr1->scale_radiance(4.5);
	light_ptr1->set_cast_shadow(true);
	add_light(light_ptr1);


	// point light

	PointLight* light_ptr2 = new PointLight;
	light_ptr2->set_location(-30, 50, 10);
	light_ptr2->scale_radiance(4.5);
	light_ptr2->set_cast_shadow(true);
	add_light(light_ptr2);


	// transparent sphere

	Transparent* glass_ptr = new Transparent;
	glass_ptr->set_ks(0.2);
	glass_ptr->set_exp(2000.0);
	glass_ptr->set_ior(0.8);
	glass_ptr->set_kr(0.1);
	glass_ptr->set_kt(0.9);

	Sphere* sphere_ptr1 = new Sphere(Point3D(3, 3.5, 0), 1);
	sphere_ptr1->set_material(glass_ptr);
	add_object(sphere_ptr1);


	// red sphere

	Reflective*	reflective_ptr = new Reflective;
	reflective_ptr->set_ka(0.3);
	reflective_ptr->set_kd(0.3);
	reflective_ptr->set_cd(red);
	reflective_ptr->set_ks(0.2);
	reflective_ptr->set_exp(2000.0);
	reflective_ptr->set_kr(0.25);

	Sphere* sphere_ptr2 = new Sphere(Point3D(4, 4, -6), 1.2);
	sphere_ptr2->set_material(reflective_ptr);
	add_object(sphere_ptr2);


	Phong* phong_ptr2 = new Phong;
	phong_ptr2->set_ka(0.4);
	phong_ptr2->set_kd(0.8);
	phong_ptr2->set_cd(0.5, 0.5, 1.0);
	phong_ptr2->set_ks(0.5);
//	phong_ptr2->set_cs(1.0, 1.0, 0.0);
	phong_ptr2->set_exp(50.0);


	// rectangle

	Rectangle* rectangle_ptr = new Rectangle(Point3D(-20, 2.3, -100), Vector3D(0, 0, 120), Vector3D(40, 0, 0));
	rectangle_ptr->set_material(phong_ptr2);
	add_object(rectangle_ptr);
}


#elif ChapterNum == 28 // realistic transparency

#include "World.h"
#include "Ambient.h"
#include "Pinhole.h"
#include "Directional.h"
#include "RayCast.h"
#include "Matte.h"
#include "Phong.h"
#include "Plane.h"
#include "OpenCylinder.h"
#include "MultiJittered.h"
#include "AmbientOccluder.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "AreaLighting.h"
#include "Instance.h"
#include "Grid.h"
#include "PointLight.h"#include "Reflective.h"
#include "GlossyReflector.h"
#include "Whitted.h"
#include "PathTrace.h"
#include "GlobalTrace.h"
#include "Rectangle.h"
#include "Transparent.h"
#include "Dielectric.h"
#include "SphereConcave.h"

#define TYPE 0
// TYPE = 0: sphere
// TYPE = 1: bunny
// TYPE = 2: horse
// TYPE = 3: concave sphere

void
World::build(void){
	int num_samples = 16;

	vp.set_hres(300);
	vp.set_vres(300);
	vp.set_samples(num_samples);
//	vp.set_max_depth(2);
	vp.set_max_depth(4);
//	vp.set_max_depth(8);
//	vp.set_max_depth(16);

	background_color = RGBColor(0.0, 0.3, 0.25);

	tracer_ptr = new Whitted(this);

	Ambient* ambient_ptr = new Ambient;
	ambient_ptr->scale_radiance(0.25);
	set_ambient_light(ambient_ptr);

	Pinhole* pinhole_ptr = new Pinhole;


	pinhole_ptr->set_eye(3.5, 5.5, 40);
	pinhole_ptr->set_lookat(3.5, 4, 0);
	pinhole_ptr->set_view_distance(2400.0);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);


	// point light

	PointLight* light_ptr1 = new PointLight;
	light_ptr1->set_location(30, 50, 10);
	light_ptr1->scale_radiance(4.5);
	light_ptr1->set_cast_shadow(true);
	add_light(light_ptr1);


	// point light

	PointLight* light_ptr2 = new PointLight;
	light_ptr2->set_location(-30, 50, 10);
	light_ptr2->scale_radiance(4.5);
	light_ptr2->set_cast_shadow(true);
	add_light(light_ptr2);


	// transparent sphere

	Dielectric* dielectric_ptr = new Dielectric;
	dielectric_ptr->set_ks(0.2);
	dielectric_ptr->set_exp(2000.0);
	dielectric_ptr->set_eta_in(1.6);
	dielectric_ptr->set_eta_out(1.0);
	dielectric_ptr->set_cf_in(1.0, 1.0, 0.0);
	dielectric_ptr->set_cf_out(1.0, 1.0, 1.0);

#if TYPE == 2
	char* fileName = "/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/Horse97K.ply";

	Grid* bunny_ptr = new Grid(new Mesh);
//	bunny_ptr->read_flat_triangles(fileName);		// read PLY file
	bunny_ptr->read_smooth_triangles(fileName);		// read PLY file
	bunny_ptr->setup_cells();

    Instance* instance_ptr = new Instance;
    instance_ptr->set_object(bunny_ptr);
    instance_ptr->set_material(dielectric_ptr);
    instance_ptr->scale(4.0);
    instance_ptr->translate(3.0, 3.5, 0.0);
	add_object(instance_ptr);
#endif // TYPE

#if TYPE == 1
//	char* fileName = "/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/Bunny4K.ply"; 		// 4000 triangles
	char* fileName = "/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/Bunny10K.ply"; 	    // 10000 triangles
//	char* fileName = "/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/Bunny16K.ply"; 	    // 16000 triangles
//	char* fileName = "/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/Bunny69K.ply"; 	    // 69000 triangles

	Grid* bunny_ptr = new Grid(new Mesh);
	bunny_ptr->reverse_mesh_normals();				// you must use this for the 10K model
//	bunny_ptr->read_flat_triangles(fileName);		// read PLY file
	bunny_ptr->read_smooth_triangles(fileName);		// read PLY file
	bunny_ptr->setup_cells();

    Instance* instance_ptr = new Instance;
    instance_ptr->set_object(bunny_ptr);
    instance_ptr->set_material(dielectric_ptr);
    instance_ptr->scale(16.0);
//    instance_ptr->rotate_y(45.0);
    instance_ptr->translate(3.0, 2.0, 0.0);
//    instance_ptr->compute_bounding_box();
	add_object(instance_ptr);
#endif // TYPE

#if TYPE == 0
	Sphere* sphere_ptr1 = new Sphere(Point3D(3, 3.5, 0), 1);
	sphere_ptr1->set_material(dielectric_ptr);
	add_object(sphere_ptr1);
#endif // TYPE

#if TYPE == 3
	SphereConcave* sphere_c_ptr1 = new SphereConcave(Point3D(3, 3.5, 0), 1);
	sphere_c_ptr1->set_material(dielectric_ptr);
	add_object(sphere_c_ptr1);
#endif // TYPE

	// red sphere

	Reflective*	reflective_ptr = new Reflective;
	reflective_ptr->set_ka(0.3);
	reflective_ptr->set_kd(0.3);
	reflective_ptr->set_cd(red);
	reflective_ptr->set_ks(0.2);
	reflective_ptr->set_exp(2000.0);
	reflective_ptr->set_kr(0.25);

	Sphere* sphere_ptr2 = new Sphere(Point3D(4, 4, -6), 1.2);
	sphere_ptr2->set_material(reflective_ptr);
	add_object(sphere_ptr2);


	Phong* phong_ptr2 = new Phong;
	phong_ptr2->set_ka(0.4);
	phong_ptr2->set_kd(0.8);
	phong_ptr2->set_cd(0.5, 0.5, 1.0);
	phong_ptr2->set_ks(0.5);
//	phong_ptr2->set_cs(1.0, 1.0, 0.0);
	phong_ptr2->set_exp(50.0);


	// rectangle

	Rectangle* rectangle_ptr = new Rectangle(Point3D(-20, 2.3, -100), Vector3D(0, 0, 120), Vector3D(40, 0, 0));
	rectangle_ptr->set_material(phong_ptr2);
	add_object(rectangle_ptr);
}


#elif ChapterNum == 282 // realistic transparency: a glass of water

#include "World.h"
#include "Ambient.h"
#include "Pinhole.h"
#include "Directional.h"
#include "RayCast.h"
#include "Matte.h"
#include "Phong.h"
#include "Plane.h"
#include "OpenCylinder.h"
#include "MultiJittered.h"
#include "AmbientOccluder.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "AreaLighting.h"
#include "Instance.h"
#include "Grid.h"
#include "PointLight.h"#include "Reflective.h"
#include "GlossyReflector.h"
#include "Whitted.h"
#include "PathTrace.h"
#include "GlobalTrace.h"
#include "Rectangle.h"
#include "Transparent.h"
#include "Dielectric.h"
#include "GlassOfWater.h"


void
World::build(void) {
	int num_samples = 9;

	vp.set_hres(600);
	vp.set_vres(600);
	vp.set_samples(num_samples);
	vp.set_max_depth(10);

	background_color = RGBColor(0.5);

	tracer_ptr = new Whitted(this);

	Ambient* ambient_ptr = new Ambient;
	ambient_ptr->scale_radiance(0.5);
	set_ambient_light(ambient_ptr);


	Pinhole* pinhole_ptr = new Pinhole;
	pinhole_ptr->set_eye(5, 6, 10);  // for a, c
//	pinhole_ptr->set_eye(2.5, 0, 5); // for b
	pinhole_ptr->set_lookat(0, 1, 0);
	pinhole_ptr->set_view_distance(2000.0); // for a, c
//	pinhole_ptr->set_view_distance(1000.0); // for b
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);


	PointLight* light_ptr1 = new PointLight;
	light_ptr1->set_location(40, 50, 30);
	light_ptr1->scale_radiance(3.0);
	light_ptr1->set_cast_shadow(true); // a: false; c: true
	add_light(light_ptr1);


	// materials for the glass of water

	// glass-air boundary

	RGBColor glass_color(0.65, 1, 0.75);
	RGBColor water_color(1, 0.25, 1);

	Dielectric* glass_ptr = new Dielectric;
	glass_ptr->set_eta_in(1.50);			// glass
	glass_ptr->set_eta_out(1.0);			// air
	glass_ptr->set_cf_in(glass_color);
	glass_ptr->set_cf_out(white);

	// water-air boundary

	Dielectric* water_ptr = new Dielectric;
	water_ptr->set_eta_in(1.33);			// water
	water_ptr->set_eta_out(1.0);			// air
	water_ptr->set_cf_in(water_color);
	water_ptr->set_cf_out(white);

	// water-glass boundary

	Dielectric* dielectric_ptr = new Dielectric;
	dielectric_ptr->set_eta_in(1.33); 		// water
	dielectric_ptr->set_eta_out(1.50); 		// glass
	dielectric_ptr->set_cf_in(water_color);
	dielectric_ptr->set_cf_out(glass_color);


	// Define the GlassOfWater object
	// The parameters below are the default values, but using the constructor that
	// takes these as arguments makes it easier to experiment with different values

	double height 			= 2.0;
	double inner_radius 	= 0.9;
	double wall_thickness 	= 0.1;
	double base_thickness 	= 0.3;
	double water_height 	= 1.5;
	double meniscus_radius 	= 0.1;

	GlassOfWater* glass_of_water_ptr = new GlassOfWater(height,
														inner_radius,
														wall_thickness,
														base_thickness,
														water_height,
														meniscus_radius);

	glass_of_water_ptr->set_glass_air_material(glass_ptr);
	glass_of_water_ptr->set_water_air_material(water_ptr);
	glass_of_water_ptr->set_water_glass_material(dielectric_ptr);
	add_object(glass_of_water_ptr);


	// define the straw

	Matte* matte_ptr = new Matte;
	matte_ptr->set_cd(1, 1, 0);
	matte_ptr->set_ka(0.25);
	matte_ptr->set_kd(0.65);
//	matte_ptr->set_shadows(false);  // there are no shadows cast on the straw

	Instance* straw_ptr = new Instance(new OpenCylinder(-1.2, 1.7, 0.05));
	straw_ptr->set_material(matte_ptr);
	straw_ptr->rotate_z(40);
	straw_ptr->translate(0, 1.25, 0);
	add_object(straw_ptr);

	// ground plane


	Phong* phong_ptr2 = new Phong;
	phong_ptr2->set_ka(0.4);
	phong_ptr2->set_kd(0.8);
	phong_ptr2->set_cd(0.5, 0.5, 1.0);
	phong_ptr2->set_ks(0.5);
//	phong_ptr2->set_cs(1.0, 1.0, 0.0);
	phong_ptr2->set_exp(50.0);

	Plane* plane_ptr = new Plane(Point3D(0, -0.01, 0), Normal(0, 1, 0));
	plane_ptr->set_material(phong_ptr2);
	add_object(plane_ptr);
}


#elif ChapterNum == 283 // realistic transparency: fish bowl

#include "World.h"
#include "Ambient.h"
#include "Pinhole.h"
#include "Directional.h"
#include "RayCast.h"
#include "Matte.h"
#include "Phong.h"
#include "Plane.h"
#include "OpenCylinder.h"
#include "MultiJittered.h"
#include "AmbientOccluder.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "AreaLighting.h"
#include "Instance.h"
#include "Grid.h"
#include "PointLight.h"#include "Reflective.h"
#include "GlossyReflector.h"
#include "Whitted.h"
#include "PathTrace.h"
#include "GlobalTrace.h"
#include "Rectangle.h"
#include "Transparent.h"
#include "Dielectric.h"
#include "FishBowl.h"


void
World::build(void) {
	int num_samples = 9;

	vp.set_hres(600);
	vp.set_vres(600);
	vp.set_samples(num_samples);
	vp.set_max_depth(10);

	tracer_ptr = new Whitted(this);

	background_color = RGBColor(0.75);

	Pinhole* pinhole_ptr = new Pinhole;
	pinhole_ptr->set_eye(4.5, 6, 4);
	pinhole_ptr->set_lookat(0.0);
	pinhole_ptr->set_view_distance(1800.0);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);


	PointLight* light_ptr1 = new PointLight;
	light_ptr1->set_location(40, 25, -10);
	light_ptr1->scale_radiance(5.0);
	light_ptr1->set_cast_shadow(false);
	add_light(light_ptr1);


	// fishbowl

	// glass-air interface

	float c = 2;
	RGBColor glass_color(0.27*c, 0.49*c, 0.42*c);
	RGBColor water_color(0.75, 1, 0.75);

	Dielectric* glass_ptr = new Dielectric;
	glass_ptr->set_ks(0.5);
	glass_ptr->set_exp(8000.0);
	glass_ptr->set_eta_in(1.50);			// glass
	glass_ptr->set_eta_out(1.0);			// air
	glass_ptr->set_cf_in(glass_color);
	glass_ptr->set_cf_out(white);

	// water-air interface

	Dielectric* water_ptr = new Dielectric;
	water_ptr->set_ks(0.5);
	water_ptr->set_exp(8000);
	water_ptr->set_eta_in(1.33);			// water
	water_ptr->set_eta_out(1.0);			// air
	water_ptr->set_cf_in(water_color);
	water_ptr->set_cf_out(white);

	// water-glass interface

	Dielectric* dielectric_ptr = new Dielectric;
	dielectric_ptr->set_ks(0.5);
	dielectric_ptr->set_exp(8000);
	dielectric_ptr->set_eta_in(1.33); 		// water
	dielectric_ptr->set_eta_out(1.5); 		// glass
	dielectric_ptr->set_cf_in(water_color);
	dielectric_ptr->set_cf_out(glass_color);

	// physical bowl parameters (also the defaults)

	double inner_radius		= 1.0;
	double glass_thickness	= 0.1;
	double water_depth		= 1.25;
	double meniscus_radius 	= 0.05;
	double opening_angle 	= 90.0;


	FishBowl* fishbowl_ptr = new FishBowl(	inner_radius,
											glass_thickness,
											water_depth,
											meniscus_radius,
											opening_angle);
	fishbowl_ptr->set_glass_air_material(glass_ptr);
	fishbowl_ptr->set_water_air_material(water_ptr);
	fishbowl_ptr->set_water_glass_material(dielectric_ptr);
	add_object(fishbowl_ptr);



	// goldfish

	Phong* phong_ptr = new Phong;
	phong_ptr->set_ka(0.4);
	phong_ptr->set_kd(0.8);
	phong_ptr->set_cd(1.0, 0.15, 0.0);   	// orange
	phong_ptr->set_ks(0.5);
//	phong_ptr->set_cs(1.0, 0.35, 0.0);		// orange
	phong_ptr->set_exp(50.0);
//	phong_ptr->set_shadows(false);


//	const char* file_name = "goldfish_low_res.ply";		// for scene design
	char* file_name = "/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/goldfish_high_res.ply";  // for production
	Grid* grid_ptr = new Grid(new Mesh);
//	grid_ptr->read_flat_triangles(file_name);
	grid_ptr->read_smooth_triangles(file_name);
	grid_ptr->set_material(phong_ptr);
	grid_ptr->setup_cells();

	Instance* gold_fish_ptr = new Instance(grid_ptr);
	gold_fish_ptr->scale(0.03);
	gold_fish_ptr->translate(0.5, 0.0, 0.0);
	add_object(gold_fish_ptr);


	// plane

	Phong* phong_ptr2 = new Phong;
	phong_ptr2->set_ka(0.4);
	phong_ptr2->set_kd(0.8);
	phong_ptr2->set_cd(0.8, 0.8, 0.8);
	phong_ptr2->set_ks(0.5);
//	phong_ptr2->set_cs(1.0, 1.0, 0.0);
	phong_ptr2->set_exp(50.0);

	Plane* plane_ptr = new Plane(Point3D(0, -1.51, 0), Normal(0, 1, 0));
	plane_ptr->set_material(phong_ptr2);
	add_object(plane_ptr);
}

#elif ChapterNum == 284 // realistic transparency: fish bowl flat bottom

#include "World.h"
#include "Ambient.h"
#include "Pinhole.h"
#include "Directional.h"
#include "RayCast.h"
#include "Matte.h"
#include "Phong.h"
#include "Plane.h"
#include "OpenCylinder.h"
#include "MultiJittered.h"
#include "AmbientOccluder.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "AreaLighting.h"
#include "Instance.h"
#include "Grid.h"
#include "PointLight.h"#include "Reflective.h"
#include "GlossyReflector.h"
#include "Whitted.h"
#include "PathTrace.h"
#include "GlobalTrace.h"
#include "Rectangle.h"
#include "Transparent.h"
#include "Dielectric.h"
#include "FishBowlFlatBottom.h"
#include "OpenCylinderConvex.h"


void
World::build(void) {
	int num_samples = 9;

	vp.set_hres(600);
	vp.set_vres(600);
	vp.set_samples(num_samples);
	vp.set_max_depth(10);

	tracer_ptr = new Whitted(this);

	background_color = RGBColor(0.9);

	Pinhole* pinhole_ptr = new Pinhole;
	pinhole_ptr->set_eye(-0.5, -1.0, 4);
	pinhole_ptr->set_lookat(0.0);
	pinhole_ptr->set_view_distance(900.0);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);


	PointLight* light_ptr1 = new PointLight;
	light_ptr1->set_location(40, 25, 10);
	light_ptr1->scale_radiance(2.5);
	light_ptr1->set_cast_shadow(false);
	add_light(light_ptr1);

	Directional* light_ptr2 = new Directional;
	light_ptr2->set_direction(-1, 0, 0);
	light_ptr2->scale_radiance(2.5);
	light_ptr2->set_cast_shadow(false);
	add_light(light_ptr2);


	// fishbowl

	// glass-air interface

	float c = 2;
	RGBColor glass_color(0.27*c, 0.49*c, 0.42*c);
	RGBColor water_color(0.75, 1, 0.75);

	Dielectric* glass_ptr = new Dielectric;
	glass_ptr->set_ks(0.5);
	glass_ptr->set_exp(8000.0);
	glass_ptr->set_eta_in(1.50);			// glass
	glass_ptr->set_eta_out(1.0);			// air
	glass_ptr->set_cf_in(glass_color);
	glass_ptr->set_cf_out(white);

	// water-air interface

	Dielectric* water_ptr = new Dielectric;
	water_ptr->set_ks(0.5);
	water_ptr->set_exp(8000);
	water_ptr->set_eta_in(1.33);			// water
	water_ptr->set_eta_out(1.0);			// air
	water_ptr->set_cf_in(water_color);
	water_ptr->set_cf_out(white);

	// water-glass interface

	Dielectric* dielectric_ptr1 = new Dielectric;
	dielectric_ptr1->set_ks(0.5);
	dielectric_ptr1->set_exp(8000);
	dielectric_ptr1->set_eta_in(1.33); 		// water
	dielectric_ptr1->set_eta_out(1.5); 		// glass
	dielectric_ptr1->set_cf_in(water_color);
	dielectric_ptr1->set_cf_out(glass_color);

	// physical bowl parameters (also the defaults)

	double inner_radius		= 1.0;
	double glass_thickness	= 0.1;
	double water_depth		= 1.25;
	double meniscus_radius 	= 0.05;
	double opening_angle 	= 90.0;
	double bottom_angle 	= 90.0;


	FishBowlFlatBottom* fishbowl_ptr = new FishBowlFlatBottom(	inner_radius,
                                                                glass_thickness,
                                                                water_depth,
                                                                meniscus_radius,
                                                                opening_angle,
                                                                bottom_angle);
	fishbowl_ptr->set_glass_air_material(glass_ptr);
	fishbowl_ptr->set_water_air_material(water_ptr);
	fishbowl_ptr->set_water_glass_material(dielectric_ptr1);
	add_object(fishbowl_ptr);



	// goldfish

	Phong* phong_ptr1 = new Phong;
	phong_ptr1->set_ka(0.4);
	phong_ptr1->set_kd(0.8);
	phong_ptr1->set_cd(1.0, 0.15, 0.0);   	// orange
	phong_ptr1->set_ks(0.5);
//	phong_ptr1->set_cs(1.0, 0.35, 0.0);		// orange
	phong_ptr1->set_exp(50.0);
//	phong_ptr1->set_shadows(false);


	// we read the fish file once, and instance it

	char* file_name = "/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/goldfish_high_res.ply";	// for production
	Grid* grid_ptr = new Grid(new Mesh);
//	grid_ptr->read_flat_triangles(file_name);
	grid_ptr->read_smooth_triangles(file_name);
	grid_ptr->set_material(phong_ptr1);
	grid_ptr->setup_cells();

	Instance* gold_fish_ptr1 = new Instance(grid_ptr);
	gold_fish_ptr1->scale(0.03);
	gold_fish_ptr1->rotate_y(-45);
	gold_fish_ptr1->translate(0.5, 0.0, 0.0);
	add_object(gold_fish_ptr1);

	Instance* goldfish_ptr2 = new Instance(grid_ptr);
	goldfish_ptr2->scale(0.02);
	goldfish_ptr2->rotate_y(90);
	goldfish_ptr2->translate(-0.75, 0.0, 0.0);
	goldfish_ptr2->rotate_y(-60);
	add_object(goldfish_ptr2);

	Instance* goldfish_ptr3 = new Instance(grid_ptr);
	goldfish_ptr3->scale(0.02);
	goldfish_ptr3->rotate_x(20);
	goldfish_ptr3->rotate_y(-45);
	goldfish_ptr3->translate(-0.1, -0.4, 0.0);
	add_object(goldfish_ptr3);


	// cylinder under the bowl

	Phong* phong_ptr2 = new Phong;
	phong_ptr2->set_ka(0.4);
	phong_ptr2->set_kd(0.8);
	phong_ptr2->set_cd(0.05);
	phong_ptr2->set_ks(0.2);
	phong_ptr2->set_exp(100.0);

	double bottom = -1.2;
	double radius = 0.5;
	double top = -sqrt(1.1 * 1.1 - radius * radius);

	OpenCylinderConvex* cylinder_ptr = new OpenCylinderConvex(bottom, top, radius);
	cylinder_ptr->set_material(phong_ptr2);
	add_object(cylinder_ptr);


	// single air bubble

	Dielectric* dielectric_ptr2 = new Dielectric;
	dielectric_ptr2->set_eta_in(1.0); 		// air
	dielectric_ptr2->set_eta_out(1.33); 	// water
	dielectric_ptr2->set_cf_in(white);
	dielectric_ptr2->set_cf_out(water_color);

	Sphere* bubble_ptr = new Sphere(Point3D(0.2, 0.2, 0.2), 0.05);
	bubble_ptr->set_material(dielectric_ptr2);
	add_object(bubble_ptr);


	// streams of air bubbles

	set_rand_seed(1000);

	double bubble_radius		= 0.045;
	double yc_bottom 			= -0.9;    			// height of bottom bubble center
	double yc_top 				= 0.2;    			// height of top bubble center
	double num_bubbles 			= 8;				// number of bubbles in stream
	double spacing 				= (yc_top - yc_bottom) / num_bubbles; // vertical spacing between bubble centers
	double translation_factor 	= bubble_radius / 2.0;
	double min 					= 0.9;   			// minimum bubble scaling
	double max 					= 1.1;				// maximum bubble scaling
	double xc 					= -0.1;   			// center x
	double zc 					= 0.3;				// center y



	// bubble stream 1

	Grid* bubble_stream_ptr_1 = new Grid;

	for (int j = 0; j <= num_bubbles; j++) {
		Instance* bubble_ptr = new Instance(new Sphere);

		bubble_ptr->scale(	min + rand_float() * (max - min),
							min + rand_float() * (max - min),
							min + rand_float() * (max - min));

		bubble_ptr->scale(bubble_radius);

		bubble_ptr->rotate_x(360.0 * rand_float());
		bubble_ptr->rotate_y(360.0 * rand_float());
		bubble_ptr->rotate_z(360.0 * rand_float());

		bubble_ptr->translate(	xc + (2.0 * rand_float() - 1.0) * translation_factor,
								yc_bottom + j * spacing + (2.0 * rand_float() - 1.0) * translation_factor,
								zc + (2.0 * rand_float() - 1.0) * translation_factor);

		bubble_ptr->set_material(dielectric_ptr2);
		bubble_stream_ptr_1->add_object(bubble_ptr);
	}

	bubble_stream_ptr_1->setup_cells();
	add_object(bubble_stream_ptr_1);



	// bubble stream 2

	num_bubbles = 7;
	xc = 0.075;
	zc = 0.1;

	Grid* bubble_stream_ptr_2 = new Grid;

	for (int j = 0; j <= num_bubbles; j++) {
		Instance* bubble_ptr = new Instance(new Sphere);

		bubble_ptr->scale(	min + rand_float() * (max - min),
							min + rand_float() * (max - min),
							min + rand_float() * (max - min));

		bubble_ptr->scale(bubble_radius);

		bubble_ptr->rotate_x(360.0 * rand_float());
		bubble_ptr->rotate_y(360.0 * rand_float());
		bubble_ptr->rotate_z(360.0 * rand_float());

		bubble_ptr->translate(	xc + (2.0 * rand_float() - 1.0) * translation_factor,
								yc_bottom + j * spacing + (2.0 * rand_float() - 1.0) * translation_factor,
								zc + (2.0 * rand_float() - 1.0) * translation_factor);

		bubble_ptr->set_material(dielectric_ptr2);
		bubble_stream_ptr_2->add_object(bubble_ptr);
	}

	bubble_stream_ptr_2->setup_cells();
	add_object(bubble_stream_ptr_2);



	// bubble stream 3

	num_bubbles = 9;
	xc = -0.15;
	zc = -0.3;

	Grid* bubble_stream_ptr_3 = new Grid;

	for (int j = 0; j <= num_bubbles; j++) {
		Instance* bubble_ptr = new Instance(new Sphere);

		bubble_ptr->scale(	min + rand_float() * (max - min),
							min + rand_float() * (max - min),
							min + rand_float() * (max - min));

		bubble_ptr->scale(bubble_radius);

		bubble_ptr->rotate_x(360.0 * rand_float());
		bubble_ptr->rotate_y(360.0 * rand_float());
		bubble_ptr->rotate_z(360.0 * rand_float());

		bubble_ptr->translate(	xc + (2.0 * rand_float() - 1.0) * translation_factor,
								yc_bottom + j * spacing + (2.0 * rand_float() - 1.0) * translation_factor,
								zc + (2.0 * rand_float() - 1.0) * translation_factor);

		bubble_ptr->set_material(dielectric_ptr2);
		bubble_stream_ptr_3->add_object(bubble_ptr);
	}

	bubble_stream_ptr_3->setup_cells();
	add_object(bubble_stream_ptr_3);




	// plane

	Phong* phong_ptr3 = new Phong;
	phong_ptr3->set_ka(0.4);
	phong_ptr3->set_kd(0.8);
	phong_ptr3->set_cd(1.0, 0.5, 0.5);
	phong_ptr3->set_ks(0.5);
//	phong_ptr3->set_cs(1.0, 1.0, 0.0);
	phong_ptr3->set_exp(50.0);

	Plane* plane_ptr = new Plane(Point3D(0, -1.2, 0), Normal(0, 1, 0));
	plane_ptr->set_material(phong_ptr3);

	Instance* plane_ptr2 = new Instance(plane_ptr); // to adjust the reflection of the grid lines off the top of the water
	plane_ptr2->rotate_y(30);
	plane_ptr2->translate(0.25, 0, 0.15);
	add_object(plane_ptr2);
}


#elif ChapterNum == 291 // texture mapping: constant color, image texture for sphere and cylinder

#include "World.h"
#include "Ambient.h"
#include "Pinhole.h"
#include "Directional.h"
#include "PointLight.h"
#include "RayCast.h"
#include "Matte.h"
#include "Plane.h"
#include "Phong.h"
#include "MultiJittered.h"
#include "AmbientOccluder.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "Rectangle.h"
#include "AreaLighting.h"
#include "Instance.h"
#include "Disk.h"
#include "Annulus.h"
#include "OpenCylinder.h"
#include "OpenCylinderConvex.h"
#include "OpenCylinderConcave.h"
#include "Torus.h"
#include "TorusPart.h"
#include "TorusPartConcave.h"
#include "ConstantColor.h"
#include "SV_Matte.h"
#include "Instance.h"
#include "Image.h"
#include "SphericalMap.h"
#include "ImageTexture.h"

#define TYPE 1
// TYPE = 0: constant color
// TYPE = 1: image texture

void
World::build(void){
	int num_samples = 16;

	vp.set_hres(300);
	vp.set_vres(300);
	vp.set_samples(num_samples);

	tracer_ptr = new RayCast(this);

	Pinhole* pinhole_ptr = new Pinhole;

	pinhole_ptr->set_eye(0, 10, 20);
	pinhole_ptr->set_lookat(0, 0, 0);
	pinhole_ptr->set_view_distance(1000); // 500
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

	Directional* light_ptr = new Directional;
	light_ptr->set_direction(-10, 20, 20);
	light_ptr->scale_radiance(3.0);
	light_ptr->set_cast_shadow(false);
	add_light(light_ptr);

#if TYPE == 0 // constant color
	ConstantColor* texture_ptr = new ConstantColor;
	texture_ptr->set_color(0.0, 1.0, 0.0);
#endif // TYPE




#if TYPE == 1 // image texture
	// image:

	Image* image_ptr = new Image;
//	image_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/EarthHighRes.ppm");
//	image_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/BilliardBall.ppm");
//	image_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/SphereGrid.ppm");
	image_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/CountryScene.ppm"); // for cylinder


	// mapping:

//	SphericalMap* map_ptr = new SphericalMap;


	// image based texture:

	ImageTexture* texture_ptr = new ImageTexture;
	texture_ptr->set_image(image_ptr);
//	texture_ptr->set_mapping(map_ptr);
#endif // TYPE


	// textured material:

    SV_Matte* matte_ptr = new SV_Matte;
    matte_ptr->set_ka(0.25);
    matte_ptr->set_kd(0.75);
    matte_ptr->set_cd(texture_ptr);


	// generic sphere:

//	Sphere* sphere_ptr = new Sphere();
    OpenCylinder* open_cylinder_ptr = new OpenCylinder();

	// affine transformed sphere

	Instance* earth_ptr = new Instance(open_cylinder_ptr);
    earth_ptr->scale(2.5);      // sphere: 3
//	earth_ptr->rotate_y(60);    // for earth
//	earth_ptr->rotate_y(-60);   // for billiard ball
	earth_ptr->rotate_y(0);     // for sphere grid
    earth_ptr->set_material(matte_ptr);
	add_object(earth_ptr);

}

#elif ChapterNum == 292

#include "World.h"
#include "Pinhole.h"
#include "Directional.h"
#include "PointLight.h"
#include "RayCast.h"
#include "Matte.h"
#include "Plane.h"
#include "Phong.h"
#include "MultiJittered.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "Rectangle.h"
#include "AreaLighting.h"
#include "Instance.h"
#include "SV_Matte.h"
#include "Instance.h"
#include "Image.h"
#include "ImageTexture.h"
#include "Grid.h"
#include "ConstantColor.h"

#define TYPE 0
// TYPE = 0 : two triangles

#if TYPE == 0
void
World::build(void) {
	int num_samples = 16;

	vp.set_hres(400);
	vp.set_vres(400);
	vp.set_samples(num_samples);

	tracer_ptr = new RayCast(this);

	background_color = black;

	Pinhole* pinhole_ptr = new Pinhole;
	pinhole_ptr->set_eye(11, 5, 9);
	pinhole_ptr->set_view_distance(1600.0);
	pinhole_ptr->set_lookat(0, -0.5, 0);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);


	Directional* directional_ptr = new Directional;
	directional_ptr->set_direction(0.75, 1, -0.15);
	directional_ptr->scale_radiance(4.5);
//	directional_ptr->set_shadows(true);
	add_light(directional_ptr);

	Image* image_ptr = new Image;
	image_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/BlueGlass.ppm");

	ImageTexture* texture_ptr = new ImageTexture;
	texture_ptr->set_image(image_ptr);

	ConstantColor* texture_ptr_c = new ConstantColor;
	texture_ptr_c->set_color(0.0, 1.0, 0.0);

	SV_Matte* sv_matte_ptr = new SV_Matte;
	sv_matte_ptr->set_ka(0.1);
	sv_matte_ptr->set_kd(0.75);
	sv_matte_ptr->set_cd(texture_ptr_c);

	char* file_name = "/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/TwoTriangles.ply";
	Grid* grid_ptr = new Grid(new Mesh);
//	grid_ptr->read_flat_uv_triangles(file_name);		// for flat shading
	grid_ptr->read_smooth_uv_triangles(file_name);		// for smooth shading
	grid_ptr->set_material(sv_matte_ptr);
	grid_ptr->setup_cells();
	add_object(grid_ptr);


	Matte* matte_ptr = new Matte;
	matte_ptr->set_cd(1, 0.9, 0.6);
	matte_ptr->set_ka(0.25);
	matte_ptr->set_kd(0.4);

	Plane* plane_ptr1 = new Plane(Point3D(0, -2.0, 0), Normal(0, 1, 0));
	plane_ptr1->set_material(matte_ptr);
	add_object(plane_ptr1);
}
#endif // TYPE


#elif ChapterNum == 300 // procedural textures, 3D checker

#include "World.h"
#include "Ambient.h"
#include "Pinhole.h"
#include "Directional.h"
#include "PointLight.h"
#include "RayCast.h"
#include "Matte.h"
#include "Plane.h"
#include "Phong.h"
#include "MultiJittered.h"
#include "AmbientOccluder.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "Rectangle.h"
#include "AreaLighting.h"
#include "Instance.h"
#include "Disk.h"
#include "Annulus.h"
#include "OpenCylinder.h"
#include "OpenCylinderConvex.h"
#include "OpenCylinderConcave.h"
#include "Torus.h"
#include "TorusPart.h"
#include "TorusPartConcave.h"
#include "SV_Matte.h"
#include "Instance.h"
#include "Checker3D.h"
#include "SolidCylinder.h"

#define TYPE 0
// TYPE = 0: sphere
// TYPE = 1:

void
World::build(void){
	int num_samples = 16;

	vp.set_hres(300);
	vp.set_vres(300);
	vp.set_samples(num_samples);

	tracer_ptr = new RayCast(this);

	Pinhole* pinhole_ptr = new Pinhole;

	pinhole_ptr->set_eye(0, 10, 20);
	pinhole_ptr->set_lookat(0, 0, 0);
	pinhole_ptr->set_view_distance(1000); // 500
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

	Directional* light_ptr = new Directional;
	light_ptr->set_direction(-10, 20, 20);
	light_ptr->scale_radiance(3.0);
	light_ptr->set_cast_shadow(false);
	add_light(light_ptr);


    //------------------------------Checker3D

	Checker3D* texture_ptr = new Checker3D();
	texture_ptr->set_size(0.2);
    texture_ptr->set_color1(1.0, 0.0, 0.0);
    texture_ptr->set_color2(0.0, 1.0, 0.0);


	// textured material:

    SV_Matte* matte_ptr = new SV_Matte;
    matte_ptr->set_ka(0.25);
    matte_ptr->set_kd(0.75);
    matte_ptr->set_cd(texture_ptr);


	// generic object:

	Sphere* object_ptr = new Sphere();
//	SolidCylinder* object_ptr = new SolidCylinder();
//	Plane* object_ptr = new Plane();


	// affine transformed object

	Instance* instance_ptr = new Instance(object_ptr);
    instance_ptr->scale(1);
//	instance_ptr->rotate_y(0);
    instance_ptr->set_material(matte_ptr);
	add_object(instance_ptr);
}

#elif ChapterNum == 301 // procedural textures, 2D checker

#include "World.h"
#include "Ambient.h"
#include "Pinhole.h"
#include "Directional.h"
#include "PointLight.h"
#include "RayCast.h"
#include "Matte.h"
#include "Plane.h"
#include "Phong.h"
#include "MultiJittered.h"
#include "AmbientOccluder.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "Rectangle.h"
#include "AreaLighting.h"
#include "Instance.h"
#include "Disk.h"
#include "Annulus.h"
#include "OpenCylinder.h"
#include "OpenCylinderConvex.h"
#include "OpenCylinderConcave.h"
#include "Torus.h"
#include "TorusPart.h"
#include "TorusPartConcave.h"
#include "SV_Matte.h"
#include "Instance.h"
#include "Image.h"
#include "SphericalMap.h"
#include "ImageTexture.h"
#include "SphereChecker.h"
#include "PlaneChecker.h"
#include "SolidCylinderChecker.h"
#include "SolidCylinder.h"

#define TYPE 0
// TYPE = 0: sphere
// TYPE = 1:

void
World::build(void){
	int num_samples = 16;

	vp.set_hres(300);
	vp.set_vres(300);
	vp.set_samples(num_samples);

	tracer_ptr = new RayCast(this);

	Pinhole* pinhole_ptr = new Pinhole;

	pinhole_ptr->set_eye(0, 10, 20);
	pinhole_ptr->set_lookat(0, 0, 0);
	pinhole_ptr->set_view_distance(1000); // 500
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

	Directional* light_ptr = new Directional;
	light_ptr->set_direction(-10, 20, 20);
	light_ptr->scale_radiance(3.0);
	light_ptr->set_cast_shadow(false);
	add_light(light_ptr);


	// SphereChecker:
	SphereChecker* texture_ptr = new SphereChecker();
	texture_ptr->set_num_horizontal_checkers(20);
	texture_ptr->set_num_vertical_checkers(10);
	texture_ptr->set_horizontal_line_width(0.05);
	texture_ptr->set_vertical_line_width(0.05);
    texture_ptr->set_color1(1.0, 0.0, 0.0);
    texture_ptr->set_color2(0.0, 1.0, 0.0);
    texture_ptr->set_line_color(1.0, 1.0, 1.0);


	// textured material:
    SV_Matte* matte_ptr = new SV_Matte;
    matte_ptr->set_ka(0.25);
    matte_ptr->set_kd(0.75);
    matte_ptr->set_cd(texture_ptr);


	// generic sphere:
	Sphere* sphere_ptr = new Sphere();


	// affine transformed sphere
	Instance* earth_ptr = new Instance(sphere_ptr);
    earth_ptr->scale(2);
	earth_ptr->rotate_y(0);
    earth_ptr->set_material(matte_ptr);
//	add_object(earth_ptr);


	// SolidCylinderChecker:
	SolidCylinderChecker* texture_ptr3 = new SolidCylinderChecker();
	texture_ptr3->set_num_horizontal_checkers(20);
	texture_ptr3->set_num_vertical_checkers(10);
	texture_ptr3->set_num_radius_checkers_disk(3);
	texture_ptr3->set_horizontal_line_width(0.05);
	texture_ptr3->set_vertical_line_width(0.02);
    texture_ptr3->set_color1(1.0, 0.0, 0.0);
    texture_ptr3->set_color2(0.0, 1.0, 0.0);
    texture_ptr3->set_line_color(1.0, 1.0, 1.0);


	// textured material:
    SV_Matte* matte_ptr3 = new SV_Matte;
    matte_ptr3->set_ka(0.25);
    matte_ptr3->set_kd(0.75);
    matte_ptr3->set_cd(texture_ptr3);


	// generic sphere:
	SolidCylinder* solid_cylinder_ptr = new SolidCylinder();


	// affine transformed sphere
	Instance* sc_ptr = new Instance(solid_cylinder_ptr);
    sc_ptr->scale(2);
	sc_ptr->rotate_y(0);
    sc_ptr->set_material(matte_ptr3);
	add_object(sc_ptr);


//-----------------------------------------------------------PlaneChecker
	PlaneChecker* texture_ptr2 = new PlaneChecker();
	texture_ptr2->set_size(0.5);
	texture_ptr2->set_line_width(0.05);
    texture_ptr2->set_color1(1.0, 0.0, 0.0);
    texture_ptr2->set_color2(0.0, 1.0, 0.0);
    texture_ptr2->set_line_color(1.0, 1.0, 1.0);


	// textured material:

    SV_Matte* matte_ptr2 = new SV_Matte;
    matte_ptr2->set_ka(0.25);
    matte_ptr2->set_kd(0.75);
    matte_ptr2->set_cd(texture_ptr2);

	Plane* plane_ptr1 = new Plane(Point3D(0, -3.5, 0), Normal(0, 1, 0));
	plane_ptr1->set_material(matte_ptr2);
//	add_object(plane_ptr1);

}


#elif ChapterNum == 302 // procedural textures, transformation

#include "World.h"
#include "Ambient.h"
#include "Pinhole.h"
#include "Directional.h"
#include "PointLight.h"
#include "RayCast.h"
#include "Matte.h"
#include "Plane.h"
#include "Phong.h"
#include "MultiJittered.h"
#include "AmbientOccluder.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "Rectangle.h"
#include "AreaLighting.h"
#include "Instance.h"
#include "Disk.h"
#include "Annulus.h"
#include "OpenCylinder.h"
#include "OpenCylinderConvex.h"
#include "OpenCylinderConcave.h"
#include "Torus.h"
#include "TorusPart.h"
#include "TorusPartConcave.h"
#include "SV_Matte.h"
#include "Instance.h"
#include "Checker3D.h"
#include "SolidCylinder.h"
#include "InstanceTexture.h"

#define TYPE 0
// TYPE = 0: sphere
// TYPE = 1:

void
World::build(void){
	int num_samples = 16;

	vp.set_hres(300);
	vp.set_vres(300);
	vp.set_samples(num_samples);

	tracer_ptr = new RayCast(this);

	Pinhole* pinhole_ptr = new Pinhole;

	pinhole_ptr->set_eye(0, 0, 30);
	pinhole_ptr->set_lookat(0, 0, 0);
	pinhole_ptr->set_view_distance(1000); // 500
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

	Directional* light_ptr = new Directional;
	light_ptr->set_direction(-10, 20, 20);
	light_ptr->scale_radiance(3.0);
	light_ptr->set_cast_shadow(false);
	add_light(light_ptr);


    //------------------------------Checker3D

	Checker3D* texture_ptr = new Checker3D();
	texture_ptr->set_size(0.8);
    texture_ptr->set_color1(1.0, 0.0, 0.0);
    texture_ptr->set_color2(0.0, 1.0, 0.0);

    InstanceTexture* it_ptr = new InstanceTexture(texture_ptr);
    it_ptr->scale(1.0/6);

	// textured material:

    SV_Matte* matte_ptr = new SV_Matte;
    matte_ptr->set_ka(0.25);
    matte_ptr->set_kd(0.75);
    matte_ptr->set_cd(it_ptr);


	// generic object:

	Sphere* object_ptr = new Sphere(Point3D(0, 0, 0), 0.5);


	// affine transformed object

	Instance* instance_ptr = new Instance(object_ptr);
    instance_ptr->scale(1);
//	instance_ptr->rotate_y(0);
    instance_ptr->set_material(matte_ptr);
	add_object(instance_ptr);
}

#elif ChapterNum == 31 // noise-based texture: basic and wrapped

#include "World.h"
#include "Ambient.h"
#include "Pinhole.h"
#include "Directional.h"
#include "PointLight.h"
#include "RayCast.h"
#include "Matte.h"
#include "Plane.h"
#include "Phong.h"
#include "MultiJittered.h"
#include "AmbientOccluder.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "Rectangle.h"
#include "AreaLighting.h"
#include "Instance.h"
#include "Disk.h"
#include "Annulus.h"
#include "OpenCylinder.h"
#include "OpenCylinderConvex.h"
#include "OpenCylinderConcave.h"
#include "Torus.h"
#include "TorusPart.h"
#include "TorusPartConcave.h"
#include "SV_Matte.h"
#include "Instance.h"
#include "Checker3D.h"
#include "SolidCylinder.h"
#include "CubicNoise.h"
#include "FBmTexture.h"
#include "TurbulenceTexture.h"
#include "FBmTextureWrapped.h"

#define TYPE 2
// TYPE = 0: FBmTexture
// TYPE = 1: TurbulenceTexture
// TYPE = 2: FBmTextureWrapped

void
World::build(void) {
	int num_samples = 16;
	vp.set_hres(300);
	vp.set_vres(300);
	vp.set_samples(num_samples);
	vp.set_gamut_display(true);

	background_color = RGBColor(0.5);
	tracer_ptr = new RayCast(this);

	Pinhole* pinhole_ptr = new Pinhole;

	pinhole_ptr->set_eye(0, 0, 100);
	pinhole_ptr->set_lookat(0.0);
	pinhole_ptr->set_view_distance(4750.0);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);


	PointLight* light_ptr = new PointLight;
	light_ptr->set_location(10, 10, 20);
	light_ptr->scale_radiance(2.5);
	add_light(light_ptr);


	// noise:

	CubicNoise* noise_ptr = new CubicNoise;
	noise_ptr->set_num_octaves(8);
	noise_ptr->set_gain(0.5);			// not relevant when num_octaves = 1
	noise_ptr->set_lacunarity(2.0);     // not relevant when num_octaves = 1

	// texture:

#if TYPE == 0
	FBmTexture* texture_ptr = new FBmTexture(noise_ptr);
#endif // TYPE
#if TYPE == 1
	TurbulenceTexture* texture_ptr = new TurbulenceTexture(noise_ptr);
#endif // TYPE
#if TYPE == 2
	FBmTextureWrapped* texture_ptr = new FBmTextureWrapped(noise_ptr);
	texture_ptr->set_expansion_number(8.0);
#endif // TYPE
    texture_ptr->set_color(0.7, 1.0, 0.5);   // light green
	texture_ptr->set_min_value(0.0);
	texture_ptr->set_max_value(1.0);

	// material:

	SV_Matte* sv_matte_ptr = new SV_Matte;
	sv_matte_ptr->set_ka(0.25);
	sv_matte_ptr->set_kd(0.85);
	sv_matte_ptr->set_cd(texture_ptr);

	// the sphere:

	Sphere* sphere_ptr = new Sphere(Point3D(0.0), 3.0);
	sphere_ptr->set_material(sv_matte_ptr);
	add_object(sphere_ptr);
}



#elif ChapterNum == 312 // noise-based texture: marble

#include "World.h"
#include "Ambient.h"
#include "Pinhole.h"
#include "Directional.h"
#include "PointLight.h"
#include "RayCast.h"
#include "Matte.h"
#include "Plane.h"
#include "Phong.h"
#include "MultiJittered.h"
#include "AmbientOccluder.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "Rectangle.h"
#include "AreaLighting.h"
#include "Instance.h"
#include "Disk.h"
#include "Annulus.h"
#include "OpenCylinder.h"
#include "OpenCylinderConvex.h"
#include "OpenCylinderConcave.h"
#include "Torus.h"
#include "TorusPart.h"
#include "TorusPartConcave.h"
#include "SV_Matte.h"
#include "Instance.h"
#include "Checker3D.h"
#include "SolidCylinder.h"
#include "CubicNoise.h"
#include "FBmTexture.h"
#include "TurbulenceTexture.h"
#include "FBmTextureWrapped.h"
#include "Image.h"
#include "FBmTextureRamp.h"
#include "InstanceTexture.h"

#define TYPE 0
// TYPE = 0: FBmTexture
// TYPE = 1: TurbulenceTexture
// TYPE = 2: FBmTextureWrapped

void
World::build(void) {
	int num_samples = 16;
	vp.set_hres(300);
	vp.set_vres(300);
	vp.set_samples(num_samples);
	vp.set_gamut_display(true);

	background_color = RGBColor(0.5);
	tracer_ptr = new RayCast(this);

	Pinhole* pinhole_ptr = new Pinhole;

	pinhole_ptr->set_eye(0, 20, 100); // sphere: (0, 0, 100)
	pinhole_ptr->set_lookat(0.0);
	pinhole_ptr->set_view_distance(4750.0);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);


	PointLight* light_ptr = new PointLight;
	light_ptr->set_location(10, 10, 20);
	light_ptr->scale_radiance(2.5);
	add_light(light_ptr);


	// noise:

	CubicNoise* noise_ptr = new CubicNoise;
	noise_ptr->set_num_octaves(6);
	noise_ptr->set_gain(0.5);			// not relevant when num_octaves = 1
	noise_ptr->set_lacunarity(2.0);     // not relevant when num_octaves = 1

	// ramp image:

	Image* image_ptr = new Image;
//	image_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/BlueMarbleRamp.ppm");
	image_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/sandstone_ramp4.ppm");

	// marble texture:

	FBmTextureRamp* marble_ptr = new FBmTextureRamp(image_ptr);
	marble_ptr->set_noise(noise_ptr);
	marble_ptr->set_perturbation(0.1);

	InstanceTexture* it_ptr = new InstanceTexture(marble_ptr);
//	it_ptr->scale(0.1);
//	it_ptr->rotate_x(-60);
//	it_ptr->rotate_y(-60);
	it_ptr->rotate_z(110);
	it_ptr->translate(1.0, 4.0, 0.0);

	// material:

	SV_Matte* sv_matte_ptr = new SV_Matte;
	sv_matte_ptr->set_ka(0.25);
	sv_matte_ptr->set_kd(0.85);
	sv_matte_ptr->set_cd(it_ptr);

	// the sphere:

//	Sphere* sphere_ptr = new Sphere(Point3D(0.0), 3.0);
	SolidCylinder* sphere_ptr = new SolidCylinder();

    Instance* instance_ptr = new Instance(sphere_ptr);
    instance_ptr->scale(2.0);
//	instance_ptr->rotate_x(-60);
//	instance_ptr->rotate_y(-30);
    instance_ptr->set_material(sv_matte_ptr);
    add_object(instance_ptr);
}

#elif ChapterNum == 313 // noise-based texture: wood

#include "World.h"
#include "Ambient.h"
#include "Pinhole.h"
#include "Directional.h"
#include "PointLight.h"
#include "RayCast.h"
#include "Matte.h"
#include "Plane.h"
#include "Phong.h"
#include "MultiJittered.h"
#include "AmbientOccluder.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "Rectangle.h"
#include "AreaLighting.h"
#include "Instance.h"
#include "Disk.h"
#include "Annulus.h"
#include "OpenCylinder.h"
#include "OpenCylinderConvex.h"
#include "OpenCylinderConcave.h"
#include "Torus.h"
#include "TorusPart.h"
#include "TorusPartConcave.h"
#include "SV_Matte.h"
#include "Instance.h"
#include "Checker3D.h"
#include "SolidCylinder.h"
#include "CubicNoise.h"
#include "FBmTexture.h"
#include "TurbulenceTexture.h"
#include "FBmTextureWrapped.h"
#include "Image.h"
#include "FBmTextureRamp.h"
#include "InstanceTexture.h"
#include "Wood.h"

#define TYPE 0
// TYPE = 0: (0,1,0)
// TYPE = 1: (0,0,1)

void
World::build(void) {
	int num_samples = 9;

	vp.set_hres(300);
	vp.set_vres(300);
	vp.set_samples(num_samples);

	tracer_ptr = new RayCast(this);
	background_color = black;

	Ambient* ambient_ptr = new Ambient;
	ambient_ptr->scale_radiance(1.0);
	set_ambient_light(ambient_ptr);


	Pinhole* pinhole_ptr = new Pinhole;
#if TYPE == 0
	pinhole_ptr->set_eye(0, 100, 0);
#else
	pinhole_ptr->set_eye(0, 0, 100);
#endif // TYPE
	pinhole_ptr->set_lookat(0.0);
	pinhole_ptr->set_view_distance(15000.0);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);


	Directional* light_ptr = new Directional;
#if TYPE == 0
	light_ptr->set_direction(0, 1, 0);
#else
	light_ptr->set_direction(0, 0, 1);
#endif // TYPE
	light_ptr->scale_radiance(4.0);
	add_light(light_ptr);


	// wood texture

	// This Wood constructor just specifies the light and dark colors.
	// The other parameters are defaults.

	RGBColor light_color(0.5, 0.2, 0.065);
	RGBColor dark_color(0.05);

	Wood* wood_ptr = new Wood(light_color, dark_color);
	wood_ptr->set_grainy(1.0);
	wood_ptr->set_ringy(1.0);

	InstanceTexture* transformed_wood_ptr = new InstanceTexture(wood_ptr);
	transformed_wood_ptr->scale(0.4);
//	transformed_wood_ptr->rotate_x(5); // for figure 3


	// material

	SV_Matte* sv_matte_ptr = new SV_Matte;
	sv_matte_ptr->set_ka(0.75);
	sv_matte_ptr->set_kd(0.85);
	sv_matte_ptr->set_cd(transformed_wood_ptr);


	// plane

	Point3D p0(0.0, 0.0, 0.0);

#if TYPE == 0
	Plane* plane_ptr = new Plane(p0, Normal(0, 1, 0));
#else
	Plane* plane_ptr = new Plane(p0, Normal(0, 0, 1));
#endif // TYPE
	plane_ptr->set_material(sv_matte_ptr);
	add_object(plane_ptr);
}

#elif ChapterNum == 314 // noise-based texture: test wood

#include "World.h"
#include "Ambient.h"
#include "Pinhole.h"
#include "Directional.h"
#include "PointLight.h"
#include "RayCast.h"
#include "Matte.h"
#include "Plane.h"
#include "Phong.h"
#include "MultiJittered.h"
#include "AmbientOccluder.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "Rectangle.h"
#include "AreaLighting.h"
#include "Instance.h"
#include "Disk.h"
#include "Annulus.h"
#include "OpenCylinder.h"
#include "OpenCylinderConvex.h"
#include "OpenCylinderConcave.h"
#include "Torus.h"
#include "TorusPart.h"
#include "TorusPartConcave.h"
#include "SV_Matte.h"
#include "Instance.h"
#include "Checker3D.h"
#include "SolidCylinder.h"
#include "CubicNoise.h"
#include "FBmTexture.h"
#include "TurbulenceTexture.h"
#include "FBmTextureWrapped.h"
#include "Image.h"
#include "FBmTextureRamp.h"
#include "InstanceTexture.h"
#include "Wood.h"

#define TYPE 0
// TYPE = 0: FBmTexture
// TYPE = 1: TurbulenceTexture
// TYPE = 2: FBmTextureWrapped

void
World::build(void) {
	int num_samples = 16;
	vp.set_hres(300);
	vp.set_vres(300);
	vp.set_samples(num_samples);
	vp.set_gamut_display(true);

	background_color = RGBColor(0.5);
	tracer_ptr = new RayCast(this);

	Pinhole* pinhole_ptr = new Pinhole;

	pinhole_ptr->set_eye(0, 20, 100); // sphere: (0, 0, 100)
	pinhole_ptr->set_lookat(0.0);
	pinhole_ptr->set_view_distance(4750.0);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);


	PointLight* light_ptr = new PointLight;
	light_ptr->set_location(10, 10, 20);
	light_ptr->scale_radiance(2.5);
	add_light(light_ptr);


	// wood texture

	// This Wood constructor just specifies the light and dark colors.
	// The other parameters are defaults.

	RGBColor light_color(0.5, 0.2, 0.065);
	RGBColor dark_color(0.05);

	Wood* wood_ptr = new Wood(light_color, dark_color);
	wood_ptr->set_grainy(1.0);
	wood_ptr->set_ringy(1.0);

	InstanceTexture* transformed_wood_ptr = new InstanceTexture(wood_ptr);
	transformed_wood_ptr->scale(0.4);
//	transformed_wood_ptr->rotate_z(110);


	// material:

	SV_Matte* sv_matte_ptr = new SV_Matte;
	sv_matte_ptr->set_ka(0.25);
	sv_matte_ptr->set_kd(0.85);
	sv_matte_ptr->set_cd(transformed_wood_ptr);

	// the sphere:

//	Sphere* sphere_ptr = new Sphere();
	SolidCylinder* sphere_ptr = new SolidCylinder();

    Instance* instance_ptr = new Instance(sphere_ptr);
    instance_ptr->scale(2.0);
//	instance_ptr->rotate_x(-60);
//	instance_ptr->rotate_y(-30);
    instance_ptr->set_material(sv_matte_ptr);
    add_object(instance_ptr);
}

#elif ChapterNum == 32 // rotational sweeping, glass

#include "World.h"
#include "Ambient.h"
#include "Pinhole.h"
#include "Directional.h"
#include "RayCast.h"
#include "Matte.h"
#include "Phong.h"
#include "Plane.h"
#include "OpenCylinder.h"
#include "MultiJittered.h"
#include "AmbientOccluder.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "AreaLighting.h"
#include "Instance.h"
#include "Grid.h"
#include "PointLight.h"#include "Reflective.h"
#include "GlossyReflector.h"
#include "Whitted.h"
#include "PathTrace.h"
#include "GlobalTrace.h"
#include "Rectangle.h"
#include "Transparent.h"
#include "Dielectric.h"
#include "SphereConcave.h"
#include "Checker3D.h"
#include "SV_Matte.h"


void
World::build(void){
	int num_samples = 16;

	vp.set_hres(300);
	vp.set_vres(300);
	vp.set_samples(num_samples);
//	vp.set_max_depth(2);
	vp.set_max_depth(4);
//	vp.set_max_depth(8);
//	vp.set_max_depth(16);

	background_color = RGBColor(0.0, 0.3, 0.25);

	tracer_ptr = new Whitted(this);

	Ambient* ambient_ptr = new Ambient;
	ambient_ptr->scale_radiance(0.25);
	set_ambient_light(ambient_ptr);

	Pinhole* pinhole_ptr = new Pinhole;

	pinhole_ptr->set_eye(0, 20, 80);
	pinhole_ptr->set_lookat(0, 0, 0);
	pinhole_ptr->set_view_distance(3600);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

	// point light

	PointLight* light_ptr1 = new PointLight;
	light_ptr1->set_location(30, 50, 10);
	light_ptr1->scale_radiance(4.5);
	light_ptr1->set_cast_shadow(true);
	add_light(light_ptr1);


	// point light

	PointLight* light_ptr2 = new PointLight;
	light_ptr2->set_location(-30, 50, 10);
	light_ptr2->scale_radiance(4.5);
	light_ptr2->set_cast_shadow(true);
	add_light(light_ptr2);


	// transparent sphere

	Dielectric* dielectric_ptr = new Dielectric;
	dielectric_ptr->set_ks(0.2);
	dielectric_ptr->set_exp(2000.0);
	dielectric_ptr->set_eta_in(1.5);
	dielectric_ptr->set_eta_out(1.0);
	dielectric_ptr->set_cf_in(1.0, 1.0, 0.0);
	dielectric_ptr->set_cf_out(1.0, 1.0, 1.0);


	Grid* grid_ptr = new Grid;

	Point2D ctrl_points[6] = {Point2D(-1.0,  5.0), Point2D( 2.0,  4.0),
                              Point2D( 2.0,  1.0), Point2D(-0.5,  1.0),
                              Point2D( 1.5, -3.0), Point2D( 3.0,  0.0)};
    grid_ptr->tessellate_flat_rotational_sweeping(200, 50, ctrl_points, 6);
	grid_ptr->setup_cells();


    Instance* instance_ptr = new Instance(grid_ptr);
    instance_ptr->scale(0.8);
//    instance_ptr->rotate_x(-90);
    instance_ptr->translate(0, -1, 0);
    instance_ptr->set_material(dielectric_ptr);

	add_object(instance_ptr);


    //------------------------------Checker3D

	Checker3D* texture_ptr1 = new Checker3D();
	texture_ptr1->set_size(0.5);
    texture_ptr1->set_color1(0.0, 0.0, 0.0);
    texture_ptr1->set_color2(1.0, 1.0, 1.0);


	// textured material:

    SV_Matte* matte_ptr1 = new SV_Matte;
    matte_ptr1->set_ka(0.25);
    matte_ptr1->set_kd(0.75);
    matte_ptr1->set_cd(texture_ptr1);

    Plane* plane_ptr1 = new Plane(Point3D(0, -3, 0), Normal(0, 1, 0));
    plane_ptr1->set_material(matte_ptr1);

	add_object(plane_ptr1);


    //------------------------------Checker3D

	Checker3D* texture_ptr2 = new Checker3D();
	texture_ptr2->set_size(0.5);
    texture_ptr2->set_color1(0.4, 0.4, 0.4);
    texture_ptr2->set_color2(1.0, 1.0, 1.0);


	// textured material:

    SV_Matte* matte_ptr2 = new SV_Matte;
    matte_ptr2->set_ka(0.25);
    matte_ptr2->set_kd(0.75);
    matte_ptr2->set_cd(texture_ptr2);

    Plane* plane_ptr2 = new Plane(Point3D(0, 0, -5), Normal(0, 0, 1));
    plane_ptr2->set_material(matte_ptr2);

	add_object(plane_ptr2);

}


#elif ChapterNum == 321 // RayTracing Scenes, 2, PLYs

#include "World.h"
#include "Ambient.h"
#include "Pinhole.h"
#include "Directional.h"
#include "PointLight.h"
#include "RayCast.h"
#include "Whitted.h"
#include "Matte.h"
#include "Plane.h"
#include "Phong.h"
#include "MultiJittered.h"
#include "AmbientOccluder.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "Rectangle.h"
#include "AreaLighting.h"
#include "Instance.h"
#include "Disk.h"
#include "Grid.h"
#include "CubicNoise.h"
#include "Image.h"
#include "FBmTextureRamp.h"
#include "InstanceTexture.h"
#include "SV_Matte.h"
#include "SV_Phong.h"
#include "Wood.h"
#include "Checker3D.h"
#include "Dielectric.h"
#include "Annulus.h"
#include "TorusPartConvex.h"
#include "SphericalMap.h"
#include "Image.h"
#include "ImageTexture.h"
#include "OpenCylinder.h"
#include "Reflective.h"
#include "SolidCylinder.h"
#include "SolidCylinderChecker.h"
#include "PlaneChecker.h"
#include "SV_Reflective.h"
#include "SV_GlossyReflector.h"

#include <iostream>
#include <fstream>
using namespace std;


void
World::build(void){
	int num_samples = 100;
	int a = 1;

	vp.set_hres(600/a); // 600*500
	vp.set_vres(500/a);
	vp.set_samples(num_samples);
	vp.set_max_depth(4);

//	tracer_ptr = new RayCast(this);
	tracer_ptr = new Whitted(this);

	Pinhole* pinhole_ptr = new Pinhole;

	pinhole_ptr->set_eye(0, 20, 80);
	pinhole_ptr->set_lookat(0, 1, 0);
	pinhole_ptr->set_view_distance(3600/a); // 3600
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

	PointLight* point_light_ptr1 = new PointLight;
	point_light_ptr1->set_location(30, 10, 30);
	point_light_ptr1->scale_radiance(3.0);
	point_light_ptr1->set_cast_shadow(false);
	add_light(point_light_ptr1);

	PointLight* point_light_ptr2 = new PointLight;
	point_light_ptr2->set_location(-30, 10, 30);
	point_light_ptr2->scale_radiance(3.0);
	point_light_ptr2->set_cast_shadow(false);
	add_light(point_light_ptr2);


////////////////////////////////////////bunny////////////////////////////////////////////////

	char* file_bunny__name = "/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/Bunny69K.ply";
	Grid* grid_bunny_ptr = new Grid(new Mesh);
	grid_bunny_ptr->read_smooth_triangles(file_bunny__name);
	grid_bunny_ptr->setup_cells();

	// material:

	Dielectric* dielectric_bunny_ptr = new Dielectric;
	dielectric_bunny_ptr->set_ks(0.2);
	dielectric_bunny_ptr->set_exp(2000.0);
	dielectric_bunny_ptr->set_eta_in(1.5);
	dielectric_bunny_ptr->set_eta_out(1.0);
	dielectric_bunny_ptr->set_cf_in(0.35, 0.65, 0.45);   // green
	dielectric_bunny_ptr->set_cf_out(1.0, 1.0, 1.0);


    Instance* instance_bunny_ptr = new Instance(grid_bunny_ptr);
    instance_bunny_ptr->scale(40);
    instance_bunny_ptr->translate(1, -2.5, -2.5);
    instance_bunny_ptr->set_material(dielectric_bunny_ptr);

    add_object(instance_bunny_ptr);


////////////////////////////////////////hand////////////////////////////////////////////////

	char* file_hand_name = "/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/hand.ply";
	Grid* grid_hand_ptr = new Grid(new Mesh);
	grid_hand_ptr->read_smooth_triangles(file_hand_name);
	grid_hand_ptr->setup_cells();

	// wood texture

	RGBColor light_color(0.5, 0.2, 0.065);
	RGBColor dark_color(0.1);

	Wood* wood_ptr = new Wood(light_color, dark_color);
	wood_ptr->set_grainy(1.0);
	wood_ptr->set_ringy(1.0);

	InstanceTexture* transformed_wood_ptr = new InstanceTexture(wood_ptr);
	transformed_wood_ptr->scale(0.4);
	transformed_wood_ptr->rotate_z(110);


	// material:

	SV_Phong* sv_phong_hand_ptr = new SV_Phong;
	sv_phong_hand_ptr->set_ka(0.25);
	sv_phong_hand_ptr->set_kd(0.75);
	sv_phong_hand_ptr->set_cd(transformed_wood_ptr);
	sv_phong_hand_ptr->set_ks(0.1);
	sv_phong_hand_ptr->set_exp(20.0);

    Instance* instance_hand_ptr = new Instance(grid_hand_ptr);
    instance_hand_ptr->rotate_y(-60);
    instance_hand_ptr->translate(3.5, 1.5, -5.0);
    instance_hand_ptr->set_material(sv_phong_hand_ptr);

    add_object(instance_hand_ptr);


////////////////////////////////////////dragon////////////////////////////////////////////////

	char* file_dragon_name = "/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/dragon.ply";
	Grid* grid_dragon_ptr = new Grid(new Mesh);
	grid_dragon_ptr->read_smooth_triangles(file_dragon_name);
	grid_dragon_ptr->setup_cells();

	// noise:

	CubicNoise* noise_dragon_ptr = new CubicNoise;
	noise_dragon_ptr->set_num_octaves(6);
	noise_dragon_ptr->set_gain(0.5);
	noise_dragon_ptr->set_lacunarity(2.0);

	// ramp image:

	Image* image_dragon_ptr = new Image;
	image_dragon_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/BlueMarbleRamp.ppm");

	// marble texture:

	FBmTextureRamp* marble_dragon_ptr = new FBmTextureRamp(image_dragon_ptr);
	marble_dragon_ptr->set_noise(noise_dragon_ptr);
	marble_dragon_ptr->set_perturbation(4.0);

	InstanceTexture* it_dragon_ptr = new InstanceTexture(marble_dragon_ptr);
	it_dragon_ptr->scale(0.01);
	it_dragon_ptr->rotate_z(110);
	it_dragon_ptr->translate(1.0, 4.0, 0.0);


	// material:

	SV_Phong* sv_phong_dragon_ptr2 = new SV_Phong;
	sv_phong_dragon_ptr2->set_ka(0.25);
	sv_phong_dragon_ptr2->set_kd(0.75);
	sv_phong_dragon_ptr2->set_cd(it_dragon_ptr);
	sv_phong_dragon_ptr2->set_ks(0.1);
	sv_phong_dragon_ptr2->set_exp(20.0);


    Instance* instance_dragon_ptr2 = new Instance(grid_dragon_ptr);
    instance_dragon_ptr2->scale(30);
    instance_dragon_ptr2->rotate_y(30);
    instance_dragon_ptr2->translate(4, -4.5, 5);
    instance_dragon_ptr2->set_material(sv_phong_dragon_ptr2);

    add_object(instance_dragon_ptr2);


////////////////////////////////////////horse////////////////////////////////////////////////

	char* file_horse_name = "/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/Horse97K.ply";
	Grid* grid_horse_ptr = new Grid(new Mesh);
	grid_horse_ptr->read_smooth_triangles(file_horse_name);
	grid_horse_ptr->setup_cells();

	// noise:

	CubicNoise* noise_horse_ptr = new CubicNoise;
	noise_horse_ptr->set_num_octaves(6);
	noise_horse_ptr->set_gain(0.5);
	noise_horse_ptr->set_lacunarity(2.0);

	// ramp image:

	Image* image_horse_ptr = new Image;
	image_horse_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/sandstone_ramp4.ppm");

	// marble texture:

	FBmTextureRamp* marble_horse_ptr = new FBmTextureRamp(image_horse_ptr);
	marble_horse_ptr->set_noise(noise_horse_ptr);
	marble_horse_ptr->set_perturbation(0.1);

	InstanceTexture* it_horse_ptr = new InstanceTexture(marble_horse_ptr);
	it_horse_ptr->rotate_z(110);
	it_horse_ptr->translate(1.0, 4.0, 0.0);


	// material:

	SV_Phong* sv_phong_horse_ptr = new SV_Phong;
	sv_phong_horse_ptr->set_ka(0.25);
	sv_phong_horse_ptr->set_kd(0.75);
	sv_phong_horse_ptr->set_cd(it_horse_ptr);
	sv_phong_horse_ptr->set_ks(0.1);
	sv_phong_horse_ptr->set_exp(20.0);


    Instance* instance_horse_ptr = new Instance(grid_horse_ptr);
    instance_horse_ptr->scale(9);
    instance_horse_ptr->rotate_y(-40);
    instance_horse_ptr->translate(-4, -0.2, 1);
    instance_horse_ptr->set_material(sv_phong_horse_ptr);

    add_object(instance_horse_ptr);


////////////////////////////////////////SolidCylinder////////////////////////////////////////////////

    SolidCylinder* sc_ptr = new SolidCylinder();

	// SolidCylinderChecker:
	SolidCylinderChecker* texture_ptr3 = new SolidCylinderChecker();
	texture_ptr3->set_num_horizontal_checkers(20);
	texture_ptr3->set_num_vertical_checkers(10);
	texture_ptr3->set_num_radius_checkers_disk(5);
	texture_ptr3->set_horizontal_line_width(0.02);
	texture_ptr3->set_vertical_line_width(0.02);
    texture_ptr3->set_color1(1.0, 0.0, 0.0);
    texture_ptr3->set_color2(0.0, 1.0, 0.0);
    texture_ptr3->set_line_color(1.0, 1.0, 1.0);

	// material:

	SV_Phong* sv_phong_sc_ptr = new SV_Phong;
	sv_phong_sc_ptr->set_ka(0.25);
	sv_phong_sc_ptr->set_kd(0.75);
	sv_phong_sc_ptr->set_cd(texture_ptr3);
	sv_phong_sc_ptr->set_ks(0.1);
	sv_phong_sc_ptr->set_exp(20.0);

	Instance* instance_sc_ptr = new Instance(sc_ptr);
	instance_sc_ptr->scale(2, 1, 2);
	instance_sc_ptr->translate(0, -2, -2);
	instance_sc_ptr->set_material(sv_phong_sc_ptr);
    add_object(instance_sc_ptr);

////////////////////////////////////////plane down////////////////////////////////////////////////

    Plane* plane_down_ptr = new Plane();

	// textured material:

	SV_Reflective* sv_reflective_down_ptr = new SV_Reflective;
	sv_reflective_down_ptr->set_ka(0.0);
	sv_reflective_down_ptr->set_kd(0.0);
	sv_reflective_down_ptr->set_cd(new ConstantColor(black));
	sv_reflective_down_ptr->set_ks(0.1);
	sv_reflective_down_ptr->set_exp(50.0);
	sv_reflective_down_ptr->set_kr(0.75);
	sv_reflective_down_ptr->set_cr(new ConstantColor(RGBColor(0.8, 0.2, 0.2)));

    Instance* instance_plane_down_ptr = new Instance(plane_down_ptr);
    instance_plane_down_ptr->translate(0, -3, 0);
    instance_plane_down_ptr->set_material(sv_reflective_down_ptr);
	add_object(instance_plane_down_ptr);


////////////////////////////////////////plane back////////////////////////////////////////////////

    Plane* plane_back_ptr = new Plane();

	PlaneChecker* plane_checker_back_ptr = new PlaneChecker;
	plane_checker_back_ptr->set_size(1.0);
	plane_checker_back_ptr->set_line_width(0.125);
    plane_checker_back_ptr->set_color1(white);
    plane_checker_back_ptr->set_color2(white);
    plane_checker_back_ptr->set_line_color(black);


	// textured material:

	float exp = 100000.0;

	SV_GlossyReflector* sv_glossy_back_ptr = new SV_GlossyReflector;
	sv_glossy_back_ptr->set_samples(num_samples, exp);
	sv_glossy_back_ptr->set_ka(0.25);                // 0.4
	sv_glossy_back_ptr->set_kd(0.75);                // 0.8
	sv_glossy_back_ptr->set_ks(0.1);                // 0.5
	sv_glossy_back_ptr->set_exp(exp);              // for phong
	sv_glossy_back_ptr->set_cd(plane_checker_back_ptr);
	sv_glossy_back_ptr->set_kr(0.75);
	sv_glossy_back_ptr->set_exponent(exp);         // for glossy.
	sv_glossy_back_ptr->set_cr(new ConstantColor(white));   // green

    Instance* instance_plane_back_ptr = new Instance(plane_back_ptr);
    instance_plane_back_ptr->rotate_x(90);
    instance_plane_back_ptr->translate(0, 0, -6);
    instance_plane_back_ptr->set_material(sv_glossy_back_ptr);
	add_object(instance_plane_back_ptr);

}


#elif ChapterNum == 322 // RayTracing Scenes, 3, two horses

#include "World.h"
#include "Ambient.h"
#include "Pinhole.h"
#include "Directional.h"
#include "PointLight.h"
#include "RayCast.h"
#include "Whitted.h"
#include "Matte.h"
#include "Plane.h"
#include "Phong.h"
#include "MultiJittered.h"
#include "AmbientOccluder.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "Rectangle.h"
#include "AreaLighting.h"
#include "Instance.h"
#include "Disk.h"
#include "Grid.h"
#include "CubicNoise.h"
#include "Image.h"
#include "FBmTextureRamp.h"
#include "InstanceTexture.h"
#include "SV_Matte.h"
#include "SV_Phong.h"
#include "Wood.h"
#include "Checker3D.h"
#include "Dielectric.h"
#include "Annulus.h"
#include "TorusPartConvex.h"
#include "SphericalMap.h"
#include "Image.h"
#include "ImageTexture.h"
#include "OpenCylinder.h"
#include "Reflective.h"
#include "SolidCylinder.h"
#include "SolidCylinderChecker.h"
#include "SV_PlaneChecker.h"
#include "SV_Reflective.h"
#include "SV_GlossyReflector.h"

#include <iostream>
#include <fstream>
using namespace std;


void
World::build(void){
	int num_samples = 16;
	int a = 1;

	vp.set_hres(600/a); // 600*500
	vp.set_vres(500/a);
	vp.set_samples(num_samples);
	vp.set_max_depth(4);

//	tracer_ptr = new RayCast(this);
//	tracer_ptr = new Whitted(this);
	tracer_ptr = new AreaLighting(this);


	Pinhole* pinhole_ptr = new Pinhole;
	pinhole_ptr->set_eye(20, 10, 80);
	pinhole_ptr->set_lookat(-4, -2, -2);
	pinhole_ptr->set_view_distance(3600/a); // 3600
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

	Ambient* ambient_ptr = new Ambient;
	ambient_ptr->scale_radiance(5.0);
	set_ambient_light(ambient_ptr);

	MultiJittered* sampler_ptr = new MultiJittered(num_samples);

	AmbientOccluder* occluder_ptr = new AmbientOccluder;
	occluder_ptr->scale_radiance(1.0);
	occluder_ptr->set_min_amount(0.0);
	occluder_ptr->set_sampler(sampler_ptr);
//	set_ambient_light(occluder_ptr);


	// emissive sphere

	Emissive* emissive_ptr = new Emissive;
	emissive_ptr->scale_radiance(8.0);
	emissive_ptr->set_ce(white);

	Disk* disk_ptr = new Disk(Point3D(2.0, 10.0, 10.0), 1.5, Normal(0, -1, 0));
	disk_ptr->set_material(emissive_ptr);
	disk_ptr->set_sampler(new MultiJittered(num_samples));
	add_object(disk_ptr);

	AreaLight* area_light_ptr = new AreaLight;
	area_light_ptr->set_object(disk_ptr);
	area_light_ptr->set_cast_shadow(true);
	add_light(area_light_ptr);


	PointLight* point_light_ptr1 = new PointLight;
	point_light_ptr1->set_location(30, 10, 30);
	point_light_ptr1->scale_radiance(6.0);
	point_light_ptr1->set_cast_shadow(false);
//	add_light(point_light_ptr1);


////////////////////////////////////////plane left////////////////////////////////////////////////

    Plane* plane_left_ptr = new Plane();

	// wood texture

	RGBColor light_color(0.5, 0.2, 0.065);
	RGBColor dark_color(0.05);

	Wood* wood_ptr = new Wood(light_color, dark_color);
	wood_ptr->set_grainy(1.0);
	wood_ptr->set_ringy(1.0);

	InstanceTexture* transformed_wood_plane_ptr = new InstanceTexture(wood_ptr);
	transformed_wood_plane_ptr->scale(0.4);
	transformed_wood_plane_ptr->rotate_z(110);

	SV_PlaneChecker* sv_plane_checker_left_ptr = new SV_PlaneChecker;
	sv_plane_checker_left_ptr->set_size(2.0);
	sv_plane_checker_left_ptr->set_line_width(0.4);
    sv_plane_checker_left_ptr->set_color1(new ConstantColor(RGBColor(0.5)));
    sv_plane_checker_left_ptr->set_color2(new ConstantColor(RGBColor(0.5)));
    sv_plane_checker_left_ptr->set_line_color(transformed_wood_plane_ptr);


	// textured material:

	float exp_left = 1000.0;

	SV_GlossyReflector* sv_glossy_left_ptr = new SV_GlossyReflector;
	sv_glossy_left_ptr->set_samples(num_samples, exp_left);
	sv_glossy_left_ptr->set_ka(0.1);                // 0.4
	sv_glossy_left_ptr->set_kd(0.25);                // 0.8
	sv_glossy_left_ptr->set_ks(0.1);                // 0.5
	sv_glossy_left_ptr->set_exp(exp_left);              // for phong
	sv_glossy_left_ptr->set_cd(sv_plane_checker_left_ptr);
	sv_glossy_left_ptr->set_kr(0.75);
	sv_glossy_left_ptr->set_exponent(exp_left);         // for glossy.
	sv_glossy_left_ptr->set_cr(new ConstantColor(white));   // green

    Instance* instance_plane_left_ptr = new Instance(plane_left_ptr);
    instance_plane_left_ptr->rotate_z(90);
    instance_plane_left_ptr->translate(-4, 0, 0);
    instance_plane_left_ptr->set_material(sv_glossy_left_ptr);
	add_object(instance_plane_left_ptr);


////////////////////////////////////////rectangle back 1////////////////////////////////////////////////

    Rectangle* rectangle_back_ptr1 = new Rectangle(Point3D(-4, -2, 0), Vector3D(20, 0, 0), Vector3D(0, 2, 0), Normal(0, 0, 1));


	// noise:

	CubicNoise* noise_back1_ptr = new CubicNoise;
	noise_back1_ptr->set_num_octaves(6);
	noise_back1_ptr->set_gain(0.5);
	noise_back1_ptr->set_lacunarity(2.0);

	// ramp image:

	Image* image_back1_ptr = new Image;
	image_back1_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/sandstone_ramp4.ppm");

	// marble texture:

	FBmTextureRamp* texture_back1_ptr = new FBmTextureRamp(image_back1_ptr);
	texture_back1_ptr->set_noise(noise_back1_ptr);
	texture_back1_ptr->set_perturbation(0.1);

	InstanceTexture* it_back1_ptr = new InstanceTexture(texture_back1_ptr);
    it_back1_ptr->scale(0.5);
	it_back1_ptr->rotate_z(60);
	it_back1_ptr->translate(1.0, 4.0, 0.0);


	// textured material:

	float exp_back1 = 100.0;

	SV_GlossyReflector* sv_glossy_back1_ptr = new SV_GlossyReflector;
	sv_glossy_back1_ptr->set_samples(num_samples, exp_back1);
	sv_glossy_back1_ptr->set_ka(0.1);                // 0.4
	sv_glossy_back1_ptr->set_kd(0.25);                // 0.8
	sv_glossy_back1_ptr->set_ks(0.1);                // 0.5
	sv_glossy_back1_ptr->set_exp(exp_back1);              // for phong
	sv_glossy_back1_ptr->set_cd(it_back1_ptr);
	sv_glossy_back1_ptr->set_kr(0.75);
	sv_glossy_back1_ptr->set_exponent(exp_back1);         // for glossy.
	sv_glossy_back1_ptr->set_cr(new ConstantColor(white));   // green


    Instance* instance_rectangle_back1_ptr = new Instance(rectangle_back_ptr1);
    instance_rectangle_back1_ptr->translate(0, 0, -4);
    instance_rectangle_back1_ptr->set_material(sv_glossy_back1_ptr);
	add_object(instance_rectangle_back1_ptr);


////////////////////////////////////////rectangle back 2////////////////////////////////////////////////

    Rectangle* rectangle_back_ptr2 = new Rectangle();

	// image:

	Image* image_back_ptr2 = new Image;
	image_back_ptr2->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/CloudsLowRes.ppm");

	// image based texture:

	ImageTexture* texture_image_ptr = new ImageTexture;
	texture_image_ptr->set_image(image_back_ptr2);


	// material:

	SV_Phong* sv_phong_back_ptr2 = new SV_Phong;
	sv_phong_back_ptr2->set_ka(0.1);
	sv_phong_back_ptr2->set_kd(0.5);
	sv_phong_back_ptr2->set_cd(texture_image_ptr);
	sv_phong_back_ptr2->set_ks(0.1);
	sv_phong_back_ptr2->set_exp(20.0);


    Instance* instance_rectangle_back2_ptr = new Instance(rectangle_back_ptr2);
    instance_rectangle_back2_ptr->scale(4, 1, 4);
    instance_rectangle_back2_ptr->rotate_x(90);
    instance_rectangle_back2_ptr->translate(0, 4, -4);
    instance_rectangle_back2_ptr->set_material(sv_phong_back_ptr2);
	add_object(instance_rectangle_back2_ptr);


////////////////////////////////////////plane down////////////////////////////////////////////////

    Plane* plane_down_ptr = new Plane();

	// noise:

	CubicNoise* noise_down_ptr = new CubicNoise;
	noise_down_ptr->set_num_octaves(6);
	noise_down_ptr->set_gain(0.5);
	noise_down_ptr->set_lacunarity(2.0);

	// ramp image:

	Image* image_down_ptr = new Image;
	image_down_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/BlueMarbleRamp.ppm");

	// marble texture:

	FBmTextureRamp* texture_down_ptr = new FBmTextureRamp(image_down_ptr);
	texture_down_ptr->set_noise(noise_down_ptr);
	texture_down_ptr->set_perturbation(4.0);

	InstanceTexture* it_down_ptr = new InstanceTexture(texture_down_ptr);
//	it_down_ptr->scale(0.5);
	it_down_ptr->rotate_z(60);
	it_down_ptr->translate(1.0, 4.0, 0.0);


	SV_PlaneChecker* sv_plane_checker_down_ptr = new SV_PlaneChecker;
	sv_plane_checker_down_ptr->set_size(4.0);
	sv_plane_checker_down_ptr->set_line_width(0.05);
    sv_plane_checker_down_ptr->set_color1(it_down_ptr);
    sv_plane_checker_down_ptr->set_color2(it_down_ptr);
    sv_plane_checker_down_ptr->set_line_color(new ConstantColor(RGBColor(0.2)));


	// textured material:

	float exp_down = 1000.0;

	SV_GlossyReflector* sv_glossy_down_ptr = new SV_GlossyReflector;
	sv_glossy_down_ptr->set_samples(num_samples, exp_down);
	sv_glossy_down_ptr->set_ka(0.1);                // 0.4
	sv_glossy_down_ptr->set_kd(0.25);                // 0.8
	sv_glossy_down_ptr->set_ks(0.1);                // 0.5
	sv_glossy_down_ptr->set_exp(exp_down);              // for phong
	sv_glossy_down_ptr->set_cd(sv_plane_checker_down_ptr);
	sv_glossy_down_ptr->set_kr(0.75);
	sv_glossy_down_ptr->set_exponent(exp_down);         // for glossy.
	sv_glossy_down_ptr->set_cr(new ConstantColor(white));   // green

    Instance* instance_plane_down_ptr = new Instance(plane_down_ptr);
    instance_plane_down_ptr->translate(0, -2, 0);
    instance_plane_down_ptr->set_material(sv_glossy_down_ptr);
	add_object(instance_plane_down_ptr);


////////////////////////////////////////horse////////////////////////////////////////////////

	char* file_horse_name = "/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/Horse97K.ply";
	Grid* grid_horse_ptr = new Grid(new Mesh);
	grid_horse_ptr->read_smooth_triangles(file_horse_name);
	grid_horse_ptr->setup_cells();


	// textured material:

	float exp_horse1 = 1000000.0;

	SV_GlossyReflector* sv_glossy_horse_ptr1 = new SV_GlossyReflector;
	sv_glossy_horse_ptr1->set_samples(num_samples, exp_horse1);
	sv_glossy_horse_ptr1->set_ka(0.1);                // 0.4
	sv_glossy_horse_ptr1->set_kd(0.4);                // 0.8
	sv_glossy_horse_ptr1->set_ks(0.1);                // 0.5
	sv_glossy_horse_ptr1->set_exp(exp_horse1);              // for phong
	sv_glossy_horse_ptr1->set_cd(new ConstantColor(RGBColor(1.0, 0.2, 0.2)));
	sv_glossy_horse_ptr1->set_kr(0.75);
	sv_glossy_horse_ptr1->set_exponent(exp_horse1);         // for glossy.
	sv_glossy_horse_ptr1->set_cr(new ConstantColor(white));   // green


    Instance* instance_horse_ptr1 = new Instance(grid_horse_ptr);
    instance_horse_ptr1->scale(9);
    instance_horse_ptr1->rotate_y(-30);
    instance_horse_ptr1->translate(-1, 0.5, 14);
    instance_horse_ptr1->set_material(sv_glossy_horse_ptr1);
    add_object(instance_horse_ptr1);


	// textured material:

	float exp_horse2 = 1000000.0;

	SV_GlossyReflector* sv_glossy_horse_ptr2 = new SV_GlossyReflector;
	sv_glossy_horse_ptr2->set_samples(num_samples, exp_horse2);
	sv_glossy_horse_ptr2->set_ka(0.1);                // 0.4
	sv_glossy_horse_ptr2->set_kd(0.4);                // 0.8
	sv_glossy_horse_ptr2->set_ks(0.1);                // 0.5
	sv_glossy_horse_ptr2->set_exp(exp_horse2);              // for phong
	sv_glossy_horse_ptr2->set_cd(new ConstantColor(RGBColor(0.5, 1.0, 0.5)));
	sv_glossy_horse_ptr2->set_kr(0.75);
	sv_glossy_horse_ptr2->set_exponent(exp_horse2);         // for glossy.
	sv_glossy_horse_ptr2->set_cr(new ConstantColor(white));   // green


    Instance* instance_horse_ptr2 = new Instance(grid_horse_ptr);
    instance_horse_ptr2->scale(9);
    instance_horse_ptr2->rotate_y(-30);
    instance_horse_ptr2->translate(2, 0.5, 8);
    instance_horse_ptr2->set_material(sv_glossy_horse_ptr2);
    add_object(instance_horse_ptr2);


}



#elif ChapterNum == 323 // RayTracing Scenes, 4, Seaside

#include "World.h"
#include "Ambient.h"
#include "Pinhole.h"
#include "Directional.h"
#include "PointLight.h"
#include "RayCast.h"
#include "Whitted.h"
#include "Matte.h"
#include "Plane.h"
#include "Phong.h"
#include "MultiJittered.h"
#include "AmbientOccluder.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "Rectangle.h"
#include "AreaLighting.h"
#include "Instance.h"
#include "Disk.h"
#include "Grid.h"
#include "CubicNoise.h"
#include "Image.h"
#include "FBmTextureRamp.h"
#include "InstanceTexture.h"
#include "SV_Matte.h"
#include "SV_Phong.h"
#include "Wood.h"
#include "Checker3D.h"
#include "Dielectric.h"
#include "Annulus.h"
#include "TorusPartConvex.h"
#include "SphericalMap.h"
#include "Image.h"
#include "ImageTexture.h"
#include "OpenCylinder.h"
#include "Reflective.h"
#include "SolidCylinder.h"
#include "SolidCylinderChecker.h"
#include "PlaneChecker.h"
#include "SV_Reflective.h"
#include "SV_GlossyReflector.h"
#include "SpherePartConcave.h"
#include "SpherePartConvex.h"
#include "FBmTextureWrapped.h"

#include <iostream>
#include <fstream>
using namespace std;


void
World::build(void){
	int num_samples = 100;
	int a = 1;

	vp.set_hres(600/a); // 600*500
	vp.set_vres(500/a);
	vp.set_samples(num_samples);
	vp.set_max_depth(4);

//	tracer_ptr = new RayCast(this);
	tracer_ptr = new Whitted(this);

	Pinhole* pinhole_ptr = new Pinhole;

	pinhole_ptr->set_eye(0, 1.2, -1.7);
	pinhole_ptr->set_lookat(0, 0.5, -20);
	pinhole_ptr->set_view_distance(300/a); // 3600
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

	PointLight* point_light_ptr1 = new PointLight;
	point_light_ptr1->set_location(0, 3, 0);
	point_light_ptr1->scale_radiance(3.0);
	point_light_ptr1->set_cast_shadow(true);
	add_light(point_light_ptr1);


////////////////////////////////////////sky, SpherePartConcave///////////////////////////////////////

    SpherePartConcave* spc_sky_ptr = new SpherePartConcave(Point3D(0, 0, 0), 1, 0, 360, 0, 90);

	// image:

	Image* image_sky_ptr = new Image;
//	image_sky_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/MorningSky.ppm");
	image_sky_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/EveningSky.ppm");

	// image based texture:

	ImageTexture* texture_image_sky_ptr = new ImageTexture;
	texture_image_sky_ptr->set_image(image_sky_ptr);

	// material:

	SV_Phong* sv_phong_sky_ptr = new SV_Phong;
	sv_phong_sky_ptr->set_ka(0.1);
	sv_phong_sky_ptr->set_kd(0.25);
	sv_phong_sky_ptr->set_cd(texture_image_sky_ptr);
	sv_phong_sky_ptr->set_ks(0.1);
	sv_phong_sky_ptr->set_exp(20.0);


    Instance* instance_spc_sky_ptr = new Instance(spc_sky_ptr);
    instance_spc_sky_ptr->scale(1000000);
    instance_spc_sky_ptr->rotate_x(-5);
//    instance_spc_sky_ptr->translate(0, -1000, 0);
    instance_spc_sky_ptr->set_material(sv_phong_sky_ptr);
	add_object(instance_spc_sky_ptr);


////////////////////////////////////////roof, SpherePartConcave///////////////////////////////////////

    SpherePartConcave* spc_roof_ptr = new SpherePartConcave(Point3D(0, 0, 0), 1, 0, 360, 0, 60);

	// image:

	Image* image_roof_ptr = new Image;
	image_roof_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/fresco3.ppm");

	// image based texture:

	ImageTexture* texture_image_roof_ptr = new ImageTexture;
	texture_image_roof_ptr->set_image(image_roof_ptr);

	// material:

	SV_Phong* sv_phong_roof_ptr = new SV_Phong;
	sv_phong_roof_ptr->set_ka(0.25);
	sv_phong_roof_ptr->set_kd(0.75);
	sv_phong_roof_ptr->set_cd(texture_image_roof_ptr);
	sv_phong_roof_ptr->set_ks(0.1);
	sv_phong_roof_ptr->set_exp(20.0);


    Instance* instance_spc_roof_ptr = new Instance(spc_roof_ptr);
    instance_spc_roof_ptr->scale(4);
    instance_spc_roof_ptr->set_material(sv_phong_roof_ptr);
	add_object(instance_spc_roof_ptr);

////////////////////////////////////////floor, SolidCylinder////////////////////////////////////////////////

    SolidCylinder* sc_floor_ptr = new SolidCylinder();

	// noise:

	CubicNoise* noise_floor_ptr = new CubicNoise;
	noise_floor_ptr->set_num_octaves(6);
	noise_floor_ptr->set_gain(0.5);
	noise_floor_ptr->set_lacunarity(2.0);

	// ramp image:

	Image* image_floor_ptr = new Image;
	image_floor_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/sandstone_ramp4.ppm");

	// marble texture:

	FBmTextureRamp* texture_floor_ptr = new FBmTextureRamp(image_floor_ptr);
	texture_floor_ptr->set_noise(noise_floor_ptr);
	texture_floor_ptr->set_perturbation(4.0);

	InstanceTexture* it_floor_ptr = new InstanceTexture(texture_floor_ptr);
    it_floor_ptr->scale(0.5);
	it_floor_ptr->rotate_z(60);
//	it_floor_ptr->translate(1.0, 4.0, 0.0);

	// textured material:

	float exp_floor = 250.0;
	SV_GlossyReflector* sv_glossy_floor_ptr = new SV_GlossyReflector;
	sv_glossy_floor_ptr->set_samples(num_samples, exp_floor);
	sv_glossy_floor_ptr->set_ka(0.1);
	sv_glossy_floor_ptr->set_kd(0.25);
	sv_glossy_floor_ptr->set_ks(0.1);
	sv_glossy_floor_ptr->set_exp(exp_floor);
	sv_glossy_floor_ptr->set_cd(it_floor_ptr);
	sv_glossy_floor_ptr->set_kr(0.75);
	sv_glossy_floor_ptr->set_exponent(exp_floor);
	sv_glossy_floor_ptr->set_cr(new ConstantColor(white));

    Instance* instance_sc_floor_ptr = new Instance(sc_floor_ptr);
    instance_sc_floor_ptr->scale(3.6, 0.1, 3.6);
//    instance_sc_floor_ptr->translate(0, 0.2, 0);
    instance_sc_floor_ptr->set_material(sv_glossy_floor_ptr);
	add_object(instance_sc_floor_ptr);


////////////////////////////////////////step, SolidCylinder////////////////////////////////////////////////

    SolidCylinder* sc_step_ptr = new SolidCylinder();

	// noise:

	CubicNoise* noise_step_ptr = new CubicNoise;
	noise_step_ptr->set_num_octaves(6);
	noise_step_ptr->set_gain(0.5);
	noise_step_ptr->set_lacunarity(2.0);

	// ramp image:

	Image* image_step_ptr = new Image;
	image_step_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/BlueMarbleRamp.ppm");

	// marble texture:

	FBmTextureRamp* texture_step_ptr = new FBmTextureRamp(image_step_ptr);
	texture_step_ptr->set_noise(noise_step_ptr);
	texture_step_ptr->set_perturbation(4.0);

	InstanceTexture* it_step_ptr = new InstanceTexture(texture_step_ptr);
    it_step_ptr->scale(0.05);
	it_step_ptr->rotate_z(110);
//	it_step_ptr->translate(1.0, 4.0, 0.0);

	// textured material:

	float exp_step = 500.0;
	SV_GlossyReflector* sv_glossy_step_ptr = new SV_GlossyReflector;
	sv_glossy_step_ptr->set_samples(num_samples, exp_step);
	sv_glossy_step_ptr->set_ka(0.1);
	sv_glossy_step_ptr->set_kd(0.25);
	sv_glossy_step_ptr->set_ks(0.1);
	sv_glossy_step_ptr->set_exp(exp_step);
	sv_glossy_step_ptr->set_cd(it_step_ptr);
	sv_glossy_step_ptr->set_kr(0.75);
	sv_glossy_step_ptr->set_exponent(exp_step);
	sv_glossy_step_ptr->set_cr(new ConstantColor(white));

    Instance* instance_sc_step_ptr = new Instance(sc_step_ptr);
    instance_sc_step_ptr->scale(4.6, 0.1, 4.6);
    instance_sc_step_ptr->translate(0, -0.2, 0);
    instance_sc_step_ptr->set_material(sv_glossy_step_ptr);
	add_object(instance_sc_step_ptr);


////////////////////////////////////////square, SolidCylinder////////////////////////////////////////////////

    SolidCylinder* sc_square_ptr = new SolidCylinder();

	// noise:

	CubicNoise* noise_square_ptr = new CubicNoise;
	noise_square_ptr->set_num_octaves(8);
	noise_square_ptr->set_gain(0.5);
	noise_square_ptr->set_lacunarity(8.0);

	FBmTextureWrapped* texture_square_ptr = new FBmTextureWrapped(noise_square_ptr);
	texture_square_ptr->set_expansion_number(8.0);
    texture_square_ptr->set_color(0.7, 1.0, 0.5);   // light green
	texture_square_ptr->set_min_value(0.0);
	texture_square_ptr->set_max_value(1.0);

	// textured material:

	float exp_square = 1000.0;
	SV_GlossyReflector* sv_glossy_square_ptr = new SV_GlossyReflector;
	sv_glossy_square_ptr->set_samples(num_samples, exp_square);
	sv_glossy_square_ptr->set_ka(0.1);
	sv_glossy_square_ptr->set_kd(0.25);
	sv_glossy_square_ptr->set_ks(0.1);
	sv_glossy_square_ptr->set_exp(exp_square);
	sv_glossy_square_ptr->set_cd(texture_square_ptr);
	sv_glossy_square_ptr->set_kr(0.75);
	sv_glossy_square_ptr->set_exponent(exp_square);
	sv_glossy_square_ptr->set_cr(new ConstantColor(white));

    Instance* instance_sc_square_ptr = new Instance(sc_square_ptr);
    instance_sc_square_ptr->scale(9.6, 0.1, 9.6);
    instance_sc_square_ptr->translate(0, -0.4, 0);
    instance_sc_square_ptr->set_material(sv_glossy_square_ptr);
	add_object(instance_sc_square_ptr);


////////////////////////////////////////sea, Sphere////////////////////////////////////////////////

    Sphere* sphere_sea_ptr = new Sphere();

	// textured material:

	float exp_sea = 10000.0;
	SV_GlossyReflector* sv_glossy_sea_ptr = new SV_GlossyReflector;
	sv_glossy_sea_ptr->set_samples(num_samples, exp_sea);
	sv_glossy_sea_ptr->set_ka(0.1);
	sv_glossy_sea_ptr->set_kd(0.25);
	sv_glossy_sea_ptr->set_ks(0.1);
	sv_glossy_sea_ptr->set_exp(exp_sea);
	sv_glossy_sea_ptr->set_cd(new ConstantColor(RGBColor(0.3, 0.3, 1.0)));
	sv_glossy_sea_ptr->set_kr(0.75);
	sv_glossy_sea_ptr->set_exponent(exp_sea);
	sv_glossy_sea_ptr->set_cr(new ConstantColor(white));

    Instance* instance_sphere_sea_ptr = new Instance(sphere_sea_ptr);
    instance_sphere_sea_ptr->scale(50000);
    instance_sphere_sea_ptr->translate(0, -50000.5, -9.6);
    instance_sphere_sea_ptr->set_material(sv_glossy_sea_ptr);
	add_object(instance_sphere_sea_ptr);


////////////////////////////////////////desk, SolidCylinder////////////////////////////////////////////////

    SolidCylinder* sc_desk_ptr = new SolidCylinder();

	// wood texture

	RGBColor desk_light_color(0.5, 0.2, 0.065);
	RGBColor desk_dark_color(0.05);

	Wood* wood_desk_ptr = new Wood(desk_light_color, desk_dark_color);
	wood_desk_ptr->set_grainy(1.0);
	wood_desk_ptr->set_ringy(1.0);

	InstanceTexture* transformed_wood_desk_ptr = new InstanceTexture(wood_desk_ptr);
	transformed_wood_desk_ptr->scale(0.4);
//	transformed_wood_desk_ptr->rotate_z(110);

	// material:

	SV_Phong* sv_phong_desk_ptr = new SV_Phong;
	sv_phong_desk_ptr->set_ka(0.25);
	sv_phong_desk_ptr->set_kd(0.75);
	sv_phong_desk_ptr->set_cd(transformed_wood_desk_ptr);
	sv_phong_desk_ptr->set_ks(0.1);
	sv_phong_desk_ptr->set_exp(20.0);

    Instance* instance_sc_desk_ptr = new Instance(sc_desk_ptr);
    instance_sc_desk_ptr->scale(0.3, 0.2, 0.3);
    instance_sc_desk_ptr->translate(-0.4, 0.3, -3);
    instance_sc_desk_ptr->set_material(sv_phong_desk_ptr);
	add_object(instance_sc_desk_ptr);


////////////////////////////////////////cushion, SolidCylinder////////////////////////////////////////////////

    SolidCylinder* sc_cushion_ptr = new SolidCylinder();

	// SolidCylinderChecker:
	SolidCylinderChecker* sc_checker_cushion_ptr = new SolidCylinderChecker();
	sc_checker_cushion_ptr->set_num_horizontal_checkers(6);
	sc_checker_cushion_ptr->set_num_vertical_checkers(2);
	sc_checker_cushion_ptr->set_num_radius_checkers_disk(3);
	sc_checker_cushion_ptr->set_horizontal_line_width(0.01);
	sc_checker_cushion_ptr->set_vertical_line_width(0.01);
    sc_checker_cushion_ptr->set_color1(0.5, 0.2, 0.065);
    sc_checker_cushion_ptr->set_color2(0.4);
    sc_checker_cushion_ptr->set_line_color(1.0, 0.0, 0.0);

	// material:

	SV_Phong* sv_phong_sc_cushion_ptr = new SV_Phong;
	sv_phong_sc_cushion_ptr->set_ka(0.25);
	sv_phong_sc_cushion_ptr->set_kd(0.75);
	sv_phong_sc_cushion_ptr->set_cd(sc_checker_cushion_ptr);
	sv_phong_sc_cushion_ptr->set_ks(0.1);
	sv_phong_sc_cushion_ptr->set_exp(20.0);

	Instance* instance_sc_cushion_ptr = new Instance(sc_cushion_ptr);
	instance_sc_cushion_ptr->scale(0.15, 0.05, 0.15);
	instance_sc_cushion_ptr->translate(-1, 0.15, -3);
	instance_sc_cushion_ptr->set_material(sv_phong_sc_cushion_ptr);
    add_object(instance_sc_cushion_ptr);


////////////////////////////////////////teapot, bezier patches////////////////////////////////////////////////

	Grid* grid_teapot_ptr = new Grid;

    int patches[32][16];
    float vertices[306][3];
    get_teapot_data(patches, vertices);
    grid_teapot_ptr->tessellate_flat_bezier_patches(40, 40, vertices, patches, 32);
	grid_teapot_ptr->setup_cells();

	// noise:

	CubicNoise* noise_teapot_ptr = new CubicNoise;
	noise_teapot_ptr->set_num_octaves(6);
	noise_teapot_ptr->set_gain(0.5);			// not relevant when num_octaves = 1
	noise_teapot_ptr->set_lacunarity(2.0);     // not relevant when num_octaves = 1

	// ramp image:

	Image* image_teapot_ptr = new Image;
	image_teapot_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/BlueMarbleRamp.ppm");

	// marble texture:

	FBmTextureRamp* marble_teapot_ptr = new FBmTextureRamp(image_teapot_ptr);
	marble_teapot_ptr->set_noise(noise_teapot_ptr);
	marble_teapot_ptr->set_perturbation(4.0);

	InstanceTexture* it_teapot_ptr = new InstanceTexture(marble_teapot_ptr);
//	it_teapot_ptr->scale(0.1);
//	it_teapot_ptr->rotate_x(-60);
//	it_teapot_ptr->rotate_y(-60);
	it_teapot_ptr->rotate_z(110);
	it_teapot_ptr->translate(1.0, 4.0, 0.0);

	// material:

	SV_Phong* sv_phong_teapot_ptr = new SV_Phong;
	sv_phong_teapot_ptr->set_ka(0.25);
	sv_phong_teapot_ptr->set_kd(0.75);
	sv_phong_teapot_ptr->set_cd(it_teapot_ptr);
	sv_phong_teapot_ptr->set_ks(0.1);
	sv_phong_teapot_ptr->set_exp(20.0);

	Instance* instance_teapot_ptr = new Instance(grid_teapot_ptr);
    instance_teapot_ptr->rotate_x(-90);
    instance_teapot_ptr->rotate_y(160);
	instance_teapot_ptr->scale(0.06);
	instance_teapot_ptr->translate(-0.25, 0.5, -2.9);
	instance_teapot_ptr->set_material(sv_phong_teapot_ptr);
    add_object(instance_teapot_ptr);

////////////////////////////////////////cup, rotational sweeping////////////////////////////////////////////////


	Grid* grid_cup_ptr = new Grid;

	Point2D ctrl_points1[6] = {Point2D(-1.0,  5.0), Point2D( 2.0,  4.0),
                               Point2D( 2.0,  1.0), Point2D(-0.5,  1.0),
                               Point2D( 1.5, -3.0), Point2D( 3.0,  0.0)};
    grid_cup_ptr->tessellate_flat_rotational_sweeping(200, 50, ctrl_points1, 6, false);
	grid_cup_ptr->setup_cells();

	// noise:

	CubicNoise* noise_cup_ptr = new CubicNoise;
	noise_cup_ptr->set_num_octaves(6);
	noise_cup_ptr->set_gain(0.5);			// not relevant when num_octaves = 1
	noise_cup_ptr->set_lacunarity(2.0);     // not relevant when num_octaves = 1

	// ramp image:

	Image* image_cup_ptr = new Image;
	image_cup_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/BlueMarbleRamp.ppm");

	// marble texture:

	FBmTextureRamp* marble_cup_ptr = new FBmTextureRamp(image_cup_ptr);
	marble_cup_ptr->set_noise(noise_cup_ptr);
	marble_cup_ptr->set_perturbation(4.0);

	InstanceTexture* it_cup_ptr = new InstanceTexture(marble_cup_ptr);
	it_cup_ptr->rotate_z(110);
	it_cup_ptr->translate(1.0, 4.0, 0.0);

	// material:

	SV_Phong* sv_phong_cup_ptr = new SV_Phong;
	sv_phong_cup_ptr->set_ka(0.25);
	sv_phong_cup_ptr->set_kd(0.75);
	sv_phong_cup_ptr->set_cd(it_cup_ptr);
	sv_phong_cup_ptr->set_ks(0.1);
	sv_phong_cup_ptr->set_exp(20.0);

	Instance* instance_cup_ptr = new Instance(grid_cup_ptr);
	instance_cup_ptr->scale(0.0353);
	instance_cup_ptr->translate(-0.5, 0.5646, -2.85);
	instance_cup_ptr->set_material(sv_phong_cup_ptr);
    add_object(instance_cup_ptr);


////////////////////////////////////////bunny////////////////////////////////////////////////

	char* file_bunny_name = "/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/Bunny69K.ply";
	Grid* grid_bunny_ptr = new Grid(new Mesh);
	grid_bunny_ptr->read_smooth_triangles(file_bunny_name);
	grid_bunny_ptr->setup_cells();

	// noise:

	CubicNoise* noise_bunny_ptr = new CubicNoise;
	noise_bunny_ptr->set_num_octaves(6);
	noise_bunny_ptr->set_gain(0.5);			// not relevant when num_octaves = 1
	noise_bunny_ptr->set_lacunarity(2.0);     // not relevant when num_octaves = 1

	// ramp image:

	Image* image_bunny_ptr = new Image;
	image_bunny_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/BlueMarbleRamp.ppm");

	// marble texture:

	FBmTextureRamp* marble_bunny_ptr = new FBmTextureRamp(image_bunny_ptr);
	marble_bunny_ptr->set_noise(noise_bunny_ptr);
	marble_bunny_ptr->set_perturbation(4.0);

	InstanceTexture* it_bunny_ptr = new InstanceTexture(marble_bunny_ptr);
	it_bunny_ptr->rotate_z(110);
	it_bunny_ptr->translate(1.0, 4.0, 0.0);

	// material:

	SV_Phong* sv_phong_bunny_ptr = new SV_Phong;
	sv_phong_bunny_ptr->set_ka(0.25);
	sv_phong_bunny_ptr->set_kd(0.75);
	sv_phong_bunny_ptr->set_cd(it_bunny_ptr);
	sv_phong_bunny_ptr->set_ks(0.1);
	sv_phong_bunny_ptr->set_exp(20.0);

    Instance* instance_bunny_ptr = new Instance(grid_bunny_ptr);
    instance_bunny_ptr->scale(2);
    instance_bunny_ptr->rotate_y(-60);
    instance_bunny_ptr->translate(0.4, 0.0, -3);
    instance_bunny_ptr->set_material(sv_phong_bunny_ptr);
    add_object(instance_bunny_ptr);


////////////////////////////////////////bunny2////////////////////////////////////////////////

	char* file_bunny_name2 = "/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/Bunny69K.ply";
	Grid* grid_bunny_ptr2 = new Grid(new Mesh);
	grid_bunny_ptr2->read_smooth_triangles(file_bunny_name2);
	grid_bunny_ptr2->setup_cells();

	// noise:

	CubicNoise* noise_bunny_ptr2 = new CubicNoise;
	noise_bunny_ptr2->set_num_octaves(6);
	noise_bunny_ptr2->set_gain(0.5);			// not relevant when num_octaves = 1
	noise_bunny_ptr2->set_lacunarity(2.0);     // not relevant when num_octaves = 1

	// ramp image:

	Image* image_bunny_ptr2 = new Image;
	image_bunny_ptr2->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/BlueMarbleRamp.ppm");

	// marble texture:

	FBmTextureRamp* marble_bunny_ptr2 = new FBmTextureRamp(image_bunny_ptr2);
	marble_bunny_ptr2->set_noise(noise_bunny_ptr2);
	marble_bunny_ptr2->set_perturbation(4.0);

	InstanceTexture* it_bunny_ptr2 = new InstanceTexture(marble_bunny_ptr2);
	it_bunny_ptr2->scale(0.5);
	it_bunny_ptr2->rotate_z(110);
	it_bunny_ptr2->translate(1.0, 4.0, 0.0);

	// material:

	SV_Phong* sv_phong_bunny_ptr2 = new SV_Phong;
	sv_phong_bunny_ptr2->set_ka(0.25);
	sv_phong_bunny_ptr2->set_kd(0.75);
	sv_phong_bunny_ptr2->set_cd(it_bunny_ptr2);
	sv_phong_bunny_ptr2->set_ks(0.1);
	sv_phong_bunny_ptr2->set_exp(20.0);

    Instance* instance_bunny_ptr2 = new Instance(grid_bunny_ptr2);
    instance_bunny_ptr2->scale(2);
    instance_bunny_ptr2->rotate_y(-120);
    instance_bunny_ptr2->translate(0.8, 0.0, -3.3);
    instance_bunny_ptr2->set_material(sv_phong_bunny_ptr2);
    add_object(instance_bunny_ptr2);


////////////////////////////////////////boat, SpherePartConvex///////////////////////////////////////

    SpherePartConvex* spc_boat_ptr = new SpherePartConvex(Point3D(0, 0, 0), 1, 0, 360, 90, 180);

	// wood texture

	RGBColor boat_light_color(0.5, 0.2, 0.065);
	RGBColor boat_dark_color(0.05);

	Wood* wood_boat_ptr = new Wood(boat_light_color, boat_dark_color);
	wood_boat_ptr->set_grainy(1.0);
	wood_boat_ptr->set_ringy(1.0);

	InstanceTexture* transformed_wood_boat_ptr = new InstanceTexture(wood_boat_ptr);
	transformed_wood_boat_ptr->scale(0.4);
//	transformed_wood_boat_ptr->rotate_z(110);

	// material:

	SV_Phong* sv_phong_boat_ptr = new SV_Phong;
	sv_phong_boat_ptr->set_ka(0.25);
	sv_phong_boat_ptr->set_kd(0.75);
	sv_phong_boat_ptr->set_cd(transformed_wood_boat_ptr);
	sv_phong_boat_ptr->set_ks(0.1);
	sv_phong_boat_ptr->set_exp(20.0);

    Instance* instance_sc_boat_ptr = new Instance(spc_boat_ptr);
    instance_sc_boat_ptr->scale(2);
    instance_sc_boat_ptr->translate(-5, 0.8, -25);
    instance_sc_boat_ptr->set_material(sv_phong_boat_ptr);
	add_object(instance_sc_boat_ptr);


////////////////////////////////////////horse////////////////////////////////////////////////

	char* file_horse_name = "/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/Horse97K.ply";
	Grid* grid_horse_ptr = new Grid(new Mesh);
	grid_horse_ptr->read_smooth_triangles(file_horse_name);
	grid_horse_ptr->setup_cells();

	// material:

	SV_Phong* sv_phong_horse_ptr = new SV_Phong;
	sv_phong_horse_ptr->set_ka(0.25);
	sv_phong_horse_ptr->set_kd(0.75);
	sv_phong_horse_ptr->set_cd(new ConstantColor(RGBColor(0.816, 0.169, 0.09)));
	sv_phong_horse_ptr->set_ks(0.1);
	sv_phong_horse_ptr->set_exp(20.0);

    Instance* instance_horse_ptr = new Instance(grid_horse_ptr);
    instance_horse_ptr->scale(4);
    instance_horse_ptr->rotate_y(180);
    instance_horse_ptr->translate(1.5, 0.9, -8.6);
    instance_horse_ptr->set_material(sv_phong_horse_ptr);
    add_object(instance_horse_ptr);

}



#elif ChapterNum == 324 // RayTracing Scenes, 5, dragon

#include "World.h"
#include "Ambient.h"
#include "Pinhole.h"
#include "PointLight.h"
#include "Directional.h"
#include "RayCast.h"
#include "Whitted.h"
#include "Plane.h"
#include "MultiJittered.h"
#include "AmbientOccluder.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "Rectangle.h"
#include "AreaLighting.h"
#include "Instance.h"
#include "Disk.h"
#include "Grid.h"
#include "CubicNoise.h"
#include "Image.h"
#include "FBmTextureRamp.h"
#include "InstanceTexture.h"
#include "Matte.h"
#include "SV_Matte.h"
#include "SV_Phong.h"
#include "Wood.h"
#include "Image.h"
#include "ImageTexture.h"
#include "SV_Reflective.h"
#include "SV_GlossyReflector.h"
#include "FBmTextureWrapped.h"
#include "Rosette.h"
#include "BeveledWedge.h"
#include "BeveledBox.h"
#include "Phong.h"
#include "SpherePartConcave.h"

#include <iostream>
#include <fstream>
using namespace std;

#define TYPE 3
// 0: rosette
// 1: wedge
// 2: beveled box
// 3: scene
// 4: scene with grid

#if TYPE == 0
void
World::build(void) {
	int num_samples = 1;
	int a = 1;

	vp.set_hres(600/a);
	vp.set_vres(400/a);
	vp.set_samples(num_samples);

	tracer_ptr = new Whitted(this);

	Pinhole* pinhole_ptr = new Pinhole;
	pinhole_ptr->set_eye(50, 50, 30);
	pinhole_ptr->set_lookat(0);
	pinhole_ptr->set_view_distance(5000/a);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

	Directional* light_ptr2 = new Directional;
	light_ptr2->set_direction(20, 30, 30);
	light_ptr2->scale_radiance(2.0);
//	light_ptr2->set_shadows(true);
	add_light(light_ptr2);


	// rosette parameters

	int		num_rings = 3;		// maximum of 6, default value is 3
	float	hole_radius = 0.75;
	float	ring_width = 1.0;
	float	rb = 0.1;			// bevel radius
	float	y0 = -0.25;			// minimum y value
	float 	y1 = 0.25;			// minimum y value


	Rosette* rosette_ptr = new Rosette(num_rings, hole_radius, ring_width, rb, y0, y1);

	// put a different random marble texture on each wedge

    // ramp image:

    Image* image_ptr = new Image;
    image_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/BlueMarbleRamp.ppm");

	// marble texture parameters

	int num_octaves = 4;
	float lacunarity = 2.0;
	float gain = 0.5;
	float perturbation = 3.0;

	int num_objects = rosette_ptr->get_num_objects();

	for (int j = 0; j < num_objects; j++) {

        // noise:

        CubicNoise* noise_ptr = new CubicNoise;
        noise_ptr->set_num_octaves(num_octaves);
        noise_ptr->set_gain(gain);			// not relevant when num_octaves = 1
        noise_ptr->set_lacunarity(lacunarity);     // not relevant when num_octaves = 1


        // marble texture:

        FBmTextureRamp* marble_ptr = new FBmTextureRamp(image_ptr);
        marble_ptr->set_noise(noise_ptr);
        marble_ptr->set_perturbation(perturbation);


		// transformed marble texture

		InstanceTexture* wedge_marble_ptr = new InstanceTexture(marble_ptr);
		set_rand_seed(j * 10);
		wedge_marble_ptr->scale(0.25);
		wedge_marble_ptr->rotate_x(20.0 * (2.0 * rand_float() - 1.0));
		wedge_marble_ptr->rotate_y(30.0 * (2.0 * rand_float() - 1.0));
		wedge_marble_ptr->rotate_z(45.0 * (2.0 * rand_float() - 1.0));
		wedge_marble_ptr->translate(10.0 * (2.0 * rand_float() - 1.0), 20.0 * (2.0 * rand_float() - 1.0), 30.0 * (2.0 * rand_float() - 1.0));

		// marble material

		SV_Matte* sv_matte_ptr = new SV_Matte;
		sv_matte_ptr->set_ka(0.35);
		sv_matte_ptr->set_kd(0.75);
		sv_matte_ptr->set_cd(wedge_marble_ptr);

		rosette_ptr->store_material(sv_matte_ptr, j);	// store material in the specified wedge
	}

	rosette_ptr->setup_cells();
	add_object(rosette_ptr);
}
#endif // TYPE

#if TYPE == 1

void
World::build(void) {
	int num_samples = 1;
	int a = 4;

	vp.set_hres(500/a);
	vp.set_vres(400/a);
	vp.set_samples(num_samples);

	tracer_ptr = new RayCast(this);

	Pinhole* pinhole_ptr = new Pinhole;
	pinhole_ptr->set_eye(0, 40, 80); // (50, 40, 30)
	pinhole_ptr->set_lookat(0, 0, 0);
	pinhole_ptr->set_view_distance(4000/a); // 4000
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

	Directional* light_ptr2 = new Directional;
	light_ptr2->set_direction(20, 30, 30);
	light_ptr2->scale_radiance(2.5);
//	light_ptr2->set_shadows(true);
	add_light(light_ptr2);

	Matte* matte_ptr1 = new Matte;
	matte_ptr1->set_ka(0.25);
	matte_ptr1->set_kd(0.65);
	matte_ptr1->set_cd(0.5, 1, 0.5);	  // green

	// wedge1 parameters

	float y0 = -1.0;		// minimum y value
	float y1 = 2;			// maximum y value
	float r0 = 1.5;			// inner radius, 1.5
	float r1 = 3;			// outer radius
	float rb = 0.25;		// bevel radius, 0.25
	float phi0 = 0;		// minimum azimuth angle in degrees, 140
	float phi1 = 90;		// maximum azimuth angle in degrees, 350

	BeveledWedge* wedge_ptr1 = new BeveledWedge(y0, y1, r0, r1, rb, phi0, phi1);
	wedge_ptr1->set_material(matte_ptr1);
	add_object(wedge_ptr1);


	Matte* matte_ptr2 = new Matte;
	matte_ptr2->set_ka(0.25);
	matte_ptr2->set_kd(0.85);
	matte_ptr2->set_cd(1.0, 0.5, 0.0);	// orange

	// wedge2 parameters

	y0 = -1.5;		// minimum y value
	y1 = 1.25;		// minimum y value
	r0 = 0.5;		// inner radius
	r1 = 4.0;		// outer radius
	rb = 0.075;		// bevel radius
	phi0 = 110;		// minimum azimuth angle in degrees
	phi1 = 130;		// maximum azimuth angle in degrees

	BeveledWedge* wedge_ptr2 = new BeveledWedge(y0, y1, r0, r1, rb, phi0, phi1);
	wedge_ptr2->set_material(matte_ptr2);
//	add_object(wedge_ptr2);


	Matte* matte_ptr3 = new Matte;
	matte_ptr3->set_cd(1, 1, 0.0);	// yellow
	matte_ptr3->set_ka(0.25);
	matte_ptr3->set_kd(0.85);

	// wedge3 parameters

	y0 = -0.75;		// minimum y value
	y1 = 0.5;		// minimum y value
	r0 = 1.25;		// inner radius
	r1 = 3.75;		// outer radius
	rb = 0.1;		// bevel radius
	phi0 = 0;		// minimum azimuth angle in degrees
	phi1 = 90;		// maximum azimuth angle in degrees

	BeveledWedge* wedge_ptr3 = new BeveledWedge(y0, y1, r0, r1, rb, phi0, phi1);
	wedge_ptr3->set_material(matte_ptr3);
//	add_object(wedge_ptr3);
}

#endif // TYPE

#if TYPE == 2

void
World::build(void) {
	int num_samples = 1;
	int a = 4;

	vp.set_hres(600/a);
	vp.set_vres(280/a);
	vp.set_samples(num_samples);

	tracer_ptr = new RayCast(this);

	Pinhole* pinhole_ptr = new Pinhole;
	pinhole_ptr->set_eye(10, 15, 50);
	pinhole_ptr->set_lookat(0, 0.75, 0);
	pinhole_ptr->set_view_distance(4000/a);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

	PointLight* light_ptr1 = new PointLight;
	light_ptr1->set_location(10, 15, 20);
	light_ptr1->scale_radiance(3.0);
//	light_ptr1->set_shadows(true);
	add_light(light_ptr1);

	Phong* phong_ptr = new Phong;
	phong_ptr->set_ka(0.3);
	phong_ptr->set_kd(0.75);
	phong_ptr->set_cd(0.7, 0.5, 0);		// orange
	phong_ptr->set_ks(0.2);
	phong_ptr->set_exp(3);

	float bevel_radius = 0.05;  // for all objects

	// box
	// the untransformed box is centered on the origin

	Point3D p0(-0.75, -1.125, -0.75);
	Point3D p1(0.75, 1.125, 0.75);

	Instance* box_ptr = new Instance(new BeveledBox(p0, p1, bevel_radius));
	box_ptr->rotate_y(-10);
	box_ptr->translate(2.5, 0.38, -1);
	box_ptr->set_material(phong_ptr);
	add_object(box_ptr);

	// ground plane

	Matte* matte_ptr = new Matte;
	matte_ptr->set_ka(0.5);
	matte_ptr->set_kd(0.85);
	matte_ptr->set_cd(0.25);

	Plane* plane_ptr = new Plane(Point3D(0, -0.75, 0), Normal(0, 1, 0));
	plane_ptr->set_material(matte_ptr);
	add_object(plane_ptr);
}

#endif // TYPE

#if TYPE == 3
void
World::build(void) {
	int num_samples = 16;
	int a = 1;

	vp.set_hres(1000/a);
	vp.set_vres(500/a);
	vp.set_samples(num_samples);

	tracer_ptr = new Whitted(this);

	Pinhole* pinhole_ptr = new Pinhole;
	pinhole_ptr->set_eye(0.0, 2.6, 6.8);
	pinhole_ptr->set_lookat(0.0, 0.0, -8);
	pinhole_ptr->set_view_distance(600/a);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

	PointLight* point_light_ptr1 = new PointLight;
	point_light_ptr1->set_location(0, 3, -5);
	point_light_ptr1->scale_radiance(2.0);
	point_light_ptr1->set_cast_shadow(true);
	add_light(point_light_ptr1);

	PointLight* point_light_ptr2 = new PointLight;
	point_light_ptr2->set_location(0, 3, 5);
	point_light_ptr2->scale_radiance(2.0);
	point_light_ptr2->set_cast_shadow(true);
	add_light(point_light_ptr2);

////////////////////////////////////////sky, SpherePartConcave///////////////////////////////////////

    SpherePartConcave* spc_sky_ptr = new SpherePartConcave(Point3D(0, 0, 0), 1, 0, 360, 0, 90);

	// image:

	Image* image_sky_ptr = new Image;
//	image_sky_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/MorningSky.ppm");
	image_sky_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/EveningSky.ppm");

	// image based texture:

	ImageTexture* texture_image_sky_ptr = new ImageTexture;
	texture_image_sky_ptr->set_image(image_sky_ptr);

    InstanceTexture* it_image_sky_ptr = new InstanceTexture(texture_image_sky_ptr);
    it_image_sky_ptr->scale(1000000);

	// material:

	SV_Phong* sv_phong_sky_ptr = new SV_Phong;
	sv_phong_sky_ptr->set_ka(0.1);
	sv_phong_sky_ptr->set_kd(0.25);
	sv_phong_sky_ptr->set_cd(texture_image_sky_ptr);
	sv_phong_sky_ptr->set_ks(0.1);
	sv_phong_sky_ptr->set_exp(20.0);


    Instance* instance_spc_sky_ptr = new Instance(spc_sky_ptr);
    instance_spc_sky_ptr->scale(1000000);
    instance_spc_sky_ptr->rotate_x(-5);
//    instance_spc_sky_ptr->translate(0, -1000, 0);
    instance_spc_sky_ptr->set_material(sv_phong_sky_ptr);
	add_object(instance_spc_sky_ptr);



////////////////////////////////////////floor, Rosette///////////////////////////////////////

	// rosette parameters

	int		num_rings = 3;		// maximum of 6
	float	hole_radius = 1.0;
	float	ring_width = 1.0;
	float	rb = 0.025;			// bevel radius
	float	y0 = -0.1;			// minimum y value
	float 	y1 = 0.1;			// minimum y value


	Rosette* rosette_floor_ptr = new Rosette(num_rings, hole_radius, ring_width, rb, y0, y1);

	// put a different random marble texture on each wedge

    // ramp image:

    Image* image_floor_ptr = new Image;
    image_floor_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/BlueMarbleRamp.ppm");

	// marble texture parameters

	int floor_num_octaves = 4;
	float floor_lacunarity = 2.0;
	float floor_gain = 0.5;
	float floor_perturbation = 3.0;

	int floor_num_objects = rosette_floor_ptr->get_num_objects();

	for (int j = 0; j < floor_num_objects; j++) {

        // noise:

        CubicNoise* noise_floor_ptr = new CubicNoise;
        noise_floor_ptr->set_num_octaves(floor_num_octaves);
        noise_floor_ptr->set_gain(floor_gain);			// not relevant when num_octaves = 1
        noise_floor_ptr->set_lacunarity(floor_lacunarity);     // not relevant when num_octaves = 1


        // marble texture:

        FBmTextureRamp* marble_floor_ptr = new FBmTextureRamp(image_floor_ptr);
        marble_floor_ptr->set_noise(noise_floor_ptr);
        marble_floor_ptr->set_perturbation(floor_perturbation);


		// transformed marble texture

		InstanceTexture* wedge_marble_floor_ptr = new InstanceTexture(marble_floor_ptr);
		set_rand_seed(j * 10);
		wedge_marble_floor_ptr->scale(0.25);
		wedge_marble_floor_ptr->rotate_x(20.0 * (2.0 * rand_float() - 1.0));
		wedge_marble_floor_ptr->rotate_y(30.0 * (2.0 * rand_float() - 1.0));
		wedge_marble_floor_ptr->rotate_z(45.0 * (2.0 * rand_float() - 1.0));
		wedge_marble_floor_ptr->translate(10.0 * (2.0 * rand_float() - 1.0), 20.0 * (2.0 * rand_float() - 1.0), 30.0 * (2.0 * rand_float() - 1.0));

        // material:

        SV_Phong* sv_phong_floor_ptr = new SV_Phong;
        sv_phong_floor_ptr->set_ka(0.25);
        sv_phong_floor_ptr->set_kd(0.75);
        sv_phong_floor_ptr->set_cd(wedge_marble_floor_ptr);
        sv_phong_floor_ptr->set_ks(0.1);
        sv_phong_floor_ptr->set_exp(20.0);

		rosette_floor_ptr->store_material(sv_phong_floor_ptr, j);	// store material in the specified wedge
	}

	rosette_floor_ptr->setup_cells();
	add_object(rosette_floor_ptr);



////////////////////////////////////////platform, beveled box and rectangle//////////////////////////

    // noise:

    CubicNoise* noise_bevel_b_platform_ptr = new CubicNoise;
    noise_bevel_b_platform_ptr->set_num_octaves(6);
    noise_bevel_b_platform_ptr->set_gain(2);			// not relevant when num_octaves = 1
    noise_bevel_b_platform_ptr->set_lacunarity(0.5);     // not relevant when num_octaves = 1

    // ramp image:

    Image* image_bevel_b_platform_ptr = new Image;
    image_bevel_b_platform_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/sandstone_ramp4.ppm");

    // marble texture:

    FBmTextureRamp* sandstone_bevel_b_platform_ptr = new FBmTextureRamp(image_bevel_b_platform_ptr);
    sandstone_bevel_b_platform_ptr->set_noise(noise_bevel_b_platform_ptr);
    sandstone_bevel_b_platform_ptr->set_perturbation(0.1);


    // transformed sandstone texture

    InstanceTexture* wedge_sandstone_bevel_b_platform_ptr = new InstanceTexture(sandstone_bevel_b_platform_ptr);
    set_rand_seed(20);
    wedge_sandstone_bevel_b_platform_ptr->scale(0.25);
    wedge_sandstone_bevel_b_platform_ptr->rotate_x(20.0 * (2.0 * rand_float() - 1.0));
    wedge_sandstone_bevel_b_platform_ptr->rotate_y(30.0 * (2.0 * rand_float() - 1.0));
    wedge_sandstone_bevel_b_platform_ptr->rotate_z(45.0 * (2.0 * rand_float() - 1.0));
    wedge_sandstone_bevel_b_platform_ptr->translate(10.0 * (2.0 * rand_float() - 1.0), 20.0 * (2.0 * rand_float() - 1.0), 30.0 * (2.0 * rand_float() - 1.0));


    // material:

    SV_Phong* sv_phong_bevel_b_platform_ptr = new SV_Phong;
    sv_phong_bevel_b_platform_ptr->set_ka(0.25);
    sv_phong_bevel_b_platform_ptr->set_kd(0.75);
    sv_phong_bevel_b_platform_ptr->set_cd(wedge_sandstone_bevel_b_platform_ptr);
    sv_phong_bevel_b_platform_ptr->set_ks(0.1);
    sv_phong_bevel_b_platform_ptr->set_exp(20.0);

    BeveledBox* bevel_b_platform_ptr1 = new BeveledBox(Point3D(-2.9, 0.1,-1.9), Point3D(-2.7, 0.3,  1.9), 0.1);
    bevel_b_platform_ptr1->set_material(sv_phong_bevel_b_platform_ptr);
    add_object(bevel_b_platform_ptr1);
    BeveledBox* bevel_b_platform_ptr2 = new BeveledBox(Point3D( 2.7, 0.1,-1.9), Point3D( 2.9, 0.3,  1.9), 0.1);
    bevel_b_platform_ptr2->set_material(sv_phong_bevel_b_platform_ptr);
    add_object(bevel_b_platform_ptr2);
    BeveledBox* bevel_b_platform_ptr3 = new BeveledBox(Point3D(-2.7, 0.1, 1.7), Point3D( 2.7, 0.3,  1.9), 0.1);
    bevel_b_platform_ptr3->set_material(sv_phong_bevel_b_platform_ptr);
    add_object(bevel_b_platform_ptr3);
    BeveledBox* bevel_b_platform_ptr4 = new BeveledBox(Point3D(-2.7, 0.1,-1.9), Point3D( 2.7, 0.3, -1.7), 0.1);
    bevel_b_platform_ptr4->set_material(sv_phong_bevel_b_platform_ptr);
    add_object(bevel_b_platform_ptr4);


    Rectangle* rectangle_platform_ptr = new Rectangle(Point3D(-2.7, 0.2, 1.7), Vector3D(5.4, 0, 0), Vector3D(0, 0, -3.4));

	// noise:

	CubicNoise* noise_rectangle_platform_ptr = new CubicNoise;
	noise_rectangle_platform_ptr->set_num_octaves(8);
	noise_rectangle_platform_ptr->set_gain(0.5);
	noise_rectangle_platform_ptr->set_lacunarity(8.0);

	FBmTextureWrapped* texture_rectangle_platform_ptr = new FBmTextureWrapped(noise_rectangle_platform_ptr);
	texture_rectangle_platform_ptr->set_expansion_number(8.0);
    texture_rectangle_platform_ptr->set_color(0.7, 1.0, 0.5);   // light green
	texture_rectangle_platform_ptr->set_min_value(0.0);
	texture_rectangle_platform_ptr->set_max_value(1.0);

	// textured material:

	float exp_rectangle_platform = 1000.0;
	SV_GlossyReflector* sv_glossy_rectangle_platform_ptr = new SV_GlossyReflector;
	sv_glossy_rectangle_platform_ptr->set_samples(num_samples, exp_rectangle_platform);
	sv_glossy_rectangle_platform_ptr->set_ka(0.1);
	sv_glossy_rectangle_platform_ptr->set_kd(0.25);
	sv_glossy_rectangle_platform_ptr->set_ks(0.1);
	sv_glossy_rectangle_platform_ptr->set_exp(exp_rectangle_platform);
	sv_glossy_rectangle_platform_ptr->set_cd(texture_rectangle_platform_ptr);
	sv_glossy_rectangle_platform_ptr->set_kr(0.75);
	sv_glossy_rectangle_platform_ptr->set_exponent(exp_rectangle_platform);
	sv_glossy_rectangle_platform_ptr->set_cr(new ConstantColor(white));

    Instance* instance_sc_rectangle_platform_ptr = new Instance(rectangle_platform_ptr);
    instance_sc_rectangle_platform_ptr->set_material(sv_glossy_rectangle_platform_ptr);
	add_object(instance_sc_rectangle_platform_ptr);


////////////////////////////////////////steps and roof, Wedge///////////////////////////////////////

    double angle_start_s = 0;
    double angle_end_s = 360;
    int num_wedges = 9;
    double y0_s = - 0.1;
    double y1_s = 0.3;
    double rb_s = 0.025;
    double y0_r = 3.7;
    double y1_r = 4.1;
    double r0_s = 5.0;
    double r1_s = 7.0;

    for (int sj = 0; sj < num_wedges; sj++) {
        double angle_width = (angle_end_s - angle_start_s) / num_wedges;  // the azimuth angle extent of each wedge
        double phi0_s = angle_start_s + sj * angle_width;
        double phi1_s = phi0_s + angle_width;
        BeveledWedge* wedge_steps_ptr = new BeveledWedge(y0_s, y1_s, r0_s, r1_s, rb_s, phi0_s , phi1_s);
        BeveledWedge* wedge_roof_ptr = new BeveledWedge(y0_r, y1_r, r0_s, r1_s, rb_s, phi0_s , phi1_s);

        // noise:

        CubicNoise* noise_step_ptr = new CubicNoise;
        noise_step_ptr->set_num_octaves(6);
        noise_step_ptr->set_gain(2);			// not relevant when num_octaves = 1
        noise_step_ptr->set_lacunarity(0.5);     // not relevant when num_octaves = 1

        // ramp image:

        Image* image_step_ptr = new Image;
        image_step_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/sandstone_ramp4.ppm");

        // marble texture:

        FBmTextureRamp* sandstone_step_ptr = new FBmTextureRamp(image_step_ptr);
        sandstone_step_ptr->set_noise(noise_step_ptr);
        sandstone_step_ptr->set_perturbation(0.1);


		// transformed sandstone texture

		InstanceTexture* wedge_sandstone_step_ptr = new InstanceTexture(sandstone_step_ptr);
		set_rand_seed(20);
		wedge_sandstone_step_ptr->scale(0.25);
		wedge_sandstone_step_ptr->rotate_x(20.0 * (2.0 * rand_float() - 1.0));
		wedge_sandstone_step_ptr->rotate_y(30.0 * (2.0 * rand_float() - 1.0));
		wedge_sandstone_step_ptr->rotate_z(45.0 * (2.0 * rand_float() - 1.0));
		wedge_sandstone_step_ptr->translate(10.0 * (2.0 * rand_float() - 1.0), 20.0 * (2.0 * rand_float() - 1.0), 30.0 * (2.0 * rand_float() - 1.0));


        // material:

        SV_Phong* sv_phong_step_ptr = new SV_Phong;
        sv_phong_step_ptr->set_ka(0.25);
        sv_phong_step_ptr->set_kd(0.75);
        sv_phong_step_ptr->set_cd(wedge_sandstone_step_ptr);
        sv_phong_step_ptr->set_ks(0.1);
        sv_phong_step_ptr->set_exp(20.0);

        wedge_steps_ptr->set_material(sv_phong_step_ptr);
        add_object(wedge_steps_ptr);

        wedge_roof_ptr->set_material(sv_phong_step_ptr);
        add_object(wedge_roof_ptr);
    }


////////////////////////////////////////pillar, Rosette///////////////////////////////////////

	// rosette parameters

	int		num_rings_p = 1;		// maximum of 6
	float	hole_radius_p = 0.05;
	float	ring_width_p = 0.25;
	float	rb_p = 0.02;			// bevel radius
	float	y0_p = 0.3;			// minimum y value
	float 	y1_p = 3.7;			// minimum y value


    int pillar_num = 4;
    double pillar_angle_start = -45;
    double pillar_angle_end = 315;
    double pillar_angle_width = (pillar_angle_end - pillar_angle_start) / pillar_num;

    int rosette_num_pillar = 20;
    double rosette_height = (y1_p - y0_p) / rosette_num_pillar;

    for (int pi=0; pi<=pillar_num; pi++) {
        double phi0_pillar = (pillar_angle_start + pillar_angle_width * pi) * PI_ON_180;

        double pillar_center_x = - (hole_radius_p + ring_width_p + r0_s) * sin((phi0_pillar-1e-6));
        double pillar_center_z =   (hole_radius_p + ring_width_p + r0_s) * cos((phi0_pillar-1e-6));

        for (int ri=0; ri<rosette_num_pillar; ri++) {
            double rosette_y0_temp = y0_p + ri*rosette_height;
            double rosette_y1_temp = rosette_y0_temp + rosette_height;

            Rosette* rosette_pillar_ptr = new Rosette(num_rings_p, hole_radius_p, ring_width_p, rb_p, rosette_y0_temp, rosette_y1_temp);

            // put a different random marble texture on each wedge

            // ramp image:

            Image* image_rp_ptr = new Image;
            image_rp_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/sandstone_ramp3.ppm");

            // marble texture parameters

            int num_octaves_rp = 4;
            float lacunarity_rp = 2.0;
            float gain_rp = 0.5;
            float perturbation_rp = 0.1;

            int num_objects_rp = rosette_pillar_ptr->get_num_objects();

            for (int rpj = 0; rpj < num_objects_rp; rpj++) {

                // noise:

                CubicNoise* noise_rp_ptr = new CubicNoise;
                noise_rp_ptr->set_num_octaves(num_octaves_rp);
                noise_rp_ptr->set_gain(gain_rp);			// not relevant when num_octaves = 1
                noise_rp_ptr->set_lacunarity(lacunarity_rp);     // not relevant when num_octaves = 1


                // marble texture:

                FBmTextureRamp* sandstone_rp_ptr = new FBmTextureRamp(image_rp_ptr);
                sandstone_rp_ptr->set_noise(noise_rp_ptr);
                sandstone_rp_ptr->set_perturbation(perturbation_rp);


                // transformed marble texture

                InstanceTexture* wedge_sandstone_rp_ptr = new InstanceTexture(sandstone_rp_ptr);
                set_rand_seed(rpj * 10);
                wedge_sandstone_rp_ptr->scale(0.25);
                wedge_sandstone_rp_ptr->rotate_x(20.0 * (2.0 * rand_float() - 1.0));
                wedge_sandstone_rp_ptr->rotate_y(30.0 * (2.0 * rand_float() - 1.0));
                wedge_sandstone_rp_ptr->rotate_z(45.0 * (2.0 * rand_float() - 1.0));
                wedge_sandstone_rp_ptr->translate(10.0 * (2.0 * rand_float() - 1.0), 20.0 * (2.0 * rand_float() - 1.0), 30.0 * (2.0 * rand_float() - 1.0));

                // material:

                SV_Phong* sv_phong_rp_ptr = new SV_Phong;
                sv_phong_rp_ptr->set_ka(0.25);
                sv_phong_rp_ptr->set_kd(0.75);
                sv_phong_rp_ptr->set_cd(wedge_sandstone_rp_ptr);
                sv_phong_rp_ptr->set_ks(0.1);
                sv_phong_rp_ptr->set_exp(20.0);

                rosette_pillar_ptr->store_material(sv_phong_rp_ptr, rpj);	// store material in the specified wedge
            }

            rosette_pillar_ptr->setup_cells();

            Instance* instance_rp_ptr = new Instance(rosette_pillar_ptr);
            instance_rp_ptr->translate(pillar_center_x, 0, pillar_center_z);
            add_object(instance_rp_ptr);
        }
    }



////////////////////////////////////////door and wall, Wedge///////////////////////////////////////

    double _wall_width      = 0.5;
    double _frame_width     = 0.1;
    double _window_width    = 0.8;
    double _door_width      = 0.95;
    double _gap_width       = 0.1;
    double unit             = 5.0; // (wall+frame+window+frame+door)*2+gap


    double _wall_width_rate     = _wall_width    / unit;
    double _frame_width_rate    = _frame_width   / unit;
    double _window_width_rate   = _window_width  / unit;
    double _door_width_rate     = _door_width    / unit;
    double _gap_width_rate      = _gap_width     / unit;

    int unit_num = 10;
    double angle_start = 0; // -90,270
    double angle_end = 360;
    double unit_angle_width = (angle_end - angle_start) / unit_num;

    double _wall_angle_width    = _wall_width_rate      * unit_angle_width;
    double _frame_angle_width   = _frame_width_rate     * unit_angle_width;
    double _window_angle_width  = _window_width_rate    * unit_angle_width;
    double _door_angle_width    = _door_width_rate      * unit_angle_width;
    double _gap_angle_width     = _gap_width_rate       * unit_angle_width;

    double y10_unit = 0.3;          // bottom of the unit
    double y11_unit = 3.7;          // top of the unit
    double y20_unit = 0.3;          // the height of frame is 0.1
    double y21_unit = 0.4;
    double y30_unit = 0.4;          // the height of window is 1
    double y31_unit = 1.4;
    double y40_unit = 1.4;
    double y41_unit = 1.5;
    double y50_unit = 1.5;
    double y51_unit = 2.5;
    double y60_unit = 2.5;
    double y61_unit = 2.6;
    double y70_unit = 2.6;
    double y71_unit = 3.6;
    double y80_unit = 3.6;
    double y81_unit = 3.7;

    double r10_unit = 6.9;          // inner radius for phong material objects: wall, frame
    double r11_unit = 7.0;          // outer radius for phong material objects: wall, frame
    double r20_unit = 6.94;         // inner radius for glossy material objects: door, window
    double r21_unit = 6.96;         // outer radius for glossy material objects: door, window
    double rb1_unit  = 0.01;        // beveled radius for phong material objects
    double rb2_unit  = 0.005;       // beveled radius for glossy material objects

//---------------------------------------------wall material-----------------------------------

    // noise:

    CubicNoise* noise_wall_ptr1 = new CubicNoise;
    noise_wall_ptr1->set_num_octaves(6);
    noise_wall_ptr1->set_gain(0.5);			// not relevant when num_octaves = 1
    noise_wall_ptr1->set_lacunarity(2.0);     // not relevant when num_octaves = 1

    // ramp image:

    Image* image_wall_ptr1 = new Image;
//    image_wall_ptr1->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/BlueMarbleRamp.ppm");
    image_wall_ptr1->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/sandstone_ramp1.ppm");

    // marble texture:

    FBmTextureRamp* marble_wall_ptr1 = new FBmTextureRamp(image_wall_ptr1);
    marble_wall_ptr1->set_noise(noise_wall_ptr1);
    marble_wall_ptr1->set_perturbation(0.2);

    InstanceTexture* it_wall_ptr1 = new InstanceTexture(marble_wall_ptr1);
    it_wall_ptr1->scale(0.5);
    it_wall_ptr1->rotate_z(110);
    it_wall_ptr1->translate(1.0, 4.0, 0.0);

    // material:

    SV_Phong* sv_phong_wall_ptr1 = new SV_Phong;
    sv_phong_wall_ptr1->set_ka(0.25);
    sv_phong_wall_ptr1->set_kd(0.75);
    sv_phong_wall_ptr1->set_cd(it_wall_ptr1);
    sv_phong_wall_ptr1->set_ks(0.1);
    sv_phong_wall_ptr1->set_exp(20.0);

//---------------------------------------------vertical frame material-----------------------------------

    // wood texture

    RGBColor frame_light_color1(0.8274, 0.5059, 0.2392);
    RGBColor frame_dark_color1(0.05);

    Wood* wood_frame_ptr1 = new Wood(frame_light_color1, frame_dark_color1);
    wood_frame_ptr1->set_grainy(1.0);
    wood_frame_ptr1->set_ringy(1.0);

    InstanceTexture* transformed_wood_frame_ptr1 = new InstanceTexture(wood_frame_ptr1);
    set_rand_seed(40);
    transformed_wood_frame_ptr1->scale(0.4);
    transformed_wood_frame_ptr1->rotate_x(20.0 * (2.0 * rand_float() - 1.0));
    transformed_wood_frame_ptr1->rotate_y(30.0 * (2.0 * rand_float() - 1.0));
    transformed_wood_frame_ptr1->rotate_z(45.0 * (2.0 * rand_float() - 1.0));

    // material:

    SV_Phong* sv_phong_frame_ptr1 = new SV_Phong;
    sv_phong_frame_ptr1->set_ka(0.25);
    sv_phong_frame_ptr1->set_kd(0.75);
    sv_phong_frame_ptr1->set_cd(transformed_wood_frame_ptr1);
    sv_phong_frame_ptr1->set_ks(0.1);
    sv_phong_frame_ptr1->set_exp(20.0);

//---------------------------------------------horizontal frame material-----------------------------------

    // wood texture

    RGBColor frame_light_color2(0.8274, 0.5059, 0.2392);
    RGBColor frame_dark_color2(0.05);

    Wood* wood_frame_ptr2 = new Wood(frame_light_color2, frame_dark_color2);
    wood_frame_ptr2->set_grainy(1.0);
    wood_frame_ptr2->set_ringy(1.0);

    InstanceTexture* transformed_wood_frame_ptr2 = new InstanceTexture(wood_frame_ptr2);
    set_rand_seed(80);
    transformed_wood_frame_ptr2->scale(0.8);
    transformed_wood_frame_ptr2->rotate_x(20.0 * (2.0 * rand_float() - 1.0));
    transformed_wood_frame_ptr2->rotate_y(30.0 * (2.0 * rand_float() - 1.0));
    transformed_wood_frame_ptr2->rotate_z(45.0 * (2.0 * rand_float() - 1.0));

    // material:

    SV_Phong* sv_phong_frame_ptr2 = new SV_Phong;
    sv_phong_frame_ptr2->set_ka(0.25);
    sv_phong_frame_ptr2->set_kd(0.75);
    sv_phong_frame_ptr2->set_cd(transformed_wood_frame_ptr2);
    sv_phong_frame_ptr2->set_ks(0.1);
    sv_phong_frame_ptr2->set_exp(20.0);

//---------------------------------------------window material-----------------------------------

	// textured material:

	float exp_window = 100000.0; // 10000
	SV_GlossyReflector* sv_glossy_window_ptr = new SV_GlossyReflector;
	sv_glossy_window_ptr->set_samples(num_samples, exp_window);
	sv_glossy_window_ptr->set_ka(0.1);
	sv_glossy_window_ptr->set_kd(0.25);
	sv_glossy_window_ptr->set_ks(0.1);
	sv_glossy_window_ptr->set_exp(exp_window);
	sv_glossy_window_ptr->set_cd(new ConstantColor(RGBColor(0.5)));
	sv_glossy_window_ptr->set_kr(0.75);
	sv_glossy_window_ptr->set_exponent(exp_window);
	sv_glossy_window_ptr->set_cr(new ConstantColor(white));


    for (int ui=0; ui<unit_num; ui++) {
        double phi0_unit = angle_start + unit_angle_width * ui;
        double phi1_unit = angle_start + unit_angle_width * (ui+1);

        double phi0_wall1 = phi0_unit;
        double phi1_wall1 = phi0_wall1 + _wall_angle_width;

        BeveledWedge* wedge_wall_ptr1 = new BeveledWedge(y10_unit, y11_unit, r10_unit, r11_unit, rb1_unit, phi0_wall1 , phi1_wall1);
        wedge_wall_ptr1->set_material(sv_phong_wall_ptr1);
        add_object(wedge_wall_ptr1);


        double phi0_frame11 = phi1_wall1;
        double phi1_frame11 = phi1_wall1 + _frame_angle_width;

        BeveledWedge* wedge_frame11_ptr = new BeveledWedge(y10_unit, y11_unit, r10_unit, r11_unit, rb1_unit, phi0_frame11 , phi1_frame11);
        wedge_frame11_ptr->set_material(sv_phong_frame_ptr1);
        add_object(wedge_frame11_ptr);


        double phi0_frame21 = phi1_frame11;
        double phi1_frame21 = phi0_frame21 + _window_angle_width;

        BeveledWedge* wedge_frame21_ptr1 = new BeveledWedge(y20_unit, y21_unit, r10_unit, r11_unit, rb1_unit, phi0_frame21 , phi1_frame21);
        wedge_frame21_ptr1->set_material(sv_phong_frame_ptr2);
        add_object(wedge_frame21_ptr1);
        BeveledWedge* wedge_frame21_ptr2 = new BeveledWedge(y40_unit, y41_unit, r10_unit, r11_unit, rb1_unit, phi0_frame21 , phi1_frame21);
        wedge_frame21_ptr2->set_material(sv_phong_frame_ptr2);
        add_object(wedge_frame21_ptr2);
        BeveledWedge* wedge_frame21_ptr3 = new BeveledWedge(y60_unit, y61_unit, r10_unit, r11_unit, rb1_unit, phi0_frame21 , phi1_frame21);
        wedge_frame21_ptr3->set_material(sv_phong_frame_ptr2);
        add_object(wedge_frame21_ptr3);
        BeveledWedge* wedge_frame21_ptr4 = new BeveledWedge(y80_unit, y81_unit, r10_unit, r11_unit, rb1_unit, phi0_frame21 , phi1_frame21);
        wedge_frame21_ptr4->set_material(sv_phong_frame_ptr2);
        add_object(wedge_frame21_ptr4);

        BeveledWedge* wedge_window_ptr1 = new BeveledWedge(y30_unit, y31_unit, r20_unit, r21_unit, rb2_unit, phi0_frame21 , phi1_frame21);
        wedge_window_ptr1->set_material(sv_glossy_window_ptr);
        add_object(wedge_window_ptr1);
        BeveledWedge* wedge_window_ptr2 = new BeveledWedge(y50_unit, y51_unit, r20_unit, r21_unit, rb2_unit, phi0_frame21 , phi1_frame21);
        wedge_window_ptr2->set_material(sv_glossy_window_ptr);
        add_object(wedge_window_ptr2);
        BeveledWedge* wedge_window_ptr3 = new BeveledWedge(y70_unit, y71_unit, r20_unit, r21_unit, rb2_unit, phi0_frame21 , phi1_frame21);
        wedge_window_ptr3->set_material(sv_glossy_window_ptr);
        add_object(wedge_window_ptr3);


        double phi0_frame12 = phi1_frame21;
        double phi1_frame12 = phi0_frame12 + _frame_angle_width;

        BeveledWedge* wedge_frame12_ptr = new BeveledWedge(y10_unit, y11_unit, r10_unit, r11_unit, rb1_unit, phi0_frame12 , phi1_frame12);
        wedge_frame12_ptr->set_material(sv_phong_frame_ptr1);
        add_object(wedge_frame12_ptr);


        double phi0_frame22 = phi1_frame12;
        double phi1_frame22 = phi0_frame22 + _door_angle_width;

        BeveledWedge* wedge_frame22_ptr1 = new BeveledWedge(y20_unit, y21_unit, r10_unit, r11_unit, rb1_unit, phi0_frame22 , phi1_frame22);
        wedge_frame22_ptr1->set_material(sv_phong_frame_ptr2);
        add_object(wedge_frame22_ptr1);
        BeveledWedge* wedge_frame22_ptr2 = new BeveledWedge(y60_unit, y61_unit, r10_unit, r11_unit, rb1_unit, phi0_frame22 , phi1_frame22);
        wedge_frame22_ptr2->set_material(sv_phong_frame_ptr2);
        add_object(wedge_frame22_ptr2);
        BeveledWedge* wedge_frame22_ptr3 = new BeveledWedge(y80_unit, y81_unit, r10_unit, r11_unit, rb1_unit, phi0_frame22 , phi1_frame22);
        wedge_frame22_ptr3->set_material(sv_phong_frame_ptr2);
        add_object(wedge_frame22_ptr3);

        BeveledWedge* wedge_door_ptr1 = new BeveledWedge(y30_unit, y51_unit, r20_unit, r21_unit, rb2_unit, phi0_frame22 , phi1_frame22);
        wedge_door_ptr1->set_material(sv_glossy_window_ptr);
        add_object(wedge_door_ptr1);
        BeveledWedge* wedge_window2_ptr1 = new BeveledWedge(y70_unit, y71_unit, r20_unit, r21_unit, rb2_unit, phi0_frame22 , phi1_frame22);
        wedge_window2_ptr1->set_material(sv_glossy_window_ptr);
        add_object(wedge_window2_ptr1);


        double phi0_frame13 = phi1_frame22;
        double phi1_frame13 = phi0_frame13 + _gap_angle_width;
        BeveledWedge* wedge_frame13_ptr = new BeveledWedge(y60_unit, y81_unit, r10_unit, r11_unit, rb1_unit, phi0_frame13 , phi1_frame13);
        wedge_frame13_ptr->set_material(sv_phong_frame_ptr1);
        add_object(wedge_frame13_ptr);


        double phi0_frame23 = phi1_frame13;
        double phi1_frame23 = phi0_frame23 + _door_angle_width;

        BeveledWedge* wedge_frame23_ptr1 = new BeveledWedge(y20_unit, y21_unit, r10_unit, r11_unit, rb1_unit, phi0_frame23 , phi1_frame23);
        wedge_frame23_ptr1->set_material(sv_phong_frame_ptr2);
        add_object(wedge_frame23_ptr1);
        BeveledWedge* wedge_frame23_ptr2 = new BeveledWedge(y60_unit, y61_unit, r10_unit, r11_unit, rb1_unit, phi0_frame23 , phi1_frame23);
        wedge_frame23_ptr2->set_material(sv_phong_frame_ptr2);
        add_object(wedge_frame23_ptr2);
        BeveledWedge* wedge_frame23_ptr3 = new BeveledWedge(y80_unit, y81_unit, r10_unit, r11_unit, rb1_unit, phi0_frame23 , phi1_frame23);
        wedge_frame23_ptr3->set_material(sv_phong_frame_ptr2);
        add_object(wedge_frame23_ptr3);

        BeveledWedge* wedge_door_ptr2 = new BeveledWedge(y30_unit, y51_unit, r20_unit, r21_unit, rb2_unit, phi0_frame23 , phi1_frame23);
        wedge_door_ptr2->set_material(sv_glossy_window_ptr);
        add_object(wedge_door_ptr2);
        BeveledWedge* wedge_window3_ptr1 = new BeveledWedge(y70_unit, y71_unit, r20_unit, r21_unit, rb2_unit, phi0_frame23 , phi1_frame23);
        wedge_window3_ptr1->set_material(sv_glossy_window_ptr);
        add_object(wedge_window3_ptr1);


        double phi0_frame14 = phi1_frame23;
        double phi1_frame14 = phi0_frame14 + _frame_angle_width;

        BeveledWedge* wedge_frame14_ptr = new BeveledWedge(y10_unit, y11_unit, r10_unit, r11_unit, rb1_unit, phi0_frame14 , phi1_frame14);
        wedge_frame14_ptr->set_material(sv_phong_frame_ptr1);
        add_object(wedge_frame14_ptr);


        double phi0_frame24 = phi1_frame14;
        double phi1_frame24 = phi0_frame24 + _window_angle_width;

        BeveledWedge* wedge_frame24_ptr1 = new BeveledWedge(y20_unit, y21_unit, r10_unit, r11_unit, rb1_unit, phi0_frame24 , phi1_frame24);
        wedge_frame24_ptr1->set_material(sv_phong_frame_ptr2);
        add_object(wedge_frame24_ptr1);
        BeveledWedge* wedge_frame24_ptr2 = new BeveledWedge(y40_unit, y41_unit, r10_unit, r11_unit, rb1_unit, phi0_frame24 , phi1_frame24);
        wedge_frame24_ptr2->set_material(sv_phong_frame_ptr2);
        add_object(wedge_frame24_ptr2);
        BeveledWedge* wedge_frame24_ptr3 = new BeveledWedge(y60_unit, y61_unit, r10_unit, r11_unit, rb1_unit, phi0_frame24 , phi1_frame24);
        wedge_frame24_ptr3->set_material(sv_phong_frame_ptr2);
        add_object(wedge_frame24_ptr3);
        BeveledWedge* wedge_frame24_ptr4 = new BeveledWedge(y80_unit, y81_unit, r10_unit, r11_unit, rb1_unit, phi0_frame24 , phi1_frame24);
        wedge_frame24_ptr4->set_material(sv_phong_frame_ptr2);
        add_object(wedge_frame24_ptr4);

        BeveledWedge* wedge_window4_ptr1 = new BeveledWedge(y30_unit, y31_unit, r20_unit, r21_unit, rb2_unit, phi0_frame24 , phi1_frame24);
        wedge_window4_ptr1->set_material(sv_glossy_window_ptr);
        add_object(wedge_window4_ptr1);
        BeveledWedge* wedge_window4_ptr2 = new BeveledWedge(y50_unit, y51_unit, r20_unit, r21_unit, rb2_unit, phi0_frame24 , phi1_frame24);
        wedge_window4_ptr2->set_material(sv_glossy_window_ptr);
        add_object(wedge_window4_ptr2);
        BeveledWedge* wedge_window4_ptr3 = new BeveledWedge(y70_unit, y71_unit, r20_unit, r21_unit, rb2_unit, phi0_frame24 , phi1_frame24);
        wedge_window4_ptr3->set_material(sv_glossy_window_ptr);
        add_object(wedge_window4_ptr3);


        double phi0_frame15 = phi1_frame24;
        double phi1_frame15 = phi0_frame15 + _frame_angle_width;

        BeveledWedge* wedge_frame15_ptr = new BeveledWedge(y10_unit, y11_unit, r10_unit, r11_unit, rb1_unit, phi0_frame15 , phi1_frame15);
        wedge_frame15_ptr->set_material(sv_phong_frame_ptr1);
        add_object(wedge_frame15_ptr);


        double phi0_wall2 = phi1_frame15;
        double phi1_wall2 = phi1_unit;

        BeveledWedge* wedge_wall_ptr2 = new BeveledWedge(y10_unit, y11_unit, r10_unit, r11_unit, rb1_unit, phi0_wall2 , phi1_wall2);
        wedge_wall_ptr2->set_material(sv_phong_wall_ptr1);
        add_object(wedge_wall_ptr2);
    }


////////////////////////////////////////dragon////////////////////////////////////////////////

	char* file_dragon_name = "/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/dragon.ply";
	Grid* grid_dragon_ptr = new Grid(new Mesh);
	grid_dragon_ptr->read_smooth_triangles(file_dragon_name);
	grid_dragon_ptr->setup_cells();

	// noise:

	CubicNoise* noise_dragon_ptr = new CubicNoise;
	noise_dragon_ptr->set_num_octaves(6);
	noise_dragon_ptr->set_gain(0.5);
	noise_dragon_ptr->set_lacunarity(2.0);

	// ramp image:

	Image* image_dragon_ptr = new Image;
	image_dragon_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/BlueMarbleRamp.ppm");

	// marble texture:

	FBmTextureRamp* marble_dragon_ptr = new FBmTextureRamp(image_dragon_ptr);
	marble_dragon_ptr->set_noise(noise_dragon_ptr);
	marble_dragon_ptr->set_perturbation(4.0);

	InstanceTexture* it_dragon_ptr = new InstanceTexture(marble_dragon_ptr);
	it_dragon_ptr->scale(0.05);
//	it_dragon_ptr->rotate_z(110);
//	it_dragon_ptr->translate(1.0, 4.0, 0.0);


	// material:

	SV_Phong* sv_phong_dragon_ptr = new SV_Phong;
	sv_phong_dragon_ptr->set_ka(0.25);
	sv_phong_dragon_ptr->set_kd(0.75);
	sv_phong_dragon_ptr->set_cd(it_dragon_ptr);
	sv_phong_dragon_ptr->set_ks(0.1);
	sv_phong_dragon_ptr->set_exp(20.0);


    Instance* instance_dragon_ptr = new Instance(grid_dragon_ptr);
    instance_dragon_ptr->scale(20, 10, 20);
    instance_dragon_ptr->translate(0, -0.3, 0);
    instance_dragon_ptr->set_material(sv_phong_dragon_ptr);

    add_object(instance_dragon_ptr);
}
#endif // TYPE

#if TYPE == 4
void
World::build(void) {
	int num_samples = 1;
	int a = 5;

	vp.set_hres(1000/a);
	vp.set_vres(500/a);
	vp.set_samples(num_samples);

	tracer_ptr = new Whitted(this);

	Pinhole* pinhole_ptr = new Pinhole;
	pinhole_ptr->set_eye(0.0, 2.6, 6.8);
	pinhole_ptr->set_lookat(0.0, 0.0, -8);
	pinhole_ptr->set_view_distance(200/a);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

	PointLight* point_light_ptr1 = new PointLight;
	point_light_ptr1->set_location(0, 3, -5);
	point_light_ptr1->scale_radiance(2.0);
	point_light_ptr1->set_cast_shadow(true);
	add_light(point_light_ptr1);

	PointLight* point_light_ptr2 = new PointLight;
	point_light_ptr2->set_location(0, 3, 5);
	point_light_ptr2->scale_radiance(2.0);
	point_light_ptr2->set_cast_shadow(true);
	add_light(point_light_ptr2);


	Grid* scene_grid_ptr = new Grid;

////////////////////////////////////////sky, SpherePartConcave///////////////////////////////////////

    SpherePartConcave* spc_sky_ptr = new SpherePartConcave(Point3D(0, 0, 0), 1, 0, 360, 0, 90);

	// image:

	Image* image_sky_ptr = new Image;
//	image_sky_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/MorningSky.ppm");
	image_sky_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/EveningSky.ppm");

	// image based texture:

	ImageTexture* texture_image_sky_ptr = new ImageTexture;
	texture_image_sky_ptr->set_image(image_sky_ptr);

    InstanceTexture* it_image_sky_ptr = new InstanceTexture(texture_image_sky_ptr);
    it_image_sky_ptr->scale(1000000);

	// material:

	SV_Phong* sv_phong_sky_ptr = new SV_Phong;
	sv_phong_sky_ptr->set_ka(0.1);
	sv_phong_sky_ptr->set_kd(0.25);
	sv_phong_sky_ptr->set_cd(texture_image_sky_ptr);
	sv_phong_sky_ptr->set_ks(0.1);
	sv_phong_sky_ptr->set_exp(20.0);


    Instance* instance_spc_sky_ptr = new Instance(spc_sky_ptr);
    instance_spc_sky_ptr->scale(1000000);
    instance_spc_sky_ptr->rotate_x(-5);
//    instance_spc_sky_ptr->translate(0, -1000, 0);
    instance_spc_sky_ptr->set_material(sv_phong_sky_ptr);
	add_object(instance_spc_sky_ptr);



////////////////////////////////////////floor, Rosette///////////////////////////////////////

	// rosette parameters

	int		num_rings = 3;		// maximum of 6
	float	hole_radius = 1.0;
	float	ring_width = 1.0;
	float	rb = 0.025;			// bevel radius
	float	y0 = -0.1;			// minimum y value
	float 	y1 = 0.1;			// minimum y value


	Rosette* rosette_floor_ptr = new Rosette(num_rings, hole_radius, ring_width, rb, y0, y1);

	// put a different random marble texture on each wedge

    // ramp image:

    Image* image_floor_ptr = new Image;
    image_floor_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/BlueMarbleRamp.ppm");

	// marble texture parameters

	int floor_num_octaves = 4;
	float floor_lacunarity = 2.0;
	float floor_gain = 0.5;
	float floor_perturbation = 3.0;

	int floor_num_objects = rosette_floor_ptr->get_num_objects();

	for (int j = 0; j < floor_num_objects; j++) {

        // noise:

        CubicNoise* noise_floor_ptr = new CubicNoise;
        noise_floor_ptr->set_num_octaves(floor_num_octaves);
        noise_floor_ptr->set_gain(floor_gain);			// not relevant when num_octaves = 1
        noise_floor_ptr->set_lacunarity(floor_lacunarity);     // not relevant when num_octaves = 1


        // marble texture:

        FBmTextureRamp* marble_floor_ptr = new FBmTextureRamp(image_floor_ptr);
        marble_floor_ptr->set_noise(noise_floor_ptr);
        marble_floor_ptr->set_perturbation(floor_perturbation);


		// transformed marble texture

		InstanceTexture* wedge_marble_floor_ptr = new InstanceTexture(marble_floor_ptr);
		set_rand_seed(j * 10);
		wedge_marble_floor_ptr->scale(0.25);
		wedge_marble_floor_ptr->rotate_x(20.0 * (2.0 * rand_float() - 1.0));
		wedge_marble_floor_ptr->rotate_y(30.0 * (2.0 * rand_float() - 1.0));
		wedge_marble_floor_ptr->rotate_z(45.0 * (2.0 * rand_float() - 1.0));
		wedge_marble_floor_ptr->translate(10.0 * (2.0 * rand_float() - 1.0), 20.0 * (2.0 * rand_float() - 1.0), 30.0 * (2.0 * rand_float() - 1.0));

        // material:

        SV_Phong* sv_phong_floor_ptr = new SV_Phong;
        sv_phong_floor_ptr->set_ka(0.25);
        sv_phong_floor_ptr->set_kd(0.75);
        sv_phong_floor_ptr->set_cd(wedge_marble_floor_ptr);
        sv_phong_floor_ptr->set_ks(0.1);
        sv_phong_floor_ptr->set_exp(20.0);

		rosette_floor_ptr->store_material(sv_phong_floor_ptr, j);	// store material in the specified wedge
	}

	rosette_floor_ptr->setup_cells();
	scene_grid_ptr->add_object(rosette_floor_ptr);



////////////////////////////////////////platform, beveled box and rectangle//////////////////////////

    Grid* platform_grid_ptr = new Grid;

    // noise:

    CubicNoise* noise_bevel_b_platform_ptr = new CubicNoise;
    noise_bevel_b_platform_ptr->set_num_octaves(6);
    noise_bevel_b_platform_ptr->set_gain(2);			// not relevant when num_octaves = 1
    noise_bevel_b_platform_ptr->set_lacunarity(0.5);     // not relevant when num_octaves = 1

    // ramp image:

    Image* image_bevel_b_platform_ptr = new Image;
    image_bevel_b_platform_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/sandstone_ramp4.ppm");

    // marble texture:

    FBmTextureRamp* sandstone_bevel_b_platform_ptr = new FBmTextureRamp(image_bevel_b_platform_ptr);
    sandstone_bevel_b_platform_ptr->set_noise(noise_bevel_b_platform_ptr);
    sandstone_bevel_b_platform_ptr->set_perturbation(0.1);


    // transformed sandstone texture

    InstanceTexture* wedge_sandstone_bevel_b_platform_ptr = new InstanceTexture(sandstone_bevel_b_platform_ptr);
    set_rand_seed(20);
    wedge_sandstone_bevel_b_platform_ptr->scale(0.25);
    wedge_sandstone_bevel_b_platform_ptr->rotate_x(20.0 * (2.0 * rand_float() - 1.0));
    wedge_sandstone_bevel_b_platform_ptr->rotate_y(30.0 * (2.0 * rand_float() - 1.0));
    wedge_sandstone_bevel_b_platform_ptr->rotate_z(45.0 * (2.0 * rand_float() - 1.0));
    wedge_sandstone_bevel_b_platform_ptr->translate(10.0 * (2.0 * rand_float() - 1.0), 20.0 * (2.0 * rand_float() - 1.0), 30.0 * (2.0 * rand_float() - 1.0));


    // material:

    SV_Phong* sv_phong_bevel_b_platform_ptr = new SV_Phong;
    sv_phong_bevel_b_platform_ptr->set_ka(0.25);
    sv_phong_bevel_b_platform_ptr->set_kd(0.75);
    sv_phong_bevel_b_platform_ptr->set_cd(wedge_sandstone_bevel_b_platform_ptr);
    sv_phong_bevel_b_platform_ptr->set_ks(0.1);
    sv_phong_bevel_b_platform_ptr->set_exp(20.0);

    BeveledBox* bevel_b_platform_ptr1 = new BeveledBox(Point3D(-2.9, 0.1,-1.9), Point3D(-2.7, 0.3,  1.9), 0.1);
    bevel_b_platform_ptr1->set_material(sv_phong_bevel_b_platform_ptr);
    platform_grid_ptr->add_object(bevel_b_platform_ptr1);
    BeveledBox* bevel_b_platform_ptr2 = new BeveledBox(Point3D( 2.7, 0.1,-1.9), Point3D( 2.9, 0.3,  1.9), 0.1);
    bevel_b_platform_ptr2->set_material(sv_phong_bevel_b_platform_ptr);
    platform_grid_ptr->add_object(bevel_b_platform_ptr2);
    BeveledBox* bevel_b_platform_ptr3 = new BeveledBox(Point3D(-2.7, 0.1, 1.7), Point3D( 2.7, 0.3,  1.9), 0.1);
    bevel_b_platform_ptr3->set_material(sv_phong_bevel_b_platform_ptr);
    platform_grid_ptr->add_object(bevel_b_platform_ptr3);
    BeveledBox* bevel_b_platform_ptr4 = new BeveledBox(Point3D(-2.7, 0.1,-1.9), Point3D( 2.7, 0.3, -1.7), 0.1);
    bevel_b_platform_ptr4->set_material(sv_phong_bevel_b_platform_ptr);
    platform_grid_ptr->add_object(bevel_b_platform_ptr4);


    Rectangle* rectangle_platform_ptr = new Rectangle(Point3D(-2.7, 0.2, 1.7), Vector3D(5.4, 0, 0), Vector3D(0, 0, -3.4));

	// noise:

	CubicNoise* noise_rectangle_platform_ptr = new CubicNoise;
	noise_rectangle_platform_ptr->set_num_octaves(8);
	noise_rectangle_platform_ptr->set_gain(0.5);
	noise_rectangle_platform_ptr->set_lacunarity(8.0);

	FBmTextureWrapped* texture_rectangle_platform_ptr = new FBmTextureWrapped(noise_rectangle_platform_ptr);
	texture_rectangle_platform_ptr->set_expansion_number(8.0);
    texture_rectangle_platform_ptr->set_color(0.7, 1.0, 0.5);   // light green
	texture_rectangle_platform_ptr->set_min_value(0.0);
	texture_rectangle_platform_ptr->set_max_value(1.0);

	// textured material:

	float exp_rectangle_platform = 1000.0;
	SV_GlossyReflector* sv_glossy_rectangle_platform_ptr = new SV_GlossyReflector;
	sv_glossy_rectangle_platform_ptr->set_samples(num_samples, exp_rectangle_platform);
	sv_glossy_rectangle_platform_ptr->set_ka(0.1);
	sv_glossy_rectangle_platform_ptr->set_kd(0.25);
	sv_glossy_rectangle_platform_ptr->set_ks(0.1);
	sv_glossy_rectangle_platform_ptr->set_exp(exp_rectangle_platform);
	sv_glossy_rectangle_platform_ptr->set_cd(texture_rectangle_platform_ptr);
	sv_glossy_rectangle_platform_ptr->set_kr(0.75);
	sv_glossy_rectangle_platform_ptr->set_exponent(exp_rectangle_platform);
	sv_glossy_rectangle_platform_ptr->set_cr(new ConstantColor(white));

    Instance* instance_sc_rectangle_platform_ptr = new Instance(rectangle_platform_ptr);
    instance_sc_rectangle_platform_ptr->set_material(sv_glossy_rectangle_platform_ptr);
	platform_grid_ptr->add_object(instance_sc_rectangle_platform_ptr);

	platform_grid_ptr->setup_cells();
    scene_grid_ptr->add_object(platform_grid_ptr);

////////////////////////////////////////steps and roof, Wedge///////////////////////////////////////
    Grid* steps_roof_grid_ptr = new Grid;

    double angle_start_s = 0;
    double angle_end_s = 360;
    int num_wedges = 9;
    double y0_s = - 0.1;
    double y1_s = 0.3;
    double rb_s = 0.025;
    double y0_r = 3.7;
    double y1_r = 4.1;
    double r0_s = 5.0;
    double r1_s = 7.0;

    for (int sj = 0; sj < num_wedges; sj++) {
        double angle_width = (angle_end_s - angle_start_s) / num_wedges;  // the azimuth angle extent of each wedge
        double phi0_s = angle_start_s + sj * angle_width;
        double phi1_s = phi0_s + angle_width;
        BeveledWedge* wedge_steps_ptr = new BeveledWedge(y0_s, y1_s, r0_s, r1_s, rb_s, phi0_s , phi1_s);
        BeveledWedge* wedge_roof_ptr = new BeveledWedge(y0_r, y1_r, r0_s, r1_s, rb_s, phi0_s , phi1_s);

        // noise:

        CubicNoise* noise_step_ptr = new CubicNoise;
        noise_step_ptr->set_num_octaves(6);
        noise_step_ptr->set_gain(2);			// not relevant when num_octaves = 1
        noise_step_ptr->set_lacunarity(0.5);     // not relevant when num_octaves = 1

        // ramp image:

        Image* image_step_ptr = new Image;
        image_step_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/sandstone_ramp4.ppm");

        // marble texture:

        FBmTextureRamp* sandstone_step_ptr = new FBmTextureRamp(image_step_ptr);
        sandstone_step_ptr->set_noise(noise_step_ptr);
        sandstone_step_ptr->set_perturbation(0.1);


		// transformed sandstone texture

		InstanceTexture* wedge_sandstone_step_ptr = new InstanceTexture(sandstone_step_ptr);
		set_rand_seed(20);
		wedge_sandstone_step_ptr->scale(0.25);
		wedge_sandstone_step_ptr->rotate_x(20.0 * (2.0 * rand_float() - 1.0));
		wedge_sandstone_step_ptr->rotate_y(30.0 * (2.0 * rand_float() - 1.0));
		wedge_sandstone_step_ptr->rotate_z(45.0 * (2.0 * rand_float() - 1.0));
		wedge_sandstone_step_ptr->translate(10.0 * (2.0 * rand_float() - 1.0), 20.0 * (2.0 * rand_float() - 1.0), 30.0 * (2.0 * rand_float() - 1.0));


        // material:

        SV_Phong* sv_phong_step_ptr = new SV_Phong;
        sv_phong_step_ptr->set_ka(0.25);
        sv_phong_step_ptr->set_kd(0.75);
        sv_phong_step_ptr->set_cd(wedge_sandstone_step_ptr);
        sv_phong_step_ptr->set_ks(0.1);
        sv_phong_step_ptr->set_exp(20.0);

        wedge_steps_ptr->set_material(sv_phong_step_ptr);
        steps_roof_grid_ptr->add_object(wedge_steps_ptr);

        wedge_roof_ptr->set_material(sv_phong_step_ptr);
        steps_roof_grid_ptr->add_object(wedge_roof_ptr);
    }
    steps_roof_grid_ptr->setup_cells();
    scene_grid_ptr->add_object(steps_roof_grid_ptr);


////////////////////////////////////////pillar, Rosette///////////////////////////////////////
    Grid* pillar_grid_ptr = new Grid;
	// rosette parameters

	int		num_rings_p = 1;		// maximum of 6
	float	hole_radius_p = 0.05;
	float	ring_width_p = 0.25;
	float	rb_p = 0.02;			// bevel radius
	float	y0_p = 0.3;			// minimum y value
	float 	y1_p = 3.7;			// minimum y value


    int pillar_num = 4;
    double pillar_angle_start = -45;
    double pillar_angle_end = 315;
    double pillar_angle_width = (pillar_angle_end - pillar_angle_start) / pillar_num;

    int rosette_num_pillar = 20;
    double rosette_height = (y1_p - y0_p) / rosette_num_pillar;

    for (int pi=0; pi<=pillar_num; pi++) {
        double phi0_pillar = (pillar_angle_start + pillar_angle_width * pi) * PI_ON_180;

        double pillar_center_x = - (hole_radius_p + ring_width_p + r0_s) * sin((phi0_pillar-1e-6));
        double pillar_center_z =   (hole_radius_p + ring_width_p + r0_s) * cos((phi0_pillar-1e-6));

        Grid* single_pillar_grid_ptr = new Grid;

        for (int ri=0; ri<rosette_num_pillar; ri++) {
            double rosette_y0_temp = y0_p + ri*rosette_height;
            double rosette_y1_temp = rosette_y0_temp + rosette_height;

            Rosette* rosette_pillar_ptr = new Rosette(num_rings_p, hole_radius_p, ring_width_p, rb_p, rosette_y0_temp, rosette_y1_temp);

            // put a different random marble texture on each wedge

            // ramp image:

            Image* image_rp_ptr = new Image;
            image_rp_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/sandstone_ramp3.ppm");

            // marble texture parameters

            int num_octaves_rp = 4;
            float lacunarity_rp = 2.0;
            float gain_rp = 0.5;
            float perturbation_rp = 0.1;

            int num_objects_rp = rosette_pillar_ptr->get_num_objects();

            for (int rpj = 0; rpj < num_objects_rp; rpj++) {

                // noise:

                CubicNoise* noise_rp_ptr = new CubicNoise;
                noise_rp_ptr->set_num_octaves(num_octaves_rp);
                noise_rp_ptr->set_gain(gain_rp);			// not relevant when num_octaves = 1
                noise_rp_ptr->set_lacunarity(lacunarity_rp);     // not relevant when num_octaves = 1


                // marble texture:

                FBmTextureRamp* sandstone_rp_ptr = new FBmTextureRamp(image_rp_ptr);
                sandstone_rp_ptr->set_noise(noise_rp_ptr);
                sandstone_rp_ptr->set_perturbation(perturbation_rp);


                // transformed marble texture

                InstanceTexture* wedge_sandstone_rp_ptr = new InstanceTexture(sandstone_rp_ptr);
                set_rand_seed(rpj * 10);
                wedge_sandstone_rp_ptr->scale(0.25);
                wedge_sandstone_rp_ptr->rotate_x(20.0 * (2.0 * rand_float() - 1.0));
                wedge_sandstone_rp_ptr->rotate_y(30.0 * (2.0 * rand_float() - 1.0));
                wedge_sandstone_rp_ptr->rotate_z(45.0 * (2.0 * rand_float() - 1.0));
                wedge_sandstone_rp_ptr->translate(10.0 * (2.0 * rand_float() - 1.0), 20.0 * (2.0 * rand_float() - 1.0), 30.0 * (2.0 * rand_float() - 1.0));

                // material:

                SV_Phong* sv_phong_rp_ptr = new SV_Phong;
                sv_phong_rp_ptr->set_ka(0.25);
                sv_phong_rp_ptr->set_kd(0.75);
                sv_phong_rp_ptr->set_cd(wedge_sandstone_rp_ptr);
                sv_phong_rp_ptr->set_ks(0.1);
                sv_phong_rp_ptr->set_exp(20.0);

                rosette_pillar_ptr->store_material(sv_phong_rp_ptr, rpj);	// store material in the specified wedge
            }

            rosette_pillar_ptr->setup_cells();

            Instance* instance_rp_ptr = new Instance(rosette_pillar_ptr);
            instance_rp_ptr->translate(pillar_center_x, 0, pillar_center_z);
            single_pillar_grid_ptr->add_object(instance_rp_ptr);
        }
        single_pillar_grid_ptr->setup_cells();
        pillar_grid_ptr->add_object(single_pillar_grid_ptr);
    }
    pillar_grid_ptr->setup_cells();
    scene_grid_ptr->add_object(pillar_grid_ptr);


////////////////////////////////////////door and wall, Wedge///////////////////////////////////////

    Grid* door_wall_grid_ptr = new Grid;

    double _wall_width      = 0.5;
    double _frame_width     = 0.1;
    double _window_width    = 0.8;
    double _door_width      = 0.95;
    double _gap_width       = 0.1;
    double unit             = 5.0; // (wall+frame+window+frame+door)*2+gap


    double _wall_width_rate     = _wall_width    / unit;
    double _frame_width_rate    = _frame_width   / unit;
    double _window_width_rate   = _window_width  / unit;
    double _door_width_rate     = _door_width    / unit;
    double _gap_width_rate      = _gap_width     / unit;

    int unit_num = 10;
    double angle_start = 0;
    double angle_end = 360;
    double unit_angle_width = (angle_end - angle_start) / unit_num;

    double _wall_angle_width    = _wall_width_rate      * unit_angle_width;
    double _frame_angle_width   = _frame_width_rate     * unit_angle_width;
    double _window_angle_width  = _window_width_rate    * unit_angle_width;
    double _door_angle_width    = _door_width_rate      * unit_angle_width;
    double _gap_angle_width     = _gap_width_rate       * unit_angle_width;

    double y10_unit = 0.3;
    double y11_unit = 3.7;
    double y20_unit = 0.3;
    double y21_unit = 0.4;
    double y30_unit = 0.4;
    double y31_unit = 1.4;
    double y40_unit = 1.4;
    double y41_unit = 1.5;
    double y50_unit = 1.5;
    double y51_unit = 2.5;
    double y60_unit = 2.5;
    double y61_unit = 2.6;
    double y70_unit = 2.6;
    double y71_unit = 3.6;
    double y80_unit = 3.6;
    double y81_unit = 3.7;

    double r10_unit = 6.9;
    double r11_unit = 7.0;
    double r20_unit = 6.94;
    double r21_unit = 6.96;
    double rb1_unit  = 0.01;
    double rb2_unit  = 0.005;

//---------------------------------------------wall material-----------------------------------

    // noise:

    CubicNoise* noise_wall_ptr1 = new CubicNoise;
    noise_wall_ptr1->set_num_octaves(6);
    noise_wall_ptr1->set_gain(0.5);			// not relevant when num_octaves = 1
    noise_wall_ptr1->set_lacunarity(2.0);     // not relevant when num_octaves = 1

    // ramp image:

    Image* image_wall_ptr1 = new Image;
//    image_wall_ptr1->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/BlueMarbleRamp.ppm");
    image_wall_ptr1->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/sandstone_ramp1.ppm");

    // marble texture:

    FBmTextureRamp* marble_wall_ptr1 = new FBmTextureRamp(image_wall_ptr1);
    marble_wall_ptr1->set_noise(noise_wall_ptr1);
    marble_wall_ptr1->set_perturbation(0.2);

    InstanceTexture* it_wall_ptr1 = new InstanceTexture(marble_wall_ptr1);
    it_wall_ptr1->scale(0.5);
    it_wall_ptr1->rotate_z(110);
    it_wall_ptr1->translate(1.0, 4.0, 0.0);

    // material:

    SV_Phong* sv_phong_wall_ptr1 = new SV_Phong;
    sv_phong_wall_ptr1->set_ka(0.25);
    sv_phong_wall_ptr1->set_kd(0.75);
    sv_phong_wall_ptr1->set_cd(it_wall_ptr1);
    sv_phong_wall_ptr1->set_ks(0.1);
    sv_phong_wall_ptr1->set_exp(20.0);

//---------------------------------------------vertical frame material-----------------------------------

    // wood texture

    RGBColor frame_light_color1(0.8274, 0.5059, 0.2392);
    RGBColor frame_dark_color1(0.05);

    Wood* wood_frame_ptr1 = new Wood(frame_light_color1, frame_dark_color1);
    wood_frame_ptr1->set_grainy(1.0);
    wood_frame_ptr1->set_ringy(1.0);

    InstanceTexture* transformed_wood_frame_ptr1 = new InstanceTexture(wood_frame_ptr1);
    set_rand_seed(40);
    transformed_wood_frame_ptr1->scale(0.4);
    transformed_wood_frame_ptr1->rotate_x(20.0 * (2.0 * rand_float() - 1.0));
    transformed_wood_frame_ptr1->rotate_y(30.0 * (2.0 * rand_float() - 1.0));
    transformed_wood_frame_ptr1->rotate_z(45.0 * (2.0 * rand_float() - 1.0));

    // material:

    SV_Phong* sv_phong_frame_ptr1 = new SV_Phong;
    sv_phong_frame_ptr1->set_ka(0.25);
    sv_phong_frame_ptr1->set_kd(0.75);
    sv_phong_frame_ptr1->set_cd(transformed_wood_frame_ptr1);
    sv_phong_frame_ptr1->set_ks(0.1);
    sv_phong_frame_ptr1->set_exp(20.0);

//---------------------------------------------horizontal frame material-----------------------------------

    // wood texture

    RGBColor frame_light_color2(0.8274, 0.5059, 0.2392);
    RGBColor frame_dark_color2(0.05);

    Wood* wood_frame_ptr2 = new Wood(frame_light_color2, frame_dark_color2);
    wood_frame_ptr2->set_grainy(1.0);
    wood_frame_ptr2->set_ringy(1.0);

    InstanceTexture* transformed_wood_frame_ptr2 = new InstanceTexture(wood_frame_ptr2);
    set_rand_seed(80);
    transformed_wood_frame_ptr2->scale(0.8);
    transformed_wood_frame_ptr2->rotate_x(20.0 * (2.0 * rand_float() - 1.0));
    transformed_wood_frame_ptr2->rotate_y(30.0 * (2.0 * rand_float() - 1.0));
    transformed_wood_frame_ptr2->rotate_z(45.0 * (2.0 * rand_float() - 1.0));

    // material:

    SV_Phong* sv_phong_frame_ptr2 = new SV_Phong;
    sv_phong_frame_ptr2->set_ka(0.25);
    sv_phong_frame_ptr2->set_kd(0.75);
    sv_phong_frame_ptr2->set_cd(transformed_wood_frame_ptr2);
    sv_phong_frame_ptr2->set_ks(0.1);
    sv_phong_frame_ptr2->set_exp(20.0);

//---------------------------------------------window material-----------------------------------

	// textured material:

	float exp_window = 10000.0;
	SV_GlossyReflector* sv_glossy_window_ptr = new SV_GlossyReflector;
	sv_glossy_window_ptr->set_samples(num_samples, exp_window);
	sv_glossy_window_ptr->set_ka(0.1);
	sv_glossy_window_ptr->set_kd(0.25);
	sv_glossy_window_ptr->set_ks(0.1);
	sv_glossy_window_ptr->set_exp(exp_window);
	sv_glossy_window_ptr->set_cd(new ConstantColor(RGBColor(0.5)));
	sv_glossy_window_ptr->set_kr(0.75);
	sv_glossy_window_ptr->set_exponent(exp_window);
	sv_glossy_window_ptr->set_cr(new ConstantColor(white));


    for (int ui=0; ui<unit_num; ui++) {
        double phi0_unit = angle_start + unit_angle_width * ui;
        double phi1_unit = angle_start + unit_angle_width * (ui+1);

        double phi0_wall1 = phi0_unit;
        double phi1_wall1 = phi0_wall1 + _wall_angle_width;

        BeveledWedge* wedge_wall_ptr1 = new BeveledWedge(y10_unit, y11_unit, r10_unit, r11_unit, rb1_unit, phi0_wall1 , phi1_wall1);
        wedge_wall_ptr1->set_material(sv_phong_wall_ptr1);
        door_wall_grid_ptr->add_object(wedge_wall_ptr1);


        double phi0_frame11 = phi1_wall1;
        double phi1_frame11 = phi1_wall1 + _frame_angle_width;

        BeveledWedge* wedge_frame11_ptr = new BeveledWedge(y10_unit, y11_unit, r10_unit, r11_unit, rb1_unit, phi0_frame11 , phi1_frame11);
        wedge_frame11_ptr->set_material(sv_phong_frame_ptr1);
        door_wall_grid_ptr->add_object(wedge_frame11_ptr);


        double phi0_frame21 = phi1_frame11;
        double phi1_frame21 = phi0_frame21 + _window_angle_width;

        BeveledWedge* wedge_frame21_ptr1 = new BeveledWedge(y20_unit, y21_unit, r10_unit, r11_unit, rb1_unit, phi0_frame21 , phi1_frame21);
        wedge_frame21_ptr1->set_material(sv_phong_frame_ptr2);
        door_wall_grid_ptr->add_object(wedge_frame21_ptr1);
        BeveledWedge* wedge_frame21_ptr2 = new BeveledWedge(y40_unit, y41_unit, r10_unit, r11_unit, rb1_unit, phi0_frame21 , phi1_frame21);
        wedge_frame21_ptr2->set_material(sv_phong_frame_ptr2);
        door_wall_grid_ptr->add_object(wedge_frame21_ptr2);
        BeveledWedge* wedge_frame21_ptr3 = new BeveledWedge(y60_unit, y61_unit, r10_unit, r11_unit, rb1_unit, phi0_frame21 , phi1_frame21);
        wedge_frame21_ptr3->set_material(sv_phong_frame_ptr2);
        door_wall_grid_ptr->add_object(wedge_frame21_ptr3);
        BeveledWedge* wedge_frame21_ptr4 = new BeveledWedge(y80_unit, y81_unit, r10_unit, r11_unit, rb1_unit, phi0_frame21 , phi1_frame21);
        wedge_frame21_ptr4->set_material(sv_phong_frame_ptr2);
        door_wall_grid_ptr->add_object(wedge_frame21_ptr4);

        BeveledWedge* wedge_window_ptr1 = new BeveledWedge(y30_unit, y31_unit, r20_unit, r21_unit, rb2_unit, phi0_frame21 , phi1_frame21);
        wedge_window_ptr1->set_material(sv_glossy_window_ptr);
        door_wall_grid_ptr->add_object(wedge_window_ptr1);
        BeveledWedge* wedge_window_ptr2 = new BeveledWedge(y50_unit, y51_unit, r20_unit, r21_unit, rb2_unit, phi0_frame21 , phi1_frame21);
        wedge_window_ptr2->set_material(sv_glossy_window_ptr);
        door_wall_grid_ptr->add_object(wedge_window_ptr2);
        BeveledWedge* wedge_window_ptr3 = new BeveledWedge(y70_unit, y71_unit, r20_unit, r21_unit, rb2_unit, phi0_frame21 , phi1_frame21);
        wedge_window_ptr3->set_material(sv_glossy_window_ptr);
        door_wall_grid_ptr->add_object(wedge_window_ptr3);


        double phi0_frame12 = phi1_frame21;
        double phi1_frame12 = phi0_frame12 + _frame_angle_width;

        BeveledWedge* wedge_frame12_ptr = new BeveledWedge(y10_unit, y11_unit, r10_unit, r11_unit, rb1_unit, phi0_frame12 , phi1_frame12);
        wedge_frame12_ptr->set_material(sv_phong_frame_ptr1);
        door_wall_grid_ptr->add_object(wedge_frame12_ptr);


        double phi0_frame22 = phi1_frame12;
        double phi1_frame22 = phi0_frame22 + _door_angle_width;

        BeveledWedge* wedge_frame22_ptr1 = new BeveledWedge(y20_unit, y21_unit, r10_unit, r11_unit, rb1_unit, phi0_frame22 , phi1_frame22);
        wedge_frame22_ptr1->set_material(sv_phong_frame_ptr2);
        door_wall_grid_ptr->add_object(wedge_frame22_ptr1);
        BeveledWedge* wedge_frame22_ptr2 = new BeveledWedge(y60_unit, y61_unit, r10_unit, r11_unit, rb1_unit, phi0_frame22 , phi1_frame22);
        wedge_frame22_ptr2->set_material(sv_phong_frame_ptr2);
        door_wall_grid_ptr->add_object(wedge_frame22_ptr2);
        BeveledWedge* wedge_frame22_ptr3 = new BeveledWedge(y80_unit, y81_unit, r10_unit, r11_unit, rb1_unit, phi0_frame22 , phi1_frame22);
        wedge_frame22_ptr3->set_material(sv_phong_frame_ptr2);
        door_wall_grid_ptr->add_object(wedge_frame22_ptr3);

        BeveledWedge* wedge_door_ptr1 = new BeveledWedge(y30_unit, y51_unit, r20_unit, r21_unit, rb2_unit, phi0_frame22 , phi1_frame22);
        wedge_door_ptr1->set_material(sv_glossy_window_ptr);
        door_wall_grid_ptr->add_object(wedge_door_ptr1);
        BeveledWedge* wedge_window2_ptr1 = new BeveledWedge(y70_unit, y71_unit, r20_unit, r21_unit, rb2_unit, phi0_frame22 , phi1_frame22);
        wedge_window2_ptr1->set_material(sv_glossy_window_ptr);
        door_wall_grid_ptr->add_object(wedge_window2_ptr1);


        double phi0_frame13 = phi1_frame22;
        double phi1_frame13 = phi0_frame13 + _gap_angle_width;
        BeveledWedge* wedge_frame13_ptr = new BeveledWedge(y60_unit, y81_unit, r10_unit, r11_unit, rb1_unit, phi0_frame13 , phi1_frame13);
        wedge_frame13_ptr->set_material(sv_phong_frame_ptr1);
        door_wall_grid_ptr->add_object(wedge_frame13_ptr);


        double phi0_frame23 = phi1_frame13;
        double phi1_frame23 = phi0_frame23 + _door_angle_width;

        BeveledWedge* wedge_frame23_ptr1 = new BeveledWedge(y20_unit, y21_unit, r10_unit, r11_unit, rb1_unit, phi0_frame23 , phi1_frame23);
        wedge_frame23_ptr1->set_material(sv_phong_frame_ptr2);
        door_wall_grid_ptr->add_object(wedge_frame23_ptr1);
        BeveledWedge* wedge_frame23_ptr2 = new BeveledWedge(y60_unit, y61_unit, r10_unit, r11_unit, rb1_unit, phi0_frame23 , phi1_frame23);
        wedge_frame23_ptr2->set_material(sv_phong_frame_ptr2);
        door_wall_grid_ptr->add_object(wedge_frame23_ptr2);
        BeveledWedge* wedge_frame23_ptr3 = new BeveledWedge(y80_unit, y81_unit, r10_unit, r11_unit, rb1_unit, phi0_frame23 , phi1_frame23);
        wedge_frame23_ptr3->set_material(sv_phong_frame_ptr2);
        door_wall_grid_ptr->add_object(wedge_frame23_ptr3);

        BeveledWedge* wedge_door_ptr2 = new BeveledWedge(y30_unit, y51_unit, r20_unit, r21_unit, rb2_unit, phi0_frame23 , phi1_frame23);
        wedge_door_ptr2->set_material(sv_glossy_window_ptr);
        door_wall_grid_ptr->add_object(wedge_door_ptr2);
        BeveledWedge* wedge_window3_ptr1 = new BeveledWedge(y70_unit, y71_unit, r20_unit, r21_unit, rb2_unit, phi0_frame23 , phi1_frame23);
        wedge_window3_ptr1->set_material(sv_glossy_window_ptr);
        door_wall_grid_ptr->add_object(wedge_window3_ptr1);


        double phi0_frame14 = phi1_frame23;
        double phi1_frame14 = phi0_frame14 + _frame_angle_width;

        BeveledWedge* wedge_frame14_ptr = new BeveledWedge(y10_unit, y11_unit, r10_unit, r11_unit, rb1_unit, phi0_frame14 , phi1_frame14);
        wedge_frame14_ptr->set_material(sv_phong_frame_ptr1);
        door_wall_grid_ptr->add_object(wedge_frame14_ptr);


        double phi0_frame24 = phi1_frame14;
        double phi1_frame24 = phi0_frame24 + _window_angle_width;

        BeveledWedge* wedge_frame24_ptr1 = new BeveledWedge(y20_unit, y21_unit, r10_unit, r11_unit, rb1_unit, phi0_frame24 , phi1_frame24);
        wedge_frame24_ptr1->set_material(sv_phong_frame_ptr2);
        door_wall_grid_ptr->add_object(wedge_frame24_ptr1);
        BeveledWedge* wedge_frame24_ptr2 = new BeveledWedge(y40_unit, y41_unit, r10_unit, r11_unit, rb1_unit, phi0_frame24 , phi1_frame24);
        wedge_frame24_ptr2->set_material(sv_phong_frame_ptr2);
        door_wall_grid_ptr->add_object(wedge_frame24_ptr2);
        BeveledWedge* wedge_frame24_ptr3 = new BeveledWedge(y60_unit, y61_unit, r10_unit, r11_unit, rb1_unit, phi0_frame24 , phi1_frame24);
        wedge_frame24_ptr3->set_material(sv_phong_frame_ptr2);
        door_wall_grid_ptr->add_object(wedge_frame24_ptr3);
        BeveledWedge* wedge_frame24_ptr4 = new BeveledWedge(y80_unit, y81_unit, r10_unit, r11_unit, rb1_unit, phi0_frame24 , phi1_frame24);
        wedge_frame24_ptr4->set_material(sv_phong_frame_ptr2);
        door_wall_grid_ptr->add_object(wedge_frame24_ptr4);

        BeveledWedge* wedge_window4_ptr1 = new BeveledWedge(y30_unit, y31_unit, r20_unit, r21_unit, rb2_unit, phi0_frame24 , phi1_frame24);
        wedge_window4_ptr1->set_material(sv_glossy_window_ptr);
        door_wall_grid_ptr->add_object(wedge_window4_ptr1);
        BeveledWedge* wedge_window4_ptr2 = new BeveledWedge(y50_unit, y51_unit, r20_unit, r21_unit, rb2_unit, phi0_frame24 , phi1_frame24);
        wedge_window4_ptr2->set_material(sv_glossy_window_ptr);
        door_wall_grid_ptr->add_object(wedge_window4_ptr2);
        BeveledWedge* wedge_window4_ptr3 = new BeveledWedge(y70_unit, y71_unit, r20_unit, r21_unit, rb2_unit, phi0_frame24 , phi1_frame24);
        wedge_window4_ptr3->set_material(sv_glossy_window_ptr);
        door_wall_grid_ptr->add_object(wedge_window4_ptr3);


        double phi0_frame15 = phi1_frame24;
        double phi1_frame15 = phi0_frame15 + _frame_angle_width;

        BeveledWedge* wedge_frame15_ptr = new BeveledWedge(y10_unit, y11_unit, r10_unit, r11_unit, rb1_unit, phi0_frame15 , phi1_frame15);
        wedge_frame15_ptr->set_material(sv_phong_frame_ptr1);
        door_wall_grid_ptr->add_object(wedge_frame15_ptr);


        double phi0_wall2 = phi1_frame15;
        double phi1_wall2 = phi1_unit;

        BeveledWedge* wedge_wall_ptr2 = new BeveledWedge(y10_unit, y11_unit, r10_unit, r11_unit, rb1_unit, phi0_wall2 , phi1_wall2);
        wedge_wall_ptr2->set_material(sv_phong_wall_ptr1);
        door_wall_grid_ptr->add_object(wedge_wall_ptr2);
    }
    door_wall_grid_ptr->setup_cells();
    scene_grid_ptr->add_object(door_wall_grid_ptr);


////////////////////////////////////////dragon////////////////////////////////////////////////

	char* file_dragon_name = "/Users/libingzeng/CG/RayTraceGroundUp/PLYFiles/dragon.ply";
	Grid* grid_dragon_ptr = new Grid(new Mesh);
	grid_dragon_ptr->read_smooth_triangles(file_dragon_name);
	grid_dragon_ptr->setup_cells();

	// noise:

	CubicNoise* noise_dragon_ptr = new CubicNoise;
	noise_dragon_ptr->set_num_octaves(6);
	noise_dragon_ptr->set_gain(0.5);
	noise_dragon_ptr->set_lacunarity(2.0);

	// ramp image:

	Image* image_dragon_ptr = new Image;
	image_dragon_ptr->read_ppm_file("/Users/libingzeng/CG/RayTraceGroundUp/TextureFiles/ppm/BlueMarbleRamp.ppm");

	// marble texture:

	FBmTextureRamp* marble_dragon_ptr = new FBmTextureRamp(image_dragon_ptr);
	marble_dragon_ptr->set_noise(noise_dragon_ptr);
	marble_dragon_ptr->set_perturbation(4.0);

	InstanceTexture* it_dragon_ptr = new InstanceTexture(marble_dragon_ptr);
	it_dragon_ptr->scale(0.05);
//	it_dragon_ptr->rotate_z(110);
//	it_dragon_ptr->translate(1.0, 4.0, 0.0);


	// material:

	SV_Phong* sv_phong_dragon_ptr = new SV_Phong;
	sv_phong_dragon_ptr->set_ka(0.25);
	sv_phong_dragon_ptr->set_kd(0.75);
	sv_phong_dragon_ptr->set_cd(it_dragon_ptr);
	sv_phong_dragon_ptr->set_ks(0.1);
	sv_phong_dragon_ptr->set_exp(20.0);


    Instance* instance_dragon_ptr = new Instance(grid_dragon_ptr);
    instance_dragon_ptr->scale(20, 10, 20);
    instance_dragon_ptr->translate(0, -0.3, 0);
    instance_dragon_ptr->set_material(sv_phong_dragon_ptr);

    scene_grid_ptr->add_object(instance_dragon_ptr);

    scene_grid_ptr->setup_cells();
    add_object(scene_grid_ptr);
}
#endif // TYPE


#endif // ChapterNum
