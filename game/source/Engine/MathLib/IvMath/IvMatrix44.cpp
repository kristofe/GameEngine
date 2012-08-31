//===============================================================================
// @ IvMatrix44.cpp
// 
// 3x3 matrix class
// ------------------------------------------------------------------------------
// Copyright (C) 2004 by Elsevier, Inc. All rights reserved.
//
//===============================================================================

//-------------------------------------------------------------------------------
//-- Dependencies ---------------------------------------------------------------
//-------------------------------------------------------------------------------

#include "IvMatrix33.h"
#include "IvMatrix44.h"
#include "IvMath.h"
#include "IvQuat.h"
#include "IvVector3.h"
#include "IvVector4.h"

#include "IvAssert.h"

#define PI          3.14159265358979323846f
#define HALF_PI	    1.57079632679489661923f
//-------------------------------------------------------------------------------
//-- Static Members -------------------------------------------------------------
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//-- Methods --------------------------------------------------------------------
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// @ IvMatrix44::IvMatrix44()
//-------------------------------------------------------------------------------
// Axis-angle constructor
//-------------------------------------------------------------------------------
IvMatrix44::IvMatrix44( const IvQuat& quat )
{
    (void) Rotation( quat );

}   // End of IvMatrix44::IvMatrix44()


//-------------------------------------------------------------------------------
// @ IvMatrix44::IvMatrix44()
//-------------------------------------------------------------------------------
// Copy constructor
//-------------------------------------------------------------------------------
IvMatrix44::IvMatrix44(const IvMatrix44& other)
{
    mV[0] = other.mV[0];
    mV[1] = other.mV[1];
    mV[2] = other.mV[2];
    mV[3] = other.mV[3];
    mV[4] = other.mV[4];
    mV[5] = other.mV[5];
    mV[6] = other.mV[6];
    mV[7] = other.mV[7];
    mV[8] = other.mV[8];
    mV[9] = other.mV[9];
    mV[10] = other.mV[10];
    mV[11] = other.mV[11];
    mV[12] = other.mV[12];
    mV[13] = other.mV[13];
    mV[14] = other.mV[14];
    mV[15] = other.mV[15];

}   // End of IvMatrix44::IvMatrix44()

//-------------------------------------------------------------------------------
// @ IvMatrix44::IvMatrix44()
//-------------------------------------------------------------------------------
// IvMatrix33 conversion constructor
//-------------------------------------------------------------------------------
IvMatrix44::IvMatrix44(const IvMatrix33& other)
{
    (void) Rotation( other );

}   // End of IvMatrix44::IvMatrix44()


//-------------------------------------------------------------------------------
// @ IvMatrix44::operator=()
//-------------------------------------------------------------------------------
// Assignment operator
//-------------------------------------------------------------------------------
IvMatrix44&
IvMatrix44::operator=(const IvMatrix44& other)
{
    // if same object
    if ( this == &other )
        return *this;
        
    mV[0] = other.mV[0];
    mV[1] = other.mV[1];
    mV[2] = other.mV[2];
    mV[3] = other.mV[3];
    mV[4] = other.mV[4];
    mV[5] = other.mV[5];
    mV[6] = other.mV[6];
    mV[7] = other.mV[7];
    mV[8] = other.mV[8];
    mV[9] = other.mV[9];
    mV[10] = other.mV[10];
    mV[11] = other.mV[11];
    mV[12] = other.mV[12];
    mV[13] = other.mV[13];
    mV[14] = other.mV[14];
    mV[15] = other.mV[15];
    
    return *this;

}   // End of IvMatrix44::operator=()


//-------------------------------------------------------------------------------
// @ operator<<()
//-------------------------------------------------------------------------------
// Text output for debugging
//-------------------------------------------------------------------------------
IvWriter& 
operator<<(IvWriter& out, const IvMatrix44& source)
{
    // row
    for (unsigned int i = 0; i < 4; ++i)
    {
        out << "| ";
        // column
        for (unsigned int j = 0; j < 4; ++j )
        {
            out << source.mV[ j*4 + i ] << ' ';
        }
        out << '|' << eol;
    }

    return out;
    
}   // End of operator<<()
    

//-------------------------------------------------------------------------------
// @ IvMatrix44::operator==()
//-------------------------------------------------------------------------------
// Comparison operator
//-------------------------------------------------------------------------------
bool 
IvMatrix44::operator==( const IvMatrix44& other ) const
{
    for (unsigned int i = 0; i < 16; ++i)
    {
        if ( !IvAreEqual(mV[i], other.mV[i]) )
            return false;
    }
    return true;

}   // End of IvMatrix44::operator==()


//-------------------------------------------------------------------------------
// @ IvMatrix44::operator!=()
//-------------------------------------------------------------------------------
// Comparison operator
//-------------------------------------------------------------------------------
bool 
IvMatrix44::operator!=( const IvMatrix44& other ) const
{
    for (unsigned int i = 0; i < 16; ++i)
    {
        if ( !IvAreEqual(mV[i], other.mV[i]) )
            return true;
    }
    return false;

}   // End of IvMatrix44::operator!=()


//-------------------------------------------------------------------------------
// @ IvMatrix44::IsZero()
//-------------------------------------------------------------------------------
// Check for zero matrix
//-------------------------------------------------------------------------------
bool 
IvMatrix44::IsZero() const
{
    for (unsigned int i = 0; i < 16; ++i)
    {
        if ( !::IsZero( mV[i] ) )
            return false;
    }
    return true;

}   // End of IvMatrix44::IsZero()


//-------------------------------------------------------------------------------
// @ IvMatrix44::IsIdentity()
//-------------------------------------------------------------------------------
// Check for identity matrix
//-------------------------------------------------------------------------------
bool 
IvMatrix44::IsIdentity() const
{
    return ::IvAreEqual( 1.0f, mV[0] )
        && ::IvAreEqual( 1.0f, mV[5] )
        && ::IvAreEqual( 1.0f, mV[10] )
        && ::IvAreEqual( 1.0f, mV[15] )
        && ::IsZero( mV[1] ) 
        && ::IsZero( mV[2] )
        && ::IsZero( mV[3] )
        && ::IsZero( mV[4] ) 
        && ::IsZero( mV[6] )
        && ::IsZero( mV[7] )
        && ::IsZero( mV[8] )
        && ::IsZero( mV[9] )
        && ::IsZero( mV[11] )
        && ::IsZero( mV[12] )
        && ::IsZero( mV[13] )
        && ::IsZero( mV[14] );

}   // End of IvMatrix44::IsIdentity()


//-------------------------------------------------------------------------------
// @ IvMatrix44::Clean()
//-------------------------------------------------------------------------------
// Set elements close to zero equal to zero
//-------------------------------------------------------------------------------
void
IvMatrix44::Clean()
{
    for (unsigned int i = 0; i < 16; ++i)
    {
        if ( ::IsZero( mV[i] ) )
            mV[i] = 0.0f;
    }

}   // End of IvMatrix44::Clean()


//-------------------------------------------------------------------------------
// @ IvMatrix44::Identity()
//-------------------------------------------------------------------------------
// Set to identity matrix
//-------------------------------------------------------------------------------
void
IvMatrix44::Identity()
{
    mV[0] = 1.0f;
    mV[1] = 0.0f;
    mV[2] = 0.0f;
    mV[3] = 0.0f;
    mV[4] = 0.0f;
    mV[5] = 1.0f;
    mV[6] = 0.0f;
    mV[7] = 0.0f;
    mV[8] = 0.0f;
    mV[9] = 0.0f;
    mV[10] = 1.0f;
    mV[11] = 0.0f;
    mV[12] = 0.0f;
    mV[13] = 0.0f;
    mV[14] = 0.0f;
    mV[15] = 1.0f;

}   // End of IvMatrix44::Identity()




//-----------------------------------------------------------------------------
// @ AffineInverse()
//-----------------------------------------------------------------------------
/// Compute matrix inverse, assuming a standard affine matrix 
/// (bottom row is 0 0 0 1)
//-----------------------------------------------------------------------------
IvMatrix44 
AffineInverse( const IvMatrix44& mat )
{
    IvMatrix44 result;
    
    // compute upper left 3x3 matrix determinant
    float cofactor0 = mat.mV[5]*mat.mV[10] - mat.mV[6]*mat.mV[9];
    float cofactor4 = mat.mV[2]*mat.mV[9] - mat.mV[1]*mat.mV[10];
    float cofactor8 = mat.mV[1]*mat.mV[6] - mat.mV[2]*mat.mV[5];
    float det = mat.mV[0]*cofactor0 + mat.mV[4]*cofactor4 + mat.mV[8]*cofactor8;
    if (::IsZero( det ))
    {
        ASSERT( false );
        ERROR_OUT( "Matrix44::Inverse() -- singular matrix\n" );
        return result;
    }

    // create adjunct matrix and multiply by 1/det to get upper 3x3
    float invDet = 1.0f/det;
    result.mV[0] = invDet*cofactor0;
    result.mV[1] = invDet*cofactor4;
    result.mV[2] = invDet*cofactor8;
   
    result.mV[4] = invDet*(mat.mV[6]*mat.mV[8] - mat.mV[4]*mat.mV[10]);
    result.mV[5] = invDet*(mat.mV[0]*mat.mV[10] - mat.mV[2]*mat.mV[8]);
    result.mV[6] = invDet*(mat.mV[2]*mat.mV[4] - mat.mV[0]*mat.mV[6]);

    result.mV[8] = invDet*(mat.mV[4]*mat.mV[9] - mat.mV[5]*mat.mV[8]);
    result.mV[9] = invDet*(mat.mV[1]*mat.mV[8] - mat.mV[0]*mat.mV[9]);
    result.mV[10] = invDet*(mat.mV[0]*mat.mV[5] - mat.mV[1]*mat.mV[4]);

    // multiply -translation by inverted 3x3 to get its inverse
    
    result.mV[12] = -result.mV[0]*mat.mV[12] - result.mV[4]*mat.mV[13] - result.mV[8]*mat.mV[14];
    result.mV[13] = -result.mV[1]*mat.mV[12] - result.mV[5]*mat.mV[13] - result.mV[9]*mat.mV[14];
    result.mV[14] = -result.mV[2]*mat.mV[12] - result.mV[6]*mat.mV[13] - result.mV[10]*mat.mV[14];

    return result;

}   // End of AffineInverse()

//-----------------------------------------------------------------------------
// @ IvMatrix44::AffineInverse()
//-----------------------------------------------------------------------------
/// Set self to matrix inverse, assuming a standard affine matrix 
/// (bottom row is 0 0 0 1)
//-----------------------------------------------------------------------------
IvMatrix44& 
IvMatrix44::AffineInverse()
{
    *this = ::AffineInverse( *this );
    
    return *this;
	
}   // End of IvMatrix44::AffineInverse()


//-------------------------------------------------------------------------------
// @ IvMatrix44::Transpose()
//-------------------------------------------------------------------------------
// Set self to transpose
//-------------------------------------------------------------------------------
IvMatrix44& 
IvMatrix44::Transpose()
{
    float temp = mV[1];
    mV[1] = mV[4];
    mV[4] = temp;

    temp = mV[2];
    mV[2] = mV[8];
    mV[8] = temp;

    temp = mV[3];
    mV[2] = mV[12];
    mV[12] = temp;

    temp = mV[6];
    mV[6] = mV[9];
    mV[9] = temp;

    temp = mV[7];
    mV[7] = mV[13];
    mV[13] = temp;

    temp = mV[11];
    mV[11] = mV[14];
    mV[14] = temp;

    return *this;

}   // End of IvMatrix44::Transpose()


//-------------------------------------------------------------------------------
// @ Transpose()
//-------------------------------------------------------------------------------
// Compute matrix transpose
//-------------------------------------------------------------------------------
IvMatrix44 
Transpose( const IvMatrix44& mat )
{
    IvMatrix44 result;

    result.mV[0] = mat.mV[0];
    result.mV[1] = mat.mV[4];
    result.mV[2] = mat.mV[8];
    result.mV[3] = mat.mV[12];
    result.mV[4] = mat.mV[1];
    result.mV[5] = mat.mV[5];
    result.mV[6] = mat.mV[9];
    result.mV[7] = mat.mV[13];
    result.mV[8] = mat.mV[2];
    result.mV[9] = mat.mV[6];
    result.mV[10] = mat.mV[10];
    result.mV[11] = mat.mV[14];
    result.mV[12] = mat.mV[3];
    result.mV[13] = mat.mV[7];
    result.mV[14] = mat.mV[11];
    result.mV[15] = mat.mV[15];

    return result;

}   // End of Transpose()


//-------------------------------------------------------------------------------
// @ IvMatrix44::Translation()
//-------------------------------------------------------------------------------
// Set as translation matrix based on vector
//-------------------------------------------------------------------------------
IvMatrix44& 
IvMatrix44::Translation( const IvVector3& xlate )
{
    mV[0] = 1.0f;
    mV[1] = 0.0f;
    mV[2] = 0.0f;
    mV[3] = 0.0f;
    mV[4] = 0.0f;
    mV[5] = 1.0f;
    mV[6] = 0.0f;
    mV[7] = 0.0f;
    mV[8] = 0.0f;
    mV[9] = 0.0f;
    mV[10] = 1.0f;
    mV[11] = 0.0f;
    mV[12] = xlate.x;
    mV[13] = xlate.y;
    mV[14] = xlate.z;
    mV[15] = 1.0f;

    return *this;

}   // End of IvMatrix44::Translation()


//-------------------------------------------------------------------------------
// @ IvMatrix44::Rotation()
//-------------------------------------------------------------------------------
// Set as rotation matrix based on quaternion
//-------------------------------------------------------------------------------
IvMatrix44& 
IvMatrix44::Rotation( const IvQuat& rotate )
{
    ASSERT( rotate.IsUnit() );

    float xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;

    xs = rotate.x+rotate.x;   
    ys = rotate.y+rotate.y;
    zs = rotate.z+rotate.z;
    wx = rotate.w*xs;
    wy = rotate.w*ys;
    wz = rotate.w*zs;
    xx = rotate.x*xs;
    xy = rotate.x*ys;
    xz = rotate.x*zs;
    yy = rotate.y*ys;
    yz = rotate.y*zs;
    zz = rotate.z*zs;

    mV[0] = 1.0f - (yy + zz);
    mV[4] = xy - wz;
    mV[8] = xz + wy;
    mV[12] = 0.0f;
    
    mV[1] = xy + wz;
    mV[5] = 1.0f - (xx + zz);
    mV[9] = yz - wx;
    mV[13] = 0.0f;
    
    mV[2] = xz - wy;
    mV[6] = yz + wx;
    mV[10] = 1.0f - (xx + yy);
    mV[14] = 0.0f;

    mV[3] = 0.0f;
    mV[7] = 0.0f;
    mV[11] = 0.0f;
    mV[15] = 1.0f;

    return *this;

}   // End of Rotation()


//-------------------------------------------------------------------------------
// @ IvMatrix44::Rotation()
//-------------------------------------------------------------------------------
// Set to rotation matrix based on 3x3 matrix (which better be a rotation)
//-------------------------------------------------------------------------------
IvMatrix44&
IvMatrix44::Rotation(const IvMatrix33& other)
{
    mV[0] = other.mV[0];
    mV[1] = other.mV[1];
    mV[2] = other.mV[2];
    mV[3] = 0;
    mV[4] = other.mV[3];
    mV[5] = other.mV[4];
    mV[6] = other.mV[5];
    mV[7] = 0;
    mV[8] = other.mV[6];
    mV[9] = other.mV[7];
    mV[10] = other.mV[8];
    mV[11] = 0;
    mV[12] = 0;
    mV[13] = 0;
    mV[14] = 0;
    mV[15] = 1;

    return *this;

}   // End of IvMatrix44::IvMatrix44()


//----------------------------------------------------------------------------
// @ IvMatrix44::Rotation()
// ---------------------------------------------------------------------------
// Sets the matrix to a rotation matrix (by Euler angles).
//----------------------------------------------------------------------------
IvMatrix44 &
IvMatrix44::Rotation( float zRotation, float yRotation, float xRotation )
{
    // This is an "unrolled" contatenation of rotation matrices X Y & Z
    float Cx, Sx;
    IvSinCos(xRotation, Sx, Cx);

    float Cy, Sy;
    IvSinCos(yRotation, Sy, Cy);

    float Cz, Sz;
    IvSinCos(zRotation, Sz, Cz);

    mV[0] =  (Cy * Cz);
    mV[4] = -(Cy * Sz);  
    mV[8] =  Sy;
    mV[12] = 0.0f;

    mV[1] =  (Sx * Sy * Cz) + (Cx * Sz);
    mV[5] = -(Sx * Sy * Sz) + (Cx * Cz);
    mV[9] = -(Sx * Cy); 
    mV[13] = 0.0f;

    mV[2] = -(Cx * Sy * Cz) + (Sx * Sz);
    mV[6] =  (Cx * Sy * Sz) + (Sx * Cz);
    mV[10] =  (Cx * Cy);
    mV[14] = 0.0f;

    mV[3] = 0.0f;
    mV[7] = 0.0f;
    mV[11] = 0.0f;
    mV[15] = 1.0f;

    return *this;

}  // End of IvMatrix44::Rotation()


//----------------------------------------------------------------------------
// @ IvMatrix44::Rotation()
// ---------------------------------------------------------------------------
// Sets the matrix to a rotation matrix (by axis and angle).
//----------------------------------------------------------------------------
IvMatrix44 &
IvMatrix44::Rotation( const IvVector3& axis, float angle )
{
    float c, s;
    IvSinCos(angle, s, c);
    float t = 1.0f - c;

    IvVector3 nAxis = axis;
    nAxis.Normalize();

    // intermediate values
    float tx = t*nAxis.x;  float ty = t*nAxis.y;  float tz = t*nAxis.z;
    float sx = s*nAxis.x;  float sy = s*nAxis.y;  float sz = s*nAxis.z;
    float txy = tx*nAxis.y; float tyz = tx*nAxis.z; float txz = tx*nAxis.z;

    // set matrix
    mV[0] = tx*nAxis.x + c;
    mV[4] = txy - sz; 
    mV[8] = txz + sy;
    mV[12] = 0.0f;

    mV[1] = txy + sz;
    mV[5] = ty*nAxis.y + c;
    mV[9] = tyz - sx;
    mV[13] = 0.0f;

    mV[2] = txz - sy;
    mV[6] = tyz + sx;
    mV[10] = tz*nAxis.z + c;
    mV[14] = 0.0f;

    mV[3] = 0.0f;
    mV[7] = 0.0f;
    mV[11] = 0.0f;
    mV[15] = 1.0f;

    return *this;

}  // End of IvMatrix44::Rotation()


//-------------------------------------------------------------------------------
// @ IvMatrix44::Scaling()
//-------------------------------------------------------------------------------
// Set as scaling matrix based on vector
//-------------------------------------------------------------------------------
IvMatrix44& 
IvMatrix44::Scaling( const IvVector3& scaleFactors )
{
    mV[0] = scaleFactors.x;
    mV[1] = 0.0f;
    mV[2] = 0.0f;
    mV[3] = 0.0f;
    mV[4] = 0.0f;
    mV[5] = scaleFactors.y;
    mV[6] = 0.0f;
    mV[7] = 0.0f;
    mV[8] = 0.0f;
    mV[9] = 0.0f;
    mV[10] = scaleFactors.z;
    mV[11] = 0.0f;
    mV[12] = 0.0f;
    mV[13] = 0.0f;
    mV[14] = 0.0f;
    mV[15] = 1.0f;

    return *this;

}   // End of Scaling()


//-------------------------------------------------------------------------------
// @ IvMatrix44::RotationX()
//-------------------------------------------------------------------------------
// Set as rotation matrix, rotating by 'angle' radians around x-axis
//-------------------------------------------------------------------------------
IvMatrix44& 
IvMatrix44::RotationX( float angle )
{
    float sintheta, costheta;
    IvSinCos(angle, sintheta, costheta);

    mV[0] = 1.0f;
    mV[1] = 0.0f;
    mV[2] = 0.0f;
    mV[3] = 0.0f;
    mV[4] = 0.0f;
    mV[5] = costheta;
    mV[6] = sintheta;
    mV[7] = 0.0f;
    mV[8] = 0.0f;
    mV[9] = -sintheta;
    mV[10] = costheta;
    mV[11] = 0.0f;
    mV[12] = 0.0f;
    mV[13] = 0.0f;
    mV[14] = 0.0f;
    mV[15] = 1.0f;

    return *this;

}   // End of RotationX()


//-------------------------------------------------------------------------------
// @ IvMatrix44::RotationY()
//-------------------------------------------------------------------------------
// Set as rotation matrix, rotating by 'angle' radians around y-axis
//-------------------------------------------------------------------------------
IvMatrix44& 
IvMatrix44::RotationY( float angle )
{
    float sintheta, costheta;
    IvSinCos(angle, sintheta, costheta);

    mV[0] = costheta;
    mV[1] = 0.0f;
    mV[2] = -sintheta;
    mV[3] = 0.0f;
    mV[4] = 0.0f;
    mV[5] = 1.0f;
    mV[6] = 0.0f;
    mV[7] = 0.0f;
    mV[8] = sintheta;
    mV[9] = 0.0f;
    mV[10] = costheta;
    mV[11] = 0.0f;
    mV[12] = 0.0f;
    mV[13] = 0.0f;
    mV[14] = 0.0f;
    mV[15] = 1.0f;    

    return *this;

}   // End of RotationY()


//-------------------------------------------------------------------------------
// @ IvMatrix44::RotationZ()
//-------------------------------------------------------------------------------
// Set as rotation matrix, rotating by 'angle' radians around z-axis
//-------------------------------------------------------------------------------
IvMatrix44& 
IvMatrix44::RotationZ( float angle )
{
    float sintheta, costheta;
    IvSinCos(angle, sintheta, costheta);

    mV[0] = costheta;
    mV[1] = sintheta;
    mV[2] = 0.0f;
    mV[3] = 0.0f;
    mV[4] = -sintheta;
    mV[5] = costheta;
    mV[6] = 0.0f;
    mV[7] = 0.0f;
    mV[8] = 0.0f;
    mV[9] = 0.0f;
    mV[10] = 1.0f;
    mV[11] = 0.0f;
    mV[12] = 0.0f;
    mV[13] = 0.0f;
    mV[14] = 0.0f;
    mV[15] = 1.0f;

    return *this;

}   // End of RotationZ()


//----------------------------------------------------------------------------
// @ IvMatrix44::GetFixedAngles()
// ---------------------------------------------------------------------------
// Gets one set of possible z-y-x fixed angles that will generate this matrix
// Assumes that upper 3x3 is a rotation matrix
//----------------------------------------------------------------------------
void
IvMatrix44::GetFixedAngles( float& zRotation, float& yRotation, float& xRotation )
{
    float Cx, Sx;
    float Cy, Sy;
    float Cz, Sz;

    Sy = mV[8];
    Cy = ::IvSqrt( 1.0f - Sy*Sy );
    // normal case
    if ( !::IsZero( Cy ) )
    {
        float factor = 1.0f/Cy;
        Sx = -mV[9]*factor;
        Cx = mV[10]*factor;
        Sz = -mV[4]*factor;
        Cz = mV[0]*factor;
    }
    // x and z axes aligned
    else
    {
        Sz = 0.0f;
        Cz = 1.0f;
        Sx = mV[6];
        Cx = mV[5];
    }

    zRotation = atan2f( Sz, Cz );
    yRotation = atan2f( Sy, Cy );
    xRotation = atan2f( Sx, Cx );

}  // End of IvMatrix44::GetFixedAngles()


//----------------------------------------------------------------------------
// @ IvMatrix44::GetAxisAngle()
// ---------------------------------------------------------------------------
// Gets one possible axis-angle pair that will generate this matrix
// Assumes that upper 3x3 is a rotation matrix
//----------------------------------------------------------------------------
void
IvMatrix44::GetAxisAngle( IvVector3& axis, float& angle )
{
    float trace = mV[0] + mV[5] + mV[10];
    float cosTheta = 0.5f*(trace - 1.0f);
    angle = acosf( cosTheta );

    // angle is zero, axis can be anything
    if ( ::IsZero( angle ) )
    {
        axis = IvVector3::xAxis;
    }
    // standard case
    else if ( angle < kPI-kEpsilon )
    {
        axis.Set( mV[6]-mV[9], mV[8]-mV[2], mV[1]-mV[4] );
        axis.Normalize();
    }
    // angle is 180 degrees
    else
    {
        unsigned int i = 0;
        if ( mV[5] > mV[0] )
            i = 1;
        if ( mV[10] > mV[i + 4*i] )
            i = 2;
        unsigned int j = (i+1)%3;
        unsigned int k = (j+1)%3;
        float s = ::IvSqrt( mV[i + 4*i] - mV[j + 4*j] - mV[k + 4*k] + 1.0f );
        axis[i] = 0.5f*s;
        float recip = 0.5f/s;
        axis[j] = (mV[i + 4*j])*recip;
        axis[k] = (mV[k + 4*i])*recip;
    }

}  // End of IvMatrix44::GetAxisAngle()


//-------------------------------------------------------------------------------
// @ IvMatrix44::operator+()
//-------------------------------------------------------------------------------
// Matrix addition 
//-------------------------------------------------------------------------------
IvMatrix44
IvMatrix44::operator+( const IvMatrix44& other ) const
{
    IvMatrix44 result;

    for (unsigned int i = 0; i < 16; ++i)
    {
        result.mV[i] = mV[i] + other.mV[i];
    }

    return result;

}   // End of IvMatrix44::operator-()


//-------------------------------------------------------------------------------
// @ IvMatrix44::operator+=()
//-------------------------------------------------------------------------------
// Matrix addition by self
//-------------------------------------------------------------------------------
IvMatrix44&
IvMatrix44::operator+=( const IvMatrix44& other )
{
    for (unsigned int i = 0; i < 16; ++i)
    {
        mV[i] += other.mV[i];
    }

    return *this;

}   // End of IvMatrix44::operator+=()


//-------------------------------------------------------------------------------
// @ IvMatrix44::operator-()
//-------------------------------------------------------------------------------
// Matrix subtraction 
//-------------------------------------------------------------------------------
IvMatrix44
IvMatrix44::operator-( const IvMatrix44& other ) const
{
    IvMatrix44 result;

    for (unsigned int i = 0; i < 16; ++i)
    {
        result.mV[i] = mV[i] - other.mV[i];
    }

    return result;

}   // End of IvMatrix44::operator-()


//-------------------------------------------------------------------------------
// @ IvMatrix44::operator-=()
//-------------------------------------------------------------------------------
// Matrix subtraction by self
//-------------------------------------------------------------------------------
IvMatrix44&
IvMatrix44::operator-=( const IvMatrix44& other )
{
    for (unsigned int i = 0; i < 16; ++i)
    {
        mV[i] -= other.mV[i];
    }

    return *this;

}   // End of IvMatrix44::operator-=()


//-------------------------------------------------------------------------------
// @ IvMatrix44::operator-=() (unary)
//-------------------------------------------------------------------------------
// Negate self and return
//-------------------------------------------------------------------------------
IvMatrix44
IvMatrix44::operator-() const
{
    IvMatrix44 result;

    for (unsigned int i = 0; i < 16; ++i)
    {
        result.mV[i] = -mV[i];
    }

    return result;

}    // End of IvQuat::operator-()


//-------------------------------------------------------------------------------
// @ IvMatrix44::operator*()
//-------------------------------------------------------------------------------
// Matrix multiplication
//-------------------------------------------------------------------------------
IvMatrix44  
IvMatrix44::operator*( const IvMatrix44& other ) const
{
    IvMatrix44 result;

    result.mV[0] = mV[0]*other.mV[0] + mV[4]*other.mV[1] + mV[8]*other.mV[2] 
                    + mV[12]*other.mV[3];
    result.mV[1] = mV[1]*other.mV[0] + mV[5]*other.mV[1] + mV[9]*other.mV[2] 
                    + mV[13]*other.mV[3];
    result.mV[2] = mV[2]*other.mV[0] + mV[6]*other.mV[1] + mV[10]*other.mV[2] 
                    + mV[14]*other.mV[3];
    result.mV[3] = mV[3]*other.mV[0] + mV[7]*other.mV[1] + mV[11]*other.mV[2] 
                    + mV[15]*other.mV[3];

    result.mV[4] = mV[0]*other.mV[4] + mV[4]*other.mV[5] + mV[8]*other.mV[6] 
                    + mV[12]*other.mV[7];
    result.mV[5] = mV[1]*other.mV[4] + mV[5]*other.mV[5] + mV[9]*other.mV[6] 
                    + mV[13]*other.mV[7];
    result.mV[6] = mV[2]*other.mV[4] + mV[6]*other.mV[5] + mV[10]*other.mV[6] 
                    + mV[14]*other.mV[7];
    result.mV[7] = mV[3]*other.mV[4] + mV[7]*other.mV[5] + mV[11]*other.mV[6] 
                    + mV[15]*other.mV[7];

    result.mV[8] = mV[0]*other.mV[8] + mV[4]*other.mV[9] + mV[8]*other.mV[10] 
                    + mV[12]*other.mV[11];
    result.mV[9] = mV[1]*other.mV[8] + mV[5]*other.mV[9] + mV[9]*other.mV[10] 
                    + mV[13]*other.mV[11];
    result.mV[10] = mV[2]*other.mV[8] + mV[6]*other.mV[9] + mV[10]*other.mV[10] 
                    + mV[14]*other.mV[11];
    result.mV[11] = mV[3]*other.mV[8] + mV[7]*other.mV[9] + mV[11]*other.mV[10] 
                    + mV[15]*other.mV[11];

    result.mV[12] = mV[0]*other.mV[12] + mV[4]*other.mV[13] + mV[8]*other.mV[14] 
                    + mV[12]*other.mV[15];
    result.mV[13] = mV[1]*other.mV[12] + mV[5]*other.mV[13] + mV[9]*other.mV[14] 
                    + mV[13]*other.mV[15];
    result.mV[14] = mV[2]*other.mV[12] + mV[6]*other.mV[13] + mV[10]*other.mV[14] 
                    + mV[14]*other.mV[15];
    result.mV[15] = mV[3]*other.mV[12] + mV[7]*other.mV[13] + mV[11]*other.mV[14] 
                    + mV[15]*other.mV[15];

    return result;

}   // End of IvMatrix44::operator*()


//-------------------------------------------------------------------------------
// @ IvMatrix44::operator*=()
//-------------------------------------------------------------------------------
// Matrix multiplication by self
//-------------------------------------------------------------------------------
IvMatrix44&
IvMatrix44::operator*=( const IvMatrix44& other )
{
    IvMatrix44 result;

    result.mV[0] = mV[0]*other.mV[0] + mV[4]*other.mV[1] + mV[8]*other.mV[2] 
                    + mV[12]*other.mV[3];
    result.mV[1] = mV[1]*other.mV[0] + mV[5]*other.mV[1] + mV[9]*other.mV[2] 
                    + mV[13]*other.mV[3];
    result.mV[2] = mV[2]*other.mV[0] + mV[6]*other.mV[1] + mV[10]*other.mV[2] 
                    + mV[14]*other.mV[3];
    result.mV[3] = mV[3]*other.mV[0] + mV[7]*other.mV[1] + mV[11]*other.mV[2] 
                    + mV[15]*other.mV[3];

    result.mV[4] = mV[0]*other.mV[4] + mV[4]*other.mV[5] + mV[8]*other.mV[6] 
                    + mV[12]*other.mV[7];
    result.mV[5] = mV[1]*other.mV[4] + mV[5]*other.mV[5] + mV[9]*other.mV[6] 
                    + mV[13]*other.mV[7];
    result.mV[6] = mV[2]*other.mV[4] + mV[6]*other.mV[5] + mV[10]*other.mV[6] 
                    + mV[14]*other.mV[7];
    result.mV[7] = mV[3]*other.mV[4] + mV[7]*other.mV[5] + mV[11]*other.mV[6] 
                    + mV[15]*other.mV[7];

    result.mV[8] = mV[0]*other.mV[8] + mV[4]*other.mV[9] + mV[8]*other.mV[10] 
                    + mV[12]*other.mV[11];
    result.mV[9] = mV[1]*other.mV[8] + mV[5]*other.mV[9] + mV[9]*other.mV[10] 
                    + mV[13]*other.mV[11];
    result.mV[10] = mV[2]*other.mV[8] + mV[6]*other.mV[9] + mV[10]*other.mV[10] 
                    + mV[14]*other.mV[11];
    result.mV[11] = mV[3]*other.mV[8] + mV[7]*other.mV[9] + mV[11]*other.mV[10] 
                    + mV[15]*other.mV[11];

    result.mV[12] = mV[0]*other.mV[12] + mV[4]*other.mV[13] + mV[8]*other.mV[14] 
                    + mV[12]*other.mV[15];
    result.mV[13] = mV[1]*other.mV[12] + mV[5]*other.mV[13] + mV[9]*other.mV[14] 
                    + mV[13]*other.mV[15];
    result.mV[14] = mV[2]*other.mV[12] + mV[6]*other.mV[13] + mV[10]*other.mV[14] 
                    + mV[14]*other.mV[15];
    result.mV[15] = mV[3]*other.mV[12] + mV[7]*other.mV[13] + mV[11]*other.mV[14] 
                    + mV[15]*other.mV[15];

    for (unsigned int i = 0; i < 16; ++i)
    {
        mV[i] = result.mV[i];
    }

    return *this;

}   // End of IvMatrix44::operator*=()


//-------------------------------------------------------------------------------
// @ IvMatrix44::operator*()
//-------------------------------------------------------------------------------
// Matrix-column vector multiplication
//-------------------------------------------------------------------------------
IvVector4   
IvMatrix44::operator*( const IvVector4& other ) const
{
    IvVector4 result;

    result.x = mV[0]*other.x + mV[4]*other.y + mV[8]*other.z + mV[12]*other.w;
    result.y = mV[1]*other.x + mV[5]*other.y + mV[9]*other.z + mV[13]*other.w;
    result.z = mV[2]*other.x + mV[6]*other.y + mV[10]*other.z + mV[14]*other.w;
    result.w = mV[3]*other.x + mV[7]*other.y + mV[11]*other.z + mV[15]*other.w;

    return result;

}   // End of IvMatrix44::operator*()


//-------------------------------------------------------------------------------
// @ ::operator*()
//-------------------------------------------------------------------------------
// Matrix-row vector multiplication
//-------------------------------------------------------------------------------
IvVector4 
operator*( const IvVector4& vector, const IvMatrix44& matrix )
{
    IvVector4 result;

    result.x = matrix.mV[0]*vector.x + matrix.mV[1]*vector.y 
             + matrix.mV[2]*vector.z + matrix.mV[3]*vector.w;
    result.y = matrix.mV[4]*vector.x + matrix.mV[5]*vector.y 
             + matrix.mV[6]*vector.z + matrix.mV[7]*vector.w;
    result.z = matrix.mV[8]*vector.x + matrix.mV[9]*vector.y 
             + matrix.mV[10]*vector.z + matrix.mV[11]*vector.w;
    result.w = matrix.mV[12]*vector.x + matrix.mV[13]*vector.y 
             + matrix.mV[14]*vector.z + matrix.mV[15]*vector.w;

    return result;

}   // End of IvMatrix44::operator*()


//-------------------------------------------------------------------------------
// @ IvMatrix44::*=()
//-------------------------------------------------------------------------------
// Matrix-scalar multiplication
//-------------------------------------------------------------------------------
IvMatrix44& IvMatrix44::operator*=( float scalar )
{
    mV[0] *= scalar;
    mV[1] *= scalar;
    mV[2] *= scalar;
    mV[3] *= scalar;
    mV[4] *= scalar;
    mV[5] *= scalar;
    mV[6] *= scalar;
    mV[7] *= scalar;
    mV[8] *= scalar;
    mV[9] *= scalar;
    mV[10] *= scalar;
    mV[11] *= scalar;
    mV[12] *= scalar;
    mV[13] *= scalar;
    mV[14] *= scalar;
    mV[15] *= scalar;

    return *this;
}  // End of IvMatrix44::operator*=()


//-------------------------------------------------------------------------------
// @ friend IvMatrix44 *()
//-------------------------------------------------------------------------------
// Matrix-scalar multiplication
//-------------------------------------------------------------------------------
IvMatrix44 operator*( float scalar, const IvMatrix44& matrix )
{
    IvMatrix44 result;
    result.mV[0] = matrix.mV[0] * scalar;
    result.mV[1] = matrix.mV[1] * scalar;
    result.mV[2] = matrix.mV[2] * scalar;
    result.mV[3] = matrix.mV[3] * scalar;
    result.mV[4] = matrix.mV[4] * scalar;
    result.mV[5] = matrix.mV[5] * scalar;
    result.mV[6] = matrix.mV[6] * scalar;
    result.mV[7] = matrix.mV[7] * scalar;
    result.mV[8] = matrix.mV[8] * scalar;
    result.mV[9] = matrix.mV[9] * scalar;
    result.mV[10] = matrix.mV[10] * scalar;
    result.mV[11] = matrix.mV[11] * scalar;
    result.mV[12] = matrix.mV[12] * scalar;
    result.mV[13] = matrix.mV[13] * scalar;
    result.mV[14] = matrix.mV[14] * scalar;
    result.mV[15] = matrix.mV[15] * scalar;

    return result;
}  // End of friend IvMatrix44 operator*()


//-------------------------------------------------------------------------------
// @ IvMatrix44::*()
//-------------------------------------------------------------------------------
// Matrix-scalar multiplication
//-------------------------------------------------------------------------------
IvMatrix44 IvMatrix44::operator*( float scalar ) const
{
    IvMatrix44 result;
    result.mV[0] = mV[0] * scalar;
    result.mV[1] = mV[1] * scalar;
    result.mV[2] = mV[2] * scalar;
    result.mV[3] = mV[3] * scalar;
    result.mV[4] = mV[4] * scalar;
    result.mV[5] = mV[5] * scalar;
    result.mV[6] = mV[6] * scalar;
    result.mV[7] = mV[7] * scalar;
    result.mV[8] = mV[8] * scalar;
    result.mV[9] = mV[9] * scalar;
    result.mV[10] = mV[10] * scalar;
    result.mV[11] = mV[11] * scalar;
    result.mV[12] = mV[12] * scalar;
    result.mV[13] = mV[13] * scalar;
    result.mV[14] = mV[14] * scalar;
    result.mV[15] = mV[15] * scalar;

    return result;
}  // End of IvMatrix44::operator*=()


//-------------------------------------------------------------------------------
// @ IvMatrix44::Transform()
//-------------------------------------------------------------------------------
// Matrix-point multiplication
//-------------------------------------------------------------------------------
IvVector3   
IvMatrix44::Transform( const IvVector3& other ) const
{
    IvVector3 result;

    result.x = mV[0]*other.x + mV[4]*other.y + mV[8]*other.z + mV[12];
    result.y = mV[1]*other.x + mV[5]*other.y + mV[9]*other.z + mV[13];
    result.z = mV[2]*other.x + mV[6]*other.y + mV[10]*other.z + mV[14];
 
    return result;

}   // End of IvMatrix44::Transform()


//-------------------------------------------------------------------------------
// @ IvMatrix44::TransformPoint()
//-------------------------------------------------------------------------------
// Matrix-point multiplication
//-------------------------------------------------------------------------------
IvVector3   
IvMatrix44::TransformPoint( const IvVector3& other ) const
{
    IvVector3 result;

    result.x = mV[0]*other.x + mV[4]*other.y + mV[8]*other.z + mV[12];
    result.y = mV[1]*other.x + mV[5]*other.y + mV[9]*other.z + mV[13];
    result.z = mV[2]*other.x + mV[6]*other.y + mV[10]*other.z + mV[14];
 
    return result;

}   // End of IvMatrix44::TransformPoint()



void IvMatrix44::SetColumns( const IvVector4& col1, const IvVector4& col2, 
							const IvVector4& col3, const IvVector4& col4)
{
    mV[0] = col1.x;
    mV[1] = col1.y;
    mV[2] = col1.z;
    mV[3] = col1.w;
	
	mV[4] = col2.x;
    mV[5] = col2.y;
    mV[6] = col2.z;
    mV[7] = col2.w;
	
	
	mV[8] = col3.x;
    mV[9] = col3.y;
    mV[10] = col3.z;
    mV[11] = col3.w;
	
	
	mV[12] = col4.x;
    mV[13] = col4.y;
    mV[14] = col4.z;
    mV[15] = col4.w;
	
	
}   

void IvMatrix44::GetColumns( IvVector4& col1, IvVector4& col2, 
							IvVector4& col3, IvVector4& col4)
{
    col1.x = mV[0];
    col1.y = mV[1];
    col1.z = mV[2];
    col1.w = mV[3];
	
	col2.x = mV[4];
    col2.y = mV[5];
    col2.z = mV[6];
    col2.w = mV[7];
	
	col3.x = mV[8];
    col3.y = mV[9];
    col3.z = mV[10];
    col3.w = mV[11];
	
	col4.x = mV[12];
    col4.y = mV[13];
    col4.z = mV[14];
    col4.w = mV[15];
	
	
}  

void IvMatrix44::GetColumns( IvVector3& col1, IvVector3& col2, 
							IvVector3& col3, IvVector3& col4)
{
    col1.x = mV[0];
    col1.y = mV[1];
    col1.z = mV[2];

	
	col2.x = mV[4];
    col2.y = mV[5];
    col2.z = mV[6];

	
	col3.x = mV[8];
    col3.y = mV[9];
    col3.z = mV[10];

	
	col4.x = mV[12];
    col4.y = mV[13];
    col4.z = mV[14];

	
	
}  


void IvMatrix44::SetRows( const IvVector4& row1, const IvVector4& row2, 
							const IvVector4& row3, const IvVector4& row4)
{
    mV[0] = row1.x;
    mV[4] = row1.y;
    mV[8] = row1.z;
    mV[12] = row1.w;
	
	mV[1] = row2.x;
    mV[5] = row2.y;
    mV[9] = row2.z;
    mV[13] = row2.w;
	
	
	mV[2] = row3.x;
    mV[6] = row3.y;
    mV[10] = row3.z;
    mV[14] = row3.w;
	
	
	mV[3] = row4.x;
    mV[7] = row4.y;
    mV[11] = row4.z;
    mV[15] = row4.w;
	
	
}  


void IvMatrix44::GetRows( IvVector4& row1, IvVector4& row2, 
						 IvVector4& row3, IvVector4& row4)
{
    row1.x = mV[0];
    row1.y = mV[4];
    row1.z = mV[8];
    row1.w = mV[12];
	
	row2.x = mV[1];
    row2.y = mV[5];
    row2.z = mV[9];
    row2.w = mV[13];
	
	row3.x = mV[2];
    row3.y = mV[6];
    row3.z = mV[10];
    row3.w = mV[14];

	row4.x = mV[3];
    row4.y = mV[7];
    row4.z = mV[11];
    row4.w = mV[15];
	
	
}

void IvMatrix44::GetRows( IvVector3& row1, IvVector3& row2, 
						 IvVector3& row3, IvVector3& row4)
{
    row1.x = mV[0];
    row1.y = mV[4];
    row1.z = mV[8];
	
	row2.x = mV[1];
    row2.y = mV[5];
    row2.z = mV[9];
	
	row3.x = mV[2];
    row3.y = mV[6];
    row3.z = mV[10];
	
	row4.x = mV[3];
    row4.y = mV[7];
    row4.z = mV[11];
	
	
}

void 
IvMatrix44::SetPosition( const IvVector3& pos )
{

    mV[12] = pos.x;
    mV[13] = pos.y;
    mV[14] = pos.z;
} 

void IvMatrix44::Frustum(float left, float right, float bottom, float top, float nearZ, float farZ)
{
    float       deltaX = right - left;
    float       deltaY = top - bottom;
    float       deltaZ = farZ - nearZ;
    Identity();
	
    if ( (nearZ <= 0.0f) || (farZ <= 0.0f) ||
		(deltaX <= 0.0f) || (deltaY <= 0.0f) || (deltaZ <= 0.0f) )
		return;
	
    mV[0] = 2.0f * nearZ / deltaX;
    mV[1] = mV[2] = mV[3] = 0.0f;
	
    mV[5] = 2.0f * nearZ / deltaY;
    mV[4] = mV[6] = mV[7] = 0.0f;
	
    mV[8] = (right + left) / deltaX;
    mV[9] = (top + bottom) / deltaY;
    mV[10] = -(nearZ + farZ) / deltaZ;
    mV[11] = -1.0f;
	
    mV[14] = -2.0f * nearZ * farZ / deltaZ;
    mV[12] = mV[13] = mV[15] = 0.0f;
	
}


void IvMatrix44::Perspective( float fovy, float aspect, float nearZ, float farZ)
{
	float frustumW, frustumH;
	
	frustumH = tanf( fovy / 360.0f * PI ) * nearZ;
	frustumW = frustumH * aspect;
	
	Frustum( -frustumW, frustumW, -frustumH, frustumH, nearZ, farZ );
}

void IvMatrix44::Ortho(float left, float right, float bottom, float top, float nearZ, float farZ)
{
    float       deltaX = right - left;
    float       deltaY = top - bottom;
    float       deltaZ = farZ - nearZ;
	
    if ( (deltaX == 0.0f) || (deltaY == 0.0f) || (deltaZ == 0.0f) )
        return;
	
    Identity();
    mV[0] = 2.0f / deltaX;
    mV[12] = -(right + left) / deltaX;
    mV[5] = 2.0f / deltaY;
    mV[13] = -(top + bottom) / deltaY;
    mV[10] = -2.0f / deltaZ;
    mV[14] = -(nearZ + farZ) / deltaZ;
	
    //MatrixMultiply(result, &ortho, result);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//From MESA 7.6 source  for reference
/*
 
 void GLAPIENTRY
 gluOrtho2D(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top)
 {
 glOrtho(left, right, bottom, top, -1, 1);
 }
 
 #define __glPi 3.14159265358979323846
 
 void GLAPIENTRY
 gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
 {
 GLdouble m[4][4];
 double sine, cotangent, deltaZ;
 double radians = fovy / 2 * __glPi / 180;
 
 deltaZ = zFar - zNear;
 sine = sin(radians);
 if ((deltaZ == 0) || (sine == 0) || (aspect == 0)) {
 return;
 }
 cotangent = COS(radians) / sine;
 
 __gluMakeIdentityd(&m[0][0]);
 m[0][0] = cotangent / aspect;
 m[1][1] = cotangent;
 m[2][2] = -(zFar + zNear) / deltaZ;
 m[2][3] = -1;
 m[3][2] = -2 * zNear * zFar / deltaZ;
 m[3][3] = 0;
 glMultMatrixd(&m[0][0]);
 }
 
 static void normalize(float v[3])
 {
 float r;
 
 r = sqrt( v[0]*v[0] + v[1]*v[1] + v[2]*v[2] );
 if (r == 0.0) return;
 
 v[0] /= r;
 v[1] /= r;
 v[2] /= r;
 }
 
 static void cross(float v1[3], float v2[3], float result[3])
 {
 result[0] = v1[1]*v2[2] - v1[2]*v2[1];
 result[1] = v1[2]*v2[0] - v1[0]*v2[2];
 result[2] = v1[0]*v2[1] - v1[1]*v2[0];
 }
 
 void GLAPIENTRY
 gluLookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez, GLdouble centerx,
 GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upy,
 GLdouble upz)
 {
 float forward[3], side[3], up[3];
 GLfloat m[4][4];
 
 forward[0] = centerx - eyex;
 forward[1] = centery - eyey;
 forward[2] = centerz - eyez;
 
 up[0] = upx;
 up[1] = upy;
 up[2] = upz;
 
 normalize(forward);
 
 // Side = forward x up //
cross(forward, up, side);
normalize(side);

// Recompute up as: up = side x forward //
cross(side, forward, up);

__gluMakeIdentityf(&m[0][0]);
m[0][0] = side[0];
m[1][0] = side[1];
m[2][0] = side[2];

m[0][1] = up[0];
m[1][1] = up[1];
m[2][1] = up[2];

m[0][2] = -forward[0];
m[1][2] = -forward[1];
m[2][2] = -forward[2];

glMultMatrixf(&m[0][0]);
glTranslated(-eyex, -eyey, -eyez);
}

static void __gluMultMatrixVecd(const GLdouble matrix[16], const GLdouble in[4],
								GLdouble out[4])
{
    int i;
	
    for (i=0; i<4; i++) {
		out[i] = 
	    in[0] * matrix[0*4+i] +
	    in[1] * matrix[1*4+i] +
	    in[2] * matrix[2*4+i] +
	    in[3] * matrix[3*4+i];
    }
}

//
// ** Invert 4x4 matrix.
// ** Contributed by David Moore (See Mesa bug #6748)
//
static int __gluInvertMatrixd(const GLdouble m[16], GLdouble invOut[16])
{
    double inv[16], det;
    int i;
	
    inv[0] =   m[5]*m[10]*m[15] - m[5]*m[11]*m[14] - m[9]*m[6]*m[15]
	+ m[9]*m[7]*m[14] + m[13]*m[6]*m[11] - m[13]*m[7]*m[10];
    inv[4] =  -m[4]*m[10]*m[15] + m[4]*m[11]*m[14] + m[8]*m[6]*m[15]
	- m[8]*m[7]*m[14] - m[12]*m[6]*m[11] + m[12]*m[7]*m[10];
    inv[8] =   m[4]*m[9]*m[15] - m[4]*m[11]*m[13] - m[8]*m[5]*m[15]
	+ m[8]*m[7]*m[13] + m[12]*m[5]*m[11] - m[12]*m[7]*m[9];
    inv[12] = -m[4]*m[9]*m[14] + m[4]*m[10]*m[13] + m[8]*m[5]*m[14]
	- m[8]*m[6]*m[13] - m[12]*m[5]*m[10] + m[12]*m[6]*m[9];
    inv[1] =  -m[1]*m[10]*m[15] + m[1]*m[11]*m[14] + m[9]*m[2]*m[15]
	- m[9]*m[3]*m[14] - m[13]*m[2]*m[11] + m[13]*m[3]*m[10];
    inv[5] =   m[0]*m[10]*m[15] - m[0]*m[11]*m[14] - m[8]*m[2]*m[15]
	+ m[8]*m[3]*m[14] + m[12]*m[2]*m[11] - m[12]*m[3]*m[10];
    inv[9] =  -m[0]*m[9]*m[15] + m[0]*m[11]*m[13] + m[8]*m[1]*m[15]
	- m[8]*m[3]*m[13] - m[12]*m[1]*m[11] + m[12]*m[3]*m[9];
    inv[13] =  m[0]*m[9]*m[14] - m[0]*m[10]*m[13] - m[8]*m[1]*m[14]
	+ m[8]*m[2]*m[13] + m[12]*m[1]*m[10] - m[12]*m[2]*m[9];
    inv[2] =   m[1]*m[6]*m[15] - m[1]*m[7]*m[14] - m[5]*m[2]*m[15]
	+ m[5]*m[3]*m[14] + m[13]*m[2]*m[7] - m[13]*m[3]*m[6];
    inv[6] =  -m[0]*m[6]*m[15] + m[0]*m[7]*m[14] + m[4]*m[2]*m[15]
	- m[4]*m[3]*m[14] - m[12]*m[2]*m[7] + m[12]*m[3]*m[6];
    inv[10] =  m[0]*m[5]*m[15] - m[0]*m[7]*m[13] - m[4]*m[1]*m[15]
	+ m[4]*m[3]*m[13] + m[12]*m[1]*m[7] - m[12]*m[3]*m[5];
    inv[14] = -m[0]*m[5]*m[14] + m[0]*m[6]*m[13] + m[4]*m[1]*m[14]
	- m[4]*m[2]*m[13] - m[12]*m[1]*m[6] + m[12]*m[2]*m[5];
    inv[3] =  -m[1]*m[6]*m[11] + m[1]*m[7]*m[10] + m[5]*m[2]*m[11]
	- m[5]*m[3]*m[10] - m[9]*m[2]*m[7] + m[9]*m[3]*m[6];
    inv[7] =   m[0]*m[6]*m[11] - m[0]*m[7]*m[10] - m[4]*m[2]*m[11]
	+ m[4]*m[3]*m[10] + m[8]*m[2]*m[7] - m[8]*m[3]*m[6];
    inv[11] = -m[0]*m[5]*m[11] + m[0]*m[7]*m[9] + m[4]*m[1]*m[11]
	- m[4]*m[3]*m[9] - m[8]*m[1]*m[7] + m[8]*m[3]*m[5];
    inv[15] =  m[0]*m[5]*m[10] - m[0]*m[6]*m[9] - m[4]*m[1]*m[10]
	+ m[4]*m[2]*m[9] + m[8]*m[1]*m[6] - m[8]*m[2]*m[5];
	
    det = m[0]*inv[0] + m[1]*inv[4] + m[2]*inv[8] + m[3]*inv[12];
    if (det == 0)
        return GL_FALSE;
	
    det = 1.0 / det;
	
    for (i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;
	
    return GL_TRUE;
}

static void __gluMultMatricesd(const GLdouble a[16], const GLdouble b[16],
							   GLdouble r[16])
{
    int i, j;
	
    for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			r[i*4+j] = 
			a[i*4+0]*b[0*4+j] +
			a[i*4+1]*b[1*4+j] +
			a[i*4+2]*b[2*4+j] +
			a[i*4+3]*b[3*4+j];
		}
    }
}

GLint GLAPIENTRY
gluProject(GLdouble objx, GLdouble objy, GLdouble objz, 
		   const GLdouble modelMatrix[16], 
		   const GLdouble projMatrix[16],
		   const GLint viewport[4],
		   GLdouble *winx, GLdouble *winy, GLdouble *winz)
{
    double in[4];
    double out[4];
	
    in[0]=objx;
    in[1]=objy;
    in[2]=objz;
    in[3]=1.0;
    __gluMultMatrixVecd(modelMatrix, in, out);
    __gluMultMatrixVecd(projMatrix, out, in);
    if (in[3] == 0.0) return(GL_FALSE);
    in[0] /= in[3];
    in[1] /= in[3];
    in[2] /= in[3];
    // Map x, y and z to range 0-1 //
    in[0] = in[0] * 0.5 + 0.5;
    in[1] = in[1] * 0.5 + 0.5;
    in[2] = in[2] * 0.5 + 0.5;
	
    // Map x,y to viewport //
    in[0] = in[0] * viewport[2] + viewport[0];
    in[1] = in[1] * viewport[3] + viewport[1];
	
    *winx=in[0];
    *winy=in[1];
    *winz=in[2];
    return(GL_TRUE);
}

GLint GLAPIENTRY
gluUnProject(GLdouble winx, GLdouble winy, GLdouble winz,
			 const GLdouble modelMatrix[16], 
			 const GLdouble projMatrix[16],
			 const GLint viewport[4],
			 GLdouble *objx, GLdouble *objy, GLdouble *objz)
{
    double finalMatrix[16];
    double in[4];
    double out[4];
	
    __gluMultMatricesd(modelMatrix, projMatrix, finalMatrix);
    if (!__gluInvertMatrixd(finalMatrix, finalMatrix)) return(GL_FALSE);
	
    in[0]=winx;
    in[1]=winy;
    in[2]=winz;
    in[3]=1.0;
	
    // Map x and y from window coordinates //
    in[0] = (in[0] - viewport[0]) / viewport[2];
    in[1] = (in[1] - viewport[1]) / viewport[3];
	
    // Map to range -1 to 1
    in[0] = in[0] * 2 - 1;
    in[1] = in[1] * 2 - 1;
    in[2] = in[2] * 2 - 1;
	
    __gluMultMatrixVecd(finalMatrix, in, out);
    if (out[3] == 0.0) return(GL_FALSE);
    out[0] /= out[3];
    out[1] /= out[3];
    out[2] /= out[3];
    *objx = out[0];
    *objy = out[1];
    *objz = out[2];
    return(GL_TRUE);
}

GLint GLAPIENTRY
gluUnProject4(GLdouble winx, GLdouble winy, GLdouble winz, GLdouble clipw,
			  const GLdouble modelMatrix[16], 
			  const GLdouble projMatrix[16],
			  const GLint viewport[4],
			  GLclampd nearVal, GLclampd farVal,		    
			  GLdouble *objx, GLdouble *objy, GLdouble *objz,
			  GLdouble *objw)
{
    double finalMatrix[16];
    double in[4];
    double out[4];
	
    __gluMultMatricesd(modelMatrix, projMatrix, finalMatrix);
    if (!__gluInvertMatrixd(finalMatrix, finalMatrix)) return(GL_FALSE);
	
    in[0]=winx;
    in[1]=winy;
    in[2]=winz;
    in[3]=clipw;
	
    // Map x and y from window coordinates 
    in[0] = (in[0] - viewport[0]) / viewport[2];
    in[1] = (in[1] - viewport[1]) / viewport[3];
    in[2] = (in[2] - nearVal) / (farVal - nearVal);
	
    //Map to range -1 to 1
    in[0] = in[0] * 2 - 1;
    in[1] = in[1] * 2 - 1;
    in[2] = in[2] * 2 - 1;
	
    __gluMultMatrixVecd(finalMatrix, in, out);
    if (out[3] == 0.0) return(GL_FALSE);
    *objx = out[0];
    *objy = out[1];
    *objz = out[2];
    *objw = out[3];
    return(GL_TRUE);
}
*/
