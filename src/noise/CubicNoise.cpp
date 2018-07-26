// 	Copyright (C) Kevin Suffern 2000-2008.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// This file contains the definition of the class CubicNoise

#include <math.h>       // for floor function

#include "Maths.h"      // for clamp function
#include "CubicNoise.h"

//------------------------------------------------------------------------- default constructor

CubicNoise::CubicNoise(void)
	: LatticeNoise()
{}


//------------------------------------------------------------------------- constructor

CubicNoise::CubicNoise(int octaves)
	: LatticeNoise(octaves)
{}


//------------------------------------------------------------------------- constructor

CubicNoise::CubicNoise(int octaves, float lacunarity, float gain)
	: LatticeNoise(octaves, lacunarity, gain)
{}


//------------------------------------------------------------------------- copy constructor

CubicNoise::CubicNoise(const CubicNoise& cns)
	: LatticeNoise(cns)
{}


//------------------------------------------------------------------------- assignment operator

CubicNoise&
CubicNoise::operator= (const CubicNoise& rhs) {

	if (this == &rhs)
		return (*this);

	LatticeNoise::operator= (rhs);

	return (*this);
}


//------------------------------------------------------------------------- clone

CubicNoise*
CubicNoise::clone(void) const {
	return (new CubicNoise);
}


//------------------------------------------------------------------------- destructor

CubicNoise::~CubicNoise(void) {}


//------------------------------------------------------------------------- value_noise

// By clamping here, we don't have to clamp it in the fractal_sum,
// turbulence and fbm functions
// This is based on code from Peachey (2003).

float
CubicNoise::value_noise(const Point3D& p) const {
	int 	ix, iy, iz;
    float 	fx, fy, fz;
    float 	xknots[4], yknots[4], zknots[4];

    ix = floor(p.x);
    fx = p.x - ix;

    iy = floor(p.y);
    fy = p.y - iy;

	iz = floor(p.z);
    fz = p.z - iz;

    for (int k = -1; k <= 2; k++) {
        for (int j = -1; j <= 2; j++) {
            for (int i = -1; i <= 2; i++) {
                xknots[i+1] = value_table[INDEX(ix + i, iy + j, iz + k)];
            }
            yknots[j+1] = four_knot_spline(fx, xknots);
        }
        zknots[k+1] = four_knot_spline(fy, yknots);
    }

    return (clamp(four_knot_spline(fz, zknots), -1.0, 1.0));
}



//------------------------------------------------------------------------- vector_noise
// This is based on code from Peachey (2003).

Vector3D
CubicNoise::vector_noise(const Point3D& p) const {
	int 		ix, iy, iz;
    float 		fx, fy, fz;
    Vector3D 	xknots[4], yknots[4], zknots[4];

    ix = floor(p.x);
    fx = p.x - ix;

    iy = floor(p.y);
    fy = p.y - iy;

	iz = floor(p.z);
    fz = p.z - iz;

    for (int k = -1; k <= 2; k++) {
        for (int j = -1; j <= 2; j++) {
            for (int i = -1; i <= 2; i++) {
                xknots[i+1] = vector_table[INDEX(ix + i, iy + j, iz + k)];
            }
             yknots[j+1] = four_knot_spline(fx, xknots);
        }
     	zknots[k+1] = four_knot_spline(fy, yknots);
    }

	return (four_knot_spline(fz, zknots));
}


