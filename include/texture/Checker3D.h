#ifndef CHECKER3D_H
#define CHECKER3D_H

#include "Texture.h"


class Checker3D : public Texture
{
	public:

		Checker3D(void);

		Checker3D(const Checker3D& cc);

		virtual Checker3D*
		clone(void) const;

		virtual
		~Checker3D(void);

        void
        set_size(const float s);

		void
		set_color1(const RGBColor& c);

		void
		set_color1(const float r, const float g, const float b);

		void
		set_color1(const float c);

		void
		set_color2(const RGBColor& c);

		void
		set_color2(const float r, const float g, const float b);

		void
		set_color2(const float c);

		virtual RGBColor
		get_color(const ShadeRec& sr) const;

	protected:

		Checker3D&
		operator= (const Checker3D& rhs);

    private:

		float	   size;		        // width of a single checker
        RGBColor color1;                // the color1
        RGBColor color2;                // the color2
};


// ---------------------------------------------------------------- set_size
inline void
Checker3D::set_size(const float s) {
	size = s;
}


// ---------------------------------------------------------------- set_color

inline void
Checker3D::set_color1(const RGBColor& c) {
	color1 = c;
}


// ---------------------------------------------------------------- set_color

inline void
Checker3D::set_color1(const float r, const float g, const float b) {
	color1.r = r; color1.g = g; color1.b = b;
}


// ---------------------------------------------------------------- set_color

inline void
Checker3D::set_color1(const float c) {
	color1.r = c; color1.g = c; color1.b = c;
}


// ---------------------------------------------------------------- set_color

inline void
Checker3D::set_color2(const RGBColor& c) {
	color2 = c;
}


// ---------------------------------------------------------------- set_color

inline void
Checker3D::set_color2(const float r, const float g, const float b) {
	color2.r = r; color2.g = g; color2.b = b;
}


// ---------------------------------------------------------------- set_color

inline void
Checker3D::set_color2(const float c) {
	color2.r = c; color2.g = c; color2.b = c;
}

#endif // CHECKER3D_H
