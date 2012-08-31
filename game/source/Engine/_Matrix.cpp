/* Copyright (C) Dante Treglia II and Mark A. DeLoura, 2000. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Dante Treglia II and Mark A. DeLoura, 2000"
 */
//==========================================================
// C++ Matrix Library
// Version: 2.3 <FINAL>
// Date: March 19, 2000
// Authors: Dante Treglia II and Mark A. DeLoura
// Thanks to: Miguel Gomez, Stan Melax, Pete Isensee, 
//             Gabor Nagy, Scott Bilas, James Boer
//==========================================================

#include <cmath>
#include <cstdio>
#include <cassert>
#include "Matrix.h"


////////////////////////////////////////////////////////////
// Vector2 class
//

// Constructor with initializing float values
Vector2::Vector2(float inX, float inY) 
{
	x = inX;
	y = inY;
}

// Constructor with initializing Vector2
Vector2::Vector2(const Vector2 &v) 
{
	x = v.x;
	y = v.y;
}

// Array indexing
float &Vector2::operator [] (unsigned int i) 
{
	assert (i<2);
	
	return *(&x+i);
}

// Array indexing
const float &Vector2::operator [] (unsigned int i) const 
{
	assert (i<2);
	
	return *(&x+i);
}

// Assign 
Vector2 &Vector2::operator = (const Vector2 &v) 
{
	x = v.x;
	y = v.y;
	
	return *this;
}

// Add a Vector2 to this one
Vector2 &Vector2::operator += (const Vector2 &v) 
{
	x += v.x;
	y += v.y;
	
	return *this;
}

// Subtract a Vector2 from this one
Vector2 &Vector2::operator -= (const Vector2 &v) 
{
	x -= v.x;
	y -= v.y;
	
	return *this;
}

// Multiply the Vector2 by a float
Vector2 &Vector2::operator *= (float f) 
{
	x *= f;
	y *= f;
	
	return *this;
}

// Divide the Vector2 by a float
Vector2 &Vector2::operator /= (float f) 
{
	x /= f;
	y /= f;
	
	return *this;
}

// Are these two Vector2's equal?
bool operator == (const Vector2 &a, const Vector2 &b) 
{
	return ((a.x == b.x) && (a.y == b.y));
}

// Are these two Vector2's not equal?
bool operator != (const Vector2 &a, const Vector2 &b) 
{
	return ((a.x != b.x) || (a.y != b.y));
}

// Add two Vector2's
Vector2 operator + (const Vector2 &a, const Vector2 &b) 
{
	Vector2 ret(a);
	
	ret += b;
	
	return ret;
}

// Subtract one Vector2 from another
Vector2 operator - (const Vector2 &a, const Vector2 &b) 
{
	Vector2 ret(a);
	
	ret -= b;
	
	return ret;
}

// Multiply Vector2 by a float
Vector2 operator * (const Vector2 &v, float f) 
{
	return Vector2(f * v.x, f * v.y);
}

// Multiply Vector2 by a float
Vector2 operator * (float f, const Vector2 &v) 
{
	return Vector2(f * v.x, f * v.y);
}

// Divide Vector2 by a float
Vector2 operator / (const Vector2 &v, float f) 
{
	return Vector2(v.x / f, v.y / f);
}

// Negate a Vector2
Vector2 operator - (const Vector2 &a) 
{
	return Vector2(-a.x, -a.y);
}

// Get length of a Vector2
float Vector2::length() const 
{
	return (float) sqrt(x*x + y*y);
}

// Get squared length of a Vector2
float Vector2::lengthSqr() const 
{
	return (x*x + y*y);
}

// Does Vector2 equal (0, 0)?
bool Vector2::isZero() const 
{
	return ((x == 0.0F) && (y == 0.0F));
}

// Normalize a Vector2
Vector2 &Vector2::normalize() 
{
	float m = length();
	
	if (m > 0.0F) 
		m = 1.0F / m;
	else 
		m = 0.0F;
	x *= m;
	y *= m;
	
	return *this;
}


////////////////////////////////////////////////////////////
// Vector3 class
//

// Constructor with initializing float values
Vector3::Vector3(float inX, float inY, float inZ) 
{
	x = inX;
	y = inY;
	z = inZ;
}

// Constructor with initializing Vector3
Vector3::Vector3(const Vector3 &v) 
{
	x = v.x;
	y = v.y;
	z = v.z;
}

// Constructor with initializing Vector2
Vector3::Vector3(const Vector2 &v) 
{
	x = v.x;
	y = v.y;
	z = 0.0F;
}

// Array indexing
float &Vector3::operator [] (unsigned int i) 
{
	assert (i<3);
	
	return *(&x+i);
}

// Array indexing
const float &Vector3::operator [] (unsigned int i) const 
{
	assert (i<3);
	
	return *(&x+i);
}

// Assign
Vector3 &Vector3::operator = (const Vector3 &v) 
{
	x = v.x;
	y = v.y;
	z = v.z;
	
	return *this;
}

// Assign from a Vector2
Vector3 &Vector3::operator = (const Vector2 &v) 
{
	x = v.x;
	y = v.y;
	z = 0.0F;
	
	return *this;
}

// Add a Vector3 to this one
Vector3 &Vector3::operator += (const Vector3 &v) 
{
	x += v.x;
	y += v.y;
	z += v.z;
	
	return *this;
}

// Subtract a Vector3 from this one
Vector3 &Vector3::operator -= (const Vector3 &v) 
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	
	return *this;
}

// Multiply the Vector3 by a float
Vector3 &Vector3::operator *= (float f) 
{
	x *= f;
	y *= f;
	z *= f;
	
	return *this;
}

// Divide the Vector3 by a float
Vector3 &Vector3::operator /= (float f) 
{
	x /= f;
	y /= f;
	z /= f;
	
	return *this;
}

// Are these two Vector3's equal?
bool operator == (const Vector3 &a, const Vector3 &b) 
{
	return ((a.x == b.x) && (a.y == b.y) && (a.z == b.z));
}

// Are these two Vector3's not equal?
bool operator != (const Vector3 &a, const Vector3 &b) 
{
	return ((a.x != b.x) || (a.y != b.y) || (a.z != b.z));
}

// Add two Vector3's
Vector3 operator + (const Vector3 &a, const Vector3 &b) 
{
	Vector3 ret(a);
	
	ret += b;
	
	return ret;
}

// Subtract one Vector3 from another
Vector3 operator - (const Vector3 &a, const Vector3 &b) 
{
	Vector3 ret(a);
	
	ret -= b;
	
	return ret;
}

// Multiply Vector3 by a float
Vector3 operator * (const Vector3 &v, float f) 
{
	return Vector3(f * v.x, f * v.y, f * v.z);
}

// Multiply Vector3 by a float
Vector3 operator * (float f, const Vector3 &v) 
{
	return Vector3(f * v.x, f * v.y, f * v.z);
}

// Divide Vector3 by a float
Vector3 operator / (const Vector3 &v, float f) 
{
	return Vector3(v.x / f, v.y / f, v.z / f);
}

// Negate a Vector3
Vector3 operator - (const Vector3 &a) 
{
	return Vector3(-a.x, -a.y, -a.z);
}

// Get length of a Vector3
float Vector3::length() const 
{
	return (float) sqrt(x*x + y*y + z*z);
}

// Get squared length of a Vector3
float Vector3::lengthSqr() const 
{
	return (x*x + y*y + z*z);
}

// Does Vector3 equal (0, 0, 0)?
bool Vector3::isZero() const 
{
	return ((x == 0.0F) && (y == 0.0F) && (z == 0.0F));
}

// Normalize a Vector3
Vector3 &Vector3::normalize() 
{
	float m = length();
	
	if (m > 0.0F) 
		m = 1.0F / m;
	else 
		m = 0.0F;
	x *= m;
	y *= m;
	z *= m;
	
	return *this;
}


////////////////////////////////////////////////////////////
// Vector4 class
//

// Constructor with initializing float values
Vector4::Vector4(float inX, float inY, float inZ, float inW) 
{
	x = inX;
	y = inY;
	z = inZ;
	w = inW;
}

// Constructor with initializing Vector4
Vector4::Vector4(const Vector4 &v) 
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
}

// Constructor with initializing Vector3
Vector4::Vector4(const Vector3 &v) 
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = 0.0F;
}

// Constructor with initializing Vector2
Vector4::Vector4(const Vector2 &v) 
{
	x = v.x;
	y = v.y;
	z = 0.0F;
	w = 0.0F;
}

// Array indexing
float &Vector4::operator [] (unsigned int i) 
{
	assert (i<4);
	
	return *(&x+i);
}

// Array indexing
const float &Vector4::operator [] (unsigned int i) const 
{
	assert (i<4);
	
	return *(&x+i);
}

// Assign
Vector4 &Vector4::operator = (const Vector4 &v) 
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
	
	return *this;
}

// Assign from a Vector3
Vector4 &Vector4::operator = (const Vector3 &v) 
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = 0.0F;
	
	return *this;
}

// Assign from a Vector2
Vector4 &Vector4::operator = (const Vector2 &v) 
{
	x = v.x;
	y = v.y;
	z = 0.0F;
	w = 0.0F;
	
	return *this;
}

// Add a Vector4 to this one
Vector4 &Vector4::operator += (const Vector4 &v) 
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	
	return *this;
}

// Subtract a Vector4 from this one
Vector4 &Vector4::operator -= (const Vector4 &v) 
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	
	return *this;
}

// Multiply the Vector4 by a float
Vector4 &Vector4::operator *= (float f) 
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	
	return *this;
}

// Divide the Vector4 by a float
Vector4 &Vector4::operator /= (float f) 
{
	x /= f;
	y /= f;
	z /= f;
	w /= f;
	
	return *this;
}

// Are these two Vector4's equal?
bool operator == (const Vector4 &a, const Vector4 &b) 
{
	return ((a.x == b.x) && (a.y == b.y) &&
			(a.z == b.z) && (a.w == b.w));
}

// Are these two Vector4's not equal?
bool operator != (const Vector4 &a, const Vector4 &b) 
{
	return ((a.x != b.x) || (a.y != b.y) ||
			(a.z != b.z) || (a.w != b.w));
}

// Add two Vector4's
Vector4 operator + (const Vector4 &a, const Vector4 &b) 
{
	Vector4 ret(a);
	
	ret += b;
	
	return ret;
}

// Subtract one Vector4 from another
Vector4 operator - (const Vector4 &a, const Vector4 &b) 
{
	Vector4 ret(a);
	
	ret -= b;
	
	return ret;
}

// Multiply Vector4 by a float
Vector4 operator * (const Vector4 &v, float f) 
{
	return Vector4(f * v.x, f * v.y, f * v.z, f * v.w);
}

// Multiply Vector4 by a float
Vector4 operator * (float f, const Vector4 &v) 
{
	return Vector4(f * v.x, f * v.y, f * v.z, f * v.w);
}

// Divide Vector4 by a float
Vector4 operator / (const Vector4 &v, float f) 
{
	return Vector4(v.x / f, v.y / f, v.z / f, v.w / f);
}

// Negate a Vector4
Vector4 operator - (const Vector4 &a) 
{
	return Vector4(-a.x, -a.y, -a.z, -a.w);
}

// Get length of a Vector4
float Vector4::length() const 
{
	return (float) sqrt(x*x + y*y + z*z + w*w);
}

// Get squared length of a Vector4
float Vector4::lengthSqr() const 
{
	return (x*x + y*y + z*z + w*w);
}

// Does Vector4 equal (0, 0, 0, 0)?
bool Vector4::isZero() const 
{
	return ((x == 0.0F) && (y == 0.0F) && (z == 0.0F) && (w == 0.0F));
}

// Normalize a Vector4
Vector4 &Vector4::normalize() 
{
	float m = length();
	
	if (m > 0.0F) 
		m = 1.0F / m;
	else 
		m = 0.0F;
	x *= m;
	y *= m;
	z *= m;
	w *= m;
	
	return *this;
}


Color::Color(const Color& c){
	r = c.r;
	g = c.g;
	b = c.b;
	a = c.a;
	
}


Color &Color::operator = (const Color &c) 
{
	r = c.r;
	g = c.g;
	b = c.b;
	a = c.a;
	
	
	return *this;
}

////////////////////////////////////////////////////////////
// Miscellaneous vector functions
//

// Dot product of two Vector2's
float DotProduct(const Vector2 &a, const Vector2 &b) 
{
	return a.x*b.x + a.y*b.y;
}

// Dot product of two Vector3's
float DotProduct(const Vector3 &a, const Vector3 &b) 
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

// Dot product of two Vector4's
float DotProduct(const Vector4 &a, const Vector4 &b) 
{
	return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}

// Swap two Vector2's
void SwapVec(Vector2 &a, Vector2 &b) 
{
	Vector2 tmp(a);
	
	a = b;
	b = tmp;
}

// Swap two Vector3's
void SwapVec(Vector3 &a, Vector3 &b) 
{
	Vector3 tmp(a);
	
	a = b;
	b = tmp;
}

// Swap two Vector4's
void SwapVec(Vector4 &a, Vector4 &b) 
{
	Vector4 tmp(a);
	
	a = b;
	b = tmp;
}

// Cross product of two Vector3's
Vector3 CrossProduct(const Vector3 &a, const Vector3 &b) 
{
	return Vector3(a.y*b.z - a.z*b.y,
				   a.z*b.x - a.x*b.z,
				   a.x*b.y - a.y*b.x);
}

// Are these two Vector2's nearly equal?
bool nearlyEquals( const Vector2& a, const Vector2& b, float r ) 
{
	Vector2 diff = a - b; // difference
	
	return (DotProduct(diff, diff) < r*r); // radius
}

// Are these two Vector3's nearly equal?
bool nearlyEquals( const Vector3& a, const Vector3& b, float r ) 
{
	Vector3 diff = a - b; // difference
	
	return (DotProduct(diff, diff) < r*r); // radius
}

// Are these two Vector4's nearly equal?
bool nearlyEquals( const Vector4& a, const Vector4& b, float r ) 
{
	Vector4 diff = a - b; // difference
	
	return (DotProduct(diff, diff) < r*r); // radius
}


////////////////////////////////////////////////////////////
// Matrix33 class
//

// Constructor with initializing Matrix33
Matrix33::Matrix33(const Matrix33 &m) 
{
	col[0] = m[0];
	col[1] = m[1];
	col[2] = m[2];
}

// Constructor with initializing Vector3's
Matrix33::Matrix33(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2) 
{
	col[0] = v0;
	col[1] = v1;
	col[2] = v2;
}

// Array indexing
Vector3 &Matrix33::operator [] (unsigned int i) 
{
	assert (i<3);
	
	return (Vector3&)col[i];
}

// Array indexing
const Vector3 &Matrix33::operator [] (unsigned int i) const 
{
	assert (i<3);
	
	return (Vector3&)col[i];
}

// Assign
Matrix33 &Matrix33::operator = (const Matrix33 &m) 
{
	col[0] = m[0];
	col[1] = m[1];
	col[2] = m[2];
	
	return *this;
}

// Add a Matrix33 to this one
Matrix33 &Matrix33::operator += (const Matrix33 &m) 
{
	col[0] += m[0];
	col[1] += m[1];
	col[2] += m[2];
	
	return *this;
}

// Subtract a Matrix33 from this one
Matrix33 &Matrix33::operator -= (const Matrix33 &m) 
{
	col[0] -= m[0];
	col[1] -= m[1];
	col[2] -= m[2];
	
	return *this;
}

// Multiply the Matrix33 by another Matrix33
Matrix33 &Matrix33::operator *= (const Matrix33 &m) 
{
	Matrix33 t;
	
	for (unsigned int r = 0; r < 3; r++) 
	{
		for (unsigned int c = 0; c < 3; c++) 
		{
			float f = 0;
			
			f += col[0][r] * m[c][0];
			f += col[1][r] * m[c][1];
			f += col[2][r] * m[c][2];
			
			t[c][r] = f;
		}
	}
	
	*this = t;
	
	return *this;
}

// Multiply the Matrix33 by a float
Matrix33 &Matrix33::operator *= (float f) 
{
	col[0] *= f;
	col[1] *= f;
	col[2] *= f;
	
	return *this;
}

// Are these two Matrix33's equal?
bool operator == (const Matrix33 &a, const Matrix33 &b) 
{
	return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]));
}

// Are these two Matrix33's not equal?
bool operator != (const Matrix33 &a, const Matrix33 &b) 
{
	return ((a[0] != b[0]) || (a[1] != b[1]) || (a[2] != b[2]));
}

// Add two Matrix33's
Matrix33 operator + (const Matrix33 &a, const Matrix33 &b) 
{
	Matrix33 ret(a);
	
	ret += b;
	
	return ret;
}

// Subtract one Matrix33 from another
Matrix33 operator - (const Matrix33 &a, const Matrix33 &b) 
{
	Matrix33 ret(a);
	
	ret -= b;
	
	return ret;
}

// Multiply Matrix33 by another Matrix33
Matrix33 operator * (const Matrix33 &a, const Matrix33 &b) 
{
	Matrix33 ret(a);
	
	ret *= b;
	
	return ret;
}

// Multiply a Vector3 by this Matrix33
Vector3 operator * (const Matrix33 &m, const Vector3 &v) 
{
	Vector3 ret;
	Matrix33 pose(TransposeMatrix33(m));
	
	ret.x = DotProduct(pose[0], v);
	ret.y = DotProduct(pose[1], v);
	ret.z = DotProduct(pose[2], v);
	
	return ret;
}

// Multiply a Vector3 by this Matrix33
Vector3 operator * (const Vector3 &v, const Matrix33 &m) 
{
	Vector3 ret;
	
	ret.x = DotProduct(m[0], v);
	ret.y = DotProduct(m[1], v);
	ret.z = DotProduct(m[2], v);
	
	return ret;
}

// Multiply Matrix33 by a float
Matrix33 operator * (float f, const Matrix33 &m) 
{
	Matrix33 ret(m);
	
	ret *= f;
	
	return ret;
}

// Multiply Matrix33 by a float
Matrix33 operator * (const Matrix33 &m, float f) 
{
	Matrix33 ret(m);
	
	ret *= f;
	
	return ret;
}

// Set Matrix33 to the identity matrix
Matrix33 &Matrix33::identity() 
{
	for (unsigned int c = 0; c < 3; c++) 
	{
		for (unsigned int r = 0; r < 3; r++) 
		{
			if (c == r) 
				col[c][r] = 1.0F;
			else 
				col[c][r] = 0.0F;
		} 
	}
	
	return *this;
}

// Transpose the Matrix33
Matrix33 &Matrix33::transpose() 
{
	float t;
	
	for (unsigned int c = 0; c < 3; c++) 
	{
		for (unsigned int r = c + 1; r < 3; r++) 
		{
			t = col[c][r];
			col[c][r] = col[r][c];
			col[r][c] = t;
		} 
	}
	
	return *this;
}

// Invert the Matrix33
Matrix33 &Matrix33::invert() 
{
	Matrix33 a(*this);
	Matrix33 b(IdentityMatrix33());
	
	unsigned int c, r;
	unsigned int cc;
	unsigned int rowMax; // Points to max abs value row in this column
	unsigned int row;
	float tmp;
	
	// Go through columns
	for (c=0; c<3; c++) 
	{
		// Find the row with max value in this column
		rowMax = c;
		for (r=c+1; r<3; r++) 
		{
			if (fabs(a[c][r]) > fabs(a[c][rowMax])) 
			{
				rowMax = r;
			}
		}
		
		// If the max value here is 0, we can't invert.  Return identity.
		if (a[rowMax][c] == 0.0F) 
			return (identity());
		
		// Swap row "rowMax" with row "c"
		for (cc=0; cc<3; cc++) 
		{
			tmp = a[cc][c];
			a[cc][c] = a[cc][rowMax];
			a[cc][rowMax] = tmp;
			tmp = b[cc][c];
			b[cc][c] = b[cc][rowMax];
			b[cc][rowMax] = tmp;
		}
		
		// Now everything we do is on row "c".
		// Set the max cell to 1 by dividing the entire row by that value
		tmp = a[c][c];
		for (cc=0; cc<3; cc++) 
		{
			a[cc][c] /= tmp;
			b[cc][c] /= tmp;
		}
		
		// Now do the other rows, so that this column only has a 1 and 0's
		for (row = 0; row < 3; row++) 
		{
			if (row != c) 
			{
				tmp = a[c][row];
				for (cc=0; cc<3; cc++) 
				{
					a[cc][row] -= a[cc][c] * tmp;
					b[cc][row] -= b[cc][c] * tmp;
				}
			}
		}
		
	}
	
	*this = b;
	
	return *this;
}

// Return a Matrix33 set to the identity matrix
Matrix33 IdentityMatrix33() 
{
	Matrix33 ret;
	
	return ret.identity();
}

// Return the transpose of the Matrix33
Matrix33 TransposeMatrix33(const Matrix33 &m) 
{
	Matrix33 ret(m);
	
	return ret.transpose();
}

// Return the inverted Matrix33
Matrix33 InvertMatrix33(const Matrix33 &m) 
{
	Matrix33 ret(m);
	
	return ret.invert();
}

// Return a 2D rotation Matrix33
Matrix33 RotateRadMatrix33(float rad) 
{
	Matrix33 ret;
	float sinA, cosA;
	
	sinA = (float)sin(rad);
	cosA = (float)cos(rad);
	
	ret[0][0] = cosA; ret[1][0] = -sinA; ret[2][0] = 0.0F;
	ret[0][1] = sinA; ret[1][1] =  cosA; ret[2][1] = 0.0F;
	ret[0][2] = 0.0F; ret[1][2] =  0.0F; ret[2][2] = 1.0F;
	
	return ret;
}

// Return a 2D translation Matrix33
Matrix33 TranslateMatrix33(float x, float y) 
{
	Matrix33 ret;
	
	ret.identity();
	ret[2][0] = x;
	ret[2][1] = y;
	
	return ret;
}

// Return a 2D/3D scale Matrix33
Matrix33 ScaleMatrix33(float x, float y, float z) 
{
	Matrix33 ret;
	
	ret.identity();
	ret[0][0] = x;
	ret[1][1] = y;
	ret[2][2] = z;
	
	return ret;
}


////////////////////////////////////////////////////////////
// Matrix44 class
//

// Constructor with initializing Matrix44
Matrix44::Matrix44(const Matrix44 &m) 
{
	col[0] = m[0];
	col[1] = m[1];
	col[2] = m[2];
	col[3] = m[3];
}

// Constructor with initializing Matrix33
Matrix44::Matrix44(const Matrix33 &m) 
{
	col[0] = m[0];
	col[1] = m[1];
	col[2] = m[2];
	col[3] = Vector4(0.0, 0.0, 0.0, 1.0);
}

// Constructor with initializing Vector4's
Matrix44::Matrix44(const Vector4 &v0, const Vector4 &v1, 
                   const Vector4 &v2, const Vector4 &v3) 
{
	col[0] = v0;
	col[1] = v1;
	col[2] = v2;
	col[3] = v3;
}

// Array indexing
Vector4 &Matrix44::operator [] (unsigned int i) 
{
	assert (i<4);
	
	return (Vector4&) col[i];
}

// Array indexing
const Vector4 &Matrix44::operator [] (unsigned int i) const 
{
	assert (i<4);
	
	return (Vector4&) col[i];
}

// Assign
Matrix44 &Matrix44::operator = (const Matrix44 &m) 
{
	col[0] = m[0];
	col[1] = m[1];
	col[2] = m[2];
	col[3] = m[3];
	
	return *this;
}

// Assign a Matrix33 to the Matrix44
Matrix44 &Matrix44::operator = (const Matrix33 &m) 
{
	col[0] = m[0];
	col[1] = m[1];
	col[2] = m[2];
	col[3] = Vector4(0.0, 0.0, 0.0, 1.0);
	
	return *this;
}

// Add a Matrix44 to this one
Matrix44 &Matrix44::operator += (const Matrix44 &m) 
{
	col[0] += m[0];
	col[1] += m[1];
	col[2] += m[2];
	col[3] += m[3];
	
	return *this;
}

// Subtract a Matrix44 from this one
Matrix44 &Matrix44::operator -= (const Matrix44 &m) 
{
	col[0] -= m[0];
	col[1] -= m[1];
	col[2] -= m[2];
	col[3] -= m[3];
	
	return *this;
}

// Multiply the Matrix44 by another Matrix44
Matrix44 &Matrix44::operator *= (const Matrix44 &m) 
{
	Matrix44 t;
	
	for (unsigned int r = 0; r < 4; r++) 
	{
		for (unsigned int c = 0; c < 4; c++) 
		{
			float f = 0;
			
			f += col[0][r] * m[c][0];
			f += col[1][r] * m[c][1];
			f += col[2][r] * m[c][2];
			f += col[3][r] * m[c][3];
			
			t[c][r] = f;
		}
	}
	
	*this = t;
	
	return *this;
}

// Multiply the Matrix44 by a float
Matrix44 &Matrix44::operator *= (float f) 
{
	col[0] *= f;
	col[1] *= f;
	col[2] *= f;
	col[3] *= f;
	
	return *this;
}

// Are these two Matrix44's equal?
bool operator == (const Matrix44 &a, const Matrix44 &b) 
{
	return ((a[0] == b[0]) && (a[1] == b[1]) &&
			(a[2] == b[2]) && (a[3] == b[3]));
}

// Are these two Matrix44's not equal?
bool operator != (const Matrix44 &a, const Matrix44 &b) 
{
	return ((a[0] != b[0]) || (a[1] != b[1]) ||
			(a[2] != b[2]) || (a[3] != b[3]));
}

// Add two Matrix44's
Matrix44 operator + (const Matrix44 &a, const Matrix44 &b) 
{
	Matrix44 ret(a);
	
	ret += b;
	
	return ret;
}

// Subtract one Matrix44 from another
Matrix44 operator - (const Matrix44 &a, const Matrix44 &b) 
{
	Matrix44 ret(a);
	
	ret -= b;
	
	return ret;
}

// Multiply Matrix44 by another Matrix44
Matrix44 operator * (const Matrix44 &a, const Matrix44 &b) 
{
	Matrix44 ret(a);
	
	ret *= b;
	
	return ret;
}

// Multiply a Vector4 by this Matrix44
Vector4 operator * (const Matrix44 &m, const Vector4 &v) 
{
	Vector4 ret;
	Matrix44 pose(TransposeMatrix44(m));
	
	ret.x = DotProduct(pose[0], v);
	ret.y = DotProduct(pose[1], v);
	ret.z = DotProduct(pose[2], v);
	ret.w = DotProduct(pose[3], v);
	
	return ret;
}

// Multiply a Vector4 by this Matrix44
Vector4 operator * (const Vector4 &v, const Matrix44 &m) 
{
	Vector4 ret;
	
	ret.x = DotProduct(m[0], v);
	ret.y = DotProduct(m[1], v);
	ret.z = DotProduct(m[2], v);
	ret.w = DotProduct(m[3], v);
	
	return ret;
}

// Multiply Matrix44 by a float
Matrix44 operator * (float f, const Matrix44 &m) 
{
	Matrix44 ret(m);
	
	ret *= f;
	
	return ret;
}

// Set Matrix44 to the identity matrix
Matrix44 &Matrix44::identity() 
{
	for (unsigned int c = 0; c < 4; c++) 
	{
		for (unsigned int r = 0; r < 4; r++) 
		{
			if (c == r) 
				col[c][r] = 1.0F;
			else 
				col[c][r] = 0.0F;
		} 
	} 
	
	return *this;
}

// Transpose the Matrix44
Matrix44 &Matrix44::transpose() 
{
	float t;
	
	for (unsigned int c = 0; c < 4; c++) 
	{
		for (unsigned int r = c + 1; r < 4; r++) 
		{
			t = col[c][r];
			col[c][r] = col[r][c];
			col[r][c] = t;
		} 
	} 
	
	return *this;
}

// Invert the Matrix44
Matrix44 &Matrix44::invert() 
{
	Matrix44 a(*this);
	Matrix44 b(IdentityMatrix44());
	
	unsigned int r, c;
	unsigned int cc;
	unsigned int rowMax; // Points to max abs value row in this column
	unsigned int row;
	float tmp;
	
	// Go through columns
	for (c=0; c<4; c++) 
	{
		
		// Find the row with max value in this column
		rowMax = c;
		for (r=c+1; r<4; r++) 
		{
			if (fabs(a[c][r]) > fabs(a[c][rowMax])) 
			{
				rowMax = r;
			}
		}
		
		// If the max value here is 0, we can't invert.  Return identity.
		if (a[rowMax][c] == 0.0F) 
			return (identity());
		
		// Swap row "rowMax" with row "c"
		for (cc=0; cc<4; cc++) 
		{
			tmp = a[cc][c];
			a[cc][c] = a[cc][rowMax];
			a[cc][rowMax] = tmp;
			tmp = b[cc][c];
			b[cc][c] = b[cc][rowMax];
			b[cc][rowMax] = tmp;
		}
		
		// Now everything we do is on row "c".
		// Set the max cell to 1 by dividing the entire row by that value
		tmp = a[c][c];
		for (cc=0; cc<4; cc++) 
		{
			a[cc][c] /= tmp;
			b[cc][c] /= tmp;
		}
		
		// Now do the other rows, so that this column only has a 1 and 0's
		for (row = 0; row < 4; row++) 
		{
			if (row != c) 
			{
				tmp = a[c][row];
				for (cc=0; cc<4; cc++) 
				{
					a[cc][row] -= a[cc][c] * tmp;
					b[cc][row] -= b[cc][c] * tmp;
				}
			}
		}
		
	}
	
	*this = b;
	
	return *this;
}

// Return a Matrix44 set to the identity matrix
Matrix44 IdentityMatrix44() 
{
	Matrix44 ret;
	
	return ret.identity();
}

// Return the transpose of the Matrix44
Matrix44 TransposeMatrix44(const Matrix44 &m) 
{
	Matrix44 ret(m);
	
	return ret.transpose();
}

// Return the inverted Matrix44
Matrix44 InvertMatrix44(const Matrix44 &m) 
{
	Matrix44 ret(m);
	
	return ret.invert();
}

// Return a 3D axis-rotation Matrix44
// Pass in 'x', 'y', or 'z' for the axis.
Matrix44 RotateRadMatrix44(char axis, float rad) 
{
	Matrix44 ret;
	float sinA, cosA;
	
	sinA = (float)sin(rad);
	cosA = (float)cos(rad);
	
	switch(axis) 
	{
		case 'x':
		case 'X':
			ret[0][0] =  1.0F; ret[1][0] =  0.0F; ret[2][0] =  0.0F;
			ret[0][1] =  0.0F; ret[1][1] =  cosA; ret[2][1] = -sinA;
			ret[0][2] =  0.0F; ret[1][2] =  sinA; ret[2][2] =  cosA;
			break;
			
		case 'y':
		case 'Y':
			ret[0][0] =  cosA; ret[1][0] =  0.0F; ret[2][0] =  sinA;
			ret[0][1] =  0.0F; ret[1][1] =  1.0F; ret[2][1] =  0.0F;
			ret[0][2] = -sinA; ret[1][2] =  0.0F; ret[2][2] =  cosA;
			break;
			
		case 'z':
		case 'Z':
			ret[0][0] =  cosA; ret[1][0] = -sinA; ret[2][0] =  0.0F;
			ret[0][1] =  sinA; ret[1][1] =  cosA; ret[2][1] =  0.0F;
			ret[0][2] =  0.0F; ret[1][2] =  0.0F; ret[2][2] =  1.0F;
			break;
	}
	
	ret[0][3] = 0.0F; ret[1][3] = 0.0F; ret[2][3] = 0.0F;
	ret[3][0] = 0.0F;
	ret[3][1] = 0.0F;
	ret[3][2] = 0.0F;
	ret[3][3] = 1.0F;
	
	return ret;
}

// Return a 3D axis-rotation Matrix44
// Pass in an arbitrary Vector3 axis.
Matrix44 RotateRadMatrix44(const Vector3 &axis, float rad) 
{
	Matrix44 ret;
	float sinA, cosA;
	float invCosA;
	Vector3 nrm = axis;
	float x, y, z;
	float xSq, ySq, zSq;
	
	nrm.normalize();
	sinA = (float)sin(rad);
	cosA = (float)cos(rad);
	invCosA = 1.0F - cosA;
	
	x = nrm.x;
	y = nrm.y;
	z = nrm.z;
	
	xSq = x * x;
	ySq = y * y;
	zSq = z * z;
	
	ret[0][0] = (invCosA * xSq) + (cosA);
	ret[1][0] = (invCosA * x * y) - (sinA * z );
	ret[2][0] = (invCosA * x * z) + (sinA * y );
	ret[3][0] = 0.0F;
	
	ret[0][1] = (invCosA * x * y) + (sinA * z);
	ret[1][1] = (invCosA * ySq) + (cosA);
	ret[2][1] = (invCosA * y * z) - (sinA * x);
	ret[3][1] = 0.0F;
	
	ret[0][2] = (invCosA * x * z) - (sinA * y);
	ret[1][2] = (invCosA * y * z) + (sinA * x);
	ret[2][2] = (invCosA * zSq) + (cosA);
	ret[3][2] = 0.0F;
	
	ret[0][3] = 0.0F;
	ret[1][3] = 0.0F;
	ret[2][3] = 0.0F;
	ret[3][3] = 1.0F;
	
	return ret;
}

// Return a 3D translation Matrix44
Matrix44 TranslateMatrix44(float x, float y, float z) 
{
	Matrix44 ret;
	
	ret.identity();
	ret[3][0] = x;
	ret[3][1] = y;
	ret[3][2] = z;
	
	return ret;
}

// Return a 3D/4D scale Matrix44
Matrix44 ScaleMatrix44(float x, float y, float z, float w) 
{
	Matrix44 ret;
	
	ret.identity();
	ret[0][0] = x;
	ret[1][1] = y;
	ret[2][2] = z;
	ret[3][3] = w;
	
	return ret;
}

// Return a "lookat" Matrix44 given the current camera position (Vector3),
//   camera-up Vector3, and camera-target Vector3.
Matrix44 LookAtMatrix44(const Vector3 &camPos, const Vector3 &camUp, 
                        const Vector3 &target ) 
{
	Matrix44 ret;
	
	Vector3 F = target - camPos;
	F.normalize();
	
	Vector3 upNorm = camUp;
	upNorm.normalize();
	
	Vector3 s = CrossProduct(F, upNorm);
	Vector3 u = CrossProduct(s, F);
	
	ret[0][0] = s[0]; ret[1][0] = s[1]; ret[2][0] = s[2]; ret[3][0] = 0.0F;
	ret[0][1] = u[0]; ret[1][1] = u[1]; ret[2][1] = u[2]; ret[3][1] = 0.0F;
	ret[0][2] =-F[0]; ret[1][2] =-F[1]; ret[2][2] =-F[2]; ret[3][2] = 0.0F;
	ret[0][3] = 0.0F; ret[1][3] = 0.0F; ret[2][3] = 0.0F; ret[3][3] = 1.0F;
	
	return ret;
}

// Return a frustum Matrix44 given the left, right, bottom, top,
//   near, and far values for the frustum boundaries.
Matrix44 FrustumMatrix44(float l, float r, 
                         float b, float t, float n, float f) 
{
	Matrix44 ret;
	float width = r-l;
	float height = t-b;
	float depth = f-n;
	
	ret[0][0] = (2*n) / width;
	ret[0][1] = 0.0F;
	ret[0][2] = 0.0F;
	ret[0][3] = 0.0F;
	
	ret[1][0] = 0.0F;
	ret[1][1] = (2*n) / height;
	ret[1][2] = 0.0F;
	ret[1][3] = 0.0F;
	
	ret[2][0] = (r + l) / width;
	ret[2][1] = (t + b) / height;
	ret[2][2] = -(f + n) / depth;
	ret[2][3] = -1.0F;
	
	ret[3][0] = 0.0F;
	ret[3][1] = 0.0F;
	ret[3][2] = -(2*f*n) / depth;
	ret[3][3] = 0.0F;
	
	return ret;
}

// Return a perspective Matrix44 given the field-of-view in the Y
//   direction in degrees, the aspect ratio of Y/X, and near and
//   far plane distances.
Matrix44 PerspectiveMatrix44(float fovY, float aspect, float n, float f) 
{
	Matrix44 ret;
	float angle;
	float cot;
	
	angle = fovY / 2.0F;
	angle = DegToRad( angle );
	
	cot = (float) cos(angle) / (float) sin(angle);
	
	ret[0][0] = cot / aspect;
	ret[0][1] = 0.0F;
	ret[0][2] = 0.0F;
	ret[0][3] = 0.0F;
	
	ret[1][0] = 0.0F;
	ret[1][1] = cot;
	ret[1][2] = 0.0F;
	ret[1][3] = 0.0F;
	
	ret[2][0] = 0.0F;
	ret[2][1] = 0.0F;
	ret[2][2] = -(f + n) / (f - n);
	ret[2][3] = -1.0F;
	
	
	ret[3][0] = 0.0F;
	ret[3][1] = 0.0F;
	ret[3][2] = -(2*f*n) / (f - n);
	ret[3][3] = 0.0F;
	
	return ret;
}

// Return an orthographic Matrix44 given the left, right, bottom, top,
//   near, and far values for the frustum boundaries.
Matrix44 OrthoMatrix44(float l, float r, 
                       float b, float t, float n, float f) 
{
	Matrix44 ret;
	float width = r-l;
	float height = t-b;
	float depth = f-n;
	
	ret[0][0] = 2.0F / width;
	ret[0][1] = 0.0F;
	ret[0][2] = 0.0F;
	ret[0][3] = 0.0F;
	
	ret[1][0] = 0.0F;
	ret[1][1] = 2.0F / height;
	ret[1][2] = 0.0F;
	ret[1][3] = 0.0F;
	
	ret[2][0] = 0.0F;
	ret[2][1] = 0.0F;
	ret[2][2] = -(2.0F) / depth;
	ret[2][3] = 0.0F;
	
	ret[3][0] = -(r + l) / width;
	ret[1][3] = -(t + b) / height;
	ret[3][2] = -(f + n) / depth;
	ret[3][3] = 1.0F;
	
	return ret;
}

/*
////////////////////////////////////////////////////////////
// Debug functions
//

// Print a Vector2 to a file
void Vector2::fprint(FILE* file, char* str) const 
{
	fprintf(file, "%sVector2: <%f, %f>\n", str, x, y);
}

// Print a Vector3 to a file
void Vector3::fprint(FILE* file, char* str) const 
{
	fprintf(file, "%sVector3: <%f, %f, %f>\n", str, x, y, z);
}

// Print a Vector4 to a file
void Vector4::fprint(FILE* file, char* str) const 
{
	fprintf(file, "%sVector4: <%f, %f, %f, %f>\n", str, x, y, z, w);
}

// Print a Matrix33 to a file
void Matrix33::fprint(FILE* file, char * str) const 
{
	fprintf(file, "%sMatrix33:\n", str);
	col[0].fprint(file, "\t");
	col[1].fprint(file, "\t");
	col[2].fprint(file, "\t");
}

// Print a Matrix44 to a file
void Matrix44::fprint(FILE* file, char* str) const 
{
	fprintf(file, "%sMatrix44:\n", str);
	col[0].fprint(file, "\t");
	col[1].fprint(file, "\t");
	col[2].fprint(file, "\t");
	col[3].fprint(file, "\t");
}
*/



/////////////////////////////////////////////////////////////////////////////
//Quaternion stuff
Quaternion* Quaternion::multiply(Quaternion *qb ){
	//Routine checked and works - 5/24/03
	float w1,w2;
	
	Vector3 v1(vector3.x,vector3.y,vector3.z);
	Vector3 v2(qb->vector3.x,qb->vector3.y,qb->vector3.z);
	w1 = scalar;
	w2 = qb->scalar;
	
	
	Vector3 crossResult = CrossProduct(v1,v2);
	float dotResult = DotProduct(v1,v2);
	v1 *= w2;
	v2 *= w1;
	
	this->scalar = w1*w2 - dotResult;
	this->vector3.x = v1.x + v2.x + crossResult.x;
	this->vector3.y = v1.y + v2.y + crossResult.y;
	this->vector3.z = v1.z + v2.z + crossResult.z;
	
	normalize();
	
	return this;
	
}

void Quaternion::normalize(){
	//Routine checked and works - 5/24/03
	float magnitude;
	magnitude = (float)(
						(vector3.x * vector3.x) + 
						(vector3.y * vector3.y) + 
						(vector3.z * vector3.z) + 
						(scalar * scalar)
						);
	
	// DIVIDE BY THE MAGNITUDE TO NORMALIZE
	vector3.x = vector3.x / magnitude;
	vector3.y = vector3.y / magnitude;
	vector3.z = vector3.z / magnitude;
	scalar = scalar / magnitude;
}


//This works just like calling rotateZ then rotateY then rotateX on a opengl matrix or Matrix44;
Quaternion* Quaternion::fromEulers(Vector3 *rot){
	//Routine checked and works - 5/21/03
	float rx,ry,rz,tx,ty,tz,cx,cy,cz,sx,sy,sz,cc,cs,sc,ss;
    //Quaternion * quat = new Quaternion();
	
	// FIRST STEP, CONVERT ANGLES TO RADIANS
	rx =  (rot->x * (float)PI) / (360 / 2);
	ry =  (rot->y * (float)PI) / (360 / 2);
	rz =  (rot->z * (float)PI) / (360 / 2);
	// GET THE HALF ANGLES
	tx = rx * (float)0.5;
	ty = ry * (float)0.5;
	tz = rz * (float)0.5;
	cx = (float)cos(tx);
	cy = (float)cos(ty);
	cz = (float)cos(tz);
	sx = (float)sin(tx);
	sy = (float)sin(ty);
	sz = (float)sin(tz);
	
	cc = cx * cz;
	cs = cx * sz;
	sc = sx * cz;
	ss = sx * sz;
	
	vector3.x = (cy * sc) - (sy * cs);
	vector3.y = (cy * ss) + (sy * cc);
	vector3.z = (cy * cs) - (sy * sc);
	scalar    = (cy * cc) + (sy * ss);
	
	// INSURE THE QUATERNION IS NORMALIZED
	// PROBABLY NOT NECESSARY IN MOST CASES
	normalize();
	return this;
}

Quaternion*  Quaternion::fromAxisAngle(Vector3 *prmAxis,float angle){
	//Routine checked and works - 5/23/03
	vector3 = *prmAxis;
	float angleRadians = DegToRad(angle/2.0f);
	
	float sin_a = (float)sin(angleRadians);
    float cos_a = (float)cos(angleRadians);
	
	vector3.normalize();
	vector3 *= sin_a;
	
    scalar    = cos_a;
	
	normalize();
	return this;
}

Vector4* Quaternion::toAxisAngle(){
	//Not checked!
	float scale,tw;
	Vector4* axisAngle = new Vector4();
	
	tw = (float)acos(scalar) * 2;
	scale = (float)sin(tw / 2.0);
	axisAngle->x = vector3.x / scale;
	axisAngle->y = vector3.y / scale;
	axisAngle->z = vector3.z / scale;
	
	axisAngle->w = (tw * (360 / 2)) / (float)PI;
	
	return axisAngle;
}

float* Quaternion::toMatrixf(){
	float* mat	  = (float*)malloc(sizeof(float)*16);
    float xx      = vector3.x * vector3.x;
    float xy      = vector3.x * vector3.y;
    float xz      = vector3.x * vector3.z;
    float xw      = vector3.x * scalar;
	
    float yy      = vector3.y * vector3.y;
    float yz      = vector3.y * vector3.z;
    float yw      = vector3.y * scalar;
	
    float zz      = vector3.z * vector3.z;
    float zw      = vector3.z * scalar;
	
    mat[0]  = 1 - 2 * ( yy + zz );
    mat[1]  =     2 * ( xy - zw );
    mat[2]  =     2 * ( xz + yw );
	
    mat[4]  =     2 * ( xy + zw );
    mat[5]  = 1 - 2 * ( xx + zz );
    mat[6]  =     2 * ( yz - xw );
	
    mat[8]  =     2 * ( xz - yw );
    mat[9]  =     2 * ( yz + xw );
    mat[10] = 1 - 2 * ( xx + yy );
	
    mat[3]  = mat[7] = mat[11] = mat[12] = mat[13] = mat[14] = 0.0;
    mat[15] = 1.0;
	
	return mat;
}

float* Quaternion::toMatrixd(){//This is a row major version --not OpenGL Comapatible
	//Routine checked and works - 5/21/03
	float* mat	  = new float[16];
    float xx      = vector3.x * vector3.x;
    float xy      = vector3.x * vector3.y;
    float xz      = vector3.x * vector3.z;
    float xw      = vector3.x * scalar;
	
    float yy      = vector3.y * vector3.y;
    float yz      = vector3.y * vector3.z;
    float yw      = vector3.y * scalar;
	
    float zz      = vector3.z * vector3.z;
    float zw      = vector3.z * scalar;
	
    mat[0]  = 1 - 2 * ( yy + zz );
    mat[1]  =     2 * ( xy - zw );
    mat[2]  =     2 * ( xz + yw );
	
    mat[4]  =     2 * ( xy + zw );
    mat[5]  = 1 - 2 * ( xx + zz );
    mat[6]  =     2 * ( yz - xw );
	
    mat[8]  =     2 * ( xz - yw );
    mat[9]  =     2 * ( yz + xw );
    mat[10] = 1 - 2 * ( xx + yy );
	
    mat[3]  = mat[7] = mat[11] = mat[12] = mat[13] = mat[14] = 0.0;
    mat[15] = 1.0;
	
	return mat;
}

Matrix44* Quaternion::toMatrix44(){
	float* mat = toMatrixd();
	Vector4 c1(mat[0],mat[4], mat[8], mat[12]);
	Vector4 c2(mat[1],mat[5], mat[9], mat[13]);
	Vector4 c3(mat[2],mat[6], mat[10], mat[14]);
	Vector4 c4(mat[3],mat[7], mat[11], mat[15]);
	Matrix44* m= new Matrix44(c1,c2,c3,c4);
	m->transpose();//Have to do this because toMatrixd() return row major version of matrix and we need column major
	return m;
}


void SlerpQuat(Quaternion *quat1,Quaternion *quat2,float slerp, Quaternion *result){
	//Not checked!!!
#define DELTA	0.0001		// DIFFERENCE AT WHICH TO LERP INSTEAD OF SLERP
	float omega,cosom,sinom;
	float scale0,scale1;
	// USE THE DOT PRODUCT TO GET THE COSINE OF THE ANGLE BETWEEN THE
	// QUATERNIONS
	cosom = quat1->vector3.x * quat2->vector3.x + 
	quat1->vector3.y * quat2->vector3.y + 
	quat1->vector3.z * quat2->vector3.z + 
	quat1->scalar * quat2->scalar; 
	
	// CHECK A COUPLE OF SPECIAL CASES. 
	// MAKE SURE THE TWO QUATERNIONS ARE NOT EXACTLY OPPOSITE? (WITHIN A LITTLE SLOP)
	if ((1.0 + cosom) > DELTA)
	{
		// ARE THEY MORE THAN A LITTLE BIT DIFFERENT? AVOID A DIVIDED BY ZERO AND LERP IF NOT
		if ((1.0 - cosom) > DELTA) {
			// YES, DO A SLERP
			omega = acos(cosom);
			sinom = sin(omega);
			scale0 = (float)(sin((1.0 - slerp) * omega) / sinom);
			scale1 = (float)(sin(slerp * omega) / sinom);
		} else {
			// NOT A VERY BIG DIFFERENCE, DO A LERP
			scale0 = (float)(1.0 - slerp);
			scale1 = (float)(slerp);
		}
		result->vector3.x = scale0 * quat1->vector3.x + scale1 * quat2->vector3.x;
		result->vector3.y = scale0 * quat1->vector3.y + scale1 * quat2->vector3.y;
		result->vector3.z = scale0 * quat1->vector3.z + scale1 * quat2->vector3.z;
		result->scalar = scale0 * quat1->scalar + scale1 * quat2->scalar;
	} else {
		// THE QUATERNIONS ARE NEARLY OPPOSITE SO TO AVOID A DIVIDED BY ZERO ERROR
		// CALCULATE A PERPENDICULAR QUATERNION AND SLERP THAT DIRECTION
		result->vector3.x = -quat2->vector3.y;
		result->vector3.y = quat2->vector3.x;
		result->vector3.z = -quat2->scalar;
		result->scalar = quat2->vector3.z;
		scale0 = (float)(sin((1.0 - slerp) * (float)HALF_PI));
		scale1 = (float)(sin(slerp * (float)HALF_PI));
		result->vector3.x = scale0 * quat1->vector3.x + scale1 * result->vector3.x;
		result->vector3.y = scale0 * quat1->vector3.y + scale1 * result->vector3.y;
		result->vector3.z = scale0 * quat1->vector3.z + scale1 * result->vector3.z;
		result->scalar = scale0 * quat1->scalar + scale1 * result->scalar;
	}
	
}

Quaternion* Quaternion::SLERP(Quaternion* quat2, float slerp){
	//Not checked!!!
	Quaternion* result = new Quaternion();
    SlerpQuat(this,quat2,slerp,result);
	return result;
}

Vector3* Quaternion::toEulers(){
	//Not checked!!!
	Vector3 *euler = new Vector3();
	float matrix[3][3];
	float cx,sx;
	float cy,sy,yr;
	float cz,sz;
	
	//CONVERT QUATERNION TO MATRIX - I DON'T REALLY NEED ALL OF IT
	matrix[0][0] = 1.0f - (2.0f * vector3.y * vector3.y) - (2.0f * vector3.z * vector3.z);
	//	matrix[0][1] = (2.0f * vector3.x * vector3.y) - (2.0f * scalar * vector3.z);
	//	matrix[0][2] = (2.0f * vector3.x * vector3.z) + (2.0f * scalar * vector3.y);
	matrix[1][0] = (2.0f * vector3.x * vector3.y) + (2.0f * scalar * vector3.z);
	//	matrix[1][1] = 1.0f - (2.0f * vector3.x * vector3.x) - (2.0f * vector3.z * vector3.z);
	//	matrix[1][2] = (2.0f * vector3.y * vector3.z) - (2.0f * scalar * vector3.x);
	matrix[2][0] = (2.0f * vector3.x * vector3.z) - (2.0f * scalar * vector3.y);
	matrix[2][1] = (2.0f * vector3.y * vector3.z) + (2.0f * scalar * vector3.x);
	matrix[2][2] = 1.0f - (2.0f * vector3.x * vector3.x) - (2.0f * vector3.y * vector3.y);
	
	sy = -matrix[2][0];
	cy = (float)(sqrt(1 - (sy * sy)));
	yr = (float)atan2(sy,cy);
	euler->y = (yr * 180.0f) / (float)PI;
	
	// AVOID DIVIDE BY ZERO ERROR ONLY WHERE Y= +-90 or +-270 
	// NOT CHECKING cy BECAUSE OF PRECISION ERRORS
	if (sy != 1.0f && sy != -1.0f){
		cx = matrix[2][2] / cy;
		sx = matrix[2][1] / cy;
		euler->x = ((float)atan2(sx,cx) * 180.0f) / (float)PI;	// RAD TO DEG
		
		cz = matrix[0][0] / cy;
		sz = matrix[1][0] / cy;
		euler->z = ((float)atan2(sz,cz) * 180.0f) / (float)PI;	// RAD TO DEG
	}
	else{
		// SINCE Cos(Y) IS 0, I AM SCREWED.  ADOPT THE STANDARD Z = 0
		// I THINK THERE IS A WAY TO FIX THIS BUT I AM NOT SURE.  EULERS SUCK
		// NEED SOME MORE OF THE MATRIX TERMS NOW
		matrix[1][1] = 1.0f - (2.0f * vector3.x * vector3.x) - (2.0f * vector3.z * vector3.z);
		matrix[1][2] = (2.0f * vector3.y * vector3.z) - (2.0f * scalar * vector3.x);
		cx = matrix[1][1];
		sx = -matrix[1][2];
		euler->x = ((float)atan2(sx,cx) * 180.0f) / (float)PI;	// RAD TO DEG
		
		cz = 1.0f;
		sz = 0.0f;
		euler->z = ((float)atan2(sz,cz) * 180.0f) / (float)PI;	// RAD TO DEG
	}
	return euler;
}
///////////////////////////////////////////////////////////////////////////////
// Function:	MultQuaternions
// Purpose:		Computes the product of two quaternions
// Arguments:	pointer to quaternions and dest
///////////////////////////////////////////////////////////////////////////////
void MultQuaternions(tQuaternion *quat1, tQuaternion *quat2, tQuaternion *dest)
{
	/// Local Variables ///////////////////////////////////////////////////////////
	tQuaternion v1,v2,v3,vf;
	///////////////////////////////////////////////////////////////////////////////
	v1.x = quat1->x;
	v1.y = quat1->y;
	v1.z = quat1->z;
	//CopyVector((Vector3 *)&v1, (Vector3 *)quat1);		// COPY OFF THE VECTOR PART OF THE QUAT1
	v1.x *= quat2->w;
	v1.y *= quat2->w;
	v1.z *= quat2->w;
	//ScaleVector((Vector3 *)&v1,quat2->w);	// MULTIPLY IT BY THE SCALAR PART OF QUAT2
	
	
	v2.x = quat2->x;
	v2.y = quat2->y;
	v2.z = quat2->z;
	//CopyVector((Vector3 *)&v2, (Vector3 *)quat2);		// COPY OFF THE VECTOR PART OF THE QUAT1
	v2.x *= quat1->w;
	v2.y *= quat1->w;
	v2.z *= quat1->w;
	//ScaleVector((Vector3 *)&v2,quat1->w);	// MULTIPLY IT BY THE SCALAR PART OF QUAT2
	
	
	v3.x = quat2->y*quat1->z - quat2->z*quat1->y;
	v3.y = quat2->z*quat1->x - quat2->x*quat1->z;
	v3.z = quat2->x*quat1->y - quat2->y*quat1->x;
	//CrossVectors((Vector3 *)quat2,(Vector3 *)quat1,(Vector3 *)&v3);
	vf.x = v1.x + v2.x + v3.x;
	vf.y = v1.y + v2.y + v3.y;
	vf.z = v1.z + v2.y + v3.y;
	//AddVectors((Vector3 *)&v1, (Vector3 *)&v2, (Vector3 *)&vf);
	//AddVectors((Vector3 *)&v3, (Vector3 *)&vf, (Vector3 *)&vf);
	
	float dotOfQ1Q2 = quat1->x*quat2->x + quat1->y*quat2->y + quat1->z*quat2->z;
	vf.w = (quat1->w * quat2->w) -dotOfQ1Q2;
	
	dest->x = vf.x;
	dest->y = vf.y;
	dest->z = vf.z;
	dest->w = vf.w;
}
//// MultQuaternions //////////////////////////////////////////////////////////

/* AN OPTIMIZATION/REORGANIZATION OF ABOVE CODE - NOT AS CLEAR 
 I THINK THIS IS SIMILAR TO GRAPHIC GEMS THOUGH I DON'T HAVE THE REF HANDY
 THE MATH CHECKS OUT THOUGH */
///////////////////////////////////////////////////////////////////////////////
// Function:	MultQuaternions2
// Purpose:		Computes the product of two quaternions
// Arguments:	pointer to quaternions and dest
///////////////////////////////////////////////////////////////////////////////
void MultQuaternions2(tQuaternion *quat1, tQuaternion *quat2, tQuaternion *dest)
{
    tQuaternion tmp;
    tmp.x =		quat2->w * quat1->x + quat2->x * quat1->w +
	quat2->y * quat1->z - quat2->z * quat1->y;
    tmp.y  =	quat2->w * quat1->y + quat2->y * quat1->w +
	quat2->z * quat1->x - quat2->x * quat1->z;
    tmp.z  =	quat2->w * quat1->z + quat2->z * quat1->w +
	quat2->x * quat1->y - quat2->y * quat1->x;
    tmp.w  =	quat2->w * quat1->w - quat2->x * quat1->x -
	quat2->y * quat1->y - quat2->z * quat1->z;
    dest->x = tmp.x; dest->y = tmp.y;
    dest->z = tmp.z; dest->w = tmp.w;
}
//// MultQuaternions2 //////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Function:	NormalizeQuaternion
// Purpose:		Normalize a Quaternion
// Arguments:	a quaternion to set
// Discussion:  Quaternions must follow the rules of x^2 + y^2 + z^2 + w^2 = 1
//				This function insures this
///////////////////////////////////////////////////////////////////////////////
void NormalizeQuaternion(tQuaternion *quat)
{
	/// Local Variables ///////////////////////////////////////////////////////////
	float magnitude;
	///////////////////////////////////////////////////////////////////////////////
	// FIRST STEP, FIND THE MAGNITUDE
	magnitude = (quat->x * quat->x) + 
	(quat->y * quat->y) + 
	(quat->z * quat->z) + 
	(quat->w * quat->w);
	
	// DIVIDE BY THE MAGNITUDE TO NORMALIZE
	quat->x = quat->x / magnitude;
	quat->y = quat->y / magnitude;
	quat->z = quat->z / magnitude;
	quat->w = quat->w / magnitude;
}
// NormalizeQuaternion  ///////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// THESE TWO PROCEDURES ARE FUNCTIONALLY EQUIVALENT.  TWO METHODS TO CONVERT
// A SERIES OF ROTATIONS TO QUATERNIONS.  
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Function:	EulerToQuaternion
// Purpose:		Convert a set of Euler angles to a Quaternion
// Arguments:	A rotation set of 3 angles, a quaternion to set
// Discussion:  As the order of rotations is important I am
//				using the Quantum Mechanics convention of (X,Y,Z)
//				a Yaw-Pitch-Roll (Y,X,Z) system would have to be
//				adjusted.  It is more efficient this way though.
///////////////////////////////////////////////////////////////////////////////
void EulerToQuaternion(Vector3 *rot, tQuaternion *quat)
{
	/// Local Variables ///////////////////////////////////////////////////////////
	float rx,ry,rz,tx,ty,tz,cx,cy,cz,sx,sy,sz,cc,cs,sc,ss;
	///////////////////////////////////////////////////////////////////////////////
	// FIRST STEP, CONVERT ANGLES TO RADIANS
	rx =  (rot->x * (float)PI) / (360 / 2);
	ry =  (rot->y * (float)PI) / (360 / 2);
	rz =  (rot->z * (float)PI) / (360 / 2);
	// GET THE HALF ANGLES
	tx = rx * (float)0.5;
	ty = ry * (float)0.5;
	tz = rz * (float)0.5;
	cx = (float)cos(tx);
	cy = (float)cos(ty);
	cz = (float)cos(tz);
	sx = (float)sin(tx);
	sy = (float)sin(ty);
	sz = (float)sin(tz);
	
	cc = cx * cz;
	cs = cx * sz;
	sc = sx * cz;
	ss = sx * sz;
	
	quat->x = (cy * sc) - (sy * cs);
	quat->y = (cy * ss) + (sy * cc);
	quat->z = (cy * cs) - (sy * sc);
	quat->w = (cy * cc) + (sy * ss);
	
	// INSURE THE QUATERNION IS NORMALIZED
	// PROBABLY NOT NECESSARY IN MOST CASES
	NormalizeQuaternion(quat);
}
// EulerToQuaternion  /////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Function:	EulerToQuaternion2
// Purpose:		Convert a set of Euler angles to a Quaternion
// Arguments:	A rotation set of 3 angles, a quaternion to set
// Discussion:  This is a second variation.  It creates a
//				Series of quaternions and multiplies them together
//				It would be easier to extend this for other rotation orders
///////////////////////////////////////////////////////////////////////////////
void EulerToQuaternion2(Vector3 *rot, tQuaternion *quat)
{
	/// Local Variables ///////////////////////////////////////////////////////////
	float rx,ry,rz,ti,tj,tk;
	tQuaternion qx,qy,qz,qf;
	///////////////////////////////////////////////////////////////////////////////
	// FIRST STEP, CONVERT ANGLES TO RADIANS
	rx =  (rot->x * (float)PI) / (360 / 2);
	ry =  (rot->y * (float)PI) / (360 / 2);
	rz =  (rot->z * (float)PI) / (360 / 2);
	// GET THE HALF ANGLES
	ti = rx * (float)0.5;
	tj = ry * (float)0.5;
	tk = rz * (float)0.5;
	
	qx.x = (float)sin(ti); qx.y = 0.0; qx.z = 0.0; qx.w = (float)cos(ti);
	qy.x = 0.0; qy.y = (float)sin(tj); qy.z = 0.0; qy.w = (float)cos(tj);
	qz.x = 0.0; qz.y = 0.0; qz.z = (float)sin(tk); qz.w = (float)cos(tk);
	
	MultQuaternions(&qx,&qy,&qf);
	MultQuaternions(&qf,&qz,&qf);
	
	// ANOTHER TEST VARIATION
	//	MultQuaternions2(&qx,&qy,&qf);
	//	MultQuaternions2(&qf,&qz,&qf);
	
	// INSURE THE QUATERNION IS NORMALIZED
	// PROBABLY NOT NECESSARY IN MOST CASES
	NormalizeQuaternion(&qf);
	
	quat->x = qf.x;
	quat->y = qf.y;
	quat->z = qf.z;
	quat->w = qf.w;
	
}
// EulerToQuaternion2  /////////////////////////////////////////////////////////
void AxisAngleToQuat(Vector3 *axis,float angle,tQuaternion *q){
	float sin_a = (float)sin( angle / 2 );
    float cos_a = (float)cos( angle / 2 );
	
    q->x    = axis-> x / sin_a;
    q->y    = axis-> y / sin_a;
    q->z    = axis-> z / sin_a;
    q->w    = cos_a;
}
///////////////////////////////////////////////////////////////////////////////
// Function:	QuatToAxisAngle
// Purpose:		Convert a Quaternion to Axis Angle representation
// Arguments:	A quaternion to convert, a axisAngle to set
///////////////////////////////////////////////////////////////////////////////
void QuatToAxisAngle(tQuaternion *quat,tQuaternion *axisAngle)
{
	/// Local Variables ///////////////////////////////////////////////////////////
	float scale,tw;
	///////////////////////////////////////////////////////////////////////////////
	tw = (float)acos(quat->w) * 2;
	scale = (float)sin(tw / 2.0);
	axisAngle->x = quat->x / scale;
	axisAngle->y = quat->y / scale;
	axisAngle->z = quat->z / scale;
	
	// NOW CONVERT THE ANGLE OF ROTATION BACK TO DEGREES
	axisAngle->w = (tw * (360 / 2)) / (float)PI;
}
// QuatToAxisAngle  /////////////////////////////////////////////////////////

// THIS ROUTINE IS REALLY FOR THE APRIL ARTICLE BUT IF YOU WANT TO PLAY
// AROUND WITH IT, HERE IT IS.

#define DELTA	0.0001		// DIFFERENCE AT WHICH TO LERP INSTEAD OF SLERP
///////////////////////////////////////////////////////////////////////////////
// Function:	SlerpQuat
// Purpose:		Spherical Linear Interpolation Between two Quaternions
// Arguments:	Two Quaternions, blend factor, result quaternion
// Notes:		The comments explain the handling of the special cases.
//				The comments in the magazine were wrong.  Adjust the
//				DELTA constant to play with the LERP vs. SLERP level
///////////////////////////////////////////////////////////////////////////////
void SlerpQuat(tQuaternion *quat1,tQuaternion *quat2,float slerp, tQuaternion *result)
{
	/// Local Variables ///////////////////////////////////////////////////////////
	float omega,cosom,sinom;
	float scale0,scale1;
	///////////////////////////////////////////////////////////////////////////////
	// USE THE DOT PRODUCT TO GET THE COSINE OF THE ANGLE BETWEEN THE
	// QUATERNIONS
	cosom = quat1->x * quat2->x + 
	quat1->y * quat2->y + 
	quat1->z * quat2->z + 
	quat1->w * quat2->w; 
	
	// CHECK A COUPLE OF SPECIAL CASES. 
	// MAKE SURE THE TWO QUATERNIONS ARE NOT EXACTLY OPPOSITE? (WITHIN A LITTLE SLOP)
	if ((1.0 + cosom) > DELTA)
	{
		// ARE THEY MORE THAN A LITTLE BIT DIFFERENT? AVOID A DIVIDED BY ZERO AND LERP IF NOT
		if ((1.0 - cosom) > DELTA) {
			// YES, DO A SLERP
			omega = acos(cosom);
			sinom = sin(omega);
			scale0 = (float)(sin((1.0 - slerp) * omega) / sinom);
			scale1 = (float)(sin(slerp * omega) / sinom);
		} else {
			// NOT A VERY BIG DIFFERENCE, DO A LERP
			scale0 = (float)(1.0 - slerp);
			scale1 = (float)(slerp);
		}
		result->x = scale0 * quat1->x + scale1 * quat2->x;
		result->y = scale0 * quat1->y + scale1 * quat2->y;
		result->z = scale0 * quat1->z + scale1 * quat2->z;
		result->w = scale0 * quat1->w + scale1 * quat2->w;
	} else {
		// THE QUATERNIONS ARE NEARLY OPPOSITE SO TO AVOID A DIVIDED BY ZERO ERROR
		// CALCULATE A PERPENDICULAR QUATERNION AND SLERP THAT DIRECTION
		result->x = -quat2->y;
		result->y = quat2->x;
		result->z = -quat2->w;
		result->w = quat2->z;
		scale0 = (float)(sin((1.0 - slerp) * (float)HALF_PI));
		scale1 = (float)(sin(slerp * (float)HALF_PI));
		result->x = scale0 * quat1->x + scale1 * result->x;
		result->y = scale0 * quat1->y + scale1 * result->y;
		result->z = scale0 * quat1->z + scale1 * result->z;
		result->w = scale0 * quat1->w + scale1 * result->w;
	}
	
}
// SlerpQuat  /////////////////////////////////////////////////////////////////
void QuatToEuler(const tQuaternion *quat, Vector3 *euler)
{
	/// Local Variables ///////////////////////////////////////////////////////////
	float matrix[3][3];
	float cx,sx;
	float cy,sy,yr;
	float cz,sz;
	///////////////////////////////////////////////////////////////////////////////
	// CONVERT QUATERNION TO MATRIX - I DON'T REALLY NEED ALL OF IT
	matrix[0][0] = 1.0f - (2.0f * quat->y * quat->y) - (2.0f * quat->z * quat->z);
	//	matrix[0][1] = (2.0f * quat->x * quat->y) - (2.0f * quat->w * quat->z);
	//	matrix[0][2] = (2.0f * quat->x * quat->z) + (2.0f * quat->w * quat->y);
	matrix[1][0] = (2.0f * quat->x * quat->y) + (2.0f * quat->w * quat->z);
	//	matrix[1][1] = 1.0f - (2.0f * quat->x * quat->x) - (2.0f * quat->z * quat->z);
	//	matrix[1][2] = (2.0f * quat->y * quat->z) - (2.0f * quat->w * quat->x);
	matrix[2][0] = (2.0f * quat->x * quat->z) - (2.0f * quat->w * quat->y);
	matrix[2][1] = (2.0f * quat->y * quat->z) + (2.0f * quat->w * quat->x);
	matrix[2][2] = 1.0f - (2.0f * quat->x * quat->x) - (2.0f * quat->y * quat->y);
	
	sy = -matrix[2][0];
	cy = (float)(sqrt(1 - (sy * sy)));
	yr = (float)atan2(sy,cy);
	euler->y = (yr * 180.0f) / (float)PI;
	
	// AVOID DIVIDE BY ZERO ERROR ONLY WHERE Y= +-90 or +-270 
	// NOT CHECKING cy BECAUSE OF PRECISION ERRORS
	if (sy != 1.0f && sy != -1.0f)	
	{
		cx = matrix[2][2] / cy;
		sx = matrix[2][1] / cy;
		euler->x = ((float)atan2(sx,cx) * 180.0f) / (float)PI;	// RAD TO DEG
		
		cz = matrix[0][0] / cy;
		sz = matrix[1][0] / cy;
		euler->z = ((float)atan2(sz,cz) * 180.0f) / (float)PI;	// RAD TO DEG
	}
	else
	{
		// SINCE Cos(Y) IS 0, I AM SCREWED.  ADOPT THE STANDARD Z = 0
		// I THINK THERE IS A WAY TO FIX THIS BUT I AM NOT SURE.  EULERS SUCK
		// NEED SOME MORE OF THE MATRIX TERMS NOW
		matrix[1][1] = 1.0f - (2.0f * quat->x * quat->x) - (2.0f * quat->z * quat->z);
		matrix[1][2] = (2.0f * quat->y * quat->z) - (2.0f * quat->w * quat->x);
		cx = matrix[1][1];
		sx = -matrix[1][2];
		euler->x = ((float)atan2(sx,cx) * 180.0f) / (float)PI;	// RAD TO DEG
		
		cz = 1.0f;
		sz = 0.0f;
		euler->z = ((float)atan2(sz,cz) * 180.0f) / (float)PI;	// RAD TO DEG
	}
}
// QuatToEuler  ///////////////////////////////////////////////////////////////
void QuatToMatrix(tQuaternion q, float *mat){
    float xx      = q.x * q.x;
    float xy      = q.x * q.y;
    float xz      = q.x * q.z;
    float xw      = q.x * q.w;
	
    float yy      = q.y * q.y;
    float yz      = q.y * q.z;
    float yw      = q.y * q.w;
	
    float zz      = q.z * q.z;
    float zw      = q.z * q.w;
	
    mat[0]  = 1 - 2 * ( yy + zz );
    mat[1]  =     2 * ( xy - zw );
    mat[2]  =     2 * ( xz + yw );
	
    mat[4]  =     2 * ( xy + zw );
    mat[5]  = 1 - 2 * ( xx + zz );
    mat[6]  =     2 * ( yz - xw );
	
    mat[8]  =     2 * ( xz - yw );
    mat[9]  =     2 * ( yz + xw );
    mat[10] = 1 - 2 * ( xx + yy );
	
    mat[3]  = mat[7] = mat[11] = mat[12] = mat[13] = mat[14] = 0.0;
    mat[15] = 1.0;
}