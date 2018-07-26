// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "Constants.h"
#include "InstanceTexture.h"



// ----------------------------------------------------------------  default constructor

InstanceTexture::InstanceTexture(void)
	: 	Texture(),
		texture_ptr(NULL),
		inv_matrix()
{}


// ----------------------------------------------------------------  constructor

InstanceTexture::InstanceTexture(Texture* t_ptr)
	: 	Texture(),
		texture_ptr(t_ptr),
		inv_matrix()
{}


// ---------------------------------------------------------------- copy constructor

InstanceTexture::InstanceTexture (const InstanceTexture& instance)
	: 	Texture(instance),
		inv_matrix(instance.inv_matrix)
{
	if(instance.texture_ptr)
		texture_ptr = instance.texture_ptr->clone();
	else  texture_ptr = NULL;
}


// ---------------------------------------------------------------- clone

InstanceTexture*
InstanceTexture::clone(void) const {
	return (new InstanceTexture(*this));
}


// ---------------------------------------------------------------- destructor

InstanceTexture::~InstanceTexture(void) {
	if (texture_ptr) {
		delete texture_ptr;
		texture_ptr = NULL;
	}
}


// ---------------------------------------------------------------- assignment operator

InstanceTexture&
InstanceTexture::operator= (const InstanceTexture& rhs) {
	if (this == &rhs)
		return (*this);

	Texture::operator=(rhs);

	if(texture_ptr) {
		delete texture_ptr;
		texture_ptr = NULL;
	}

	if (rhs.texture_ptr)
		texture_ptr = rhs.texture_ptr->clone();
	else
		texture_ptr = NULL;

	inv_matrix				= rhs.inv_matrix;

	return (*this);
}


//------------------------------------------------------------------ set_object

void
InstanceTexture::set_texture(Texture* t_ptr) {
	texture_ptr = t_ptr;
}



//-------------------------------------------------------------------------------- scale

void
InstanceTexture::scale(const Vector3D& s) {

	Matrix	inv_scaling_matrix;			// temporary inverse scaling matrix

	inv_scaling_matrix.m[0][0] = 1.0 / s.x;
	inv_scaling_matrix.m[1][1] = 1.0 / s.y;
	inv_scaling_matrix.m[2][2] = 1.0 / s.z;

	inv_matrix = inv_matrix * inv_scaling_matrix;
}


//-------------------------------------------------------------------------------- scale

void
InstanceTexture::scale(const double s) {

	Matrix inv_scaling_matrix;					// temporary inverse scaling matrix

	inv_scaling_matrix.m[0][0] = 1.0 / s;
	inv_scaling_matrix.m[1][1] = 1.0 / s;
	inv_scaling_matrix.m[2][2] = 1.0 / s;

	inv_matrix = inv_matrix * inv_scaling_matrix;
}


//-------------------------------------------------------------------------------- scale

void
InstanceTexture::scale(const double a, const double b, const double c) {

	Matrix inv_scaling_matrix;					// temporary inverse scaling matrix

	inv_scaling_matrix.m[0][0] = 1.0 / a;
	inv_scaling_matrix.m[1][1] = 1.0 / b;
	inv_scaling_matrix.m[2][2] = 1.0 / c;

	inv_matrix = inv_matrix * inv_scaling_matrix;
}


//-------------------------------------------------------------------------------- translate

void
InstanceTexture::translate(const Vector3D& trans) {

	Matrix inv_translation_matrix;				// temporary inverse translation matrix

	inv_translation_matrix.m[0][3] = -trans.x;
	inv_translation_matrix.m[1][3] = -trans.y;
	inv_translation_matrix.m[2][3] = -trans.z;

	inv_matrix = inv_matrix * inv_translation_matrix;
}


//-------------------------------------------------------------------------------- translate

void
InstanceTexture::translate(const double dx, const double dy, const double dz) {

	Matrix inv_translation_matrix;				// temporary inverse translation matrix

	inv_translation_matrix.m[0][3] = -dx;
	inv_translation_matrix.m[1][3] = -dy;
	inv_translation_matrix.m[2][3] = -dz;

	inv_matrix = inv_matrix * inv_translation_matrix;
}



//-------------------------------------------------------------------------------- rotate_x

void
InstanceTexture::rotate_x(const double theta) {

	double sin_theta = sin(theta * PI_ON_180);
	double cos_theta = cos(theta * PI_ON_180);

	Matrix inv_x_rotation_matrix;					// temporary inverse rotation matrix about x axis

	inv_x_rotation_matrix.m[1][1] = cos_theta;
	inv_x_rotation_matrix.m[1][2] = sin_theta;
	inv_x_rotation_matrix.m[2][1] = -sin_theta;
	inv_x_rotation_matrix.m[2][2] = cos_theta;

	inv_matrix = inv_matrix * inv_x_rotation_matrix;
}


//-------------------------------------------------------------------------------- rotate_y

void
InstanceTexture::rotate_y(const double theta) {

	double sin_theta = sin(theta * PI / 180.0);
	double cos_theta = cos(theta * PI / 180.0);

	Matrix inv_y_rotation_matrix;					// temporary inverse rotation matrix about y axis

	inv_y_rotation_matrix.m[0][0] = cos_theta;
	inv_y_rotation_matrix.m[0][2] = -sin_theta;
	inv_y_rotation_matrix.m[2][0] = sin_theta;
	inv_y_rotation_matrix.m[2][2] = cos_theta;

	inv_matrix = inv_matrix * inv_y_rotation_matrix;
}



//-------------------------------------------------------------------------------- rotate_z

void
InstanceTexture::rotate_z(const double theta) {
	double sin_theta = sin(theta * PI / 180.0);
	double cos_theta = cos(theta * PI / 180.0);

	Matrix inv_z_rotation_matrix;					// temporary inverse rotation matrix about y axis

	inv_z_rotation_matrix.m[0][0] = cos_theta;
	inv_z_rotation_matrix.m[0][1] = sin_theta;
	inv_z_rotation_matrix.m[1][0] = -sin_theta;
	inv_z_rotation_matrix.m[1][1] = cos_theta;

	inv_matrix = inv_matrix * inv_z_rotation_matrix;
}


//-------------------------------------------------------------------------------- shear

void
InstanceTexture::shear(const Matrix& s) {

	Matrix inverse_shearing_matrix;    // inverse shear matrix

	// discriminant

	double d = 1.0 	- s.m[1][0] * s.m[0][1] - s.m[2][0] * s.m[0][2]  - s.m[2][1] * s.m[1][2]
					+ s.m[1][0] * s.m[2][1] * s.m[0][2] + s.m[2][0] * s.m[0][1] * s.m[2][1];

	// diagonals

	inverse_shearing_matrix.m[0][0] = 1.0 - s.m[2][1] * s.m[1][2];
	inverse_shearing_matrix.m[1][1] = 1.0 - s.m[2][0] * s.m[0][2];
	inverse_shearing_matrix.m[2][2] = 1.0 - s.m[1][0] * s.m[0][1];
	inverse_shearing_matrix.m[3][3] = d;

	// first row

	inverse_shearing_matrix.m[0][1] = -s.m[1][0] + s.m[2][0] * s.m[1][2];
	inverse_shearing_matrix.m[0][2] = -s.m[2][0] + s.m[1][0] * s.m[2][1];

	// second row

	inverse_shearing_matrix.m[1][0] = -s.m[0][1] + s.m[2][1] * s.m[0][2];
	inverse_shearing_matrix.m[1][2] = -s.m[2][1] + s.m[2][0] * s.m[0][1];

	// third row

	inverse_shearing_matrix.m[2][0] = -s.m[0][2] + s.m[0][1] * s.m[1][2];
	inverse_shearing_matrix.m[2][1] = -s.m[1][2] + s.m[1][0] * s.m[0][2] ;

	// divide by discriminant

	inverse_shearing_matrix = inverse_shearing_matrix / d;

	inv_matrix = inv_matrix * inverse_shearing_matrix;
}



//-------------------------------------------------------------------------------- get_color

RGBColor
InstanceTexture::get_color(const ShadeRec& sr) const {

	ShadeRec local_sr(sr);
	local_sr.local_hit_point = inv_matrix * local_sr.local_hit_point;

	return (texture_ptr->get_color(local_sr));
}

