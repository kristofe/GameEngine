//===============================================================================
// @ IvColor.cpp
// 
// Color class
// ------------------------------------------------------------------------------
//
//===============================================================================

#ifndef __IvColor__h__
#define __IvColor__h__

//-------------------------------------------------------------------------------
//-- Dependencies ---------------------------------------------------------------
//-------------------------------------------------------------------------------

#include "IvWriter.h"

//-------------------------------------------------------------------------------
//-- Typedefs, Structs ----------------------------------------------------------
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//-- Classes --------------------------------------------------------------------
//-------------------------------------------------------------------------------

class IvColor
	{
	public:
		// constructor/destructor
		inline IvColor() {r=g=b=a=1.0f;}
		inline IvColor( float _r, float _g, float _b, float _a ) :
        r(_r), g(_g), b(_b), a(_a)
		{
		}
		inline ~IvColor() {}
		
		// copy operations
		IvColor(const IvColor& other);
		IvColor& operator=(const IvColor& other);
		
		// text output (for debugging)
		friend IvWriter& operator<<(IvWriter& out, const IvColor& source);
		
		// accessors
		inline float& operator[]( unsigned int i )         { return (&r)[i]; }
		inline float operator[]( unsigned int i ) const    { return (&r)[i]; }

		
		// comparison
		bool operator==( const IvColor& other ) const;
		bool operator!=( const IvColor& other ) const;

		
		// manipulators
		inline void Set( float _r, float _g, float _b, float _a );
		
		// operators
		
		// addition/subtraction
		IvColor operator+( const IvColor& other ) const;
		IvColor& operator+=( const IvColor& other );
		IvColor operator-( const IvColor& other ) const;
		IvColor& operator-=( const IvColor& other );
		
		// scalar multiplication
		IvColor    operator*( float scalar );
		friend IvColor    operator*( float scalar, const IvColor& vector );
		IvColor&          operator*=( float scalar );
		IvColor    operator/( float scalar );
		IvColor&          operator/=( float scalar );
		 
		IvColor				Lerp(float t, const IvColor& b);
		friend IvColor		Lerp(float t, const IvColor& a, const IvColor& b);

		// useful defaults
		static IvColor    red;
		static IvColor    green;
		static IvColor    blue;
		static IvColor    black;
		static IvColor    white;
        
		// member variables
		float r, g, b, a;
		
	protected:
		
	private:
	};

//-------------------------------------------------------------------------------
//-- Inlines --------------------------------------------------------------------
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// @ IvColor::Set()
//-------------------------------------------------------------------------------
// Set vector elements
//-------------------------------------------------------------------------------
inline void 
IvColor::Set( float _r, float _g, float _b, float _a )
{
    r = _r; g = _g; b = _b; a = _a;
}   // End of IvColor::Set()


#endif
