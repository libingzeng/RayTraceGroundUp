#ifndef __VECTOR_3D__
#define __VECTOR_3D__


// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "Matrix.h"

class Normal;
class Point3D;

//----------------------------------------- class Vector3D

class Vector3D {
	public:

		double	x, y, z;

	public:

		Vector3D(void);											// default constructor
		Vector3D(double a);										// constructor
		Vector3D(double _x, double _y, double _z);				// constructor
		Vector3D(const Vector3D& v);							// copy constructor
		Vector3D(const Normal& n);								// constructs a vector from a Normal
		Vector3D(const Point3D& p);								// constructs a vector from a point

		~Vector3D (void);										// destructor

		Vector3D& 												// assignment operator
		operator= (const Vector3D& rhs);

		Vector3D& 												// assign a Normal to a vector
		operator= (const Normal& rhs);

		Vector3D& 												// assign a Point3D to a vector
		operator= (const Point3D& rhs);

		Vector3D												// unary minus
		operator- (void) const;

		double													// length
		length(void);

		double													// square of the length
		len_squared(void);

		Vector3D												// multiplication by a double on the right
		operator* (const double a) const;

		Vector3D												// division by a double
		operator/ (const double a) const;

		Vector3D												// addition
		operator+ (const Vector3D& v) const;

		Vector3D& 												// compound addition
		operator+= (const Vector3D& v);

		Vector3D												// subtraction
		operator- (const Vector3D& v) const;

		double 													// dot product
		operator* (const Vector3D& b) const;

		Vector3D 												// cross product
		operator^ (const Vector3D& v) const;

		void 													// convert vector to a unit vector
		normalize(void);

		Vector3D& 												// return a unit vector, and normalize the vector
		hat(void);
};


// inlined member functions

// ------------------------------------------------------------------------ unary minus
// this does not change the current vector
// this allows ShadeRec objects to be declared as constant arguments in many shading
// functions that reverse the direction of a ray that's stored in the ShadeRec object

inline Vector3D
Vector3D::operator- (void) const {
	return (Vector3D(-x, -y, -z));
}


// ---------------------------------------------------------------------  len_squared
// the square of the length

inline double
Vector3D::len_squared(void) {
	return (x * x + y * y + z * z);
}


// ----------------------------------------------------------------------- operator*
// multiplication by a double on the right

inline Vector3D
Vector3D::operator* (const double a) const {
	return (Vector3D(x * a, y * a, z * a));
}

// ----------------------------------------------------------------------- operator/
// division by a double

inline Vector3D
Vector3D::operator/ (const double a) const {
	return (Vector3D(x / a, y / a, z / a));
}


// ----------------------------------------------------------------------- operator+
// addition

inline Vector3D
Vector3D::operator+ (const Vector3D& v) const {
	return (Vector3D(x + v.x, y + v.y, z + v.z));
}


// ----------------------------------------------------------------------- operator-
// subtraction

inline Vector3D
Vector3D::operator- (const Vector3D& v) const {
	return (Vector3D(x - v.x, y - v.y, z - v.z));
}


// ----------------------------------------------------------------------- operator*
// dot product

inline double
Vector3D::operator* (const Vector3D& v) const {
	return (x * v.x + y * v.y + z * v.z);
}


// ----------------------------------------------------------------------- operator^
// cross product

inline Vector3D
Vector3D::operator^ (const Vector3D& v) const {
	return (Vector3D(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x));
}


// ---------------------------------------------------------------------  operator+=
// compound addition

inline Vector3D&
Vector3D::operator+= (const Vector3D& v) {
	x += v.x; y += v.y; z += v.z;
	return (*this);
}




// inlined non-member function

// ----------------------------------------------------------------------- operator*
// multiplication by a double on the left

Vector3D 											// prototype
operator* (const double a, const Vector3D& v);

inline Vector3D
operator* (const double a, const Vector3D& v) {
	return (Vector3D(a * v.x, a * v.y, a * v.z));
}



// non-inlined non-member function

// ----------------------------------------------------------------------- operator*
// multiplication by a matrix on the left

Vector3D 											// prototype
operator* (const Matrix& mat, const Vector3D& v);


// ----------------------------------------------------------  get_teapot_data
// read utah teapot with 32 patches and 306 vertices from external file

bool get_teapot_data(int (&patches)[32][16], float (&vertices)[306][3]);


// ----------------------------------------------------------  matrix_4_4_multiply_4_4

void matrix_4_4_multiply_4_4(const float matrix1[4][4], const float matrix2[4][4], float (&result)[4][4]);


// ----------------------------------------------------------  matrix_4_4_multiply_4_1

void matrix_4_4_multiply_4_1(const float matrix1[4][4], const float matrix2[4][1], float (&result)[4][1]);


// ----------------------------------------------------------  matrix_1_4_multiply_4_4

void matrix_1_4_multiply_4_4(const float matrix1[1][4], const float matrix2[4][4], float (&result)[1][4]);


// ----------------------------------------------------------  matrix_1_4_multiply_4_1

void matrix_1_4_multiply_4_1(const float matrix1[1][4], const float matrix2[4][1], float &result);

#endif



