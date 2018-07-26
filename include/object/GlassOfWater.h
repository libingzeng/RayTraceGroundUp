#ifndef __GLASS_OF_WATER__
#define __GLASS_OF_WATER__

// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "Compound.h"


class GlassOfWater: public Compound {
	public:
		
		GlassOfWater(void);   									
		
		GlassOfWater( 	const double _height, 
						const double _inner_radius,
						const double _wall_thickness,
						const double _base_thickness,
						const double _water_height,
						const double _meniscus_radius);
						
		GlassOfWater(const GlassOfWater& gw); 				
		
		virtual GlassOfWater* 									
		clone(void) const;

		virtual GlassOfWater& 									
		operator= (const GlassOfWater& rhs);		
		
		virtual 													
		~GlassOfWater(void);
		
		void
		build_components(void);
		
		void
		set_glass_air_material(Material* m_ptr);
		
		void
		set_water_air_material(Material* m_ptr); 
		
		void
		set_water_glass_material(Material* m_ptr); 
		
	protected:
	
		double		height;  			// total height
		double 		inner_radius;		// inner radius of glass, outer radius of water
		double 		wall_thickness;		// thickness of the glass wall
		double		base_thickness;		// thickness of the glass base
		double 		water_height;		// height of water from bottom of glass base on (x, z) plane
		double 		meniscus_radius;	
};

#endif
