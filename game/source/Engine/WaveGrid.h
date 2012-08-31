#include "FluidGrid.h"


class WaveGrid: public FluidGrid
{
public:
	WaveGrid(int nx, int ny, float width, float height, std::string imagename);
	~WaveGrid();

	void Update();

	void MouseDown(float u, float v, int button);
	void MouseDragged(float u, float v,float lu, float lv, int button);
	void MouseUp(float u, float v, int button);
	
	virtual std::string GetTypeString()
	{
		return "FluidGrid";
	}
	
private:
	void animateWaves();
	void initWaveData();
	
private:
	
	//Wave Sim Data

	float	_waveC;
	float	_waveH;
	float	_waveL;
	float	_waveD;
	float	_waveA;
	float	_waveB;
	float	_waveDT;
	float   _waveDiagStrength;
	float   _waveHorMultiplier;
	float   _waveDiagMultiplier;
	float   _waveVelSmoothing;
	float   _waveVelDamping;
	float	_waveHeightDamping;

	//Arrays
	float*	_waveZ;
	//float*	_waveZ1;
	float*  _waveV;
	float*  _waveV1;
	//float*	_waveDamping;




	//INLINED FUNCTIONS
	/*
	 void waveSwap()
	 {
	 float* tempArray = _waveZ1;
	 _waveZ1 = _waveZ;
	 _waveZ = tempArray;			
	 }
	 */
};