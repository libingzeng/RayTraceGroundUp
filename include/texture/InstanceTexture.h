#ifndef INSTANCETEXTURE_H
#define INSTANCETEXTURE_H

// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "Matrix.h"
#include "ShadeRec.h"
#include "Texture.h"


class InstanceTexture : public Texture {
	public:

		InstanceTexture(void);

		InstanceTexture(Texture* t_ptr);

		InstanceTexture(const InstanceTexture& instance);

		virtual InstanceTexture*
		clone(void) const;

		virtual
		~InstanceTexture(void);

		InstanceTexture&
		operator= (const InstanceTexture& rhs);

		void
		set_texture(Texture* t_ptr);


		// affine tranformation functions

		void
		translate(const Vector3D& trans);

		void
		translate(const double dx, const double dy, const double dz);

		void
		scale(const Vector3D& s);

		void
		scale(const double s);

		void
		scale(const double a, const double b, const double c);

		virtual void
		rotate_x(const double r);

		virtual void
		rotate_y(const double r);

		virtual void
		rotate_z(const double r);

		void
		shear(const Matrix& m);

		virtual RGBColor
		get_color(const ShadeRec& sr) const;

	private:

		Texture*	        texture_ptr;				// object to be transformed
		Matrix				inv_matrix;		    	// inverse transformation matrix
};


#endif // INSTANCETEXTURE_H
