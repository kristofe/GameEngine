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

#ifndef _MTXLIB_H
#define _MTXLIB_H

#include <cstdio>
#include <vector>

#define PI 3.14159265f
#define HALF_PI 1.57079633f
static inline float DegToRad(float a) { return a*0.01745329252f; };
static inline float RadToDeg(float a) { return a*57.29577951f; };

class Vector2;
class Vector3;
class Vector4;
class Matrix33;
class Matrix44;


////////////////////////////////////////////////////////////
// Vector2 class
//

class Vector2 
{
	public:
		// Members
		float x, y;
		
	public:
		// Constructors
		Vector2() {};
		Vector2(float inX, float inY);
		Vector2(const Vector2 &v);
		
	public:
		// Operators
		float                 &operator [] (unsigned int index);
		const float           &operator [] (unsigned int index) const;
		Vector2               &operator =  (const Vector2 &v);
		Vector2               &operator += (const Vector2 &v);
		Vector2               &operator -= (const Vector2 &v);
		Vector2               &operator *= (float f);
		Vector2               &operator /= (float f);
		friend bool           operator == (const Vector2 &a, const Vector2 &b);
		friend bool           operator != (const Vector2 &a, const Vector2 &b);
		friend Vector2        operator - (const Vector2 &a);
		friend Vector2        operator + (const Vector2 &a, const Vector2 &b);
		friend Vector2        operator - (const Vector2 &a, const Vector2 &b);
		friend Vector2        operator * (const Vector2 &v, float f);
		friend Vector2        operator * (float f, const Vector2 &v);
		friend Vector2        operator / (const Vector2 &v, float f);
		
		//operator float*() {return &x;};
		
	public:
		// Methods
		float                 length() const;
		float                 lengthSqr() const;
		bool                  isZero() const;
		Vector2               &normalize();
		

};


////////////////////////////////////////////////////////////
// Vector3 class
//

class Vector3 
	{
	public:
		// Members
		float x, y, z;
		
	public:
		// Constructors
		Vector3() {};
		Vector3(float inX, float inY, float inZ);
		Vector3(const Vector3 &v);
		explicit Vector3(const Vector2 &v);
		
	public:
		// Operators
		float                 &operator [] (unsigned int index);
		const float           &operator [] (unsigned int index) const;
		Vector3               &operator =  (const Vector3 &v);
		Vector3               &operator =  (const Vector2 &v);
		Vector3               &operator += (const Vector3 &v);
		Vector3               &operator -= (const Vector3 &v);
		Vector3               &operator *= (float f);
		Vector3               &operator /= (float f);
		friend bool           operator == (const Vector3 &a, const Vector3 &b);
		friend bool           operator != (const Vector3 &a, const Vector3 &b);
		friend Vector3        operator - (const Vector3 &a);
		friend Vector3        operator + (const Vector3 &a, const Vector3 &b);
		friend Vector3        operator - (const Vector3 &a, const Vector3 &b);
		friend Vector3        operator * (const Vector3 &v, float f);
		friend Vector3        operator * (float f, const Vector3 &v);
		friend Vector3        operator / (const Vector3 &v, float f);
		
		//operator float*() {return &x;};
		
	public:
		// Methods
		float                 length() const;
		float                 lengthSqr() const;
		bool                  isZero() const;
		Vector3               &normalize();
		
		// Debug
		//void                  fprint(FILE* file, char* str) const;
	};


////////////////////////////////////////////////////////////
// Vector4 class
//

class Vector4 
	{
	public:
		// Members
		float x, y, z, w;
		
	public:
		// Constructors
		Vector4() {};
		Vector4(float inX, float inY, float inZ, float inW);
		Vector4(const Vector4 &v);
		explicit Vector4(const Vector3 &v);
		explicit Vector4(const Vector2 &v);
		
	public:
		// Operators
		float                 &operator [] (unsigned int index);
		const float           &operator [] (unsigned int index) const;
		Vector4               &operator =  (const Vector4 &v);
		Vector4               &operator =  (const Vector3 &v);
		Vector4               &operator =  (const Vector2 &v);
		Vector4               &operator += (const Vector4 &v);
		Vector4               &operator -= (const Vector4 &v);
		Vector4               &operator *= (float f);
		Vector4               &operator /= (float f);
		friend bool           operator == (const Vector4 &a, const Vector4 &b);
		friend bool           operator != (const Vector4 &a, const Vector4 &b);
		friend Vector4        operator - (const Vector4 &a);
		friend Vector4        operator + (const Vector4 &a, const Vector4 &b);
		friend Vector4        operator - (const Vector4 &a, const Vector4 &b);
		friend Vector4        operator * (const Vector4 &v, float f);
		friend Vector4        operator * (float f, const Vector4 &v);
		friend Vector4        operator / (const Vector4 &v, float f);
		
		//operator float*() {return &x;};
		
	public:
		// Methods
		float                 length() const;
		float                 lengthSqr() const;
		bool                  isZero() const;
		Vector4               &normalize();
		
		// Debug
		//void                  fprint(FILE* file, char* str) const;
	};

////////////////////////////////////////////////////////////
// Color class
//

class Color 
	{
	public:
		// Members
		float r, g, b, a;
		
	public:
		// Constructors
		Color():r(0),g(0),b(0),a(0) {};
		Color(const Color& c);
		Color &operator =  (const Color &c);
	};


////////////////////////////////////////////////////////////
// Miscellaneous vector functions
//

float DotProduct(const Vector2 &a, const Vector2 &b);
float DotProduct(const Vector3 &a, const Vector3 &b);
float DotProduct(const Vector4 &a, const Vector4 &b);
void SwapVec(const Vector2 &a, const Vector2 &b);
void SwapVec(const Vector3 &a, const Vector3 &b);
void SwapVec(const Vector4 &a, const Vector4 &b);
Vector3 CrossProduct(const Vector3 &a, const Vector3 &b);
bool NearlyEquals(const Vector2 &a, const Vector2 &b, float r);
bool NearlyEquals(const Vector3 &a, const Vector3 &b, float r);
bool NearlyEquals(const Vector4 &a, const Vector4 &b, float r);


////////////////////////////////////////////////////////////
// Matrix33 class
//

class Matrix33 
	{
	public:
		// Members
		Vector3       col[3];
		
	public:
		// Constructors
		Matrix33() {};
		Matrix33(const Matrix33 &m);
		Matrix33(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2);
		
	public:
		// Operators
		Vector3		&operator [] (unsigned int i);
		const Vector3		&operator [] (unsigned int i) const;
		Matrix33		&operator =  (const Matrix33 &m);
		Matrix33		&operator += (const Matrix33 &m);
		Matrix33		&operator -= (const Matrix33 &m);
		Matrix33		&operator *= (const Matrix33 &m);
		Matrix33		&operator *= (float f);
		Matrix33		&operator /= (float f);
		friend bool		operator == (const Matrix33 &a, const Matrix33 &b);
		friend bool		operator != (const Matrix33 &a, const Matrix33 &b);
		friend Matrix33	operator + (const Matrix33 &a, const Matrix33 &b);
		friend Matrix33	operator - (const Matrix33 &a, const Matrix33 &b);
		friend Matrix33	operator * (const Matrix33 &a, const Matrix33 &b);
		friend Vector3 	operator * (const Matrix33 &m, const Vector3 &v);
		friend Vector3 	operator * (const Vector3 &v, const Matrix33 &m);
		friend Matrix33	operator * (const Matrix33 &m, float f);
		friend Matrix33	operator * (float f, const Matrix33 &m);
		friend Matrix33	operator / (const Matrix33 &m, float f);
		
		//operator float*() {return (float*)col[0]; };
		
	public:
		// Methods
		Matrix33		&identity();
		Matrix33		&transpose();
		Matrix33		&invert();
		
		// Debug
		//void			fprint(FILE* file, char* str) const;
	};

Matrix33	IdentityMatrix33();
Matrix33	TransposeMatrix33(const Matrix33 &m);
Matrix33	InvertMatrix33(const Matrix33 &m);
Matrix33	RotateRadMatrix33(float rad);
Matrix33	TranslateMatrix33(float x, float y);
Matrix33	ScaleMatrix33(float x, float y, float z = 1.0);


////////////////////////////////////////////////////////////
// Matrix44 class
//

class Matrix44 
	{
	public:
		// Members
		Vector4	col[4];
		
	public:
		// Constructors
		Matrix44() {};
		Matrix44(const Matrix44 &m);
		explicit Matrix44(const Matrix33 &m);
		Matrix44(const Vector4 &v0, const Vector4 &v1, 
				 const Vector4 &v2, const Vector4 &v3);
		
	public:
		// Operators
		Vector4		&operator [] (unsigned int i);
		const Vector4		&operator [] (unsigned int i) const;
		Matrix44		&operator =  (const Matrix44 &m);
		Matrix44		&operator =  (const Matrix33 &m);
		Matrix44		&operator += (const Matrix44 &m);
		Matrix44		&operator -= (const Matrix44 &m);
		Matrix44		&operator *= (const Matrix44 &m);
		Matrix44		&operator *= (float f);
		Matrix44		&operator /= (float f);
		friend bool		operator == (const Matrix44 &a, const Matrix44 &b);
		friend bool		operator != (const Matrix44 &a, const Matrix44 &b);
		friend Matrix44	operator + (const Matrix44 &a, const Matrix44 &b);
		friend Matrix44	operator - (const Matrix44 &a, const Matrix44 &b);
		friend Matrix44	operator * (const Matrix44 &a, const Matrix44 &b);
		friend Vector3 	operator * (const Matrix44 &m, const Vector3 &v);
		friend Vector3 	operator * (const Vector3 &v, const Matrix44 &m);
		friend Vector4 	operator * (const Matrix44 &m, const Vector4 &v);
		friend Vector4 	operator * (const Vector4 &v, const Matrix44 &m);
		friend Matrix44	operator * (const Matrix44 &m, float f);
		friend Matrix44	operator * (float f, const Matrix44 &m);
		friend Matrix44	operator / (const Matrix44 &m, float f);
		
		//operator float*() {return (float*)col[0]; };
		
	public:
		// Methods
		Matrix44		&identity();
		Matrix44		&transpose();
		Matrix44		&invert();
		
		// Debug
		//void			fprint(FILE* file, char* str) const;
	};

Matrix44	IdentityMatrix44();
Matrix44	TransposeMatrix44(const Matrix44 &m);
Matrix44	InvertMatrix44(const Matrix44 &m);
Matrix44	RotateRadMatrix44(char axis, float rad);
Matrix44	RotateRadMatrix44(const Vector3 &axis, float rad);
Matrix44	TranslateMatrix44(float x, float y, float z);
Matrix44	ScaleMatrix44(float x, float y, float z, float w = 1.0);
Matrix44	LookAtMatrix44(const Vector3 &camPos, const Vector3 &camUp, 
						   const Vector3 &target );
Matrix44	FrustumMatrix44(float l, float r, float b, float t, 
							float n, float f);
Matrix44	PerspectiveMatrix44(float fovY, float aspect, 
								float n, float f);
Matrix44	OrthoMatrix44(float l, float r, float b, float t, 
						  float n, float f);




class Quaternion{
public:
	Vector3 vector3;
	float  scalar;
	
	Quaternion(){
		scalar = 0;
	};
	~Quaternion(){};
	Quaternion * multiply(Quaternion *q);
	void normalize();
	Quaternion* fromEulers(Vector3 *rot);
	Quaternion*  fromAxisAngle(Vector3 *axis,float angle);
	Vector4* toAxisAngle();
	float* toMatrixf();
	float* toMatrixd();
	Matrix44* toMatrix44();
	Quaternion* SLERP(Quaternion* quat2, float slerp);
	Vector3* toEulers();
};
void SlerpQuat(Quaternion *quat1,Quaternion *quat2,float slerp, Quaternion *result);

/// Quaternion Definitions ////////////////////////////////////////////////////
typedef struct
	{
		float x,y,z,w;
	} tQuaternion;
void EulerToQuaternion(Vector3 *rot, tQuaternion *quat);
void EulerToQuaternion2(Vector3 *rot, tQuaternion *quat);
void QuatToAxisAngle(tQuaternion *quat,tQuaternion *axisAngle);
void SlerpQuat(tQuaternion *quat1,tQuaternion *quat2,float slerp, tQuaternion *result);
void QuatToEuler(const tQuaternion *quat, Vector3 *euler);
void MultQuaternions(tQuaternion *quat1, tQuaternion *quat2, tQuaternion *dest);
void AxisAngleToQuat(Vector3 *axis,float angle,tQuaternion *q);
void QuatToMatrix(tQuaternion q, float *mat);


const float BIGPOS = 1.0e38;
const float SMALLPOS = -1.0e38;

class AABB{
public:
	Vector3	min;//Default constructor should zero them out.
	Vector3 max;
	
	AABB():min(0,0,0),max(0,0,0){

	};
	
	AABB(Vector3 pMin, Vector3 pMax){
		this->min = pMin;
		this->max = pMax;
	};
	
	AABB(AABB& aabb){
		min = aabb.min;
		max = aabb.max;
	};
	
	AABB& operator= (const AABB& aabb){
		if(*this == aabb)//Self assignment in this case really doesn't matter because there are no pointers to leave dangling
			return *this;
		
		min = aabb.min;
		max = aabb.max;
		return *this;
	};
	
	bool operator==(const AABB& a){
		return ((a.min == min) && (a.max == max));
	}
	
	void set(Vector3 pMin, Vector3 pMax){
		min = pMin;
		max = pMax;
	};
	
	void encompass(std::vector<Vector3>& pts){
        Vector3 temp_min = Vector3(BIGPOS,BIGPOS,BIGPOS);
        Vector3 temp_max = Vector3(SMALLPOS,SMALLPOS,SMALLPOS);
		for(int i = 0; i < pts.size(); i++){
			Vector3 pt = pts[i];
            if(pt.x > temp_max.x)
                temp_max.x = pt.x;
            if(pt.y > temp_max.y)
                temp_max.y = pt.y;
            if(pt.z > temp_max.z)
                temp_max.z = pt.z;
			
            if(pt.x < temp_min.x)
                temp_min.x = pt.x;
            if(pt.y < temp_min.y)
                temp_min.y = pt.y;
            if(pt.z < temp_min.z)
                temp_min.z = pt.z;
		}
        min = temp_min;
        max = temp_max;
		
	};
	
	bool contains(AABB& aabb){
        if(aabb.min.x < min.x)
            return false;
        if(aabb.max.x > max.x)
            return false;
        
        if(aabb.max.y > max.y)
            return false;
        if(aabb.min.y < min.y)
            return false;
        
        if(aabb.max.z > max.z)
            return false;
        if(aabb.min.z < min.z)
            return false;
        
        return true;
	};
    
	bool intersects(AABB& other){
        //x-axis
        if(min.x > other.max.x)
            return false;
        if(max.x < other.min.x)
            return false;
        
        //y-axis
        if(min.y > other.max.y)
            return false;
        if(max.y < other.min.y)
            return false;
        
        //z-axis
        if(min.z > other.max.z)
            return false;
        if(max.z < other.min.z)
            return false;
        
        return true;
	};
	
};



#endif
