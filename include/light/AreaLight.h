
#ifndef __AREA_LIGHT__

//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


		virtual AreaLight*
		clone(void) const;

		virtual void
		set_cast_shadow(bool cs);

		virtual bool
		get_cast_shadow(void);

// ---------------------------------------------------------------------- set_cast_shadow

inline void
AreaLight::set_cast_shadow(bool cs) {
    cast_shadow = cs;
}
