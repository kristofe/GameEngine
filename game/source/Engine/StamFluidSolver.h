#ifndef _STAMFLUIDSOLVER_H_
#define _STAMFLUIDSOLVER_H_

#define VORTICITY_CONFINEMENT 0

#include "FluidGrid.h"
class StamFluidSolver : public FluidGrid
{
		float dt, diff, visc;
		float force, source, tempIncrement;
		int dvel;
		
		float * u, * v, * u_prev, * v_prev;
		#if (VORTICITY_CONFINEMENT == 1)
			float * curl;
		#endif
		float * dens, * dens_prev;
		float * texU, * texV, *texU_prev, * texV_prev;
		//float * temp, * temp_prev;
		//bool *obs;
		
		int omx, omy, mx, my;
		float display_scale;
		bool leftMouseButtonDown;
		bool rightMouseButtonDown;
		bool drawDensities;
		bool drawVelocities;
		bool drawObstacles;
		bool drawTexture;
		bool drawTemperature;
		bool blend;
		bool addVelocities;
		bool addSources;
		bool addObstacles;
		bool addTemperature;
		bool mTextureMorph;
		bool mSimulateTemperature;
		bool mUseVorticityConfinement;
		bool mAdjustFrameRate;
		int  mTargetFrameRate;
		float mDesiredDT;
		
		float mDesiredUpdateRate;
		float mTemperatureBlend;
		float lastUpdateTime;
		float lastUpdateTimeForFrameRate;;
		float updateRate;
		float mFPSCheckInterval;
		int mGoodFPSChecks;
		int mNumFPSChecksRequired;
		
	public:
		
		StamFluidSolver(int nx, int ny, float width, float height, std::string imageMapName);
		virtual ~StamFluidSolver();
		
		//Inherited functions:
		void Update();

		void MouseDown(float u, float v, int button);
		void MouseDragged(float u, float v, float lu, float lv, int button);
		void MouseUp(float u, float v, int button);

				
		//Legacy
		bool reset();
		bool onLeftMouseButtonDown() { leftMouseButtonDown = true; return true;}
		bool onRightMouseButtonDown() { rightMouseButtonDown = true; return true;}
		bool onLeftMouseButtonUp() { leftMouseButtonDown = false; return true;}
		bool onRightMouseButtonUp() { rightMouseButtonDown = false; return true;}
		
		bool setDrawVelocities(bool val) { drawVelocities = val; return true;}
		bool getDrawVelocities() { return drawVelocities;}
		bool setDrawDensities(bool val) { drawDensities = val; return true;}
		bool getDrawDensities() { return drawDensities;}
		bool setDrawBlending(bool val) { blend = val; return true;}
		bool getDrawBlending() { return blend;}
		bool setDrawObstacles(bool val) { drawObstacles = val; return true;}
		bool getDrawObstacles() { return drawObstacles;}
		bool setDrawTemperature(bool val) { drawTemperature = val; return true;}
		bool getDrawTemperature() { return drawTemperature;}
		bool setAddVelocities(bool val) { addVelocities = val; return true;}
		bool getAddVelocities() { return addVelocities;}
		bool setAddSources(bool val) { addSources = val; return true;}
		bool getAddSources() { return addSources;}
		bool setAddObstacles(bool val) { addObstacles = val; return true;}
		bool getAddObstacles() { return addObstacles;}
		bool setAddTemperature(bool val) { addTemperature = val; return true;}
		bool getAddTemperature() { return addTemperature;}
		bool setTextureMorph(bool val) { mTextureMorph = val; return true;}
		bool getTextureMorph() { return mTextureMorph;}
		bool setSimulateTemperature(bool val) { mSimulateTemperature = val; return true;}
		bool getSimulateTemperature() { return mSimulateTemperature;}
		float  getTemperatureBlend() {return mTemperatureBlend;}
		bool setTemperatureBlend(float blend) {mTemperatureBlend = blend; return true;}
		bool getUseVorticityConfinement(){ return mUseVorticityConfinement;}
		bool setUseVorticityConfinement(bool val) { mUseVorticityConfinement = val; return true;}
		
		//bool setSimulationResolution(int n);
		//int  getSimulationResolution(){ return N;}
		bool setSimDT(float newDT);
		float getSimDT() {return dt;}
		
		float  getUpdateRate() {return updateRate;}
		bool setUpdateRate(float val) {updateRate =  mDesiredUpdateRate = val; mAdjustFrameRate = true; return true;}
		
		
		bool setDiffusionRate(float newDT);
		float getDiffusionRate(){return diff;}
		
		bool setViscosity(float newDT);
		float getViscosity(){return visc;}
		
		
		//virtual bool onAdd();
		//virtual void onRemove();
		//virtual bool loadStream( FileStream& fileStream, fxSceneGraph2D* pSceneGraph2D, Vector<fxSceneObject2D*>& ObjReferenceList, bool ignoreLayerOrder );
		//virtual bool saveStream( FileStream& fileStream, U32 serialiseID, U32 serialiseKey );
		//virtual void renderObject(  );
		//virtual void integrateObject(float sceneTime, float elapsedTime);
		
		//void adjustFrameRate(float sceneTime, float elapsedTime, CDebugStats* pStats);
		//Solver Routines
		void addSource ( float * x, float * s, float dt );
		void setBound ( int b, float * x );
		//void inline internalBound( int b,float * x );
		void linearSolve ( int b, float * x, float * x0, float a, float c );
		void diffuse ( int b, float * x, float * x0, float diff, float dt );
		void advect ( int b, float * d, float * d0, float * u, float * v, float dt );
		void project ( float * u, float * v, float * p, float * div );
		void dens_step ( float * x, float * x0, float * u, float * v, float diff, float dt );
		void vel_step ( float * u, float * v, float * u0, float * v0, float visc, float dt );
		void texture_step ( float * x, float * x0, float * u, float * v, float diff, float dt );
		//Display Interaction
		void cleanupMemory();
		void clear_data();
		int	 allocate_data();
		void draw_velocity();
		void draw_density();
		void draw_temperature();
		void draw_obstacles();
		void get_from_UI( float * d, float * u, float * v );
		void updatePhysics();
		#if (VORTICITY_CONFINEMENT == 1)
		void vorticityConfinement(float* Fvc_x, float* Fvc_y);
		#endif
		inline float getCurl(int i, int j);
		//void heatRise(float* Fbuoy);
		

		/*
		//Texture Map Stuff
		bool setImageMap( const char* imageMapName, U32 frame );
		bool setFrame( U32 frame );
		bool setHeatMap( const char* imageMapName, U32 frame );
		bool setFrameHeatMap( U32 frame );
		
		const char* getImageMapName( void )	{ if (mImageMapDataBlock) return mImageMapDataBlock->getName(); else return NULL; };
		U32 getFrame( void )				{ return mFrame; };
		const char* getHeatMapName( void )	{ if (mHeatMapDataBlock) return mHeatMapDataBlock->getName(); else return NULL; };
		U32 getFrameHeatMap( void )			{ return mFrameHeatMap; };	
		 */
		void drawPerturbedTexture();
		void drawTemperatureTextured();

		virtual std::string GetTypeString()
		{
			return "StamFluidSolver";
		}
	
};

#endif // _STAMFLUIDSOLVER_H_
