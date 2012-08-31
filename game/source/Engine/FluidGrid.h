#ifndef _FLUID_GRID_H
#define _FLUID_GRID_H

#include "RenderObject.h"

class FluidGrid : public RenderObject
{

public:
	FluidGrid(int nx, int ny, float width, float height, std::string imagename);
	FluidGrid(PersistentData& storageData);
	virtual ~FluidGrid();

	virtual void adjustUVs(float * vals);
	virtual void setUVs(float * uVals, float * vVals);
	virtual void setUVs(float * vals);

	virtual void adjustColorsOfVertices(IvColor * vals);
	virtual void adjustColorsOfVertices2(float* c);
	
	void Serialize(PersistentData& storageData);
	void DeSerialize(PersistentData& storageData);
	
	virtual std::string GetTypeString()
	{
		return "FluidGrid";
	}
	/*virtual void update(){};
	
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
	*/
	//virtual void setTexture(GLuint name, float maxU, float maxV);
	
	//virtual void updateAccelerometer(float u, float v, float w){
	//	_accelerometerValues[0] = u;
	//	_accelerometerValues[1] = v;
	//	_accelerometerValues[2] = w;
	//	
	//};
	
	
	//virtual void DrawAntialiasedLine (float *grid, short X0, short Y0, short X1, short Y1, short BaseColor, short NumLevels=255, unsigned short IntensityBits=8);

	
	inline int XY(float x, float y)
	{		
		int rows = (int)(y * _NY);
		int columns = (int)(x * _NX);
		return  IX(columns, rows);
	}
	
	inline int IX(int x, int y)
	{
		//x = CLAMP(x,0,_NX);
		//y = CLAMP(y,0,_NY);
		return y*_NX + x;
	}
	

protected:
	void Init(int nx, int ny, float width, float height, std::string imagename);
	
private:
	//Remove Copy Constructor and assignment operator
	FluidGrid		&operator =  (const FluidGrid &m) { printf("FluidGrid = THIS ASSIGNMENT OPERATOR SHOULDN'T BE GETTING CALLED"); return *this;};
	FluidGrid(const FluidGrid &m){printf("FluidGrid COPY CTOR SHOULDN'T BE GETTING CALLED");};;

	
protected:
	int		_NX;
	int		_NY;
	int		_gridCellsCount;
	float	_gridWidth;
	float	_gridHeight;
	//float	_maxU;
	//float	_maxV;
	
	//float _accelerometerValues[3];
	//float  _oldX[5];
	//float  _oldY[5];
	//float  _oldI[5];
	//float  _oldJ[5];
	
	//new Geometry
	IvVector3*			mVertices;
	IvColor*			mColors;
	IvVector2*			mUV;
	IvVector2*			mUV2;

	/*
	//OLD Geometry
	float*	_vertices;
	float* _colors;
	short*	_indices;
	float*	_textureCoords;
	float*	_textureCoordsBase;
	
	unsigned short	_indexCount;
	int		_vertexCount;
	int		_colorCount;
	unsigned short	_textureCoordCount;
	//Texturing stuff
	bool _texturingEnabled;
		
	//OpenGL name for the sprite texture
	unsigned int _spriteTexture;
	*/
	bool _paused;

	int _touchLocations[5];

};

#endif //_FLUID_GRID_H