
#include <math.h>
#include <stdio.h>
#include "Globals.h"
#include "Game.h"
#include "Mesh.h"
#include "StamFluidSolver.h"


#define SWAP(x0,x) {float * tmp=x0;x0=x;x=tmp;}
#define kds_min(a,b)            (((a) < (b)) ? (a) : (b))
//-----------------------------------------------------------------------------
StamFluidSolver::StamFluidSolver(int nx, int ny, float width, float height, std::string imagename):FluidGrid(nx,ny,width,height,imagename){
	dt = mDesiredDT = 0.1f;
	//diff = 0.0001f;
	diff = 0.0f;
	visc = 0.001f;
	force = 100.0f;
	source = 1.0f;
	tempIncrement = 1.0f;
	
	dvel = 0;
	omx = mx = omy = my = 0;
	leftMouseButtonDown = false;
	rightMouseButtonDown = false;
	addVelocities = true;
	addSources = false;
	addObstacles = false;
	drawObstacles = false;
	drawVelocities = false;
	drawTemperature = false;
	mTextureMorph = true;
	mSimulateTemperature = false;
	addTemperature = false;
	mUseVorticityConfinement = true;
	mTemperatureBlend = 0.8f;
	display_scale = 100;
	lastUpdateTime = lastUpdateTimeForFrameRate = (float)-1.0e32;
	updateRate = mDesiredUpdateRate = 0.06f;
	mFPSCheckInterval = 0.5f;
	mAdjustFrameRate = true;
	mTargetFrameRate = 30;
	mGoodFPSChecks = 0;
	mNumFPSChecksRequired = 9;	

	
	allocate_data();
	clear_data();
	
}

//-----------------------------------------------------------------------------
StamFluidSolver::~StamFluidSolver()
{
	cleanupMemory();
}

//-----------------------------------------------------------------------------
void StamFluidSolver::MouseDown(float _u, float _v, int button)
{
	if(button > 4 || button < 0) return;
	int idx = XY(_u,_v);
	if(idx >= _gridCellsCount || idx < 0){
		printf("idx out of bounds:%d\n",idx);
		return;
	}
	_touchLocations[button] = idx;
	//dens[idx] = 1.0f;
}
//-----------------------------------------------------------------------------
void StamFluidSolver::MouseDragged(float _u, float _v,float _lu, float _lv, int button)
{
	if(button > 4 || button < 0) return;
	int idx = XY(_u,_v);
	if(idx >= _gridCellsCount || idx < 0){
		printf("idx out of bounds:%d\n",idx);
		return;
	}
	_touchLocations[button] = idx;

	//int i = (int)(_u * _NX);
	//int j = (int)(_v * _NY);
	
	//int li = (int)(_lu * _NX);
	//int lj = (int)(_lv * _NY);
	
	
	u[idx] = force * (_u-_lu);
	v[idx] = force * (_v-_lv);
	
	
	//DrawAntialiasedLine(dens, i, j, li, lj, 255);
	
}
//-----------------------------------------------------------------------------
void StamFluidSolver::MouseUp(float _u, float _v, int button)
{
	if(button > 4 || button < 0) return;
	for(int i = 4; i >= button; i--)
		_touchLocations[i] = -1;
}
//-----------------------------------------------------------------------------
void StamFluidSolver::Update()
{
	
	updatePhysics();
	Mesh& m = mGame.GetRenderer().GetMesh(mMeshes[0]);
	m.Lock();
	
	if(mTextureMorph){
		setUVs(texU, texV);
		mUV = m.SwapUVPointer(mUV);
	}else{
		adjustColorsOfVertices2(dens);
		mColors = m.SwapColorPointer(mColors);
	}
	
	m.Unlock();
}


///////////////////////////////////////////////////////////////////////////////
//    Core Fluid Solver Routines
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
void StamFluidSolver::addSource ( float * x, float * s, float dt )
{
	for (int i=0 ; i<_gridCellsCount ; i++ ) x[i] += dt*s[i];
}

//-----------------------------------------------------------------------------
void StamFluidSolver::setBound ( int b, float * x )
{
	// TODO: Pull the if statements out of the loop
	// and move them up to a higher level if statement
	for (int i=0 ; i < _NY-1 ; i++ ) 
	{
		x[IX(0,i)] = b==1 ? -x[IX(1,i)] : x[IX(1,i)];
		x[IX(_NX-1,i)] = b==1 ? -x[IX(_NX-2,i)] : x[IX(_NX-2,i)];
	}
	
	for (int i=0; i < _NX-1; i++)
	{
		x[IX(i,0)] = b==2 ? -x[IX(i,1)] : x[IX(i,1)];
		x[IX(i,_NY-1)] = b==2 ? -x[IX(i,_NY-2)] : x[IX(i,_NY-2)];	
	}
	
	x[IX(0,0)] = 0.5f*(x[IX(1,0)]+x[IX(0,1)]);
	x[IX(0,_NY-1)] = 0.5f*(x[IX(1,_NY-1)]+x[IX(0,_NY-2)]);
	x[IX(_NX-1,0)] = 0.5f*(x[IX(_NX-2,0)]+x[IX(_NX-1,1)]);
	x[IX(_NX-1,_NY-1)] = 0.5f*(x[IX(_NX-2,_NY-1)]+x[IX(_NX-1,_NY-2)]);
	
	// TODO: Note that in the new version of this code,
	// we set everything to 0 when we set bounds.
	
	//internalBound(b,x);
}

//-----------------------------------------------------------------------------
#if 0
void inline StamFluidSolver::internalBound(int b, float * x)
{
	int count = 0;
	float val;
	
	for(int j = 1; j < _NY-1; j++)
	for(int i = 1; i < _NX-1; i++)
	{
		if(obs[IX(i,j)] == true)
		{
			val = 0;
			count = 0;
						
			if(!obs[IX(i,j+1)])
			{
				count++;
				if(b == 2) val -= x[IX(i,j+1)];
				else val += x[IX(i,j+1)];
			}
			if(!obs[IX(i,j-1)])
			{
				count++;
				if(b == 2) val -= x[IX(i,j-1)];
				else val += x[IX(i,j-1)];
			}
			
			if(!obs[IX(i+1,j)])
			{
				count++;
				if(b == 1) val -= x[IX(i+1,j)];
				else val += x[IX(i+1,j)];
			}
			if(!obs[IX(i-1,j)])
			{
				count++;
				if(b == 1) val -= x[IX(i-1,j)];
				else val += x[IX(i-1,j)];
			}
			
			switch(count)
			{
				case 1:
					x[IX(i,j)] = val;
					break;
				case 2:
					x[IX(i,j)] = val*0.5f;
					break;
				case 3:
					x[IX(i,j)] = val*0.33333333f;
					break;
				default:
					x[IX(i,j)] = val * 0.25f;
			}
		}
	}
}
#endif

//-----------------------------------------------------------------------------
void StamFluidSolver::linearSolve ( int b, float * x, float * x0, float a, float c )
{
	// NOTE that I don't use linear solve in the new code
	
	for (int k=0 ; k<6 ; k++ )
	{
		for (int j=1 ; j < _NY-1 ; j++ ){
			for (int i=1 ; i < _NX-1; i++ )
			{
				x[IX(i,j)] = (x0[IX(i,j)] + a*(x[IX(i-1,j)]+x[IX(i+1,j)]+x[IX(i,j-1)]+x[IX(i,j+1)]))/c;
			}
		}
		setBound ( b, x );
	}
}

//-----------------------------------------------------------------------------
void StamFluidSolver::diffuse ( int b, float * x, float * x0, float diff, float dt )
{
	// NOTE that diffuse is implemented very differently in the new code
	
	int L = kds_min(_NX,_NY); // TODO: Before I used N*N here, but now that we have _NX and _NY I'm using min of them. Not sure if that is right 
	float a=dt*diff*(L*L);
	linearSolve ( b, x, x0, a, 1+4*a );
}

//-----------------------------------------------------------------------------
void StamFluidSolver::advect ( int b, float * d, float * d0, float * u, float * v, float dt )
{
	int i0, j0, i1, j1;
	float s0, t0, s1, t1;
	
	int L = kds_min(_NX, _NY); // NOTE: This used to use N when we had symmetric grids
	
	float dt0 = dt*L;
	for (int j=0; j < _NY-1; j++ )
	for (int i=0; i < _NX-1; i++ )
	{ 
		float x = i-dt0*u[IX(i,j)];
		float y = j-dt0*v[IX(i,j)];
		if (x<0.5f) 
			x=0.5f; 
		if (x>_NX-1-0.5f) 
			x=_NX-1-0.5f;
		i0=(int)x;
		i1=i0+1;
		if (y<0.5f) 
			y=0.5f; 
		if (y>_NY-1-0.5f)
			y=_NY-1-0.5f; 
		j0=(int)y; 
		j1=j0+1;
		s1 = x-i0; s0 = 1.0f-s1; t1 = y-j0; t0 = 1.0f-t1;
		d[IX(i,j)] = s0*(t0*d0[IX(i0,j0)]+t1*d0[IX(i0,j1)])+
					s1*(t0*d0[IX(i1,j0)]+t1*d0[IX(i1,j1)]);
	}
	setBound ( b, d );
}

//-----------------------------------------------------------------------------
void StamFluidSolver::project ( float * u, float * v, float * p, float * div )
{
	int L = kds_min(_NX, _NY); // NOTE: This used to use N when we had symmetric grids
	float h = 1.0f/L; // TODO: Can include the * -0.5f as part of h.
	h *= 0.5;
	
	for (int j=1; j < _NY-1; j++ )
	for (int i=1; i < _NX-1; i++ ) 
	{ 
		div[IX(i,j)] = -h*(u[IX(i+1,j)]-u[IX(i-1,j)]+v[IX(i,j+1)]-v[IX(i,j-1)]);
		p[IX(i,j)] = 0;
	}
	setBound ( 0, div ); setBound ( 0, p );
	
	linearSolve ( 0, p, div, 1, 4 );
	
	float LL = 0.5f*L;
	// TODO: Can precalculate L*0.5f
	for (int j=1; j < _NY-1; j++ )
	for (int i=1; i < _NX-1; i++ )
	{ 
		u[IX(i,j)] -= LL*(p[IX(i+1,j)]-p[IX(i-1,j)]);
		v[IX(i,j)] -= LL*(p[IX(i,j+1)]-p[IX(i,j-1)]);
	}
	setBound ( 1, u ); setBound ( 2, v );
}

//-----------------------------------------------------------------------------
void StamFluidSolver::dens_step ( float * x, float * x0, float * u, float * v, float diff, float dt )
{
	addSource ( x, x0, dt );
	SWAP ( x0, x ); diffuse ( 0, x, x0, diff, dt );
	SWAP ( x0, x ); advect ( 0, x, x0, u, v, dt );
}

//-----------------------------------------------------------------------------
void StamFluidSolver::texture_step ( float * x, float * x0, float * u, float * v, float diff, float dt )
{
	//addSource ( N, x, x0, dt );
	SWAP ( x0, x ); diffuse ( 0, x, x0, diff, dt );
	SWAP ( x0, x ); advect ( 0, x, x0, u, v, dt );
}

//-----------------------------------------------------------------------------
void StamFluidSolver::vel_step ( float * u, float * v, float * u0, float * v0, float visc, float dt )
{
	addSource ( u, u0, dt ); 
	addSource ( v, v0, dt );
	
	#if (VORTICITY_CONFINEMENT == 1)
	if(mUseVorticityConfinement)
	{
		// add in vorticity confinement force
		vorticityConfinement(u0, v0);
		addSource ( u, u0, dt ); 
		addSource ( v, v0, dt );
	}
	#endif
	
	#if 0
	// add in temperature force 
	if(mSimulateTemperature)
	{
		heatRise(v0);
		addSource ( v, v0, dt );
	}
	#endif
	
	SWAP ( u0, u ); 
	diffuse ( 1, u, u0, visc, dt );
	SWAP ( v0, v ); 
	diffuse ( 2, v, v0, visc, dt );
	project ( u, v, u0, v0 );
	SWAP ( u0, u ); 
	SWAP ( v0, v );
	advect ( 1, u, u0, u0, v0, dt ); 
	advect ( 2, v, v0, u0, v0, dt );
	project ( u, v, u0, v0 );
}
//-----------------------------------------------------------------------------
void StamFluidSolver::updatePhysics (  )
{
	/*
	 //way to track FPS without having to modify main.cc so that extern float fpsReal is not necessary.
	 const char* buf = Con::getVariable("fps::real");
	 float fps = dAtof(buf);
	 
	 Con::warnf("FPS: %f", fps);
	 */
	
	//get_from_UI ( dens_prev, u_prev, v_prev ); // NOTE that this used to be called before
	for(int i = 0; i < 5; i++){
		int idx = _touchLocations[i];
		if(idx > -1 && idx < _gridCellsCount){
			dens[idx] = 1.0f;	
		}
	}
	// This code is a loop that was copied from get_from_UI
	/*
	for (int i=0 ; i<_gridCellsCount ; i++ ) {
		u[i] = v[i] = 0.0f;
		//r_prev[i] = g_prev[i] = b_prev[i] = 0.0f;
	}
	*/
	
	vel_step ( u, v, u_prev, v_prev, visc, dt );
	


	if(!mTextureMorph){
		// This code is a loop that was copied from get_from_UI
		for (int i=0 ; i<_gridCellsCount ; i++ ) {
			dens[i] *= 0.9f;
		}	
		for (int i=0 ; i<_gridCellsCount ; i++ ) {
			dens_prev[i] *= 0.9f;
		}
		dens_step ( dens, dens_prev, u, v, diff, dt );
		
	}
	/*if(mSimulateTemperature)
		texture_step ( N, temp, temp_prev, u, v, diff, dt );*/
	if(mTextureMorph){
		texture_step ( texU, texU_prev, u, v, diff, dt );
		texture_step (texV, texV_prev, u, v, 0, dt );
	}
	
	
}



/**
 * Calculate the buoyancy force as part of the velocity solver.
 * Fbuoy = -a*d*Y + b*(T-Tamb)*Y where Y = (0,1). The constants
 * a and b are positive with appropriate (physically meaningful)
 * units. T is the temperature at the current cell, Tamb is the
 * average temperature of the fluid grid. The density d provides
 * a mass that counteracts the buoyancy force.
 **/
//-----------------------------------------------------------------------------

#if 0
void StamFluidSolver::heatRise(float* Fbuoy)
{
	float Tamb = 0;
    float a = 0.000625f;
    float b = 0.025f;
	
    // sum all temperatures
	for (int j = 0; j < _NY; j++)
    for (int i = 0; i < _NX; i++)
	{
		Tamb += temp[IX(i, j)];
	}
	
    // get average temperature
    Tamb /= (_NX * _NY);
	
    // for each cell compute buoyancy force
	for (int j = 0; j < _NY; j++)
    for (int i = 0; i < _NX; i++)
	{
		Fbuoy[IX(i, j)] = a * temp[IX(i, j)] + -b * (temp[IX(i, j)] - Tamb);
	}
}
#endif


/**
 * Calculate the curl at position (i, j) in the fluid grid.
 * Physically this represents the vortex strength at the
 * cell. Computed as follows: w = (del x U) where U is the
 * velocity vector at (i, j).
 *
 * @param i The x index of the cell.
 * @param j The y index of the cell.
 **/
//-----------------------------------------------------------------------------
inline float StamFluidSolver::getCurl(int i, int j)
{
    float du_dy = (u[IX(i, j + 1)] - u[IX(i, j - 1)]) * 0.5f;
    float dv_dx = (v[IX(i + 1, j)] - v[IX(i - 1, j)]) * 0.5f;
	
    return du_dy - dv_dx;
}


/**
 * Calculate the vorticity confinement force for each cell
 * in the fluid grid. At a point (i,j), Fvc = N x w where
 * w is the curl at (i,j) and N = del |w| / |del |w||.
 * N is the vector pointing to the vortex center, hence we
 * add force perpendicular to N.
 *
 * @param Fvc_x The array to store the x component of the
 *        vorticity confinement force for each cell.
 * @param Fvc_y The array to store the y component of the
 *        vorticity confinement force for each cell.
 **/
//-----------------------------------------------------------------------------

#if (VORTICITY_CONFINEMENT == 1)
void StamFluidSolver::vorticityConfinement(float* Fvc_x, float* Fvc_y)
{
    float dw_dx, dw_dy;
    float length;
    float v;
	
    // Calculate magnitude of curl(u,v) for each cell. (|w|)
	for (int j = 1; j < _NY-1; j++)
    for (int i = 1; i < _NX-1; i++)
	{
		// TODO: Make sure compiler is inlining this!
		curl[IX(i, j)] = fabs(getCurl(i, j));
	}
	
	for (int j = 2; j < _NY-2; j++)
    for (int i = 2; i < _NX-2; i++)
	{		
		// Find derivative of the magnitude (n = del |w|)
		dw_dx = (curl[IX(i + 1, j)] - curl[IX(i - 1, j)]) * 0.5f;
		dw_dy = (curl[IX(i, j + 1)] - curl[IX(i, j - 1)]) * 0.5f;
		
		// Calculate vector length. (|n|)
		// Add small factor to prevent divide by zeros.
		
		// TODO: Can calculate reciprocal square root here.
		length = (float) sqrtf(dw_dx * dw_dx + dw_dy * dw_dy) + 0.000001f;
		
		// N = ( n/|n| )
		dw_dx /= length;
		dw_dy /= length;

		// TODO: Make sure compiler is inlining this!
		v = getCurl(i, j);
		
		// N x w
		Fvc_x[IX(i, j)] = dw_dy * -v;// * 2.0;
		Fvc_y[IX(i, j)] = dw_dx *  v;//* 2.0;
	}
}
#endif

//-------------------------------------------------------------------------------
void StamFluidSolver::cleanupMemory (  )
{
	if ( u ) delete[] ( u );
	if ( v ) delete[]( v );
	if ( u_prev ) delete[] ( u_prev );
	if ( v_prev ) delete[] ( v_prev );
	if ( dens ) delete[] ( dens );
	if ( dens_prev ) delete[] ( dens_prev );
	//if ( obs ) delete[] ( obs);
	#if (VORTICITY_CONFINEMENT == 1)
		if ( curl ) delete[] ( curl);
	#endif
	if ( texU ) delete[] ( texU);
	if ( texV ) delete[] ( texV);
	if ( texU_prev ) delete[] ( texU_prev);
	if ( texV_prev ) delete[] ( texV_prev);
	//if ( temp ) delete[] ( temp );
	//if ( temp_prev ) delete[] (temp_prev);
	
}

//-----------------------------------------------------------------------------
void StamFluidSolver::clear_data (  )
{
	for (int i=0 ; i < _gridCellsCount ; i++ )
	{
		u[i] = v[i] = u_prev[i] = v_prev[i] = dens[i] = dens_prev[i] = 0.0f;
		#if (VORTICITY_CONFINEMENT == 1)
			curl[i] = 0.0f;
		#endif
		//temp[i] = temp_prev[i] = 0.0f;
		//obs[i] = false;
	}
	
	#if 1
	if(texU && texV && texU_prev && texV_prev)
	{
		for(int j=0; j < _NY; j++)
		for(int i=0; i < _NX; i++)
		{
			texU[IX(i,j)] = texU_prev[IX(i,j)] = (float)(i/((float)_NX));
			texV[IX(i,j)] = texV_prev[IX(i,j)] = 1.0f -((float)(j/((float)_NY)));
		}
	}
	#endif
}

//-----------------------------------------------------------------------------
int StamFluidSolver::allocate_data (  )
{
	u			= new float[_gridCellsCount];
	v			= new float[_gridCellsCount];
	u_prev		= new float[_gridCellsCount];
	v_prev		= new float[_gridCellsCount];
	dens		= new float[_gridCellsCount];	
	dens_prev	= new float[_gridCellsCount];
	#if (VORTICITY_CONFINEMENT == 1)
		curl		= new float[_gridCellsCount];
	#endif
	texU		= new float[_gridCellsCount];
	texU_prev	= new float[_gridCellsCount];
	texV		= new float[_gridCellsCount];
	texV_prev	= new float[_gridCellsCount];
	
	
	//obs			= new bool[_gridCellsCount];

	//temp		= new float[_gridCellsCount];
	//temp_prev	= new float[_gridCellsCount];
	
	
	if ( !u || !v || !u_prev || !v_prev || !dens || !dens_prev || !texU || !texU_prev || !texV || !texV_prev /*|| !obs || !temp || !temp_prev*/) 
	{
		printf ("cannot allocate data\n" );
		return ( 0 );
	}
	
	
	return ( 1 );
}

//-----------------------------------------------------------------------------
//	Drawing Routines
//-----------------------------------------------------------------------------

#if 0

//-----------------------------------------------------------------------------
void StamFluidSolver::draw_velocity (  )
{
	int i, j;
	float x, y, h;
	
	h = 1.0f/N;
	
	glColor3f ( 1.0f, 0.0f, 0.0f );
	glLineWidth ( 1.0f );
	
	glBegin ( GL_LINES );
	
	for ( i=1 ; i<=N ; i++ ) {
		x = (i-0.5f)*h;
		for ( j=1 ; j<=N ; j++ ) {
			y = (j-0.5f)*h;
			
			glVertex2f ( x, y );
			glVertex2f ( x+u[IX(i,j)], y+v[IX(i,j)] );
		}
	}
	
	glEnd ();
}

//-----------------------------------------------------------------------------
void StamFluidSolver::draw_density ( )
{
	int i, j;
	float x, y, h, d00, d01, d10, d11;
	
	h = 1.0f/N;
	
	glBegin ( GL_QUADS );
	
	for ( i=0 ; i<=N ; i++ ) {
		x = (i-0.5f)*h;
		for ( j=0 ; j<=N ; j++ ) {
			y = (j-0.5f)*h;
			
			d00 = dens[IX(i,j)];
			d01 = dens[IX(i,j+1)];
			d10 = dens[IX(i+1,j)];
			d11 = dens[IX(i+1,j+1)];
			if(!blend){
				
				glColor3f ( d00, d00, d00 ); glVertex2f ( x, y );
				glColor3f ( d10, d10, d10 ); glVertex2f ( x+h, y );
				glColor3f ( d11, d11, d11 ); glVertex2f ( x+h, y+h );
				glColor3f ( d01, d01, d01 ); glVertex2f ( x, y+h );
			}else{
				glColor4f ( 1.0, 1.0, 1.0, d00 ); glVertex2f ( x, y );
				glColor4f ( 1.0, 1.0, 1.0, d10 ); glVertex2f ( x+h, y );
				glColor4f ( 1.0, 1.0, 1.0, d11 ); glVertex2f ( x+h, y+h );
				glColor4f ( 1.0, 1.0, 1.0, d01 ); glVertex2f ( x, y+h );
			}
		}
	}
	
	glEnd ();
}

//-----------------------------------------------------------------------------
void StamFluidSolver::draw_obstacles ( )
{
	int i, j;
	float x, y, h, d00, d01, d10, d11;
	
	h = 1.0f/N;
	
	glBegin ( GL_QUADS );
	
	for ( i=0 ; i<=N ; i++ ) {
		x = (i-0.5f)*h;
		for ( j=0 ; j<=N ; j++ ) {
			y = (j-0.5f)*h;
			
			if(obs[IX(i,j)] != true) continue;
			
			
			glColor3f ( 0, 1, 0 ); glVertex2f ( x, y );
			glColor3f ( 0, 1, 0 ); glVertex2f ( x+h, y );
			glColor3f ( 0, 1, 0 ); glVertex2f ( x+h, y+h );
			glColor3f ( 0, 1, 0 ); glVertex2f ( x, y+h );
			
		}
	}
	
	glEnd ();
}

//-----------------------------------------------------------------------------
void StamFluidSolver::drawPerturbedTexture()
{
	/*
	int i, j;
	float x, y, h, u00, u01, u10, u11, v00, v01, v10, v11;
	
	h = 1.0f/N;
	
	// Cannot render without Texture.
	if ( !mImageMapDataBlock )
		return;
	
	glEnable		( GL_TEXTURE_2D );
	glBindTexture	( GL_TEXTURE_2D, mImageMapDataBlock->getImageMapTexture().getGLName() );
	glTexEnvi		( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	
	// Set Blend Options.
	//setBlendOptions();
	
	// Fetch Current Frame Region.
	fxImageMapDatablock2D::tMapRegion region = mImageMapDataBlock->getImageMapRegion(mFrame);
	/// Fetch Positions.
	float minX = region.mMinX;
	float minY = region.mMinY;
	float maxX = region.mMaxX;
	float maxY = region.mMaxY;
	
	// Draw Object.
	glBegin(GL_QUADS);
	for ( i=0 ; i<=N ; i++ ) {
		x = (i-0.5f)*h;
		
		for ( j=0 ; j<=N ; j++ ) {
			y = (j-0.5f)*h;
			
			u00 = texU[IX(i,j)];
			u01 = texU[IX(i,j+1)];
			u10 = texU[IX(i+1,j)];
			u11 = texU[IX(i+1,j+1)];
			
			v00 = texV[IX(i,j)];
			v01 = texV[IX(i,j+1)];
			v10 = texV[IX(i+1,j)];
			v11 = texV[IX(i+1,j+1)];
			
			glTexCoord2f( u00, v00);
			glColor3f ( 1, 1, 1 ); 
			glVertex2f ( x, y );
			
			glTexCoord2f( u10, v10);
			glColor3f ( 1, 1, 1 ); 
			glVertex2f ( x+h, y );
			
			glTexCoord2f( u11, v11);
			glColor3f ( 1, 1, 1 ); 
			glVertex2f ( x+h, y+h );
			
			glTexCoord2f( u01, v01);
			glColor3f ( 1, 1, 1 ); 
			glVertex2f ( x, y+h );
		}
	}
	
	glEnd();
	
	// Disable Texturing.
	glDisable		( GL_TEXTURE_2D );
	*/
}

//-----------------------------------------------------------------------------
void StamFluidSolver::draw_temperature ( )
{
	/*
	int i, j;
	float x, y, h, d00, d01, d10, d11;
	
	h = 1.0f/N;
	
	glBegin ( GL_QUADS );
	
	for ( i=0 ; i<=N ; i++ ) {
		x = (i-0.5f)*h;
		for ( j=0 ; j<=N ; j++ ) {
			y = (j-0.5f)*h;
			
			d00 = temp[IX(i,j)];
			d01 = temp[IX(i,j+1)];
			d10 = temp[IX(i+1,j)];
			d11 = temp[IX(i+1,j+1)];
			if(!blend){
				
				glColor3f ( d00, 0.0f, d00 ); glVertex2f ( x, y );
				glColor3f ( d10, 0.0f, d10 ); glVertex2f ( x+h, y );
				glColor3f ( d11, 0.0f, d11 ); glVertex2f ( x+h, y+h );
				glColor3f ( d01, 0.0f, d01 ); glVertex2f ( x, y+h );
			}else{
				glColor4f ( 1.0f, 0.0f, 1.0f, d00 ); glVertex2f ( x, y );
				glColor4f ( 1.0f, 0.0f, 1.0f, d10 ); glVertex2f ( x+h, y );
				glColor4f ( 1.0f, 0.0f, 1.0f, d11 ); glVertex2f ( x+h, y+h );
				glColor4f ( 1.0f, 0.0f, 1.0f, d01 ); glVertex2f ( x, y+h );
			}
		}
	}
	
	glEnd ();
	 */
}

//-----------------------------------------------------------------------------
void StamFluidSolver::drawTemperatureTextured()
{
	/*
	//the u/x axis is going to be the color and opacity(?) and the v/y axis is constant
	int i, j;
	float x, y, h, u00, u01, u10, u11, v00, v01, v10, v11;
	
	h = 1.0f/N;
	
	// Cannot render without Texture.
	if ( !mHeatMapDataBlock )
		return;
	
	glEnable		( GL_TEXTURE_2D );
	glBindTexture	( GL_TEXTURE_2D, mHeatMapDataBlock->getImageMapTexture().getGLName() );
	glTexEnvi		( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	
	// Set Blend Options.
	//setBlendOptions();
	
	// Fetch Current Frame Region.
	fxImageMapDatablock2D::tMapRegion region = mHeatMapDataBlock->getImageMapRegion(mFrameHeatMap);
	/// Fetch Positions.
	float minX = region.mMinX;
	float minY = region.mMinY;
	float maxX = region.mMaxX;
	float maxY = region.mMaxY;
	
	// Draw Object.
	glBegin(GL_QUADS);
	for ( i=0 ; i<=N ; i++ ) {
		x = (i-0.5f)*h;
		
		for ( j=0 ; j<=N ; j++ ) {
			y = (j-0.5f)*h;
			
			u00 = temp[IX(i,j)];
			u01 = temp[IX(i,j+1)];
			u10 = temp[IX(i+1,j)];
			u11 = temp[IX(i+1,j+1)];
			
			//glTexCoord2f( u00 *maxX, v00 * maxY );
			glTexCoord2f( u00, 0);
			glColor4f ( 1, 1, 1, mTemperatureBlend ); 
			glVertex2f ( x, y );
			
			//glTexCoord2f( u10 *maxX, v10 * maxY );
			glTexCoord2f( u10, 0);
			glColor4f ( 1, 1, 1, mTemperatureBlend ); 
			glVertex2f ( x+h, y );
			
			//glTexCoord2f( u11 *maxX, v11 * maxY );
			glTexCoord2f( u11, 0);
			glColor4f ( 1, 1, 1, mTemperatureBlend ); 
			glVertex2f ( x+h, y+h );
			
			//glTexCoord2f( u01 *maxX, v01 * maxY );
			glTexCoord2f( u01, 0);
			glColor4f ( 1, 1, 1, mTemperatureBlend ); 
			glVertex2f ( x, y+h );
		}
	}
	
	glEnd();
	
	// Disable Texturing.
	glDisable		( GL_TEXTURE_2D );
	*/
}

//-----------------------------------------------------------------------------
void StamFluidSolver::get_from_UI ( float * d, float * u, float * v )
{
	/*
	int i, j, size = (N+2)*(N+2);
	
	for ( i=0 ; i<size ; i++ ) {
		u[i] = v[i] = d[i] = 0.0f;
	}
	
	
	
	if(!mSceneWindow) return;
	fxVector2D pos = mSceneWindow->getMousePosition();
	
	mx = pos.mX;
	my = pos.mY;
	
	float local_x = pos.mX + display_scale;
	float local_y = pos.mY;// + (display_scale*(0.5));;
	i = (int)((       pos.mX /display_scale)*N+1);
	j = (int)((       pos.mY /display_scale)*N+1);
	
	if ( i<1 || i>N || j<1 || j>N ) return;
	if(leftMouseButtonDown && addSources)
		d[IX(i,j)] = source;
	if(leftMouseButtonDown && addTemperature)
		temp[IX(i,j)] += tempIncrement;
	if(rightMouseButtonDown && addTemperature)
		temp[IX(i,j)] = 0;
	
	if(leftMouseButtonDown && addVelocities){
		u[IX(i,j)] = force * (mx-omx);
		v[IX(i,j)] = force * (my-omy);
	}
	
	if(leftMouseButtonDown && addObstacles){
		obs[IX(i,j)] = true;
	}
	
	if(rightMouseButtonDown && addObstacles){
		obs[IX(i,j)] = false;
	}
	
	omx = mx;
	omy = my;
	
	return;
	 */
}

#endif
