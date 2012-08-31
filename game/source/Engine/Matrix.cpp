#include "Matrix.h"
//#include "mtxlib.h"
#include <cassert>
//#include <malloc.h>
#include <math.h>

#define IDX(x,y) (x+(y*4))


Matrix44::Matrix44(){
	toIdentity();
}

Matrix44::Matrix44(float prmMatrix[16]){
	memcpy(matrix,prmMatrix,sizeof(float)*16);
}

Matrix44::Matrix44(const Matrix44& m){
	memcpy(matrix,m.matrix,sizeof(float)*16);
	memcpy(inverse,m.inverse,sizeof(float)*16);
}

Matrix44::Matrix44(Matrix44* m){
	memcpy(matrix,m->matrix,sizeof(float)*16);
	memcpy(inverse,m->inverse,sizeof(float)*16);
}

void Matrix44::toIdentity() {
    for (int i = 0 ; i < 4 ; i++)
    for (int j = 0 ; j < 4 ; j++)
        matrix[IDX(i,j)] = (i == j ? 1.0f : 0.0f);
}

void Matrix44::getIdentity(Matrix44* mtx) {
	float *m = mtx->matrix;
    for (int i = 0 ; i < 4 ; i++)
    for (int j = 0 ; j < 4 ; j++)
    m[IDX(i,j)] = (i == j ? 1.0f : 0.0f);
}

void Matrix44::copyInto(float dst[16]) {
    for (int i = 0 ; i < 4 ; i++)
    for (int j = 0 ; j < 4 ; j++)
        dst[IDX(i,j)] = matrix[IDX(i,j)];
}

void Matrix44::getCopy(Matrix44* mtx){
	memcpy(mtx->matrix,matrix,sizeof(float)*16);
	memcpy(mtx->inverse,inverse,sizeof(float)*16);
}

//This was taken from the MESA source
void Matrix44::translate(float x, float y, float z){
   float *m = matrix;
   m[12] = m[0] * x + m[4] * y + m[8]  * z + m[12];
   m[13] = m[1] * x + m[5] * y + m[9]  * z + m[13];
   m[14] = m[2] * x + m[6] * y + m[10] * z + m[14];
   m[15] = m[3] * x + m[7] * y + m[11] * z + m[15];
}

void Matrix44::getInverseTranspose(Matrix44* mtx){
	invert();
    for (int i = 0 ; i < 4 ; i++)
    for (int j = 0 ; j < 4 ; j++)
        mtx->matrix[IDX(i,j)] = inverse[IDX(j,i)];
}

void Matrix44::getTranspose(Matrix44* mtx) {
    for (int i = 0 ; i < 4 ; i++)
    for (int j = 0 ; j < 4 ; j++)
        mtx->matrix[IDX(j,i)] = matrix[IDX(i,j)];

}

void Matrix44::toTranspose(){
	float _tmp[16];
	memcpy(_tmp,matrix,sizeof(float)*16);
    for (int i = 0 ; i < 4 ; i++)
    for (int j = 0 ; j < 4 ; j++)
        matrix[IDX(j,i)] = _tmp[IDX(i,j)];

}


void Matrix44::getInverse(Matrix44* mtx){
	invert();
	memcpy(mtx->matrix,inverse,sizeof(float)*16);

}

//This was taken from the MESA source
// Invert an OpenGL 4x4 matrix
void Matrix44::invert(){
	float _tmp[16];

	memcpy(_tmp,matrix,sizeof(float)*16);
	float *m = _tmp;
	float *out = inverse;
	/* NB. OpenGL Matrices are COLUMN major. */
	#define SWAP_ROWS(a, b) { float *_tmp = a; (a)=(b); (b)=_tmp; }
	#define MAT(m,r,c) (m)[(c)*4+(r)]

	float wtmp[4][8];
	float m0, m1, m2, m3, s;
	float *r0, *r1, *r2, *r3;

	r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];

	r0[0] = MAT(m,0,0), r0[1] = MAT(m,0,1),
	r0[2] = MAT(m,0,2), r0[3] = MAT(m,0,3),
	r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,

	r1[0] = MAT(m,1,0), r1[1] = MAT(m,1,1),
	r1[2] = MAT(m,1,2), r1[3] = MAT(m,1,3),
	r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,

	r2[0] = MAT(m,2,0), r2[1] = MAT(m,2,1),
	r2[2] = MAT(m,2,2), r2[3] = MAT(m,2,3),
	r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,

	r3[0] = MAT(m,3,0), r3[1] = MAT(m,3,1),
	r3[2] = MAT(m,3,2), r3[3] = MAT(m,3,3),
	r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;

	/* choose pivot - or die */
	if (fabs(r3[0])>fabs(r2[0])) SWAP_ROWS(r3, r2);
	if (fabs(r2[0])>fabs(r1[0])) SWAP_ROWS(r2, r1);
	if (fabs(r1[0])>fabs(r0[0])) SWAP_ROWS(r1, r0);
	if (0.0 == r0[0])  return ;

	/* eliminate first variable     */
	m1 = r1[0]/r0[0]; m2 = r2[0]/r0[0]; m3 = r3[0]/r0[0];
	s = r0[1]; r1[1] -= m1 * s; r2[1] -= m2 * s; r3[1] -= m3 * s;
	s = r0[2]; r1[2] -= m1 * s; r2[2] -= m2 * s; r3[2] -= m3 * s;
	s = r0[3]; r1[3] -= m1 * s; r2[3] -= m2 * s; r3[3] -= m3 * s;
	s = r0[4];
	if (s != 0.0) { r1[4] -= m1 * s; r2[4] -= m2 * s; r3[4] -= m3 * s; }
	s = r0[5];
	if (s != 0.0) { r1[5] -= m1 * s; r2[5] -= m2 * s; r3[5] -= m3 * s; }
	s = r0[6];
	if (s != 0.0) { r1[6] -= m1 * s; r2[6] -= m2 * s; r3[6] -= m3 * s; }
	s = r0[7];
	if (s != 0.0) { r1[7] -= m1 * s; r2[7] -= m2 * s; r3[7] -= m3 * s; }

	/* choose pivot - or die */
	if (fabs(r3[1])>fabs(r2[1])) SWAP_ROWS(r3, r2);
	if (fabs(r2[1])>fabs(r1[1])) SWAP_ROWS(r2, r1);
	if (0.0 == r1[1])  return ;

	/* eliminate second variable */
	m2 = r2[1]/r1[1]; m3 = r3[1]/r1[1];
	r2[2] -= m2 * r1[2]; r3[2] -= m3 * r1[2];
	r2[3] -= m2 * r1[3]; r3[3] -= m3 * r1[3];
	s = r1[4]; if (0.0 != s) { r2[4] -= m2 * s; r3[4] -= m3 * s; }
	s = r1[5]; if (0.0 != s) { r2[5] -= m2 * s; r3[5] -= m3 * s; }
	s = r1[6]; if (0.0 != s) { r2[6] -= m2 * s; r3[6] -= m3 * s; }
	s = r1[7]; if (0.0 != s) { r2[7] -= m2 * s; r3[7] -= m3 * s; }

	/* choose pivot - or die */
	if (fabs(r3[2])>fabs(r2[2])) SWAP_ROWS(r3, r2);
	if (0.0 == r2[2])  return ;

	/* eliminate third variable */
	m3 = r3[2]/r2[2];
	r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
	r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6],
	r3[7] -= m3 * r2[7];

	/* last check */
	if (0.0 == r3[3]) return ;

	s = 1.0f/r3[3];              /* now back substitute row 3 */
	r3[4] *= s; r3[5] *= s; r3[6] *= s; r3[7] *= s;

	m2 = r2[3];                 /* now back substitute row 2 */
	s  = 1.0f/r2[2];
	r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
	r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
	m1 = r1[3];
	r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
	r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
	m0 = r0[3];
	r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
	r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;

	m1 = r1[2];                 /* now back substitute row 1 */
	s  = 1.0f/r1[1];
	r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
	r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
	m0 = r0[2];
	r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
	r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;

	m0 = r0[1];                 /* now back substitute row 0 */
	s  = 1.0f/r0[0];
	r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
	r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);

	MAT(out,0,0) = r0[4]; MAT(out,0,1) = r0[5],
	MAT(out,0,2) = r0[6]; MAT(out,0,3) = r0[7],
	MAT(out,1,0) = r1[4]; MAT(out,1,1) = r1[5],
	MAT(out,1,2) = r1[6]; MAT(out,1,3) = r1[7],
	MAT(out,2,0) = r2[4]; MAT(out,2,1) = r2[5],
	MAT(out,2,2) = r2[6]; MAT(out,2,3) = r2[7],
	MAT(out,3,0) = r3[4]; MAT(out,3,1) = r3[5],
	MAT(out,3,2) = r3[6]; MAT(out,3,3) = r3[7]; 

	#undef MAT
	#undef SWAP_ROWS
}

//This was taken from the MESA source
void Matrix44::multiply(float b[16]){
   //Column Major Matrix Post Multiply
	float _tmp[16];
   int i;
	#define A(row,col)  matrix[(col<<2)+row]
	#define B(row,col)  b[(col<<2)+row]
	#define P(row,col)  _tmp[(col<<2)+row]


   for (i = 0; i < 4; i++) {
      float ai0=A(i,0),  ai1=A(i,1),  ai2=A(i,2),  ai3=A(i,3);
      P(i,0) = ai0 * B(0,0) + ai1 * B(1,0) + ai2 * B(2,0) + ai3 * B(3,0);
      P(i,1) = ai0 * B(0,1) + ai1 * B(1,1) + ai2 * B(2,1) + ai3 * B(3,1);
      P(i,2) = ai0 * B(0,2) + ai1 * B(1,2) + ai2 * B(2,2) + ai3 * B(3,2);
      P(i,3) = ai0 * B(0,3) + ai1 * B(1,3) + ai2 * B(2,3) + ai3 * B(3,3);
   }
	
   memcpy(matrix,_tmp,sizeof(float)*16);
	#undef A
	#undef B
	#undef P
}

//This was taken from the MESA source
void Matrix44::multiply(Matrix44* mtx){
   //Column Major Matrix Post Multiply
	float* b = mtx->matrix;
	float _tmp[16];
   int i;
	#define A(row,col)  matrix[(col<<2)+row]
	#define B(row,col)  b[(col<<2)+row]
	#define P(row,col)  _tmp[(col<<2)+row]


   for (i = 0; i < 4; i++) {
      float ai0=A(i,0),  ai1=A(i,1),  ai2=A(i,2),  ai3=A(i,3);
      P(i,0) = ai0 * B(0,0) + ai1 * B(1,0) + ai2 * B(2,0) + ai3 * B(3,0);
      P(i,1) = ai0 * B(0,1) + ai1 * B(1,1) + ai2 * B(2,1) + ai3 * B(3,1);
      P(i,2) = ai0 * B(0,2) + ai1 * B(1,2) + ai2 * B(2,2) + ai3 * B(3,2);
      P(i,3) = ai0 * B(0,3) + ai1 * B(1,3) + ai2 * B(2,3) + ai3 * B(3,3);
   }
	
   memcpy(matrix,_tmp,sizeof(float)*16);
	#undef A
	#undef B
	#undef P
}

//This was taken from the MESA source
void Matrix44::rotate(float angleDegrees, float x, float y, float z){
	float xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c, s, c, angle;
	bool optimized;
	Matrix44 mtx;
	float *m = mtx.matrix;

	angle = DegToRad(angleDegrees);
	
	s =  sin( angle );
	c =  cos( angle );

	optimized = false;

#define M(row,col)  m[col*4+row]

   if (x == 0.0F) {
      if (y == 0.0F) {
         if (z != 0.0F) {
            optimized = true;
            // rotate only around z-axis 
            M(0,0) = c;
            M(1,1) = c;
            if (z < 0.0F) {
               M(0,1) = s;
               M(1,0) = -s;
            }
            else {
               M(0,1) = -s;
               M(1,0) = s;
            }
         }
      }
      else if (z == 0.0F) {
         optimized = true;
         // rotate only around y-axis 
         M(0,0) = c;
         M(2,2) = c;
         if (y < 0.0F) {
            M(0,2) = -s;
            M(2,0) = s;
         }
         else {
            M(0,2) = s;
            M(2,0) = -s;
         }
      }
   }
   else if (y == 0.0F) {
      if (z == 0.0F) {
         optimized = true;
         // rotate only around x-axis
         M(1,1) = c;
         M(2,2) = c;
         if (x < 0.0F) {
            M(1,2) = s;
            M(2,1) = -s;
         }
         else {
            M(1,2) = -s;
            M(2,1) = s;
         }
      }
   }

   if (!optimized) {
      const float mag = sqrt(x * x + y * y + z * z);

      if (mag <= 1.0e-4) {
         // no rotation, leave mat as-is 
         return;
      }

      x /= mag;
      y /= mag;
      z /= mag;

      xx = x * x;
      yy = y * y;
      zz = z * z;
      xy = x * y;
      yz = y * z;
      zx = z * x;
      xs = x * s;
      ys = y * s;
      zs = z * s;
      one_c = 1.0F - c;

      // We already hold the identity-matrix so we can skip some statements
      M(0,0) = (one_c * xx) + c;
      M(0,1) = (one_c * xy) - zs;
      M(0,2) = (one_c * zx) + ys;


      M(1,0) = (one_c * xy) + zs;
      M(1,1) = (one_c * yy) + c;
      M(1,2) = (one_c * yz) - xs;


      M(2,0) = (one_c * zx) - ys;
      M(2,1) = (one_c * yz) + xs;
      M(2,2) = (one_c * zz) + c;
		
	  
   }
#undef M
   multiply(m);
}

void Matrix44::print(){
	for (int i = 0 ; i < 4 ; i++){
		for (int j = 0 ; j < 4 ; j++)
			printf("%f\t",matrix[IDX(i,j)]);
		printf("\n");
	}


}

/*
 // Generate a 4x4 transformation matrix from glRotate parameters, and
 // postmultiply the input matrix by it.
 // This function contributed by Erich Boleyn (erich@uruk.org).
 // Optimizatios contributed by Rudolf Opalla (rudi@khm.de).
 
void
_math_matrix_rotate( GLmatrix *mat,
		     GLfloat angle, GLfloat x, GLfloat y, GLfloat z )
{
   GLfloat xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c, s, c;
   GLfloat m[16];
   GLboolean optimized;

   s = (GLfloat) sin( angle * DEG2RAD );
   c = (GLfloat) cos( angle * DEG2RAD );

   MEMCPY(m, Identity, sizeof(GLfloat)*16);
   optimized = GL_FALSE;

#define M(row,col)  m[col*4+row]

   if (x == 0.0F) {
      if (y == 0.0F) {
         if (z != 0.0F) {
            optimized = GL_TRUE;
            // rotate only around z-axis 
            M(0,0) = c;
            M(1,1) = c;
            if (z < 0.0F) {
               M(0,1) = s;
               M(1,0) = -s;
            }
            else {
               M(0,1) = -s;
               M(1,0) = s;
            }
         }
      }
      else if (z == 0.0F) {
         optimized = GL_TRUE;
         // rotate only around y-axis 
         M(0,0) = c;
         M(2,2) = c;
         if (y < 0.0F) {
            M(0,2) = -s;
            M(2,0) = s;
         }
         else {
            M(0,2) = s;
            M(2,0) = -s;
         }
      }
   }
   else if (y == 0.0F) {
      if (z == 0.0F) {
         optimized = GL_TRUE;
         // rotate only around x-axis
         M(1,1) = c;
         M(2,2) = c;
         if (x < 0.0F) {
            M(1,2) = s;
            M(2,1) = -s;
         }
         else {
            M(1,2) = -s;
            M(2,1) = s;
         }
      }
   }

   if (!optimized) {
      const GLfloat mag = (GLfloat) GL_SQRT(x * x + y * y + z * z);

      if (mag <= 1.0e-4) {
         // no rotation, leave mat as-is 
         return;
      }

      x /= mag;
      y /= mag;
      z /= mag;

      xx = x * x;
      yy = y * y;
      zz = z * z;
      xy = x * y;
      yz = y * z;
      zx = z * x;
      xs = x * s;
      ys = y * s;
      zs = z * s;
      one_c = 1.0F - c;

      // We already hold the identity-matrix so we can skip some statements
      M(0,0) = (one_c * xx) + c;
      M(0,1) = (one_c * xy) - zs;
      M(0,2) = (one_c * zx) + ys;


      M(1,0) = (one_c * xy) + zs;
      M(1,1) = (one_c * yy) + c;
      M(1,2) = (one_c * yz) - xs;


      M(2,0) = (one_c * zx) - ys;
      M(2,1) = (one_c * yz) + xs;
      M(2,2) = (one_c * zz) + c;

   }
#undef M

   matrix_multf( mat, m, MAT_FLAG_ROTATION );
}


void
_math_matrix_translate( GLmatrix *mat, GLfloat x, GLfloat y, GLfloat z )
{
   GLfloat *m = mat->m;
   m[12] = m[0] * x + m[4] * y + m[8]  * z + m[12];
   m[13] = m[1] * x + m[5] * y + m[9]  * z + m[13];
   m[14] = m[2] * x + m[6] * y + m[10] * z + m[14];
   m[15] = m[3] * x + m[7] * y + m[11] * z + m[15];

   mat->flags |= (MAT_FLAG_TRANSLATION |
		  MAT_DIRTY_TYPE |
		  MAT_DIRTY_INVERSE);
}

void
_math_transposef( GLfloat to[16], const GLfloat from[16] )
{
   to[0] = from[0];
   to[1] = from[4];
   to[2] = from[8];
   to[3] = from[12];
   to[4] = from[1];
   to[5] = from[5];
   to[6] = from[9];
   to[7] = from[13];
   to[8] = from[2];
   to[9] = from[6];
   to[10] = from[10];
   to[11] = from[14];
   to[12] = from[3];
   to[13] = from[7];
   to[14] = from[11];
   to[15] = from[15];
}


void
_math_transposed( GLfloat to[16], const GLfloat from[16] )
{
   to[0] = from[0];
   to[1] = from[4];
   to[2] = from[8];
   to[3] = from[12];
   to[4] = from[1];
   to[5] = from[5];
   to[6] = from[9];
   to[7] = from[13];
   to[8] = from[2];
   to[9] = from[6];
   to[10] = from[10];
   to[11] = from[14];
   to[12] = from[3];
   to[13] = from[7];
   to[14] = from[11];
   to[15] = from[15];
}

void
_math_transposefd( GLfloat to[16], const GLfloat from[16] )
{
   to[0] = (GLfloat) from[0];
   to[1] = (GLfloat) from[4];
   to[2] = (GLfloat) from[8];
   to[3] = (GLfloat) from[12];
   to[4] = (GLfloat) from[1];
   to[5] = (GLfloat) from[5];
   to[6] = (GLfloat) from[9];
   to[7] = (GLfloat) from[13];
   to[8] = (GLfloat) from[2];
   to[9] = (GLfloat) from[6];
   to[10] = (GLfloat) from[10];
   to[11] = (GLfloat) from[14];
   to[12] = (GLfloat) from[3];
   to[13] = (GLfloat) from[7];
   to[14] = (GLfloat) from[11];
   to[15] = (GLfloat) from[15];
}
*/


//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//ALL OF THE CLASSES AND CODE BELOW WERE COPIED FROM AN OLD MATH LIBRARY - EVEN OLDER THAN THE ONE ABOVE //
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

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
Vector3::Vector3(float inX, float inY, float inZ){
  x = inX;
  y = inY;
  z = inZ;
}

// Constructor with initializing Vector3
Vector3::Vector3(const Vector3 &v){
  x = v.x;
  y = v.y;
  z = v.z;
}

// Constructor with initializing Vector4
Vector3::Vector3(const Vector4 &v){
  x = v.x;
  y = v.y;
  z = v.z;
}

// Array indexing
float &Vector3::operator [] (unsigned int i){
  assert (i<3);
        
  return *(&x+i);
}

// Array indexing
const float &Vector3::operator [] (unsigned int i) const{
  assert (i<3);

  return *(&x+i);
}

// Assign
Vector3 &Vector3::operator = (const Vector3 &v){
  x = v.x;
  y = v.y;
  z = v.z;

  return *this;
}

// Add a Vector3 to this one
Vector3 &Vector3::operator += (const Vector3 &v){
  x += v.x;
  y += v.y;
  z += v.z;

  return *this;
}

// Subtract a Vector3 from this one
Vector3 &Vector3::operator -= (const Vector3 &v){
  x -= v.x;
  y -= v.y;
  z -= v.z;

  return *this;
}

// Multiply the Vector3 by a float
Vector3 &Vector3::operator *= (float f){
  x *= f;
  y *= f;
  z *= f;

  return *this;
}

// Divide the Vector3 by a float
Vector3 &Vector3::operator /= (float f){
  x /= f;
  y /= f;
  z /= f;

  return *this;
}

// Are these two Vector3's equal?
bool operator == (const Vector3 &a, const Vector3 &b){
  return ((a.x == b.x) && (a.y == b.y) && (a.z == b.z));
}

// Are these two Vector3's not equal?
bool operator != (const Vector3 &a, const Vector3 &b){
  return ((a.x != b.x) || (a.y != b.y) || (a.z != b.z));
}

// Add two Vector3's
Vector3 operator + (const Vector3 &a, const Vector3 &b){
  Vector3 ret(a);

  ret += b;

  return ret;
}

// Subtract one Vector3 from another
Vector3 operator - (const Vector3 &a, const Vector3 &b){
  Vector3 ret(a);

  ret -= b;

  return ret;
}

// Multiply Vector3 by a float
Vector3 operator * (const Vector3 &v, float f){
  return Vector3(f * v.x, f * v.y, f * v.z);
}

// Multiply Vector3 by a float
Vector3 operator * (float f, const Vector3 &v){
  return Vector3(f * v.x, f * v.y, f * v.z);
}

// Divide Vector3 by a float
Vector3 operator / (const Vector3 &v, float f){
  return Vector3(v.x / f, v.y / f, v.z / f);
}

// Negate a Vector3
Vector3 operator - (const Vector3 &a){
  return Vector3(-a.x, -a.y, -a.z);
}

// Set Values
void Vector3::set(float xIn, float yIn, float zIn){
  x = xIn;
  y = yIn;
  z = zIn;
}

// Get length of a Vector3
float Vector3::length() const{
  return (float) sqrt(x*x + y*y + z*z);
}

// Get squared length of a Vector3
float Vector3::lengthSqr() const{
  return (x*x + y*y + z*z);
}

// Does Vector3 equal (0, 0, 0)?
bool Vector3::isZero() const{
  return ((x == 0.0F) && (y == 0.0F) && (z == 0.0F));
}

// Normalize a Vector3
Vector3 &Vector3::normalize(){
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

// get direction of vector
void Vector3::getDirection (void){

   this->normalize();

}

// set the magnitude of the vector (without modifying
// direction, apportioned across the existing vector's magnitudes)
void Vector3::setMagnitudeOfVector (float velocity){

   this->normalize();

   x *= velocity;
   y *= velocity;
   z *= velocity;

}

// get distance between two points
float Vector3::getDist (Vector3 pos1, Vector3 pos2){

   Vector3  temp = pos1 - pos2;

   return (temp.length());

}

// get distance between two points
float Vector3::getDist (Vector3 pos1){

   Vector3  temp = (*this) - pos1;

   return (temp.length());

}

// get angle between two vectors
float Vector3::getAngleBetween (Vector3 vec1, Vector3 vec2){

   float mag1 = vec1.length();
   float mag2 = vec2.length();
   float dotproduct = DotProduct(vec1, vec2);

   return (float) (acos (dotproduct/(mag1 * mag2)));
}

// get angle between two vectors
float Vector3::getAngleBetween (Vector3 vec2){

   float mag1 = this->length();
   float mag2 = vec2.length();
   float dotproduct = DotProduct(*this, vec2);

   return (float) (acos (dotproduct/(mag1 * mag2)));
}

Vector3 &Vector3::scale(float f){
  x *= f; y *= f; z *= f;
  return *this;
}

void Vector3::transform(float mat[16]) {
//Column Major Version -- OpenGL Compatible
	float xx, yy, zz;
	xx =       (mat[0] * x) +
			   (mat[4] * y) +	
			   (mat[8] * z) +
			    mat[12];
	yy =       (mat[1] * x) +
			   (mat[5] * y) +	
			   (mat[9] * z) +
			    mat[13];
	zz =	   (mat[2] * x) +
			   (mat[6] * y) +	
			   (mat[10] * z) +
			    mat[14];
	x = (float)xx;
	y = (float)yy;
	z = (float)zz;
}

void Vector3::transform3(float mat[16]) {
//Column Major Version -- OpenGL Compatible
	float xx, yy, zz;
	xx =       (mat[0] * x) +
			   (mat[4] * y) +	
			   (mat[8] * z);
	yy =       (mat[1] * x) +
			   (mat[5] * y) +	
			   (mat[9] * z);
	zz =	   (mat[2] * x) +
			   (mat[6] * y) +	
			   (mat[10] * z);
	x = (float)xx;
	y = (float)yy;
	z = (float)zz;
}

float Vector3::dot(float xx, float yy, float zz, float ww) {
  return x*xx + y*yy + z*zz + ww;
}


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

// Set Values
void Vector4::set(float xIn, float yIn, float zIn, float wIn)
{
  x = xIn;
  y = yIn;
  z = zIn;
  w = wIn;
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

////////////////////////////////////////////////////////////
// Miscellaneous vector functions
//

// Return Normal of Vector3's
Vector3 Normalized(const Vector3 &a)
{
  Vector3 ret(a);
  return ret.normalize();
}
// Return Normal of Vector4's
Vector4 Normalized(const Vector4 &a)
{
  Vector4 ret(a);
  return ret.normalize();
}


float VectorSquaredDistance(const Vector3 &v1, const Vector3 &v2) 
{
	return(	((v1.x - v2.x) * (v1.x - v2.x)) + 
			((v1.y - v2.y) * (v1.y - v2.y)) + 	
			((v1.z - v2.z) * (v1.z - v2.z)) ); 	
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

// Are these two Vector3's nearly equal?
bool NearlyEquals( const Vector3& a, const Vector3& b, float r ) 
{
  Vector3 diff = a - b; // difference

  return (DotProduct(diff, diff) < r*r); // radius
}

// Are these two Vector4's nearly equal?
bool NearlyEquals( const Vector4& a, const Vector4& b, float r ) 
{
  Vector4 diff = a - b; // difference

  return (DotProduct(diff, diff) < r*r); // radius
}

bool isSameDirection(Vector3& vec1, Vector3& vec2, float threshold){
	//if you deviate by 45 degrees you are not heading the same way
	if(DotProduct(vec1,vec2) > threshold) return true;
	return false;
}
/////////////////////////////////////////////////////////////////////////////
//Quaternion stuff
Quaternion* Quaternion::multiply(Quaternion *qb ){
//Routine checked and works - 5/24/03
	float w1,w2;
	Vector3 v1,v2;

	v1.set(vector3.x,vector3.y,vector3.z);
	v2.set(qb->vector3.x,qb->vector3.y,qb->vector3.z);
	w1 = scalar;
	w2 = qb->scalar;

	
	Vector3 crossResult = CrossProduct(v1,v2);
	float dotResult = DotProduct(v1,v2);
	v1.scale(w2);
	v2.scale(w1);

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
	float angleRadians = Matrix44::DegToRad(angle/2.0f);

	float sin_a = (float)sin(angleRadians);
    float cos_a = (float)cos(angleRadians);

	vector3.normalize();
	vector3.scale(sin_a);

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
//Routine checked and works - 5/21/03
	Matrix44* m= new Matrix44(toMatrixd());
	m->toTranspose();//Have to do this because toMatrixd() return row major version of matrix and we need column major
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
// Function:	CopyVector
// Purpose:		Copy a vector
// Arguments:	pointer to destination and source
///////////////////////////////////////////////////////////////////////////////
void CopyVector(Vector3 *dest, Vector3 *src)
{
	dest->x = src->x;
	dest->y = src->y;
	dest->z = src->z;
}
//// CopyVector ///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Function:	ScaleVector
// Purpose:		Scale a vector
// Arguments:	pointer to vector and scale factor
///////////////////////////////////////////////////////////////////////////////
void ScaleVector(Vector3 *vect, float scale)
{
	vect->x *= scale;
	vect->y *= scale;
	vect->z *= scale;
}
//// ScaleVector ///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Function:	AddVectors
// Purpose:		Add two vectors
// Arguments:	pointer to vectors and dest
///////////////////////////////////////////////////////////////////////////////
void AddVectors(Vector3 *vect1, Vector3 *vect2, Vector3 *dest)
{
	dest->x = vect1->x + vect2->x;
	dest->y = vect1->y + vect2->y;
	dest->z = vect1->z + vect2->z;
}
//// AddVectors ///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Function:	DotVectors
// Purpose:		Compute the dot product of two vectors
// Arguments:	pointer to vectors
// Returns:		Dot product
///////////////////////////////////////////////////////////////////////////////
float DotVectors(Vector3 *vect1, Vector3 *vect2)
{
	return	(vect1->x * vect2->x) + 
			(vect1->y * vect2->y) + 
			(vect1->z * vect2->z);
}
//// DotVectors ///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Function:	CrossVectors
// Purpose:		Computes the cross product of two vectors
// Arguments:	pointer to vectors and dest
///////////////////////////////////////////////////////////////////////////////
void CrossVectors(Vector3 *vect1, Vector3 *vect2, Vector3 *dest)
{
	// COMPUTE THE CROSS PRODUCT
	dest->x = (vect1->y * vect2->z) - (vect1->z * vect2->y);
	dest->y = (vect1->z * vect2->x) - (vect1->x * vect2->z);
	dest->z = (vect1->x * vect2->y) - (vect1->y * vect2->x);
}
//// CrossVectors /////////////////////////////////////////////////////////////

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

	CopyVector((Vector3 *)&v1, (Vector3 *)quat1);		// COPY OFF THE VECTOR PART OF THE QUAT1
	ScaleVector((Vector3 *)&v1,quat2->w);	// MULTIPLY IT BY THE SCALAR PART OF QUAT2

	CopyVector((Vector3 *)&v2, (Vector3 *)quat2);		// COPY OFF THE VECTOR PART OF THE QUAT1
	ScaleVector((Vector3 *)&v2,quat1->w);	// MULTIPLY IT BY THE SCALAR PART OF QUAT2

	CrossVectors((Vector3 *)quat2,(Vector3 *)quat1,(Vector3 *)&v3);

	AddVectors((Vector3 *)&v1, (Vector3 *)&v2, (Vector3 *)&vf);
	AddVectors((Vector3 *)&v3, (Vector3 *)&vf, (Vector3 *)&vf);

	vf.w = (quat1->w * quat2->w) - DotVectors((Vector3 *)quat1,(Vector3 *)quat2);

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