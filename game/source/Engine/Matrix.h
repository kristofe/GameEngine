#ifndef		_MATRIX44_H
#define		_MATRIX44_H

#include <memory.h>
#include "stdio.h"
#include <list>
#include <string>

#define PI          3.14159265358979323846f
#define HALF_PI	    1.57079632679489661923f


//This class kind of repeats the functionality of what is in mtxlib.h/cpp  forgot why i did this.
class Matrix44{
public:
	float matrix[16];
	float inverse[16];

	Matrix44();
	Matrix44(float prmMatrix[16]);
	Matrix44(const Matrix44& m);
	Matrix44(Matrix44* m);

	void	toIdentity();
	void	getIdentity(Matrix44* mtx);
	void	copyInto(float dst[16]);
	void	getCopy(Matrix44* mtx);
	void	translate(float x, float y, float z);
	void	multiply(float b[16]);
	void	multiply(Matrix44* mtx);
	void	getTranspose(Matrix44 * mtx);
	void	toTranspose();
	void	rotate(float angleDegrees, float x, float y, float z);
	void	getInverseTranspose(Matrix44* mtx);
	void	getInverse(Matrix44* mtx); 
	
	void	print();

	static float DegToRad(float a) { return a*0.01745329252f; };
	static float RadToDeg(float a) { return a*57.29577951f; };
	
	virtual std::string GetTypeString()
	{
		std::string s;
		s = "Matrix44";
		return s;
	}

private:
	void	invert();
};




class MatrixStack{
	std::list<Matrix44*> stack;

public:
	MatrixStack(){ 
		Matrix44* m = new Matrix44();
		m->toIdentity();//This is redundant because the constructor should do this.
		stack.push_back(m);
	
	};
	
	~MatrixStack(){
		std::list<Matrix44*>::iterator it = stack.begin();
		while(it != stack.end()){
			delete *it;
			++it;
		}
		stack.clear();
	};

	void PushMatrix(){
		Matrix44 *b = stack.back();
		Matrix44*m = new Matrix44(b);
		stack.push_back(m);
	};

	void PopMatrix(){
		if(stack.size() > 0){
			delete stack.back();
			stack.pop_back();
		}
	};

	void LoadIdentity(){
		stack.back()->toIdentity();
	};

	void Rotate(float angleDegrees, float x, float y, float z){
		Matrix44 *newMat = new Matrix44();
		newMat->rotate(angleDegrees,x,y,z);
		stack.back()->multiply(newMat->matrix);
		delete newMat;

	};

	void Translate(float x, float y, float z){
		Matrix44 *newMat = new Matrix44();
		newMat->translate(x,y,z);
		stack.back()->multiply(newMat->matrix);
		delete newMat;
	};

	void GetTop(Matrix44* m){
		Matrix44* mm = stack.back();
		mm->copyInto(m->matrix);
	};
};

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//ALL OF THE CLASSES AND CODE BELOW WERE COPIED FROM AN OLD MATH LIBRARY - EVEN OLDER THAN THE ONE ABOVE //
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////


class Vector4;
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
  // Vector3(): x(0), y(0), z(0) {};
  Vector3(): x(0), y(0), z(0) {};
  Vector3(float inX, float inY, float inZ);
  Vector3(const Vector3 &v);
  explicit Vector3(const Vector4 &v);
  
 public:
  // Operators
  float                 &operator [] (unsigned int index);
  const float           &operator [] (unsigned int index) const;
  Vector3               &operator =  (const Vector3 &v);
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

   
   friend Vector3 operator % (const Vector3 &a, const Vector3 &b);  // cross product of a and b (a % b)
   friend float  operator * (const Vector3 &a, const Vector3 &b);  // dot product of a and b (a * b)
   //friend Vector3 operator * (const Matrix3x3 &m, const Vector3 &v);//multiply this vector by 3x3 matrix
  

  


 public:
  // Methods
  void                  set(float xIn, float yIn, float zIn);
  float                 length() const;
  float                 lengthSqr() const;
  bool                  isZero() const;
  Vector3               &normalize();
  Vector3				&scale(float);
  //Vector3				&normalized_copy();
  
  void 			getDirection (void);
  void 			setMagnitudeOfVector (float velocity);
  float 		getDist (Vector3 pos1, Vector3 pos2);
  float 		getDist (Vector3 pos1);
  float 		getAngleBetween (Vector3 vec1, Vector3 vec2);
  float 		getAngleBetween (Vector3 vec2);
  void			transform(float matrix[16]);
  void			transform3(float matrix[16]);
  float			dot(float x, float y, float z, float w);

  // Debug
  void                  fprint(FILE* file, char* str) const;
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
  // Vector4(): x(0), y(0), z(0), w(0) {};
  Vector4() {};
  Vector4(float inX, float inY, float inZ, float inW);
  Vector4(const Vector4 &v);
  explicit Vector4(const Vector3 &v);
  
 public:
  // Operators
  float                 &operator [] (unsigned int index);
  const float           &operator [] (unsigned int index) const;
  Vector4               &operator =  (const Vector4 &v);
  Vector4               &operator =  (const Vector3 &v);
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


 public:
  // Methods
  void                  set(float xIn, float yIn, float zIn, float wIn);
  float                 length() const;
  float                 lengthSqr() const;
  bool                  isZero() const;
  Vector4               &normalize();
  
  // Debug
  void                  fprint(FILE* file, char* str) const;
};


////////////////////////////////////////////////////////////
// Miscellaneous vector functions
//

Vector3 Normalized(const Vector3 &a);
Vector4 Normalized(const Vector4 &a);
float VectorSquaredDistance(const Vector3 &v1, const Vector3 &v2);
float DotProduct(const Vector3 &a, const Vector3 &b);
float DotProduct(const Vector4 &a, const Vector4 &b);
void SwapVec(Vector3 &a, Vector3 &b);
void SwapVec(Vector4 &a, Vector4 &b);
Vector3 CrossProduct(const Vector3 &a, const Vector3 &b);
bool NearlyEquals(const Vector3 &a, const Vector3 &b, float r);
bool NearlyEquals(const Vector4 &a, const Vector4 &b, float r);
bool isSameDirection(Vector3& vec1, Vector3& vec2,float threshold);

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
// Quaternion class
//

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

#endif