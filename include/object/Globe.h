#ifndef GLOBE_H
#define GLOBE_H


#include "Compound.h"


class Globe: public Compound {
	public:

		Globe(void);

		Globe(const Globe& g);

		virtual Globe*
		clone(void) const;

		virtual Globe&
		operator= (const Globe& rhs);

		virtual
		~Globe(void);

		void
		build_components(void);

		void
		set_base_material(Material* m_ptr);

		void
		set_holder_material(Material* m_ptr);

		void
		set_sphere_material(Material* m_ptr);
};

#endif // GLOBE_H
