// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// This file contains the definition of the class Vector3D

#include <math.h>    // for sqrt

#include "Vector3D.h"
#include "Normal.h"
#include "Point3D.h"

#include <iostream>
#include <fstream>
using namespace std;

#include <string.h>

// ---------------------------------------------------------- default constructor

Vector3D::Vector3D(void)
	 : x(0.0), y(0.0), z(0.0)
{}

// ---------------------------------------------------------- constructor

Vector3D::Vector3D(double a)
	 : x(a), y(a), z(a)
{}

// ---------------------------------------------------------- constructor

Vector3D::Vector3D(double _x, double _y, double _z)
	: x(_x), y(_y), z(_z)
{}

// ---------------------------------------------------------- copy constructor

Vector3D::Vector3D(const Vector3D& vector)
	: x(vector.x), y(vector.y), z(vector.z)
{}


// ---------------------------------------------------------- constructor
// constructs a vector from a normal

Vector3D::Vector3D(const Normal& n)
	: x(n.x), y(n.y), z(n.z)
{}

// ---------------------------------------------------------- constructor
// constructs a vector from a point
// this is used in the ConcaveHemisphere hit functions

Vector3D::Vector3D(const Point3D& p)
	: x(p.x), y(p.y), z(p.z)
{}


// ---------------------------------------------------------- destructor

Vector3D::~Vector3D (void) {}



// ---------------------------------------------------------- assignment operator

Vector3D&
Vector3D::operator= (const Vector3D& rhs) {
	if (this == &rhs)
		return (*this);

	x = rhs.x; y = rhs.y; z = rhs.z;

	return (*this);
}


// ----------------------------------------------------------- assignment operator
// assign a Normal to a vector

Vector3D&
Vector3D::operator= (const Normal& rhs) {
	x = rhs.x; y = rhs.y; z = rhs.z;
	return (*this);
}


// ---------------------------------------------------------- assignment operator
// assign a point to a vector

Vector3D&
Vector3D::operator= (const Point3D& rhs) {
	x = rhs.x; y = rhs.y; z = rhs.z;
	return (*this);
}


// ----------------------------------------------------------  length
// length of the vector

double
Vector3D::length(void) {
	return (sqrt(x * x + y * y + z * z));
}


// ----------------------------------------------------------  normalize
// converts the vector to a unit vector

void
Vector3D::normalize(void) {
	double length = sqrt(x * x + y * y + z * z);
	x /= length; y /= length; z /= length;
}


// ----------------------------------------------------------  hat
// converts the vector to a unit vector and returns the vector

Vector3D&
Vector3D::hat(void) {
	double length = sqrt(x * x + y * y + z * z);
	x /= length; y /= length; z /= length;
	return (*this);
}


// non-member function

// ----------------------------------------------------------  operator*
// multiplication by a matrix on the left

Vector3D
operator* (const Matrix& mat, const Vector3D& v) {
	return (Point3D(mat.m[0][0] * v.x + mat.m[0][1] * v.y + mat.m[0][2] * v.z,
					mat.m[1][0] * v.x + mat.m[1][1] * v.y + mat.m[1][2] * v.z,
					mat.m[2][0] * v.x + mat.m[2][1] * v.y + mat.m[2][2] * v.z));
}


// ----------------------------------------------------------  get_teapot_data
// read utah teapot with 32 patches and 306 vertices from external file

bool get_teapot_data(int (&patches)[32][16], float (&vertices)[306][3]) {
    ifstream infile( ".\\teaset\\teapot");
    char str[100];
    char *token;
    int item_num;
    int patch_num = 0;
    int vertex_num = 0;
    int flag = 0;
    int tokens_i[16];
    float tokens_f[3];
    while (infile >> str) {
        item_num = 0;
        token = strtok(str, ",");
        if ((flag == 0) || (flag == 1)) {
            sscanf(token, "%d", &(tokens_i[item_num]));
            if (tokens_i[0] == 32) {
                flag = 1;
            }
            else if (tokens_i[0] == 306) {
                flag = 2;
            }
            else {
                patches[patch_num][item_num] = tokens_i[item_num];
                item_num ++;
            }
            token = strtok(NULL, ",");
        }

        while (token != NULL) {
            if (flag == 1) {
                sscanf(token, "%d", &(tokens_i[item_num]));
                patches[patch_num][item_num] = tokens_i[item_num];
            }
            if (flag == 2) {
                sscanf(token, "%f", &(tokens_f[item_num]));
                vertices[vertex_num][item_num] = tokens_f[item_num];
            }
            item_num ++;
            token = strtok(NULL, ",");
        }
        if ((flag == 1) && (tokens_i[0] != 32)) {patch_num ++;}
        if (flag == 2) {
            if ((tokens_i[0] == 306)) {tokens_i[0] = 0;}
            else {vertex_num ++;}
        }
    }
    infile.close();
    return true;
}


// ----------------------------------------------------------  matrix_4_4_multiply_4_4

void matrix_4_4_multiply_4_4(const float matrix1[4][4], const float matrix2[4][4], float (&result)[4][4]) {
    for (int k=0; k<4; k++) {
        for (int i=0; i<4; i++) {
            result[i][k] = 0.0;
            for (int j=0; j<4; j++) {
                result[i][k] = result[i][k] + matrix1[i][j]*matrix2[j][k];
            }
        }
    }
}


// ----------------------------------------------------------  matrix_4_4_multiply_4_1

void matrix_4_4_multiply_4_1(const float matrix1[4][4], const float matrix2[4][1], float (&result)[4][1]) {
    for (int i=0; i<4; i++) {
        result[i][0] = 0.0;
        for (int j=0; j<4; j++) {
            result[i][0] = result[i][0] + matrix1[i][j]*matrix2[j][0];
        }
    }
}

// ----------------------------------------------------------  matrix_1_4_multiply_4_4

void matrix_1_4_multiply_4_4(const float matrix1[1][4], const float matrix2[4][4], float (&result)[1][4]) {
    for (int k=0; k<4; k++) {
        result[0][k] = 0.0;
        for (int j=0; j<4; j++) {
            result[0][k] = result[0][k] + matrix1[0][j]*matrix2[j][k];
        }
    }
}


// ----------------------------------------------------------  matrix_1_4_multiply_4_1

void matrix_1_4_multiply_4_1(const float matrix1[1][4], const float matrix2[4][1], float &result) {
    result = 0.0;
    for (int j=0; j<4; j++) {
        result = result + matrix1[0][j]*matrix2[j][0];
    }
}
