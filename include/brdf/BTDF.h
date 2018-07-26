// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#ifndef __BTDF__
#define __BTDF__

#include <math.h>

#include "Constants.h"
#include "Vector3D.h"
#include "ShadeRec.h"

class BTDF {
	public:
	
		BTDF(void);
		
		BTDF(const BTDF& btdf);
		
		virtual BTDF*
		clone(void) = 0;
		
		BTDF&							
		operator= (const BTDF& rhs);
		
		virtual
		~BTDF(void);
				
		virtual RGBColor
		f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;
		
		virtual RGBColor
		sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wt) const;
		
		virtual RGBColor
		rho(const ShadeRec& sr, const Vector3D& wo) const;
};

#endif
