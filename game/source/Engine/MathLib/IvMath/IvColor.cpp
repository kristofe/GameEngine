//===============================================================================
// @ IvColor.cpp
// 
// color class
// ------------------------------------------------------------------------------
//
//===============================================================================

//-------------------------------------------------------------------------------
//-- Dependencies ---------------------------------------------------------------
//-------------------------------------------------------------------------------

#include "IvColor.h"
#include "IvMath.h"

//-------------------------------------------------------------------------------
//-- Static Members -------------------------------------------------------------
//-------------------------------------------------------------------------------

IvColor IvColor::red( 1.0f, 0.0f, 0.0f, 1.0f );
IvColor IvColor::green( 0.0f, 1.0f, 0.0f, 1.0f );
IvColor IvColor::blue( 0.0f, 0.0f, 1.0f, 1.0f );
IvColor IvColor::black( 0.0f, 0.0f, 0.0f, 1.0f );
IvColor IvColor::white( 1.0f, 1.0f, 1.0f, 1.0f );

//-------------------------------------------------------------------------------
//-- Methods --------------------------------------------------------------------
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// @ IvColor::IvColor()
//-------------------------------------------------------------------------------
// Copg constructor
//-------------------------------------------------------------------------------
IvColor::IvColor(const IvColor& other) : 
r( other.r ),
g( other.g ),
b( other.b ),
a( other.a )
{
	
}   // End of IvColor::IvColor()


//-------------------------------------------------------------------------------
// @ IvColor::operator=()
//-------------------------------------------------------------------------------
// Assignment operator
//-------------------------------------------------------------------------------
IvColor&
IvColor::operator=(const IvColor& other)
{
    // if same object
    if ( this == &other )
        return *this;
	
    r = other.r;
    g = other.g;
    b = other.b;
    a = other.a;
    
    return *this;
	
}   // End of IvColor::operator=()


//-------------------------------------------------------------------------------
// @ operator<<()
//-------------------------------------------------------------------------------
// Text output for debugging
//-------------------------------------------------------------------------------
IvWriter& 
operator<<(IvWriter& out, const IvColor& source)
{
    out << '<' << source.r << ',' << source.g << ',' << source.b << ','
	<< source.a << '>';
	
    return out;
    
}   // End of operator<<()




//-------------------------------------------------------------------------------
// @ IvColor::operator==()
//-------------------------------------------------------------------------------
// Comparison operator
//-------------------------------------------------------------------------------
bool 
IvColor::operator==( const IvColor& other ) const
{
    if ( ::IvAreEqual( other.r, r )
        && ::IvAreEqual( other.g, g )
        && ::IvAreEqual( other.b, b )
        && ::IvAreEqual( other.a, a ) )
        return true;
	
    return false;   
}   // End of IvColor::operator==()


//-------------------------------------------------------------------------------
// @ IvColor::operator!=()
//-------------------------------------------------------------------------------
// Comparison operator
//-------------------------------------------------------------------------------
bool 
IvColor::operator!=( const IvColor& other ) const
{
    if ( ::IvAreEqual( other.r, r )
        && ::IvAreEqual( other.g, g )
        && ::IvAreEqual( other.b, b )
        && ::IvAreEqual( other.a, a ) )
        return false;
	
    return true;
}   // End of IvColor::operator!=()




//-------------------------------------------------------------------------------
// @ IvColor::operator+()
//-------------------------------------------------------------------------------
// Add vector to self and return
//-------------------------------------------------------------------------------
IvColor 
IvColor::operator+( const IvColor& other ) const
{
    return IvColor( r + other.r, g + other.g, b + other.b, a + other.a );
	
}   // End of IvColor::operator+()


//-------------------------------------------------------------------------------
// @ IvColor::operator+=()
//-------------------------------------------------------------------------------
// Add vector to self, store in self
//-------------------------------------------------------------------------------
IvColor& 
IvColor::operator+=( const IvColor& other )
{
    r += other.r;
    g += other.g;
    b += other.b;
    a += other.a;
	
    return *this;
	
}   // End of IvColor::operator+=()


//-------------------------------------------------------------------------------
// @ IvColor::operator-()
//-------------------------------------------------------------------------------
// Subtract vector from self and return
//-------------------------------------------------------------------------------
IvColor 
IvColor::operator-( const IvColor& other ) const
{
    return IvColor( r - other.r, g - other.g, b - other.b, a - other.a );
	
}   // End of IvColor::operator-()


//-------------------------------------------------------------------------------
// @ IvColor::operator-=()
//-------------------------------------------------------------------------------
// Subtract vector from self, store in self
//-------------------------------------------------------------------------------
IvColor& 
IvColor::operator-=( const IvColor& other )
{
    r -= other.r;
    g -= other.g;
    b -= other.b;
    a -= other.a;
	
    return *this;
	
}   // End of IvColor::operator-=()


//-------------------------------------------------------------------------------
// @ operator*()
//-------------------------------------------------------------------------------
// Scalar multiplication
//-------------------------------------------------------------------------------
IvColor   
IvColor::operator*( float scalar )
{
    return IvColor( scalar*r, scalar*g, scalar*b,
					 scalar*a );
	
}   // End of operator*()


//-------------------------------------------------------------------------------
// @ operator*()
//-------------------------------------------------------------------------------
// Scalar multiplication
//-------------------------------------------------------------------------------
IvColor   
operator*( float scalar, const IvColor& vector )
{
    return IvColor( scalar*vector.r, scalar*vector.g, scalar*vector.b,
					 scalar*vector.a );
	
}   // End of operator*()


//-------------------------------------------------------------------------------
// @ IvColor::operator*()
//-------------------------------------------------------------------------------
// Scalar multiplication bg self
//-------------------------------------------------------------------------------
IvColor&
IvColor::operator*=( float scalar )
{
    r *= scalar;
    g *= scalar;
    b *= scalar;
    a *= scalar;
	
    return *this;
	
}   // End of IvColor::operator*()


//-------------------------------------------------------------------------------
// @ operator/()
//-------------------------------------------------------------------------------
// Scalar division
//-------------------------------------------------------------------------------
IvColor   
IvColor::operator/( float scalar )
{
    return IvColor( r/scalar, g/scalar, b/scalar, a/scalar );
	
}   // End of operator*()


//-------------------------------------------------------------------------------
// @ IvColor::operator/=()
//-------------------------------------------------------------------------------
// Scalar division bg self
//-------------------------------------------------------------------------------
IvColor&
IvColor::operator/=( float scalar )
{
    r /= scalar;
    g /= scalar;
    b /= scalar;
    a /= scalar;
	
    return *this;
	
}   // End of IvColor::operator/=()

IvColor               
Lerp( float t, const IvColor& a, const IvColor& b )
{
	return ((1.0f-t)*a) + (t*b);

} 
IvColor	IvColor::Lerp(float t,const IvColor& b)
{
	return ((1.0f-t)*(*this)) + (t*b);
}


