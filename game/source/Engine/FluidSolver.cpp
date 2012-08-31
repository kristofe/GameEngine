/*
 *  FluidSolver.cpp
 *  XPlatformGL
 *
 *  Created by Kristofer Schlachter on 1/10/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#ifdef TARGET_OS_IPHONE
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import "Platform.h"
#import <QuartzCore/QuartzCore.h>
#elif WIN32
#include <windows.h>
#include <GL/gl.h>

#else
#import <OpenGL/gl.h>
#import <OpenGL/OpenGL.h>
#import "Platform.h"
//#import <QuartzCore/QuartzCore.h>
#endif

#include "FluidSolver.h"
#include <stdlib.h>
//#include <stdio.h>
#include <math.h>

FluidSolver::FluidSolver(int nx, int ny, bool texture){
	_NX = nx;
	_NY = ny;
	
	_maxU = 1.0f;
	_maxV = 1.0f;
	//_gridDimension = 64;
	//_triangles = new Array();
	//_vertices = new Array();
	_gridCellsCount = _NX*_NY;
	//_gridCells = new Array(_gridCellsCount);
	_texturingEnabled = texture;//texture;
	for(int i = 0; i < 5; i++){
		_touchLocations[i] = -1;
	}
	setupGeometry();
	_paused = true;
}

FluidSolver::~FluidSolver()
{
	if(_indices)
		free(_indices);
	if(_vertices)
		free(_vertices);
	if(_colors)
		free(_colors);
	if(_textureCoords)
		free(_textureCoords);
	if(_textureCoordsBase)
		free(_textureCoordsBase);
}


void FluidSolver::setTexture(GLuint name, float maxU, float maxV)
{
	if(_spriteTexture == name){
		return;
	}
	_spriteTexture = name;
	_maxU = maxU;
	_maxV = maxV;
	//setupGeometry();
	prepareGPU();
}

void FluidSolver::prepareGPU()
{	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, COLOR_TYPE,0, _colors);
	glVertexPointer(3, GL_FLOAT,0, _vertices);	

	
	if(_texturingEnabled){
		glTexCoordPointer(2, GL_FLOAT, 0, _textureCoords);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		
		// Enable use of the texture
		glEnable(GL_TEXTURE_2D);
		
		
		glBindTexture(GL_TEXTURE_2D, _spriteTexture);
		
		// Set a blending function to use
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glBlendFunc(GL_ONE, GL_ZERO);
		// Enable blending
		glEnable(GL_BLEND);	
		//glDisable(GL_BLEND);
		
		

//		int imgWidth,imgHeight;
//		GetTextureDataGL("tropical_island.png",&spriteData, &imgWidth, &imgHeight);
//
//
//		if(spriteData) {
//
//			
//			// Use OpenGL ES to generate a name for the texture.
//			glGenTextures(1, &_spriteTexture);
//			// Bind the texture name. 
//			glBindTexture(GL_TEXTURE_2D, _spriteTexture);
//			// Speidfy a 2D texture image, provideing the a pointer to the image data in memory
//			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, spriteData);
//			// Set the texture parameters to use a minifying filter and a linear filer (weighted average)
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//			
//			// Enable use of the texture
//			glEnable(GL_TEXTURE_2D);
//			// Set a blending function to use
//			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
//			//glBlendFunc(GL_ZERO, GL_ONE);
//			// Enable blending
//			glEnable(GL_BLEND);
//		}
	}
}

void FluidSolver::setupGeometry()
{
	_indexCount = ((_NX-1)*(_NY-1))*3*2;
	_vertexCount = _gridCellsCount*3;
	_colorCount = _gridCellsCount*4;
	_textureCoordCount = _gridCellsCount*2;
	
	#if(SMOOTH_GRID == 1)
		_indexCount *= 2; // We are going to have twice as much indecies (four tris per quad instead of two)
		_vertexCount += ((_NX-1)*(_NY-1))*3;
		_colorCount += ((_NX-1)*(_NY-1))*4;
	#endif

	_indices = (short*)malloc(sizeof(short)* _indexCount);
	_vertices = (float*)malloc(sizeof(float)*_vertexCount);
	_textureCoords = (float*)malloc(sizeof(float)*_textureCoordCount);
	_textureCoordsBase = (float*)malloc(sizeof(float)*_textureCoordCount);
	
	//_spriteTexture = GetTexture("Background.png",_maxU, _maxV);
	
	#if(COLOR_TYPE == GL_UNSIGNED_BYTE)
		_colors = (unsigned char*)malloc(sizeof(unsigned char)*_colorCount);
	#elif(COLOR_TYPE == GL_FLOAT)
		_colors = (float*)malloc(sizeof(float)*_colorCount);
	#endif
	
	float dx = 1.0/(_NX-1);///(_width/_gridDimension);
	float dy = 1.0/(_NY-1);//(_height/_gridDimension);

	float currX,currY;
	currX = currY = 0.0f; // TODO: These vars are not being used
	int id = 0;
	int id2 = 0;
	
	for(int y = 0; y < _NY; y++)
	{
		for(int x = 0; x < _NX; x++)
		{			
			_vertices[id++] = currX;
			_vertices[id++] = currY;
			_vertices[id++] = 0.0f;
			
			_textureCoords[id2] = currX*_maxU;
			_textureCoordsBase[id2++] = currX*_maxU;
			
			_textureCoords[id2] = (1.0 - currY)*_maxV;
			_textureCoordsBase[id2++] = (1.0 - currY)*_maxV;
			
			//printf("x: %1.4f  y:%1.4f\n",currX,currY);
			currX = currX + dx;				
		}
		currY = currY + dy;
		currX = 0.0f;
	}

	#if (SMOOTH_GRID == 1)	
		int centersOffset = _NX*_NY;
		currX = dx * 0.5f;
		currY = dy * 0.5f;
		
		for(int y = 0; y < _NY-1; y++)
		{
			for(int x = 0; x < _NX-1; x++)
			{
				_vertices[id++] = currX;
				_vertices[id++] = currY;
				_vertices[id++] = 0.0f;
				// NOTE: cool wave effect when currX and currY both set to 0
				
				//printf("x: %1.4f  y:%1.4f\n",tempX,tempY);
				currX = currX + dx;
			}
			currY = currY + dy;
			currX = dx * 0.5f;
		}
	#endif
	
	id = 0;
	for(int y = 0; y < _NY-1; y++)
	{
		for(int x = 0; x < _NX-1; x++)
		{
			short i0 = IX(x  , y  );
			short i1 = IX(x  , y+1);
			short i2 = IX(x+1, y+1);
			short i3 = IX(x+1, y  );
						
			#if (SMOOTH_GRID == 0)
				//0 3
				//1 2

				_indices[id++] = i0;
				_indices[id++] = i3;
				_indices[id++] = i1;
				
				_indices[id++] = i3;
				_indices[id++] = i2;
				_indices[id++] = i1;
			#else
				// 0   3
				//   c
				// 1   2
				short ic = y*(_NX-1)+x+centersOffset;
				_indices[id++] = i0;
				_indices[id++] = i3;
				_indices[id++] = ic;

				_indices[id++] = i3;
				_indices[id++] = i2;
				_indices[id++] = ic;
				
				_indices[id++] = i2;
				_indices[id++] = i1;
				_indices[id++] = ic;
				
				_indices[id++] = i1;
				_indices[id++] = i0;
				_indices[id++] = ic;
			#endif
		}
	}
	
	for(int i = 0; i < _colorCount; i++)
	{
		#if(COLOR_TYPE == GL_UNSIGNED_BYTE)
			_colors[i++] = 255;
			_colors[i++] = 255;
			_colors[i++] = 255;
			_colors[i] = 255;
		#elif(COLOR_TYPE == GL_FLOAT)
			_colors[i++] = 1.0f;
			_colors[i++] = 1.0f;
			_colors[i++] = 1.0f;
			_colors[i] = 1.0f;		
		#endif
	}
	 
}


void FluidSolver::render()
{
	//glColor4f(0.0f,0.0f,1.0f,1.0f);
	glDrawElements(GL_TRIANGLES,_indexCount,GL_UNSIGNED_SHORT,_indices);
	//glColor4f(0.0f,0.0f,1.0f,1.0f);
	//glDrawArrays(GL_TRIANGLES, 0, _vertexCount);
	//glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,indices);
}
//-----------------------------------------------------------------------------


void FluidSolver::adjustUVs(float * vals)
{
	for( int y=0 ; y < _NY; y++ )
	{
		for( int x=0 ; x < _NX; x++ )
		{
			int idx = IX(x,y);
			float w = vals[idx];//Assumes that color is between -1 and 1;
			_textureCoords[(idx * 2)] = _textureCoordsBase[(idx * 2)] + w*0.08;
			_textureCoords[(idx * 2)+1] = _textureCoordsBase[(idx * 2)+1] + w*0.08;
		}
	}
}

void FluidSolver::setUVs(float * uVals, float * vVals)
{
	int i = 0;
	for(i = 0; i < _gridCellsCount-1; i++){
		float t, u;
		t = _textureCoords[i*2];
		u = uVals[i];
		_textureCoords[i*2] = uVals[i]*_maxU;
	}
	i = 0;
	for(i = 0; i < _gridCellsCount-1; i++){
		_textureCoords[i*2 + 1] = vVals[i]*_maxV;
	}
}


void FluidSolver::adjustColorsOfVertices(float * vals)
{
	for( int y=0 ; y < _NY; y++ )
	{
		for( int x=0 ; x < _NX; x++ )
		{
			int idx = IX(x,y);
			float color = vals[idx];//Assumes that color is between -1 and 1;
			#if(COLOR_TYPE == GL_UNSIGNED_BYTE)
				int ccolor = int(color * 127.0f) + 127;
				_colors[(idx * 4)+2] = CLAMP(ccolor,0,255); //just adjust the blue component
			#elif(COLOR_TYPE == GL_FLOAT)
				_colors[(idx * 4)+2] = (color*0.5f) + 0.5f; //just adjust the blue component
			#endif
		}
	}
	
	#if (SMOOTH_GRID == 1)
		int centersOffset = _NX*_NY;
		for( int y=0 ; y < _NY-1; y++ )
		{
			for( int x=0 ; x < _NX-1; x++ )
			{
				int idx = IX(x,y);
				float color = (vals[idx] + vals[idx+1] + vals[idx+_NX] + vals[idx+_NX+1])*0.25f;//Assumes that color is between -1 and 1;
				int destIdx = y*(_NX-1) + x + centersOffset;
				#if(COLOR_TYPE == GL_UNSIGNED_BYTE)
					int ccolor = int(color * 127.0f) + 127;
					_colors[(destIdx * 4)+2] = CLAMP(ccolor,0,255); //just adjust the blue component
				#elif(COLOR_TYPE == GL_FLOAT)
					_colors[(destIdx * 4)+2] = (color*0.5f) + 0.5f; //just adjust the blue component
				#endif
			}
		}
	#endif
}

void FluidSolver::adjustColorsOfVertices2(float * vals)
{
	for( int y=0 ; y < _NY; y++ )
	{
		for( int x=0 ; x < _NX; x++ )
		{
			int idx = IX(x,y);
			float color = vals[idx];//Assumes that color is between 0 and 1;
#if(COLOR_TYPE == GL_UNSIGNED_BYTE)
			int ccolor = int(color * 256.0f);
			_colors[(idx * 4)] = CLAMP(ccolor,0,255); 
			_colors[(idx * 4) + 1] = CLAMP(ccolor,0,255); 
			_colors[(idx * 4) + 2] = CLAMP(ccolor,0,255); 
#elif(COLOR_TYPE == GL_FLOAT)
			_colors[(idx * 4)] = color; 
			_colors[(idx * 4)+1] = color; 
			_colors[(idx * 4)+2] = color; 
#endif
		}
	}
	
#if (SMOOTH_GRID == 1)
	int centersOffset = _NX*_NY;
	for( int y=0 ; y < _NY-1; y++ )
	{
		for( int x=0 ; x < _NX-1; x++ )
		{
			int idx = IX(x,y);
			float color = (vals[idx] + vals[idx+1] + vals[idx+_NX] + vals[idx+_NX+1])*0.25f;//Assumes that color is between -1 and 1;
			int destIdx = y*(_NX-1) + x + centersOffset;
#if(COLOR_TYPE == GL_UNSIGNED_BYTE)
			int ccolor = int(color * 256.0f);
			_colors[(destIdx * 4)] = CLAMP(ccolor,0,255); //just adjust the blue component
#elif(COLOR_TYPE == GL_FLOAT)
			_colors[(destIdx * 4)] = color; //just adjust the blue component
#endif
		}
	}
#endif
}

//-----------------------------------------------------------------------------
void FluidSolver::DrawAntialiasedLine (float *grid, short X0, short Y0, short X1, short Y1, short BaseColor, short NumLevels, unsigned short IntensityBits)
{
	unsigned short IntensityShift, ErrorAdj, ErrorAcc;
	unsigned short ErrorAccTemp, Weighting, WeightingComplementMask;
	short DeltaX, DeltaY, Temp, XDir;
	float toFloat = 1.0f/255.0f;
	
	/* Make sure the line runs top to bottom */
	if (Y0 > Y1) {
		Temp = Y0; Y0 = Y1; Y1 = Temp;
		Temp = X0; X0 = X1; X1 = Temp;
	}
	/* Draw the initial pixel, which is always exactly intersected by
	 the line and so needs no weighting */
	grid[IX(X0, Y0)] = BaseColor*toFloat;
	
	if ((DeltaX = X1 - X0) >= 0) {
		XDir = 1;
	} else {
		XDir = -1;
		DeltaX = -DeltaX; /* make DeltaX positive */
	}
	/* Special-case horizontal, vertical, and diagonal lines, which
	 require no weighting because they go right through the center of
	 every pixel */
	if ((DeltaY = Y1 - Y0) == 0) {
		/* Horizontal line */
		while (DeltaX-- != 0) {
			X0 += XDir;
			grid[IX(X0, Y0)] = BaseColor*toFloat;
		}
		return;
	}
	if (DeltaX == 0) {
		/* Vertical line */
		do {
			Y0++;
			grid[IX(X0, Y0)] = BaseColor*toFloat;
		} while (--DeltaY != 0);
		return;
	}
	if (DeltaX == DeltaY) {
		/* Diagonal line */
		do {
			X0 += XDir;
			Y0++;
			grid[IX(X0, Y0)] = BaseColor*toFloat;
		} while (--DeltaY != 0);
		return;
	}
	/* Line is not horizontal, diagonal, or vertical */
	ErrorAcc = 0;  /* initialize the line error accumulator to 0 */
	/* # of bits by which to shift ErrorAcc to get intensity level */
	IntensityShift = 16 - IntensityBits;
	/* Mask used to flip all bits in an intensity weighting, producing the
	 result (1 - intensity weighting) */
	WeightingComplementMask = NumLevels - 1;
	/* Is this an X-major or Y-major line? */
	if (DeltaY > DeltaX) {
		/* Y-major line; calculate 16-bit fixed-point fractional part of a
         pixel that X advances each time Y advances 1 pixel, truncating the
         result so that we won't overrun the endpoint along the X axis */
		ErrorAdj = ((unsigned long) DeltaX << 16) / (unsigned long) DeltaY;
		/* Draw all pixels other than the first and last */
		while (--DeltaY) {
			ErrorAccTemp = ErrorAcc;   /* remember currrent accumulated error */
			ErrorAcc += ErrorAdj;      /* calculate error for next pixel */
			if (ErrorAcc <= ErrorAccTemp) {
				/* The error accumulator turned over, so advance the X coord */
				X0 += XDir;
			}
			Y0++; /* Y-major, so always advance Y */
			/* The IntensityBits most significant bits of ErrorAcc give us the
			 intensity weighting for this pixel, and the complement of the
			 weighting for the paired pixel */
			Weighting = ErrorAcc >> IntensityShift;
			grid[IX(X0, Y0)] = (BaseColor + Weighting)*toFloat;		
			grid[IX(X0 + XDir, Y0)] = (BaseColor + (Weighting ^ WeightingComplementMask))*toFloat;
		}
		/* Draw the final pixel, which is always exactly intersected by the line
         and so needs no weighting */
		grid[IX(X1, Y1)] =  BaseColor * toFloat;
		return;
	}
	/* It's an X-major line; calculate 16-bit fixed-point fractional part of a
	 pixel that Y advances each time X advances 1 pixel, truncating the
	 result to avoid overrunning the endpoint along the X axis */
	ErrorAdj = ((unsigned long) DeltaY << 16) / (unsigned long) DeltaX;
	/* Draw all pixels other than the first and last */
	while (--DeltaX) {
		ErrorAccTemp = ErrorAcc;   /* remember currrent accumulated error */
		ErrorAcc += ErrorAdj;      /* calculate error for next pixel */
		if (ErrorAcc <= ErrorAccTemp) {
			/* The error accumulator turned over, so advance the Y coord */
			Y0++;
		}
		X0 += XDir; /* X-major, so always advance X */
		/* The IntensityBits most significant bits of ErrorAcc give us the
         intensity weighting for this pixel, and the complement of the
         weighting for the paired pixel */
		Weighting = ErrorAcc >> IntensityShift;
		grid[IX(X0, Y0)] =  (BaseColor + Weighting)*toFloat;
		grid[IX(X0, Y0 + 1)] = (BaseColor + (Weighting ^ WeightingComplementMask))*toFloat;
	}
	/* Draw the final pixel, which is always exactly intersected by the line
	 and so needs no weighting */
	grid[IX(X1, Y1)] =  BaseColor*toFloat;
}

//-----------------------------------------------------------------------------