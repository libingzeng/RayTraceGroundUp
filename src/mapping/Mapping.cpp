// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "Mapping.h"

// ---------------------------------------------------------------- default constructor

Mapping::Mapping(void) {}


// ---------------------------------------------------------------- copy constructor

Mapping::Mapping(const Mapping& m) {}
	
	
// ---------------------------------------------------------------- assignment operator

Mapping& 
Mapping::operator= (const Mapping& rhs) {
	if (this == &rhs)
		return (*this);

	return (*this);
}


// ---------------------------------------------------------------- destructor

Mapping::~Mapping(void) {} 

