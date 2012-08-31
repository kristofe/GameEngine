#include "Game.h"
#include "Mesh.h"
#include "MetaballGrid.h"
#include "Material.h"
#include "Perlin.h"
#include "DropManager.h"

MetaballGrid::MetaballGrid(int nx, int ny, float width, float height, std::string imagename):FluidGrid(nx,ny,width,height,imagename)
{
	Init(nx, ny, width, height,imagename);
}
////////////////////////////////////////////////////////////////////////////////

MetaballGrid::MetaballGrid(PersistentData& storageData):FluidGrid(storageData)
{
	mRenderUsingIsoSurface = storageData.GetPropertyB("RenderUsingIsoSurface");
	Init(_NX,_NY,_gridWidth,_gridHeight,"");
	GameID matID = mMaterials[0];
	Material& mat = RendererObjectFactory::GetMaterial(matID);
		
	PersistentData* materialData = storageData.GetNodeFromPath("Material");
	if(materialData != NULL)
		mat.Configure(*materialData);
	
	
	
	if(mRenderUsingIsoSurface)
	{
		ExpandMesh();
	}
	else
	{
		Mesh& m = mGame.GetRenderer().GetMesh(mMeshes[0]);
		for(unsigned int i = 0; i < m.mUVCount; i++)
		{
			mOriginalUVs.push_back(m.mUV[i]);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////

void MetaballGrid::Init(int nx, int ny, float width, float height, std::string imagename)
{
	mMetaballFieldValues = new float[_gridCellsCount];
	mMetaballGradientValues = new float[_gridCellsCount];
	//mZeroArray = new float[_gridCellsCount];
	//memset(mZeroArray,0,sizeof(float)*_gridCellsCount);
	memset(mMetaballFieldValues,0,sizeof(float)*_gridCellsCount);
	memset(mMetaballGradientValues,0,sizeof(float)*_gridCellsCount);
	//memcpy(mMetaballFieldValues,mZeroArray,sizeof(float)*_gridCellsCount);
	
	mWindowWidth = mGame.GetRenderer().GetWindowWidth();
	mWindowHeight = mGame.GetRenderer().GetWindowHeight();
}
////////////////////////////////////////////////////////////////////////////////

MetaballGrid::~MetaballGrid()
{
	SAFE_DELETE_ARRAY(mMetaballFieldValues);
	SAFE_DELETE_ARRAY(mMetaballGradientValues);
	//SAFE_DELETE_ARRAY(mZeroArray);
}
////////////////////////////////////////////////////////////////////////////////

void MetaballGrid::Update()
{
	MakePendingRemovals();
	if(!mRenderUsingIsoSurface)
	{
		setUVs(mMetaballFieldValues);

		Mesh& m = mGame.GetRenderer().GetMesh(mMeshes[0]);
		m.Lock();
		mUV = m.SwapUVPointer(mUV);
		mUV2 = m.SwapUV2Pointer(mUV2);
		m.Unlock();
	} 
	else
	{
		GenerateIsoSurface();
	}
	//memset(mMetaballFieldValues, 0, sizeof(float)*_gridCellsCount);
	//memset(mMetaballGradientValues, 0, sizeof(float)*_gridCellsCount);
}

////////////////////////////////////////////////////////////////////////////////
void MetaballGrid::setUVs(float * vals)
{
	/*
	for(int i = 0; i < _gridCellsCount-1; i++){
		float v = mMetaballFieldValues[i] * 0.3f;
		if(mMetaballFieldValues[i] < 0.12f)
		{
			v *= v;//Ramp the value down quickly
		}
		IvVector2 uv = mOriginalUVs[i];
		mUV[i].Set(uv.x + v,uv.y + v);
	}
	 */
	//float orientationOffsetX = (Game::mOrientation.x-0.5f)* 0.1f;
	//float orientationOffsetY = (Game::mOrientation.y-0.5f)* 0.1f;
	IvVector3 orientationY,orientationX,orientation;
	IvVector3 unit(-0.5,0.667,0);
	unit.Normalize();
	IvMatrix33 m;
	m.Rotation(-Game::mOrientation.y,0,0);
	orientation = unit * m;
	
	
	//orientation.x = fabs(orientation.x);
	//orientation.y = fabs(orientation.y);
	

	orientation.Normalize();
	//printf("Orientation Angle Length:%3.2f\n", orientation.Length());
	
	//float orientationOffsetX = (Game::mAcceleration.x-0.5f)*-2.0f;
	//float orientationOffsetY = (Game::mAcceleration.y-0.5f)* 0.3f;
	
	float o,x1,y1;
	float vec1X,vec1Y,vec1Z;
	float vec2X,vec2Y,vec2Z;
	//float normX,normY,normZ;
	//float length;
	unsigned int rowIdx;
	unsigned int x1Idx,y1Idx,oIdx;
	vec1X = 320.0f/_NX;
	vec1Y = 0.0f;
	vec2X = 0.0f;
	vec2Y = 480.0f/_NY;
	
	for(unsigned int y = 0; y < _NY; ++y)
	{
		rowIdx = (y * _NX);
		if(y == _NY)
			rowIdx = (y-1)*_NX;
		
		for(unsigned int x = 0; x < _NX; ++x)
		{
			oIdx = rowIdx + x;
			x1Idx = oIdx + 1;
			y1Idx = oIdx + _NX;
			
			if(y == _NY-1)
			{
				y1Idx = oIdx;
			}
			
			if(x == _NX-1)
				x1Idx = oIdx;// - 1;
				
			
			o = mMetaballFieldValues[oIdx];
			x1 = mMetaballFieldValues[x1Idx];
			y1 = mMetaballFieldValues[y1Idx];
			
			vec1Z = x1 - o;
			vec2Z = y1 - o;
			
			
			float u = vec1Z *0.5f + (o*Game::mAcceleration.x*0.4f);
			float v = vec2Z *0.5f + (o*Game::mAcceleration.y*-0.4f);
			
			/*
			 float lenSqr = vec1Z*vec1Z + vec2Z*vec2Z;
			lenSqr *= 20.0f;
			float u = vec1Z *0.5f + (lenSqr*Game::mAcceleration.x*-0.4f);
			float v = vec2Z *0.5f + (lenSqr*Game::mAcceleration.y*0.4f);
			*/
			
			IvVector2 uv = mOriginalUVs[oIdx];
			mUV[oIdx].Set(uv.x + u,uv.y + v);
			

			
			float scaling = 7.5f;
#ifdef TARGET_OS_IPHONE
			float u2 = 0.5f*(vec1Z*scaling*orientation.x) + 0.5f;
			float v2 = 0.5f*(vec2Z*scaling*orientation.y) + 0.5f;
#else
			float u2 = 0.5f*(vec1Z*scaling) + 0.5f;
			float v2 = 0.5f*(vec2Z*scaling) + 0.5f;
#endif
			
			//u2 += vec1Z*orientationOffsetX;
			//v2 += vec2Z*orientationOffsetY;

			
			mUV2[oIdx].Set(u2,v2);
			
		}
	}

	//mUV[(_NX-1) * (_NY -1)].Set(1.0f,0.0f);
	//mUV2[(_NX-1) * (_NY -1)].Set(0.5f,0.5f);
	/*
	//test a fix for the first row of uv2
	int firstIDX = (_NY-1)*_NX;
	int lastIDX = firstIDX+_NX;
	for(int i = firstIDX; i < lastIDX; i++)
	{
		mUV2[i].Set(0.5f,0.5f); 
	}
	
	firstIDX = _NX-1;
	lastIDX = _NY*_NX;
	for(int i = firstIDX; i < lastIDX; i += _NX)
	{
		mUV2[i].Set(0.5f,0.5f); 
	}
	*/
	
	/*
	//using forward differencing
	float u,v,o,x1,y1;
	float vec1X,vec1Y,vec1Z;
	float vec2X,vec2Y,vec2Z;
	float normX,normY,normZ;
	float length;
	unsigned int rowIdx;
	unsigned int x1Idx,y1Idx,oIdx;
	vec1X = 320.0f/_NX;
	vec1Y = 0.0f;
	vec2X = 0.0f;
	vec2Y = 480.0f/_NY;
	for(unsigned int y = 0; y < _NY - 1; ++y)
	{
		rowIdx = (y * _NX);
		for(unsigned int x = 0; x < _NX - 1; ++x)
		{
			oIdx = rowIdx + x;
			x1Idx = oIdx + 1;
			y1Idx = oIdx + _NX;
			o = vals[oIdx];
			x1 = vals[x1Idx];
			y1 = vals[y1Idx];
			
			vec1Z = x1 - o;
			vec2Z = y1 - o;
			float u = 0.5f*vec1Z + 0.5f;
			float v = 0.5f*vec2Z + 0.5f;
			
			mUV[oIdx].Set(u,1.0f-v);
			
		}
	}
	 */
}
////////////////////////////////////////////////////////////////////////////////

void MetaballGrid::EffectField(float x, float y, float radius, float power, bool staticField)
{
	return;
	IvVector2 p(x/mWindowWidth,y/mWindowHeight);
	//float rad = 1.0f/mWindowHeight * radius;
	if(!staticField)
		UpdateDynamicMetaballGrid(p, radius*radius, radius, 1.0f, power);
}
////////////////////////////////////////////////////////////////////////////////

void MetaballGrid::EffectStaticField(float x, float y, float radius, float power)
{
	IvVector2 p(x/mWindowWidth,y/mWindowHeight);
	UpdateDynamicMetaballGrid(p, radius*radius, radius, 1.0f, power);
}
////////////////////////////////////////////////////////////////////////////////

/*
void MetaballGrid::MouseDown(float u, float v, int button)
{
	IvVector2 pos(u,v);
	float radius = 0.1;
	UpdateDynamicMetaballGrid(pos, radius*radius, radius,1.0f,1.0f);
}
 ////////////////////////////////////////////////////////////////////////////////

// TODO: Add height along entire path.
void MetaballGrid::MouseDragged(float u, float v,float lu, float lv, int button)
{
	//int idx = XY(u,v);
	//mMetaballFieldValues[idx] = 1.0f;
	//_waveZ[idx] = _waveZ1[idx] = 4.0f;
	//_waveV[idx] += 5.0f;
}
 ////////////////////////////////////////////////////////////////////////////////

void MetaballGrid::MouseUp(float u, float v, int button)
{
	//int idx = XY(u,v);
	//mMetaballFieldValues[idx] = -1.0f;
	//_waveZ[idx] = _waveZ1[idx] = -4.0f;
	//_waveV[idx] += -5.0f;
}
 */
////////////////////////////////////////////////////////////////////////////////

void MetaballGrid::UpdateDynamicMetaballGrid(IvVector2& pos, float radiusSqr, float radius,  float sign,float power){
	//int N = _NY;
	//float h = N;
	int ii = (int) roundf(pos.x*_NX);//Assumes x is between 0 and 1
	int jj = (int) roundf(pos.y*_NY);//Assumes y is between 0 and 1
	float gridToWorldX = 1.0f/((float)_NX);
	float gridToWorldY = 1.0f/((float)_NY);
	float aspectRatioScaling = (float)_NX*gridToWorldY;
	
	int radiusInCellsX = (int)(0.5f + radius*_NX);
	int radiusInCellsY = (int)(0.5f + radius*_NY);
	
	int leftmostCell = ii - radiusInCellsX;
	if(leftmostCell < 0) leftmostCell = 0;
	int rightmostCell = ii + radiusInCellsX;
	if(rightmostCell > _NX-1) rightmostCell = _NX-1;
	int topmostCell = jj - radiusInCellsY;
	if(topmostCell < 0) topmostCell = 0 ;
	int bottommostCell = jj + radiusInCellsY;
	if(bottommostCell > _NY-1) bottommostCell = _NY-1;
	
	IvVector2 pointPos;
	for(int i = leftmostCell; i <= rightmostCell; i++){
		for(int j = topmostCell; j <= bottommostCell; j++){
			//for now just use the dame value no matter how far from center the point is
			pointPos.x = i * gridToWorldX;
			pointPos.y = j * gridToWorldY;
			IvVector2 diff = pos - pointPos;
			diff.x *= aspectRatioScaling;
			float distSqr = (diff.x*diff.x + diff.y*diff.y);
			if(distSqr <= radiusSqr){
				int idx = IX(i,j);
				
				distSqr /= radiusSqr;
				if(distSqr < 0.5){
					float t = (0.25 - distSqr + distSqr*distSqr);
					
					mMetaballFieldValues[idx] += t*power;
					if(mMetaballFieldValues[idx] < 0.0f)
					{
						mMetaballFieldValues[idx] = 0.0f;
					}
					//float val=1.0f-(2.25f*distSqr);//power * t;
					//mMetaballGradientValues[idx] += val;
					float val=1.0f-(distSqr);//power * t;
					mMetaballGradientValues[idx] += val*power;
				}
				
				
			}
		}
	}

}
////////////////////////////////////////////////////////////////////////////////
void MetaballGrid::Serialize(PersistentData& storageData)
{
	FluidGrid::Serialize(storageData);
	storageData.SetName("MetaballGrid");
	
}
////////////////////////////////////////////////////////////////////////////////
void MetaballGrid::DeSerialize(PersistentData& storageData)
{
	FluidGrid::DeSerialize(storageData);	
	Init(_NX,_NY,_gridWidth,_gridHeight,"");
	
}
////////////////////////////////////////////////////////////////////////////////
void MetaballGrid::GenerateIsoSurface( )
{
	/*
	mVertices.clear();
	mColors.clear();
	mUVs.clear();*/
	Mesh& m = mGame.GetRenderer().GetMesh(mMeshes[0]);
	m.Lock();
	
	mIndices.clear();

	
	int i, j;
	float x, y, hX, hY, dSE, dNE, dNW, dSW;
	float xnw,xne,xse,xsw, ynw,yne,yse,ysw; 
	bool bSE, bNE, bNW, bSW;
	
	float metaball_threshold = 0.1f;
	
	hX = 1.0f/(_NX-1);
	hY = 1.0f/(_NY-1);

	int ii = 0;
	for ( i=1 ; i<=_NX ; i++ ) {
		x = (i-0.5f)*hX;
		for ( j=_NY-1 ; j>0 ; j-- ) {
			y = (j-0.5f)*hY;
			bSE = bNE = bNW = bSW = false;
			int numAboveThresholdTri1 = 0;
			int numAboveThresholdTri2 = 0;
			
			dSE = mMetaballFieldValues[IX(i,j)];
			dSW = mMetaballFieldValues[IX(i+1,j)];
			dNE = mMetaballFieldValues[IX(i,j-1)];
			dNW = mMetaballFieldValues[IX(i+1,j-1)];
			
			float une, vne, use, vse;
			float unw, vnw, usw, vsw;
			float length = 1.0f;
			//une = vne = use = vse = unw = vnw = usw = vsw = 0.5f;
			
			//Doing a very hacky sphere mapping - approximate normals - not unit vectors
			use = mMetaballGradientValues[IX(i-1,j)] - mMetaballGradientValues[IX(i+1,j)];
			vse = mMetaballGradientValues[IX(i,j-1)] - mMetaballGradientValues[IX(i,j+1)];
			//length = sqrt(use*use + vse*vse);
			use = ((use /length)*0.5) + 0.5;
			vse = ((vse /length)*0.5) + 0.5;
			
			
			usw = mMetaballGradientValues[IX(i,j)] - mMetaballGradientValues[IX(i+2,j)];
			vsw = mMetaballGradientValues[IX(i+1,j-1)] - mMetaballGradientValues[IX(i+1,j+1)];
			//length = sqrt(usw*usw + vsw*vsw);
			usw = ((usw /length)*0.5) + 0.5;
			vsw = ((vsw /length)*0.5) + 0.5;
			
			une = mMetaballGradientValues[IX(i-1,j-1)] - mMetaballGradientValues[IX(i+1,j-1)];
			vne = mMetaballGradientValues[IX(i,j-2)] - mMetaballGradientValues[IX(i,j)];
			//length = sqrt(une*une + vne*vne);
			une = ((une /length)*0.5) + 0.5;
			vne = ((vne /length)*0.5) + 0.5;
			
			unw = mMetaballGradientValues[IX(i,j-1)] - mMetaballGradientValues[IX(i+2,j-1)];
			vnw = mMetaballGradientValues[IX(i+1,j-2)] - mMetaballGradientValues[IX(i+1,j)];
			//length = sqrt(unw*unw + vnw*vnw);
			unw = ((unw /length)*0.5) + 0.5;
			vnw = ((vnw /length)*0.5) + 0.5;
			
			xse = xne = x;
			xsw = xnw = x + hX;
			
			yse = ysw = y;
			yne = ynw = y - hY;
			
			float mx = 0.8f;
			float mn = 0.2f;
			
			une = CLAMP(une,mn,mx);
			unw = CLAMP(unw,mn,mx);
			use = CLAMP(use,mn,mx);
			usw = CLAMP(usw,mn,mx);
			
			vne = CLAMP(vne,mn,mx);
			vnw = CLAMP(vnw,mn,mx);
			vse = CLAMP(vse,mn,mx);
			vsw = CLAMP(vsw,mn,mx);
			
			vne = 1.0f - vne;
			vse = 1.0f - vse;
			vnw = 1.0f - vnw;
			vsw = 1.0f - vsw;
			
			
			//SE
			if(dSE >= metaball_threshold){
				numAboveThresholdTri1++;
				bSE = true;
				
			}
			
			//NE
			if(dNE >= metaball_threshold){
				numAboveThresholdTri1++;
				numAboveThresholdTri2++;
				bNE = true;
			}
			
			//SW
			if(dSW >= metaball_threshold){
				numAboveThresholdTri1++;
				numAboveThresholdTri2++;
				bSW = true;
			}
			
			//NW
			if(dNW >= metaball_threshold){
				numAboveThresholdTri2++;
				bNW = true;
			}
			
			
//			 triangle 1
//			 *			== 000
//			 
//			 *	*
//			 --------------------
//			 *			== 001
//			 
//			 *	0
//			 --------------------
//			 *			== 010
//			 
//			 0	*
//			 --------------------
//			 *			== 011
//			 
//			 0	0
//			 --------------------
//			 0			== 100
//			 
//			 *	*
//			 --------------------
//			 0			== 101
//			 
//			 *	0
//			 --------------------
//			 0			== 110
//			 
//			 0	*
//			 --------------------
//			 0			== 111
//			 
//			 0	0
			 
			 
			IvVector2 a_pos, b_pos;
			float a_val, b_val;//, out_x, out_y;
			int startIDX = mIndices.size();
			IvVector3 pos;
			IvColor c(1.0f,1.0f,1.0f,1.0f);
			IvVector2 uv;
			
			//TRI_1 = NE,SE,SW
			if(numAboveThresholdTri1 == 3){//Case 7		

				m.mUV[ii].Set(une,vne); 
				m.mUV[ii+1].Set(use,vse);
				m.mUV[ii+2].Set(usw,vsw); 
				
				m.mVertices[ii].Set(xne,yne);
				m.mVertices[ii+1].Set(xse,yse);
				m.mVertices[ii+2].Set(xsw,ysw);
				
				//mColors[ii] = c;
				//mColors[ii+1] = c;
				//mColors[ii+2] = c;
				
				mIndices.push_back(startIDX++);
				mIndices.push_back(startIDX++);
				mIndices.push_back(startIDX++);
				
				
								
				//glTexCoord2f( une, vne); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xne, yne );
				//glTexCoord2f( use, vse); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xse, yse );
				//glTexCoord2f( usw, vsw); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xsw, ysw );
				ii+=3;
			}else if(numAboveThresholdTri1 == 2){
				//Cases 3,5,6
				//Need to generate two triangles
				
				//Use the CheckEdge() function in the header (copied from darwin3d)
				//CheckEdge(metaball_threshold, fxVector2D& a_pos,fxVector2D& b_pos,float a_val, float b_val, float *out_x, float *out_y)
				
				if(!bNE){//Case 3
					
					//Need to interpolate the x,y of NE, 
					a_pos.Set(xse,yse);
					b_pos.Set(xne,yne);
					a_val = dSE;
					b_val = dNE;
					float xne01, yne01;
					CheckEdge(metaball_threshold, a_pos, b_pos, a_val, b_val, &xne01, &yne01);
					
					m.mUV[ii].Set(une,vne);
					m.mUV[ii+1].Set(use,vse);
					m.mUV[ii+2].Set(usw,vsw);
					
					m.mVertices[ii].Set(xne01,yne01);
					m.mVertices[ii+1].Set(xse,yse);
					m.mVertices[ii+2].Set(xsw,ysw);
					
					//mColors[ii] = c;
					////mColors[ii+1] = c;
					////mColors[ii+2] = c;
					
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					//glTexCoord2f( une, vne); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xne01, yne01 );
					//glTexCoord2f( use, vse); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xse, yse );
					//glTexCoord2f( usw, vsw); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xsw, ysw );
					
					ii+=3;
					
					a_pos.Set(xsw,ysw);
					//b_pos.Set(xne,yne);
					a_val = dSW;
					b_val = dNE;
					float xne02, yne02;
					CheckEdge(metaball_threshold, a_pos,b_pos,a_val, b_val, &xne02, &yne02);
					
					m.mUV[ii].Set(usw,vsw);
					m.mUV[ii+1].Set(une,vne);
					m.mUV[ii+2].Set(une,vne);
					
					m.mVertices[ii].Set(xsw,ysw)	;
					m.mVertices[ii+1].Set(xne01,yne01);
					m.mVertices[ii+2].Set(xne02,yne02);
					
					//mColors[ii] = c;
					////mColors[ii+1] = c;
					////mColors[ii+2] = c;
					
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					//glTexCoord2f( usw, vsw); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xsw, ysw );
					//glTexCoord2f( une, vne); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xne01, yne01 );
					//glTexCoord2f( une, vne); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xne02, yne02 );
					ii+=3;
				}else if(!bSE){//case 5
					
					a_pos.Set(xne,yne);
					b_pos.Set(xse,yse);
					a_val = dNE;
					b_val = dSE;
					float xse01, yse01;
					CheckEdge(metaball_threshold, a_pos, b_pos, a_val, b_val, &xse01, &yse01);
					
					m.mUV[ii].Set(use,vse);
					m.mUV[ii+1].Set(usw,vsw);
					m.mUV[ii+2].Set(une,vne);
					
					m.mVertices[ii].Set(xse01,yse01);
					m.mVertices[ii+1].Set(xsw,ysw);
					m.mVertices[ii+2].Set(xne,yne);
					
					//mColors[ii] = c;
					//mColors[ii] = c;
					//mColors[ii] = c;
					
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					//glTexCoord2f( use, vse); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xse01, yse01 );
					//glTexCoord2f( usw, vsw); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xsw, ysw );
					//glTexCoord2f( une, vne); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xne, yne );
					
					ii+=3;
					
					a_pos.Set(xsw,ysw);
					//b_pos.Set(xse,yse);
					a_val = dSW;
					b_val = dSE;
					float xse02, yse02;
					CheckEdge(metaball_threshold, a_pos,b_pos,a_val, b_val, &xse02, &yse02);
					
					m.mUV[ii].Set(usw,vsw);
					m.mUV[ii+1].Set(use,vse);
					m.mUV[ii+2].Set(use,vse);
					
					m.mVertices[ii].Set(xsw,ysw);
					m.mVertices[ii+1].Set(xse01,yse01);
					m.mVertices[ii+2].Set(xse02,yse02);
					
					//mColors[ii] = c;
					//mColors[ii] = c;
					//mColors[ii] = c;
					
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					//glTexCoord2f( usw, vsw); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xsw, ysw );
					//glTexCoord2f( use, vse); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xse01, yse01 );
					//glTexCoord2f( use, vse); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xse02, yse02 );
					ii+=3;
				}else{
					//!bSW  Case 6
					a_pos.Set(xne,yne);
					b_pos.Set(xsw,ysw);
					a_val = dNE;
					b_val = dSW;
					float xsw01, ysw01;
					CheckEdge(metaball_threshold, a_pos, b_pos, a_val, b_val, &xsw01, &ysw01);
					
					m.mUV[ii].Set(usw,vsw);
					m.mUV[ii+1].Set(use,vse);
					m.mUV[ii+2].Set(une,vne);
					
					m.mVertices[ii].Set(xsw01,ysw01);
					m.mVertices[ii+1].Set(xse,yse);
					m.mVertices[ii+2].Set(xne,yne);
					
					//mColors[ii] = c;
					//mColors[ii] = c;
					//mColors[ii] = c;
					
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					//glTexCoord2f( usw, vsw); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xsw01, ysw01 );
					//glTexCoord2f( use, vse); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xse, yse );
					//glTexCoord2f( une, vne); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xne, yne );
					
					ii+=3;
					
					a_pos.Set(xse,yse);
					//b_pos.Set(xse,yse);
					a_val = dSE;
					b_val = dSW;
					float xsw02, ysw02;
					CheckEdge(metaball_threshold, a_pos,b_pos,a_val, b_val, &xsw02, &ysw02);
					
					m.mUV[ii].Set(use,vse);
					m.mUV[ii+1].Set(usw,vsw);
					m.mUV[ii+2].Set(usw,vsw);
					
					m.mVertices[ii].Set(xse,yse);
					m.mVertices[ii+1].Set(xsw01,ysw01);
					m.mVertices[ii+2].Set(xsw02,ysw02);
					
					//mColors[ii] = c;
					//mColors[ii] = c;
					//mColors[ii] = c;
					
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					//glTexCoord2f( use, vse); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xse, yse );
					//glTexCoord2f( usw, vsw); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xsw01, ysw01 );
					//glTexCoord2f( usw, vsw); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xsw02, ysw02 );
					ii+=3;
				}
			}else if(numAboveThresholdTri1 == 1){
				//Cases 1,2,4
				if(bSW){
					
					//case 1 
					a_pos.Set(xsw,ysw);
					b_pos.Set(xne,yne);
					a_val = dSW;
					b_val = dNE;
					float xne01,yne01;
					CheckEdge(metaball_threshold, a_pos, b_pos, a_val, b_val, &xne01, &yne01);
					
					a_pos.Set(xsw,ysw);
					b_pos.Set(xse,yse);
					a_val = dSW;
					b_val = dSE;
					float xse01,yse01;
					CheckEdge(metaball_threshold, a_pos,b_pos,a_val, b_val, &xse01, &yse01);
					
					m.mUV[ii].Set(usw,vsw);
					m.mUV[ii+1].Set(une,vne);
					m.mUV[ii+2].Set(use,vse);
					
					m.mVertices[ii].Set(xsw,ysw);
					m.mVertices[ii+1].Set(xne01,yne01);
					m.mVertices[ii+2].Set(xse01,yse01);
					
					//mColors[ii] = c;
					//mColors[ii] = c;
					//mColors[ii] = c;
					
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);					
					//glTexCoord2f( usw, vsw); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xsw, ysw );
					//glTexCoord2f( une, vne); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xne01, yne01 );
					//glTexCoord2f( use, vse); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xse01, yse01 );
					ii+=3;
				}else if(bSE){
					
					//case 2 
					a_pos.Set(xse,yse);
					b_pos.Set(xne,yne);
					a_val = dSE;
					b_val = dNE;
					float xne01,yne01;
					CheckEdge(metaball_threshold, a_pos, b_pos, a_val, b_val, &xne01, &yne01);
					
					a_pos.Set(xse,yse);
					b_pos.Set(xsw,ysw);
					a_val = dSE;
					b_val = dSW;
					float xsw01,ysw01;
					CheckEdge(metaball_threshold, a_pos,b_pos,a_val, b_val, &xsw01, &ysw01);
					
					m.mUV[ii].Set(usw,vsw);
					m.mUV[ii+1].Set(une,vne);
					m.mUV[ii+2].Set(use,vse);
					
					m.mVertices[ii].Set(xsw01,ysw01);
					m.mVertices[ii+1].Set(xne01,yne01);
					m.mVertices[ii+2].Set(xse,yse);
					
					//mColors[ii] = c;
					//mColors[ii] = c;
					//mColors[ii] = c;
					
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					//glTexCoord2f( usw, vsw); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xsw01, ysw01 );
					//glTexCoord2f( une, vne); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xne01, yne01 );
					//glTexCoord2f( use, vse); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xse, yse );
					ii+=3;
				}else{
					
					//case 4 
					a_pos.Set(xne,yne);
					b_pos.Set(xse,yse);
					a_val = dNE;
					b_val = dSE;
					float xse01,yse01;
					CheckEdge(metaball_threshold, a_pos, b_pos, a_val, b_val, &xse01, &yse01);
					
					a_pos.Set(xne,yne);
					b_pos.Set(xsw,ysw);
					a_val = dNE;
					b_val = dSW;
					float xsw01,ysw01;
					CheckEdge(metaball_threshold, a_pos,b_pos,a_val, b_val, &xsw01, &ysw01);
					
					m.mUV[ii].Set(usw,vsw);
					m.mUV[ii+1].Set(une,vne);
					m.mUV[ii+2].Set(use,vse);
					
					m.mVertices[ii].Set(xsw01,ysw01);
					m.mVertices[ii+1].Set(xne,yne);
					m.mVertices[ii+2].Set(xse01,yse01);
					
					//mColors[ii] = c;
					//mColors[ii] = c;
					//mColors[ii] = c;
					
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					//glTexCoord2f( usw, vsw); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xsw01, ysw01 );
					//glTexCoord2f( une, vne); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xne, yne );
					//glTexCoord2f( use, vse); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xse01, yse01 );
					ii+=3;
				}
			}
			
			
			startIDX = mIndices.size();
			//TRI_2 = NE,SW,NW
			if(numAboveThresholdTri2 == 3){
				m.mUV[ii].Set(une,vne);
				m.mUV[ii+1].Set(usw,vsw);
				m.mUV[ii+2].Set(unw,vnw);
				
				m.mVertices[ii].Set(xne,yne);
				m.mVertices[ii+1].Set(xsw,ysw);
				m.mVertices[ii+2].Set(xnw,ynw);
				
				//mColors[ii] = c;
				//mColors[ii] = c;
				//mColors[ii] = c;
				
				mIndices.push_back(startIDX++);
				mIndices.push_back(startIDX++);
				mIndices.push_back(startIDX++);
				//glTexCoord2f( une, vne); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xne, yne );
				//glTexCoord2f( usw, vsw); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xsw, ysw );
				//glTexCoord2f( unw, vnw); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xnw, ynw );
				ii+=3;
			}else if(numAboveThresholdTri2 == 2){
				//Cases 3,5,6
				//Need to generate two triangles
				
				//Use the CheckEdge() function in the header (copied from darwin3d)
				//CheckEdge(metaball_threshold, fxVector2D& a_pos,fxVector2D& b_pos,float a_val, float b_val, float *out_x, float *out_y)
				
				if(!bNE){//Case 3
					
					//Need to interpolate the x,y of NE, 
					a_pos.Set(xnw,ynw);
					b_pos.Set(xne,yne);
					a_val = dNW;
					b_val = dNE;
					float xne01, yne01;
					CheckEdge(metaball_threshold, a_pos, b_pos, a_val, b_val, &xne01, &yne01);
				
					m.mUV[ii].Set(une,vne);
					m.mUV[ii+1].Set(usw,vsw);
					m.mUV[ii+2].Set(unw,vnw);
					
					m.mVertices[ii].Set(xne01,yne01);
					m.mVertices[ii+1].Set(xsw,ysw);
					m.mVertices[ii+2].Set(xnw,ynw);
					
					//mColors[ii] = c;
					//mColors[ii] = c;
					//mColors[ii] = c;
					
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					//glTexCoord2f( une, vne); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xne01, yne01 );
					//glTexCoord2f( usw, vsw); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xsw, ysw );
					//glTexCoord2f( unw, vnw); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xnw, ynw );
					
					ii+=3;
					
					a_pos.Set(xsw,ysw);
					//b_pos.Set(xne,yne);
					a_val = dSW;
					b_val = dNE;
					float xne02, yne02;
					CheckEdge(metaball_threshold, a_pos,b_pos,a_val, b_val, &xne02, &yne02);
					
					m.mUV[ii].Set(usw,vsw);
					m.mUV[ii+1].Set(une,vne);
					m.mUV[ii+2].Set(une,vne);
					
					m.mVertices[ii].Set(xsw,ysw);
					m.mVertices[ii+1].Set(xne01,yne01);
					m.mVertices[ii+2].Set(xne02,yne02);
					
					//mColors[ii] = c;
					//mColors[ii] = c;
					//mColors[ii] = c;
					
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					//glTexCoord2f( usw, vsw); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xsw, ysw );
					//glTexCoord2f( une, vne); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xne01, yne01 );
					//glTexCoord2f( une, vne); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xne02, yne02 );
					ii+=3;
				}else if(!bNW){//case 5
					
					a_pos.Set(xne,yne);
					b_pos.Set(xnw,ynw);
					a_val = dNE;
					b_val = dNW;
					float xnw01, ynw01;
					CheckEdge(metaball_threshold, a_pos, b_pos, a_val, b_val, &xnw01, &ynw01);
					
					m.mUV[ii].Set(unw,vnw);
					m.mUV[ii+1].Set(usw,vsw);
					m.mUV[ii+2].Set(une,vne);
					
					m.mVertices[ii].Set(xnw01,ynw01);
					m.mVertices[ii+1].Set(xsw,ysw);
					m.mVertices[ii+2].Set(xne,yne);
					
					//mColors[ii] = c;
					//mColors[ii] = c;
					//mColors[ii] = c;
					
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					//glTexCoord2f( unw, vnw); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xnw01, ynw01 );
					//glTexCoord2f( usw, vsw); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xsw, ysw );
					//glTexCoord2f( une, vne); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xne, yne );
					
					ii+=3;
					
					a_pos.Set(xsw,ysw);
					//b_pos.Set(xnw,ynw);
					a_val = dSW;
					b_val = dNW;
					float xnw02, ynw02;
					CheckEdge(metaball_threshold, a_pos,b_pos,a_val, b_val, &xnw02, &ynw02);
					
					m.mUV[ii].Set(usw,vsw);
					m.mUV[ii+1].Set(unw,vnw);
					m.mUV[ii+2].Set(unw,vnw);
					
					m.mVertices[ii].Set(xsw,ysw);
					m.mVertices[ii+1].Set(xnw02,ynw02);
					m.mVertices[ii+2].Set(xnw01,ynw01);
					
					//mColors[ii] = c;
					//mColors[ii] = c;
					//mColors[ii] = c;
					
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					//glTexCoord2f( usw, vsw); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xsw, ysw );
					//glTexCoord2f( unw, vnw); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xnw02, ynw02 );
					//glTexCoord2f( unw, vnw); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xnw01, ynw01 );
					ii+=3;
				}else{
					//!bSW  Case 6
					a_pos.Set(xne,yne);
					b_pos.Set(xsw,ysw);
					a_val = dNE;
					b_val = dSW;
					float xsw01, ysw01;
					CheckEdge(metaball_threshold, a_pos, b_pos, a_val, b_val, &xsw01, &ysw01);
					
					m.mUV[ii].Set(usw,vsw);
					m.mUV[ii+1].Set(unw,vnw);
					m.mUV[ii+2].Set(une,vne);
					
					m.mVertices[ii].Set(xsw01,ysw01);
					m.mVertices[ii+1].Set(xnw,ynw);
					m.mVertices[ii+2].Set(xne,yne);
					
					//mColors[ii] = c;
					//mColors[ii] = c;
					//mColors[ii] = c;
					
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					//glTexCoord2f( usw, vsw); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xsw01, ysw01 );
					//glTexCoord2f( unw, vnw); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xnw, ynw );
					//glTexCoord2f( une, vne); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xne, yne );
					
					ii+=3;
					
					a_pos.Set(xnw,ynw);
					//b_pos.Set(xse,yse);
					a_val = dNW;
					b_val = dSW;
					float xsw02, ysw02;
					CheckEdge(metaball_threshold, a_pos,b_pos,a_val, b_val, &xsw02, &ysw02);
					
					m.mUV[ii].Set(unw,vnw);
					m.mUV[ii+1].Set(usw,vsw);
					m.mUV[ii+2].Set(usw,vsw);
					
					m.mVertices[ii].Set(xnw,ynw);		
					m.mVertices[ii+1].Set(xsw02,ysw02);	
					m.mVertices[ii+2].Set(xsw01,ysw01);	
					
					//mColors[ii] = c;
					//mColors[ii] = c;
					//mColors[ii] = c;
					
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					//glTexCoord2f( unw, vnw); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xnw, ynw );
					//glTexCoord2f( usw, vsw); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xsw02, ysw02 );
					//glTexCoord2f( usw, vsw); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xsw01, ysw01 );
					ii+=3;
				}
			}else if(numAboveThresholdTri2 == 1){
				//Cases 1,2,4
				if(bSW){
					
					//case 1 
					a_pos.Set(xsw,ysw);
					b_pos.Set(xne,yne);
					a_val = dSW;
					b_val = dNE;
					float xne01,yne01;
					CheckEdge(metaball_threshold, a_pos, b_pos, a_val, b_val, &xne01, &yne01);
					
					a_pos.Set(xsw,ysw);
					b_pos.Set(xnw,ynw);
					a_val = dSW;
					b_val = dNW;
					float xnw01,ynw01;
					CheckEdge(metaball_threshold, a_pos,b_pos,a_val, b_val, &xnw01, &ynw01);
					
					m.mUV[ii].Set(usw,vsw);
					m.mUV[ii+1].Set(une,vne);
					m.mUV[ii+2].Set(unw,vnw);
					
					m.mVertices[ii].Set(xsw,ysw);		
					m.mVertices[ii+1].Set(xne01,yne01);	
					m.mVertices[ii+2].Set(xnw01,ynw01);	
					
					//mColors[ii] = c;
					//mColors[ii] = c;
					//mColors[ii] = c;
					
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					//glTexCoord2f( usw, vsw); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xsw, ysw );
					//glTexCoord2f( une, vne); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xne01, yne01 );
					//glTexCoord2f( unw, vnw); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xnw01, ynw01 );
					ii+=3;
				}else if(bNW){
					
					//case 2 
					a_pos.Set(xnw,ynw);
					b_pos.Set(xne,yne);
					a_val = dNW;
					b_val = dNE;
					float xne01,yne01;
					CheckEdge(metaball_threshold, a_pos, b_pos, a_val, b_val, &xne01, &yne01);
					
					a_pos.Set(xnw,ynw);
					b_pos.Set(xsw,ysw);
					a_val = dNW;
					b_val = dSW;
					float xsw01,ysw01;
					CheckEdge(metaball_threshold, a_pos,b_pos,a_val, b_val, &xsw01, &ysw01);
					
					m.mUV[ii].Set(usw,vsw);
					m.mUV[ii+1].Set(une,vne);
					m.mUV[ii+2].Set(unw,vnw);
					
					m.mVertices[ii].Set(xsw01,ysw01);
					m.mVertices[ii+1].Set(xne01,yne01);
					m.mVertices[ii+2].Set(xnw,ynw);	
					
					//mColors[ii] = c;
					//mColors[ii] = c;
					//mColors[ii] = c;
					
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					//glTexCoord2f( usw, vsw); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xsw01, ysw01 );
					//glTexCoord2f( une, vne); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xne01, yne01 );
					//glTexCoord2f( unw, vnw); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xnw, ynw );
					ii+=3;
					
				}else{
					//bNE
					//case 4 
					a_pos.Set(xne,yne);
					b_pos.Set(xnw,ynw);
					a_val = dNE;
					b_val = dNW;
					float xnw01,ynw01;
					CheckEdge(metaball_threshold, a_pos, b_pos, a_val, b_val, &xnw01, &ynw01);
					
					a_pos.Set(xne,yne);
					b_pos.Set(xsw,ysw);
					a_val = dNE;
					b_val = dSW;
					float xsw01,ysw01;
					CheckEdge(metaball_threshold, a_pos,b_pos,a_val, b_val, &xsw01, &ysw01);
					
					m.mUV[ii].Set(usw,vsw);
					m.mUV[ii+1].Set(une,vne);
					m.mUV[ii+2].Set(unw,vnw);
					
					m.mVertices[ii].Set(xsw01,ysw01);
					m.mVertices[ii+1].Set(xne,yne);		
					m.mVertices[ii+2].Set(xnw01,ynw01);
					
					//mColors[ii] = c;
					//mColors[ii] = c;
					//mColors[ii] = c;
					
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					mIndices.push_back(startIDX++);
					//glTexCoord2f( usw, vsw); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xsw01, ysw01 );
					//glTexCoord2f( une, vne); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xne, yne );
					//glTexCoord2f( unw, vnw); glColor3f ( 1.0f, 1.0f, 1.0f ); glVertex2f ( xnw01, ynw01 );
					ii+=3;
				}
				
			}
			
			
			
		}
	}
	
	//glEnd ();
	//glDisable		( GL_TEXTURE_2D );
	m.Unlock();
	ConstructMeshFromLocalData();
	
}

////////////////////////////////////////////////////////////////////////////////
void MetaballGrid::ConstructMeshFromLocalData()
{
	Mesh& m = mGame.GetRenderer().GetMesh(mMeshes[0]);
	m.Lock();

	int triCount = mIndices.size()/3;
	//if(triCount > m.mTriangleCount)
	//{
		SAFE_DELETE_ARRAY(m.mTriangles);
		m.mTriangleCount = triCount;
		m.mTriangles = new unsigned short[mIndices.size()];
	//}
	
	for(int i = 0; i < (int)mIndices.size(); i++)
	{
		m.mTriangles[i] = mIndices[i];
	}
	m.Unlock();
	
	
}
////////////////////////////////////////////////////////////////////////////////
void MetaballGrid::ExpandMesh()
{
	int multiplier = 4;
	Mesh& m = mGame.GetRenderer().GetMesh(mMeshes[0]);
	m.Lock();
	//m.Clear();
	m.mVertexCount = m.mVertexCount*multiplier;
	m.mNormalCount = m.mNormalCount*multiplier;
	m.mColorCount = m.mColorCount*multiplier;
	m.mUVCount = m.mUVCount*multiplier;
	m.mUV2Count = m.mUV2Count*multiplier;
	m.mTriangleCount = m.mTriangleCount*multiplier;
	
	//SAFE_DELETE_ARRAY(m.mVertices);
	//SAFE_DELETE_ARRAY(m.mNormals);
	//SAFE_DELETE_ARRAY(m.mColors);
	//SAFE_DELETE_ARRAY(m.mUV);
	
	m.mVertices = new IvVector3[m.mVertexCount];
	m.mNormals = new IvVector3[m.mNormalCount];
	m.mColors = new IvColor[m.mColorCount];
	m.mUV = new IvVector2[m.mUVCount];
	m.mUV2 = new IvVector2[m.mUVCount];
	
	m.mTriangles = new unsigned short[m.mTriangleCount*3];
	IvVector3 p;
	IvColor c(1,1,1,1);
	IvVector2 uv;
	for(int i = 0; i < (int)m.mVertexCount; i++)
	{
		m.mVertices[i] = p;
		m.mNormals[i] = p;
	}
	
	
	for(int i = 0; i < (int)m.mColorCount; i++)
	{
		m.mColors[i] = c;
	}
	
	for(int i = 0; i < (int)m.mUVCount; i++)
	{
		m.mUV[i] = uv;
		m.mUV2[i] = uv;
	}	
	
	for(int i = 0; i < (int)m.mTriangleCount*3; i++)
	{
		m.mTriangles[i]= 0;
	}
	m.Unlock();
	
	
}
////////////////////////////////////////////////////////////////////////////////
void MetaballGrid::AddObject(unsigned int id, unsigned int creator, IvVector3& pos, float radius, float power, bool leavesTrail, bool eraseable, bool erasesOthers)
{
	
	MetaballTrackingData td;
	std::map<unsigned int, MetaballTrackingData>::iterator dbEntry = mGameObjectDB.find(id);
	if(dbEntry == mGameObjectDB.end())
	{
		td.mGameID = id;
		td.mCreatorID = creator;
		td.mLastPosition = pos;
		td.mRadius = radius;
		td.mLastUpdateTime = td.mCreationTime = GetTicks();
		td.mPower = power;
		td.mLeavesTrail = leavesTrail;
		td.mErasable = eraseable;
		td.mEraseOthers = erasesOthers;
		mGameObjectDB[id] = td;
		EffectStaticField(td.mLastPosition.x,td.mLastPosition.y, td.mRadius, td.mPower);
	}
	/*else
	{
		td = (*dbEntry).second;
		//if(td.mLastPosition != pos || td.mRadius != radius || td.mPower != power)
		EffectStaticField(td.mLastPosition.x,td.mLastPosition.y, td.mRadius, -td.mPower);
		
		td.mGameID = id;
		
		td.mLastPosition = pos;
		td.mRadius = radius;
		td.mLastUpdateTime = GetTicks();
		td.mPower = power;
		mGameObjectDB[id] = td;
		EffectStaticField(td.mLastPosition.x,td.mLastPosition.y, td.mRadius, td.mPower);
	}*/
	
}
////////////////////////////////////////////////////////////////////////////////
void MetaballGrid::RemoveObject(unsigned int id)
{
	std::map<unsigned int, MetaballTrackingData>::iterator dbEntry = mGameObjectDB.find(id);
	if(dbEntry != mGameObjectDB.end())
	{
		MetaballTrackingData td = mGameObjectDB[id];
		EffectStaticField(td.mLastPosition.x,td.mLastPosition.y, td.mRadius, -td.mPower);
		mGameObjectDB.erase(dbEntry);
	}
	
	//mPendingDeletions.push_back(id);
}
////////////////////////////////////////////////////////////////////////////////
void MetaballGrid::MakePendingRemovals(){
	std::list<unsigned int>::iterator it = mPendingDeletions.begin();
	while(it != mPendingDeletions.end())
	{
		GameID id = *it;
		std::map<unsigned int, MetaballTrackingData>::iterator dbEntry = mGameObjectDB.find(id);
		if(dbEntry != mGameObjectDB.end())
		{
			MetaballTrackingData td = mGameObjectDB[id];
			EffectStaticField(td.mLastPosition.x,td.mLastPosition.y, td.mRadius, -td.mPower);
			mGameObjectDB.erase(dbEntry);
		}
		++it;
	}
	mPendingDeletions.clear();
}
////////////////////////////////////////////////////////////////////////////////
void MetaballGrid::ObjectMoved(unsigned int id, IvVector3& newPos, IvVector3& velocity)
{
	//if(newPos.x < 0 || newPos.x > mWindowWidth || newPos.y < 0 || newPos.y > mWindowHeight-20)
	//	return;
	std::map<unsigned int, MetaballTrackingData>::iterator dbEntry = mGameObjectDB.find(id);
	if(dbEntry != mGameObjectDB.end())
	{
		MetaballTrackingData td = (*dbEntry).second;
		EffectStaticField(td.mLastPosition.x,td.mLastPosition.y, td.mRadius, -td.mPower);
		
		if(td.mLeavesTrail)
		{
			//Placeholder trail
			unsigned int time = GetTicks();
			float noise = Perlin::Noise(td.mLastPosition.x*10.0f,td.mLastPosition.y*10.0f,time*0.001f);
			float noise2 = Perlin::Noise(time*0.001f,td.mLastPosition.x*0.01f);
			float normNoise = 0.5f*noise + 0.5f;
			//float normNoise2 =(0.5f*noise2 + 0.5f)*1.5f;
			
			//Get metaball field value at pos
			
			float xx = newPos.x/mWindowWidth;
			float yy = newPos.y/mWindowHeight;
			int ii = (int) (xx*_NX);//Assumes x is between 0 and 1
			int jj = (int) (yy*_NY);//Assumes y is between 0 and 1
			int idx = IX(ii,jj);
			float fieldValue = mMetaballFieldValues[idx];
			
			if(normNoise >= 0.6f){
				//Ramp down power at bottom of screen
				float heightScale = 1.0f -((480.0f-newPos.y)/480.0f);//Using 1.0f - value because we want to biad towards min in lerp.
				
				float minRadius = 0.025f;
				float maxRadius = 0.035f;
				float newRadius = LERP(normNoise,minRadius,maxRadius);
				
				float minPower = 0.35f;
				float maxPower = 0.55f;
				float newPower = LERP(normNoise*heightScale,minPower,maxPower);
				//newRadius = CLAMP(newRadius,minRadius,maxRadius);
			
				float distSqr = DistanceSquared(td.mLastPosition,td.mLastDripPosition);
				float r1 = td.mLastDripRadius*td.mLastDripRadius;
				float r2 = newRadius*newRadius;
				//block out the tile area
				if(!(newPos.x > 40 && newPos.x < 290 && newPos.y > 35 && newPos.y < 90))//Hardcoded hack
				{
					//block out trails near edges... looks bad
					if(!(newPos.y < 15 || newPos.x < 5 || newPos.x > (320-5)))
					{
						if(distSqr >  (r1+r2)*480.0f + (64.0f))//64 pixel gap
						{
							if(fieldValue < 0.001f)
							{
								td.mLastDripRadius = newRadius;
								IvVector3 newPos2(td.mLastPosition.x+(noise2*20.0f),td.mLastPosition.y+(noise2*0.0f),td.mLastPosition.z);
								td.mLastDripPosition = newPos2; 
								AddObject(Game::GetNextGameID(), td.mGameID, newPos2, newRadius, newPower, false,true, false);
								//EffectStaticField(newPos2.x,newPos2.y, newRadius,newPower);
							}
						}
					}
				}
				
			}
			else if(normNoise < 0.3f)
			{
				IvVector3 v = velocity;
				float speed = v.Length();
				if(speed > 200.0f && td.mRadius > 0.03f)
				{
					v.Set(0,-speed*0.4f + rand()%((int)(speed*0.2f)),0);
					//NOTE: Make sure radius is smaller than original drop;
					DropManager::GetInstance().AddDrop(td.mGameID,newPos,v,false,false,0.046f, 0.5f);
				}
				
			}	 
		}
		
		
		//Drop absorbtion logic
		std::list<MetaballTrackingData> dropList;
		if(td.mEraseOthers)
		{
			GetIntersectingDrops(dropList,td);
			std::list<MetaballTrackingData>::iterator dIt = dropList.begin();
			while(dIt != dropList.end())
			{
				MetaballTrackingData d = *dIt;
				if(!(td.mGameID == d.mCreatorID))
				{
					if(td.mRadius < d.mRadius)
						newPos = d.mLastPosition;
					
					//td.mRadius += dIt->mRadius *0.5f;
					td.mPower += d.mPower * 0.05f;
					RemoveObject(d.mGameID);
				}
				++dIt;
			}
			
		}
		
		td.mLastUpdateTime = GetTicks();
		td.mLastPosition = newPos;
		
		mGameObjectDB[id] = td;
		EffectStaticField(td.mLastPosition.x,td.mLastPosition.y, td.mRadius, td.mPower);
	}
}
////////////////////////////////////////////////////////////////////////////////
void MetaballGrid::GetIntersectingDrops(std::list<MetaballTrackingData>& dropList, MetaballTrackingData& td)
{
	std::map<unsigned int, MetaballTrackingData>::iterator it = mGameObjectDB.begin();
	while(it != mGameObjectDB.end())
	{
		if(it->second.mErasable && td.mGameID != it->second.mGameID)
		{
			float r = td.mRadius*320.0f + it->second.mRadius*320.0f;//it->second.mRadius*320.0f;//Since drops move vertically i can use the width as a radius without worrying
			IvVector3 pos = it->second.mLastPosition;
			IvVector3 diff = pos - td.mLastPosition;
			diff.z = 0.0f;
			float distSqr = diff.LengthSquared();
			float radiiSqr = r*r;
			if(distSqr <= radiiSqr)
			{
				dropList.push_back(it->second);
			}
		}
		++it;
	}
}


////////////////////////////////////////////////////////////////////////////////
void MetaballGrid::ClearGridValues()
{
	mGameObjectDB.clear();
	memset(mMetaballFieldValues, 0, sizeof(float)*_gridCellsCount);
	memset(mMetaballGradientValues, 0, sizeof(float)*_gridCellsCount);
}
////////////////////////////////////////////////////////////////////////////////


