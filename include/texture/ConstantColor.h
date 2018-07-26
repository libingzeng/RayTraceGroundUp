#ifndef CONSTANTCOLOR_H
#define CONSTANTCOLOR_H

#include "Texture.h"


class ConstantColor : public Texture
{
	public:

		ConstantColor(void);

		ConstantColor(const RGBColor& c);

		ConstantColor(const ConstantColor& cc);

		virtual ConstantColor*
		clone(void) const;

		virtual
		~ConstantColor(void);

		void
		set_color(const RGBColor& c);

		void
		set_color(const float r, const float g, const float b);

		void
		set_color(const float c);

		virtual RGBColor
		get_color(const ShadeRec& sr) const;

	protected:

		ConstantColor&
		operator= (const ConstantColor& rhs);

    private:

        RGBColor color; // the color
};


// ---------------------------------------------------------------- set_color

inline void
ConstantColor::set_color(const RGBColor& c) {
	color = c;
}


// ---------------------------------------------------------------- set_color

inline void
ConstantColor::set_color(const float r, const float g, const float b) {
	color.r = r; color.g = g; color.b = b;
}


// ---------------------------------------------------------------- set_color

inline void
ConstantColor::set_color(const float c) {
	color.r = c; color.g = c; color.b = c;
}

#endif // CONSTANTCOLOR_H
