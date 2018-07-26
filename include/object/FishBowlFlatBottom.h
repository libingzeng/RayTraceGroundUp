#ifndef FISHBOWLFLATBOTTOM_H
#define FISHBOWLFLATBOTTOM_H

// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// This file contains the declaration of the class FishBowlFlatBottom
// This is a spherical bowl with a specified glass thickness
// that contains water
// The bowl is centered on the world origin

#include "Compound.h"

class FishBowlFlatBottom: public Compound {
	public:

		FishBowlFlatBottom(void);

		FishBowlFlatBottom(	const double _inner_radius,
					const double _glass_thickness,
					const double _water_depth,
					const double _meniscus_radius,
					const double _opening_angle,
					const double _bottom_angle);

		FishBowlFlatBottom(const FishBowlFlatBottom& fb);

		virtual FishBowlFlatBottom*
		clone(void) const;

		virtual FishBowlFlatBottom&
		operator= (const FishBowlFlatBottom& rhs);

		virtual
		~FishBowlFlatBottom(void);

		void
		build_components(void);

		void
		set_glass_air_material(Material* m_ptr);

		void
		set_water_air_material(Material* m_ptr);

		void
		set_water_glass_material(Material* m_ptr);

	protected:

		double inner_radius;		// radius of the inside glass surface
		double glass_thickness;
		double water_depth; 		// measured from the bottom of the water-glass boundary
		double meniscus_radius;
		double opening_angle;		// specifies how wide the opening is at the top (alpha in Figure 28.40(a))
		double bottom_angle;        // specifies how wide the opening is at the bottom
};

#endif // FISHBOWLFLATBOTTOM_H
