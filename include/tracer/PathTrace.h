// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#ifndef __PATH_TRACE__
#define __PATH_TRACE__

#include "Tracer.h"

class PathTrace: public Tracer {
	public:
		
		PathTrace(void);
		
		PathTrace(World* _worldPtr);
	
		virtual RGBColor	
		trace_ray(const Ray ray, const int depth) const;
		
		virtual RGBColor	
		trace_ray(const Ray ray, double& tmin, const int depth) const;
};

#endif
