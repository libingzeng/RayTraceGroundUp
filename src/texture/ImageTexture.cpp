// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "ImageTexture.h"

// ---------------------------------------------------------------- default constructor

ImageTexture::ImageTexture(void)
	:	Texture(),
		hres(100),
		vres(100),
		image_ptr(NULL),
		mapping_ptr(NULL)
{}


// ---------------------------------------------------------------- constructor

ImageTexture::ImageTexture(Image* _image_ptr)
	:	Texture(),
		hres(_image_ptr->get_hres()),
		vres(_image_ptr->get_vres()),
		image_ptr(_image_ptr),
		mapping_ptr(NULL)
{}


// ---------------------------------------------------------------- copy constructor

ImageTexture::ImageTexture(const ImageTexture& it)
	: 	Texture(it),
		hres(it.hres),
		vres(it.vres)
{
	if (it.image_ptr)
		*image_ptr = *it.image_ptr;
	else
		image_ptr = NULL;
		
	if (it.mapping_ptr)
		mapping_ptr = it.mapping_ptr->clone();
	else
		mapping_ptr = NULL;
}


// ---------------------------------------------------------------- assignment operator

ImageTexture& 
ImageTexture::operator= (const ImageTexture& rhs) {
	if (this == &rhs)
		return (*this);
	
	Texture::operator= (rhs);
	
	hres = rhs.hres;
	vres = rhs.vres;
	
	if (image_ptr) {
		delete image_ptr;
		image_ptr = NULL;
	}
	
	if (rhs.image_ptr)
		*image_ptr = *rhs.image_ptr;
	
	if (mapping_ptr) {
		delete mapping_ptr;
		mapping_ptr = NULL;
	}
	
	if (rhs.mapping_ptr)
		mapping_ptr = rhs.mapping_ptr->clone();

	return (*this);
}


// ---------------------------------------------------------------- clone

ImageTexture*										
ImageTexture::clone(void) const {
	return (new ImageTexture (*this));
}	


// ---------------------------------------------------------------- destructor

ImageTexture::~ImageTexture (void) {

	if (image_ptr) {
		delete image_ptr;
		image_ptr = NULL;
	}
	
	if (mapping_ptr) {
		delete mapping_ptr;
		mapping_ptr = NULL;
	}
}


// ---------------------------------------------------------------- get_color

// When we ray trace a triangle mesh object with uv mapping, the mapping pointer may be NULL
// because we can define uv coordinates on an arbitrary triangle mesh.
// In this case we don't use the local hit point because the pixel coordinates are computed 
// from the uv coordinates stored in the ShadeRec object in the uv triangles' hit functions
// See, for example, Listing 29.12.

RGBColor														
ImageTexture::get_color(const ShadeRec& sr) const {	
	int row;
	int column;
		
	if (mapping_ptr)
		mapping_ptr->get_texel_coordinates(sr.local_hit_point, hres, vres, row, column);
	else {
		row 	= (int)(sr.v * (vres - 1));  	
		column 	= (int)(sr.u * (hres - 1));	
	}
	
	return (image_ptr->get_color(row, column));
}  





