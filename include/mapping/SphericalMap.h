#ifndef __SPHERICAL_MAP__
#define __SPHERICAL_MAP__

// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// This file contains the definition of the class SphericalMap
// This class inherits from Mapping, and defines a default spherical mapping
// Here, an image with an aspect ratio of 2:1 is mapped onto a sphere so
// that it just covers the whole sphere

#include "Constants.h"
#include "Mapping.h"

class SphericalMap: public Mapping {		
	public:
	
		SphericalMap(void);									
		
		SphericalMap(const SphericalMap& sm);					

		SphericalMap& 											
		operator= (const SphericalMap& rhs);		

		virtual SphericalMap*									
		clone(void) const;				

		~SphericalMap(void);									
		
		virtual void
		get_texel_coordinates(	const 	Point3D& 	local_hit_point, 
								const 	int 		xres, 
								const 	int 		yres, 
										int& 		row, 
										int& 		column) const;
};

#endif
		