#ifndef __IMAGE_TEXTURE__
#define __IMAGE_TEXTURE__

// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "Image.h"
#include "Mapping.h"
#include "Texture.h"

class ImageTexture: public Texture {
	public:

		ImageTexture(void);

		ImageTexture(Image* _image_ptr);

		ImageTexture(const ImageTexture& it);

		ImageTexture&
		operator= (const ImageTexture& rhs);

		virtual ImageTexture*
		clone(void) const;

		virtual
		~ImageTexture(void) ;

		virtual RGBColor
		get_color(const ShadeRec& sr) const;

		void
		set_image(Image* _image_ptr);

		void
		set_mapping(Mapping* map_ptr);

	private:

		int 		hres;			// horizontal resolution of the image
		int			vres;			// vertical resolution of the image
		Image*		image_ptr;		// the image
		Mapping*	mapping_ptr;	// mapping technique used, if any
};


// ---------------------------------------------------------------- set_image

inline void
ImageTexture::set_image(Image* _image_ptr) {
	image_ptr = _image_ptr;
	hres = image_ptr->get_hres();
	vres = image_ptr->get_vres();
}


// ---------------------------------------------------------------- set_mapping

inline void
ImageTexture::set_mapping(Mapping* map_ptr) {
	mapping_ptr = map_ptr;
}

#endif
