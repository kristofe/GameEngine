#include "Game.h"
#include "Mesh.h"
#include "WaveGrid.h"

WaveGrid::WaveGrid(int nx, int ny, float width, float height, std::string imagename):FluidGrid(nx,ny,width,height,imagename){
	//_waveZ = new Array(_gridCellsCount);
	//_waveZ1 = new Array(_gridCellsCount);
	//_waveDamping = new Array(_gridCellsCount);
	//_gridCells = (float*)malloc(sizeof(float)*_gridCellsCount);
	_waveZ = (float*)malloc(sizeof(float)*_gridCellsCount);
	//_waveZ1 = (float*)malloc(sizeof(float)*_gridCellsCount);
	_waveV = (float*)malloc(sizeof(float)*(_gridCellsCount+2*_NX));
	//_waveV1 = (float*)malloc(sizeof(float)*_gridCellsCount);
	_waveV1 = _waveV + 2*_NX; // WaveV1 and WaveV share the same memory space (except WaveV1 is shifted back by two rows)
	
	//_waveDamping = (float*)malloc(sizeof(float)*_gridCellsCount);
	
	_waveDT = 0.5f;
	_waveC = 1.0f; // wave speed
	_waveH = 1.0f; // grid cell width
	//_waveL = _waveH*(_gridDimension+2.0f); //width of entire grid
	_waveA = (_waveC*_waveC*_waveDT)/(_waveH*_waveH);
	_waveDiagStrength = 0.25f;
	_waveHorMultiplier = (1.0f - _waveDiagStrength) / 4.0f;
	_waveDiagMultiplier = (_waveDiagStrength) / 4.0f;
	_waveVelSmoothing = 0.1f;
	_waveVelDamping = 0.999f;
	_waveHeightDamping = 0.999f;
	//_accelerometerValues[0] = 0;
	//_accelerometerValues[1] = 0;
	//_accelerometerValues[2] = 0;
	
	initWaveData();
}

WaveGrid::~WaveGrid()
{
	if(_waveZ)
		free(_waveZ);
	//if(_waveZ1)
	//	free(_waveZ1);
	if(_waveV)
		free(_waveV);
	//if(_waveV1)
	//	free(_waveV1);
	//if(_waveDamping)
	//	free(_waveDamping);
	//if(_gridCells)
//	free(_gridCells);
}

void WaveGrid::Update()
{
	animateWaves();
	//adjustColorsOfVertices(_waveZ);
	adjustUVs(_waveZ);

	Mesh& m = mGame.GetRenderer().GetMesh(mMeshes[0]);
	m.Lock();
	mUV = m.SwapUVPointer(mUV);
	m.Unlock();
}

void WaveGrid::animateWaves()
{	
	// Enforce boundary conditions
    for( int i=0; i < _NX; i++)
	{
		_waveZ[IX(i,0)] = _waveZ[IX(i,1)];
		_waveZ[IX(i,_NY-1)] = _waveZ[IX(i,_NY-2)];
	}
	
    for( int j=0; j < _NY; j++)
	{
		_waveZ[IX(0,j)] = _waveZ[IX(1,j)];
		_waveZ[IX(_NX-1,j)] = _waveZ[IX(_NX-2,j)];
	}
	
	//int JMP = _NX*2;
	
	//edges are unchanged
	for( int j=_NY-2 ; j>=1; j-- )
	{
		for( int i=1 ; i<_NX-1; i++ )
		{
			int id = IX(i,j);
			
			//					-1,0		+1,0		0,-1		0,+1
			//					-1,-1		-1,+1		+1,-1		+1,+1
			_waveV1[id] = _waveV[id] + _waveA*(
											   _waveHorMultiplier*( _waveZ[id-1] + _waveZ[id+1] + _waveZ[id-_NX] + _waveZ[id+_NX] )
											   + _waveDiagMultiplier*( _waveZ[id-_NX-1] + _waveZ[id-_NX+1] + _waveZ[id+_NX-1] + _waveZ[id+_NX+1] )
											   - _waveZ[id]
											   );
			
			/*
			 _waveV[targetIdx] += _waveA*(
			 _waveHorMultiplier*( _waveZ[IX(i-1,j)] + _waveZ[IX(i+1,j)] + _waveZ[IX(i,j-1)] + _waveZ[IX(i,j+1)] )
			 + _waveDiagMultiplier*( _waveZ[IX(i-1,j-1)] + _waveZ[IX(i-1,j+1)] + _waveZ[IX(i+1,j-1)] + _waveZ[IX(i+1,j+1)] )
			 - _waveZ[targetIdx]
			 );
			 */
		}
	}
	
	
	for( int j = 1; j < _NY-1; j++ )
	{
		for( int i = 1; i < _NX-1; i++ )
		{
			int id = IX(i,j);
			
			//							-1,0			+1,0			0,-1				0,+1
			float neighborAvg =  (_waveV1[id-1] + _waveV1[id+1] + _waveV1[id-_NX] + _waveV1[id+_NX])*0.25f;
			_waveV[id] = LERP( _waveVelSmoothing,neighborAvg,_waveV1[id]) * _waveVelDamping;
			
			_waveZ[id] += _waveDT * _waveV[id];
			_waveZ[id] *= _waveHeightDamping; //_waveDamping[targetIdx];
			
			/*
			 int targetIdx = IX(i,j);
			 
			 _waveV[targetIdx] = LERP(_waveV1[targetIdx], (_waveV1[IX(i-1,j)] + _waveV1[IX(i+1,j)] + _waveV1[IX(i,j-1)] + _waveV1[IX(i,j+1)])*0.25f, _waveVelSmoothing) * _waveVelDamping;
			 
			 _waveZ[targetIdx] += _waveDT * _waveV[targetIdx];
			 _waveZ[targetIdx] *= _waveHeightDamping; //_waveDamping[targetIdx];
			 */
			/*
			 _waveV[targetIdx] = LERP(_waveV1[targetIdx], (_waveV1[IX(i-1,j)] + _waveV1[IX(i+1,j)] + _waveV1[IX(i,j-1)] + _waveV1[IX(i,j+1)])*0.25f, _waveVelSmoothing) * _waveVelDamping;
			 
			 _waveZ1[targetIdx] = _waveZ[targetIdx] + _waveDT * _waveV[targetIdx];
			 _waveZ1[targetIdx] *= _waveDamping[targetIdx];
			 */
			/*
			 _waveV[targetIdx] = LERP(_waveV[targetIdx], (_waveV[IX(i-1,j)] + _waveV[IX(i+1,j)] + _waveV[IX(i,j-1)] + _waveV[IX(i,j+1)])*0.25f, _waveVelSmoothing) * _waveVelDamping;
			 
			 _waveZ[targetIdx] += _waveDT * _waveV[targetIdx];
			 _waveZ[targetIdx] *= _waveDamping[targetIdx];
			 */
		}
	}
	
	//swap pointers
	/*
	 waveSwap();
	 */
	
}

void WaveGrid::initWaveData()
{
	for(int i=0; i<_gridCellsCount ; i++ )
	{	
		_waveZ[i] = 0.0f;
		//_waveZ1[i] = 0.0f;
		_waveV[i] = _waveV1[i] = 0.0f;
		//_waveDamping[i] = _waveHeightDamping;	
	}
	
	//Put a wave impulse in the middle			
	int midX = (int)(_NX*0.5f);
	int midY = (int)(_NY*0.5f);
	
	_waveZ[IX(midX,midY)] = 1.0f;
	//_waveZ[IX(midX,midY)] = _waveZ1[IX(midX,midY)] = 1.0f; 
}

void WaveGrid::MouseDown(float u, float v, int button)
{
	int idx = XY(u,v);
	_waveZ[idx] = 1.0f;
	//_waveZ[idx] = _waveZ1[idx] = 4.0f;
	//_waveV[idx] += 5.0f;
}

// TODO: Add height along entire path.
void WaveGrid::MouseDragged(float u, float v,float lu, float lv, int button)
{
	int idx = XY(u,v);
	_waveZ[idx] = 1.0f;
	//_waveZ[idx] = _waveZ1[idx] = 4.0f;
	//_waveV[idx] += 5.0f;
}

void WaveGrid::MouseUp(float u, float v, int button)
{
	int idx = XY(u,v);
	_waveZ[idx] = -1.0f;
	//_waveZ[idx] = _waveZ1[idx] = -4.0f;
	//_waveV[idx] += -5.0f;
}

