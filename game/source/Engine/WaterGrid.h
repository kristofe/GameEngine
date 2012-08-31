#ifndef _WATERGRID_H
#define _WATERGRID_H

#include "RenderObject.h"

class WaterGrid : public RenderObject
{

public:
	WaterGrid(int nx, int ny, bool texture);
	virtual ~WaterGrid();

	virtual void update(){};
	
	virtual void adjustColorsOfVertices(float * vals);
	virtual void adjustColorsOfVertices2(float * vals);
	virtual void adjustUVs(float * vals);
	virtual void setUVs(float * uVals, float * vVals);

	//virtual void render();
	//virtual void prepareGPU();
	virtual void setupGeometry();	

	
	inline int XY(float x, float y)
	{		
		int rows = y * _NY;
		int columns = x * _NX;
		return  IX(columns, rows);
	}
	
	inline int IX(int x, int y)
	{
		return y*_NX + x;
	}
	
protected:
	int		_NX;
	int		_NY;
	int		_gridCellsCount;
	float	_maxU;
	float	_maxV;
	
	//float  _oldX[5];
	//float  _oldY[5];
	//float  _oldI[5];
	//float  _oldJ[5];
	
	bool _paused;


	//This stuff goes into the mesh and material class
	//Geometry
	float* _vertices;
	float* _colors;

	short*	_indices;
	float*	_textureCoords;
	float*	_textureCoordsBase;
	
	unsigned short	_indexCount;
	int		_vertexCount;
	int		_colorCount;
	unsigned short	_textureCoordCount;
		//Texturing stuff
	/* OpenGL name for the sprite texture */
	unsigned int _spriteTexture;
	int _touchLocations[5];
	
	bool _texturingEnabled;


	//virtual void setTexture(GLuint name, float maxU, float maxV);
	virtual void DrawAntialiasedLine (float *grid, short X0, short Y0, short X1, short Y1, short BaseColor, short NumLevels=255, unsigned short IntensityBits=8);

	virtual std::string GetTypeString()
	{
		return "WaterGrid";
	}
};

#endif