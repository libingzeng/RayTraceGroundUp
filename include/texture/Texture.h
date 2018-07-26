#ifndef __TEXTURE__
#define __TEXTURE__

// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include <math.h>

#include "ShadeRec.h"
#include "RGBColor.h"

class Texture {	
	public:
	
		Texture(void);										
		
		Texture(const Texture& texture); 					
				
		virtual Texture*									
		clone(void) const = 0;	

		virtual 											
		~Texture(void);
		
		virtual RGBColor																			
		get_color(const ShadeRec& sr) const = 0;
				
	protected:
	
		Texture& 											
		operator= (const Texture& rhs);
};

#endif
		