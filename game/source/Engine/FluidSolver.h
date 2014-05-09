#pragma once

/*
 *  FluidSolver.h
 *  XPlatformGL
 *
 *  Created by Kristofer Schlachter on 1/10/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#define COLOR_TYPE  GL_FLOAT // GL_UNSIGNED_BYTE //works too
#define SMOOTH_GRID 0 // Whether grid quads are broken up into two quad each
//#define CLAMP(v,min,max) ((v) < (min) ? (min) : ((v) > (max) ? (max) : (v)))
//#define LERP(a,b,c) ((a)+((c)*((b)-(a))))
//#define min(a,b) ((a) < (b) ? (a) : (b))

#include "Globals.h"

#ifdef TARGETIPHONE
#pragma arm
#import <OpenGLES/ES1/gl.h>
#elif WIN32
#include <windows.h>
#include <GL/gl.h>

#else
#import <OpenGL/gl.h>
#import <OpenGL/OpenGL.h>
#endif

//template<class T> T min(T a, T b) { return a < b ? a : b; }

class FluidSolver
{

public:
	FluidSolver(int nx, int ny, bool texture = false);
	virtual ~FluidSolver();

	virtual void update(){};
	
	virtual void mouseDown(float u, float v, int button) = 0;
	virtual void mouseDragged(float u, float v,float lu, float lv, int button)=0;
	virtual void mouseUp(float u, float v, int button)=0;
	
	virtual void adjustColorsOfVertices(float * vals);
	virtual void adjustColorsOfVertices2(float * vals);
	virtual void adjustUVs(float * vals);
	virtual void setUVs(float * uVals, float * vVals);

	virtual void render();
	virtual void prepareGPU();
	virtual void setupGeometry();	
	
	virtual void setTexture(GLuint name, float maxU, float maxV);
	
	virtual void updateAccelerometer(float u, float v, float w){
		_accelerometerValues[0] = u;
		_accelerometerValues[1] = v;
		_accelerometerValues[2] = w;
		
	};
	
	
	virtual void DrawAntialiasedLine (float *grid, short X0, short Y0, short X1, short Y1, short BaseColor, short NumLevels=255, unsigned short IntensityBits=8);

	
	int XY(float x, float y)
	{		
		int rows = y * _NY;
		int columns = x * _NX;
		return  IX(columns, rows);
	}
	
	int IX(int x, int y)
	{
		x = CLAMP(x,0,_NX);
		y = CLAMP(y,0,_NY);
		return y*_NX + x;
	}
	
	
	virtual std::string GetTypeString()
	{
		return "FluidSolver";
	}
protected:
	int		_NX;
	int		_NY;
	int		_gridCellsCount;
	float	_maxU;
	float	_maxV;
	
	float _accelerometerValues[3];
	float  _oldX[5];
	float  _oldY[5];
	float  _oldI[5];
	float  _oldJ[5];
	
	//Geometry
	float*	_vertices;
	#if(COLOR_TYPE == GL_UNSIGNED_BYTE)
		unsigned char*	_colors;
	#elif(COLOR_TYPE == GL_FLOAT)
		float* _colors;
	#endif
	short*	_indices;
	float*	_textureCoords;
	float*	_textureCoordsBase;
	
	unsigned short	_indexCount;
	int		_vertexCount;
	int		_colorCount;
	unsigned short	_textureCoordCount;
	
	bool _texturingEnabled;
	bool _paused;
	//Texturing stuff
	/* OpenGL name for the sprite texture */
	unsigned int _spriteTexture;
	int _touchLocations[5];

};