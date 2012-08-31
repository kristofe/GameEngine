#include <assert.h>
#include "SDL.h"
#ifndef TARGET_OS_IPHONE
#include "SDL_Image.h"
//#include "ImageUtils.h"
#endif
#include "ImageUtils.h"

#include "Platform.h"
#include "Globals.h"
#include "Game.h"
#include "Mesh.h"
#include "Material.h"
#include "TextureFont.h"
#include "RendererObjectFactory.h"

////////////////////////////////////////////////////////////////////////////////
RendererObjectFactory* RendererObjectFactory::mInstance = NULL;

RendererObjectFactory& RendererObjectFactory::GetInstance()
{
	if(mInstance == NULL)
	{
		mInstance = new RendererObjectFactory();
	}
	return *mInstance;

}
////////////////////////////////////////////////////////////////////////////////
RendererObjectFactory::RendererObjectFactory()
{
	//mMaterialsDB	= new std::map<const char*,GameID>();
	//mMeshesDB		= new std::map<const char*,GameID>();
	//mModelsDB		= new std::map<const char*,RendererModelID>();
	//mTexturesDB		= new std::map<const char*,RendererTextureID>();
}

////////////////////////////////////////////////////////////////////////////////
RendererObjectFactory::~RendererObjectFactory()
{
	//SAFE_DELETE(mMaterialsDB);
	//SAFE_DELETE(mMeshesDB);
	//SAFE_DELETE(mModelsDB);
	//SAFE_DELETE(mTexturesDB);
}
/*
////////////////////////////////////////////////////////////////////////////////
GameID RendererObjectFactory::CloneMesh(GameID id)
{
	assert("CloneMesh() Not Implemented" && 0);
	return NULL;
}
*/
////////////////////////////////////////////////////////////////////////////////
GameID RendererObjectFactory::GetMesh(std::string name)
{
	std::map<std::string,GameID>::iterator it;
	RendererObjectFactory& rf = RendererObjectFactory::GetInstance();
	it = rf.mMeshesDB.find(name);
	if(it != rf.mMeshesDB.end())//Mesh with desired name already exists
	{
		return (*it).second;
	}
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
GameID RendererObjectFactory::CloneMesh(GameID id, std::string name)
{
	Mesh& m = RendererObjectFactory::GetMesh(id);
	GameID meshID = RendererObjectFactory::CreateMesh(name.c_str());
	Mesh& clone = RendererObjectFactory::GetMesh(meshID);
	m.Clone(clone);
	return clone.mGameID;
}
////////////////////////////////////////////////////////////////////////////////
void RendererObjectFactory::RemoveMesh(std::string name)
{
	std::map<std::string,GameID>::iterator it;
	RendererObjectFactory& rf = RendererObjectFactory::GetInstance();
	it = rf.mMeshesDB.find(name);
	if(it != rf.mMeshesDB.end())//Mesh with desired name already exists
	{
		rf.mMeshesDB.erase(it);
	}

}
////////////////////////////////////////////////////////////////////////////////
GameID RendererObjectFactory::GetUnitPlaneMesh()
{
	GameID id = RendererObjectFactory::GetMesh(QUOTEME(UNIT_PLANE_MESH));
	if(id == (GameID)NULL)
	{
		id = RendererObjectFactory::CreateUnitPlaneMesh();
	}
	
	return id;
}
////////////////////////////////////////////////////////////////////////////////
GameID RendererObjectFactory::GetUnitCubeMesh()
{
	GameID id = RendererObjectFactory::GetMesh(QUOTEME(UNIT_CUBE_MESH));
	if(id == (GameID)NULL)
	{
		id = RendererObjectFactory::CreateUnitCubeMesh();
	}
	
	return id;
}
////////////////////////////////////////////////////////////////////////////////
GameID RendererObjectFactory::CreateMesh(std::string name)
{
	//Do we check to see if there already is a default unit plane
	GameID id = RendererObjectFactory::GetMesh(name);
	if(id != (GameID)NULL)
	{
		return id;
	}

	Renderer& r = Game::GetInstance().GetRenderer();
	//Mesh* newMesh = new Mesh();
	Mesh& m = r.CreateMesh(name.c_str());
	
	RendererObjectFactory& rf = RendererObjectFactory::GetInstance();
	rf.mMeshesDB[name] = m.mGameID;

	return m.mGameID;

}
////////////////////////////////////////////////////////////////////////////////
GameID RendererObjectFactory::CreateUnitPlaneMesh()
{
	//Do we check to see if there already is a default unit plane
	GameID id = RendererObjectFactory::GetMesh(QUOTEME(UNIT_PLANE_MESH));
	if(id != (GameID)NULL)
	{
		return id;
	}
	id = RendererObjectFactory::CreateGridMesh(QUOTEME(UNIT_PLANE_MESH));
	Game::GetInstance().GetRenderer().Retain(0,id);
	return id;
	/*
	id = RendererObjectFactory::CreateMesh(QUOTEME(UNIT_PLANE_MESH));
	Mesh& m = RendererObjectFactory::GetMesh(id);

	m.mVertexCount = 4;
	m.mNormalCount = 4;
	m.mColorCount = 4;
	m.mUVCount = 4;
	m.mTriangleCount = 2;

	m.mVertices = new IvVector3[m.mVertexCount];
	m.mNormals = new IvVector3[m.mNormalCount];
	m.mColors = new IvColor[m.mColorCount];
	m.mUV = new IvVector2[m.mUVCount];

	m.mTriangles = new unsigned int[m.mTriangleCount * 3];

	m.mVertices[0].Set(0,0,0);
	m.mVertices[1].Set(1,0,0);
	m.mVertices[2].Set(1,1,0);
	m.mVertices[3].Set(0,1,0);

	m.mNormals[0].Set(0,0,-1);
	m.mNormals[1].Set(0,0,-1);
	m.mNormals[2].Set(0,0,-1);
	m.mNormals[3].Set(0,0,-1);

	m.mColors[0].Set(1,1,1,1);
	m.mColors[1].Set(1,1,1,1);
	m.mColors[2].Set(1,1,1,1);
	m.mColors[3].Set(1,1,1,1);

	m.mUV[0].Set(0,0);
	m.mUV[1].Set(1,0);
	m.mUV[2].Set(1,1);
	m.mUV[3].Set(0,1);

	m.mTriangles[0] = 0;
	m.mTriangles[1] = 2;
	m.mTriangles[2] = 3;

	m.mTriangles[3] = 0;
	m.mTriangles[4] = 1;
	m.mTriangles[5] = 2;

	

	return m.mGameID;
	*/

}
////////////////////////////////////////////////////////////////////////////////
GameID RendererObjectFactory::CreateUnitCubeMesh()
{
	//This mesh has separate vertices per face... every face is independent
	//Do we check to see if there already is a default unit plane
	GameID id = RendererObjectFactory::GetMesh(QUOTEME(UNIT_CUBE_MESH));
	if(id != (GameID)NULL)
	{
		return id;
	}
	
	id = RendererObjectFactory::CreateMesh(QUOTEME(UNIT_CUBE_MESH));
	Game::GetInstance().GetRenderer().Retain(0,id);
	Mesh& m = RendererObjectFactory::GetMesh(id);

	m.mVertexCount = 4*6;
	m.mNormalCount = 4*6;
	m.mColorCount = 4*6;
	m.mUVCount = 4*6;
	m.mTriangleCount = 2*6;

	m.mVertices = new IvVector3[m.mVertexCount];
	m.mNormals = new IvVector3[m.mNormalCount];
	m.mColors = new IvColor[m.mColorCount];
	m.mUV = new IvVector2[m.mUVCount];

	m.mTriangles = new unsigned short[m.mTriangleCount * 3];

	//Front face
	m.mVertices[0].Set(-0.5f,-0.5f, 0.5f);//(0,0,0);
	m.mVertices[1].Set( 0.5f,-0.5f, 0.5f);//(1,0,0);
	m.mVertices[2].Set( 0.5f, 0.5f, 0.5f);//(1,1,0);
	m.mVertices[3].Set(-0.5f, 0.5f, 0.5f);//(0,1,0);

	m.mNormals[0].Set(0,0,1);
	m.mNormals[1].Set(0,0,1);
	m.mNormals[2].Set(0,0,1);
	m.mNormals[3].Set(0,0,1);

	m.mColors[0].Set(1.0f,1.0f,1.0f,1.0f);
	m.mColors[1].Set(1.0f,1.0f,1.0f,1.0f);
	m.mColors[2].Set(1.0f,1.0f,1.0f,1.0f);
	m.mColors[3].Set(1.0f,1.0f,1.0f,1.0f);

	m.mUV[0].Set(0,1);
	m.mUV[1].Set(1,1);
	m.mUV[2].Set(1,0);
	m.mUV[3].Set(0,0);

	m.mTriangles[0] = 0;
	m.mTriangles[1] = 2;
	m.mTriangles[2] = 3;

	m.mTriangles[3] = 0;
	m.mTriangles[4] = 1;
	m.mTriangles[5] = 2;


	//Back face
	m.mVertices[4].Set(-0.5f,-0.5f,-0.5f);
	m.mVertices[5].Set( 0.5f,-0.5f,-0.5f);
	m.mVertices[6].Set( 0.5f, 0.5f,-0.5f);
	m.mVertices[7].Set(-0.5f, 0.5f,-0.5f);

	m.mNormals[4].Set(0,0,-1);
	m.mNormals[5].Set(0,0,-1);
	m.mNormals[6].Set(0,0,-1);
	m.mNormals[7].Set(0,0,-1);

	m.mColors[4].Set(1.0f,1.0f,1.0f,1.0f);
	m.mColors[5].Set(1.0f,1.0f,1.0f,1.0f);
	m.mColors[6].Set(1.0f,1.0f,1.0f,1.0f);
	m.mColors[7].Set(1.0f,1.0f,1.0f,1.0f);

	m.mUV[4].Set(0,1);
	m.mUV[5].Set(1,1);
	m.mUV[6].Set(1,0);
	m.mUV[7].Set(0,0);

	m.mTriangles[6] = 4;//add 6
	m.mTriangles[7] = 7;
	m.mTriangles[8] = 6;

	m.mTriangles[9] = 4;
	m.mTriangles[10] = 6;
	m.mTriangles[11] = 5;
	///////////////////////////////////////////////////////

	//top face - x/z plane
	m.mVertices[8].Set( -0.5f,0.5f,-0.5f);
	m.mVertices[9].Set(  0.5f,0.5f,-0.5f);
	m.mVertices[10].Set( 0.5f,0.5f, 0.5f);
	m.mVertices[11].Set(-0.5f,0.5f, 0.5f);

	m.mNormals[8].Set(0,1,0);
	m.mNormals[9].Set(0,1,0);
	m.mNormals[10].Set(0,1,0);
	m.mNormals[11].Set(0,1,0);

	m.mColors[8].Set(1,1,1,1);
	m.mColors[9].Set(1,1,1,1);
	m.mColors[10].Set(1,1,1,1);
	m.mColors[11].Set(1,1,1,1);

	m.mUV[8].Set(0,1);
	m.mUV[9].Set(1,1);
	m.mUV[10].Set(1,0);
	m.mUV[11].Set(0,0);

	m.mTriangles[12] = 8;//add 6
	m.mTriangles[13] = 10;
	m.mTriangles[14] = 11;

	m.mTriangles[15] = 8;
	m.mTriangles[16] = 9;
	m.mTriangles[17] = 10;

	//bottom face x/z plane - reorder indices
	m.mVertices[12].Set(-0.5f,-0.5f,-0.5f);
	m.mVertices[13].Set( 0.5f,-0.5f,-0.5f);
	m.mVertices[14].Set( 0.5f,-0.5f, 0.5f);
	m.mVertices[15].Set(-0.5f,-0.5f, 0.5f);

	m.mNormals[12].Set(0,-1,0);
	m.mNormals[13].Set(0,-1,0);
	m.mNormals[14].Set(0,-1,0);
	m.mNormals[15].Set(0,-1,0);

	m.mColors[12].Set(1,1,1,1);
	m.mColors[13].Set(1,1,1,1);
	m.mColors[14].Set(1,1,1,1);
	m.mColors[15].Set(1,1,1,1);

	m.mUV[12].Set(0,1);
	m.mUV[13].Set(1,1);
	m.mUV[14].Set(1,0);
	m.mUV[15].Set(0,0);

	m.mTriangles[18] = 12;//add 6
	m.mTriangles[19] = 15;
	m.mTriangles[20] = 14;

	m.mTriangles[21] = 12;
	m.mTriangles[22] = 14;
	m.mTriangles[23] = 13;

	//left face y/z plane
	m.mVertices[16].Set(-0.5f,-0.5f,-0.5f);
	m.mVertices[17].Set(-0.5f, 0.5f,-0.5f);
	m.mVertices[18].Set(-0.5f, 0.5f, 0.5f);
	m.mVertices[19].Set(-0.5f,-0.5f, 0.5f);

	m.mNormals[16].Set(-1,0,0);
	m.mNormals[17].Set(-1,0,0);
	m.mNormals[18].Set(-1,0,0);
	m.mNormals[19].Set(-1,0,0);

	m.mColors[16].Set(1,1,1,1);
	m.mColors[17].Set(1,1,1,1);
	m.mColors[18].Set(1,1,1,1);
	m.mColors[19].Set(1,1,1,1);

	m.mUV[16].Set(0,1);
	m.mUV[17].Set(1,1);
	m.mUV[18].Set(1,0);
	m.mUV[19].Set(0,0);

	m.mTriangles[24] = 16;//add 6
	m.mTriangles[25] = 18;
	m.mTriangles[26] = 19;

	m.mTriangles[27] = 16;
	m.mTriangles[28] = 17;
	m.mTriangles[29] = 18;

	//right face y/z
	m.mVertices[20].Set(0.5f,-0.5f,-0.5f);
	m.mVertices[21].Set(0.5f, 0.5f,-0.5f);
	m.mVertices[22].Set(0.5f, 0.5f, 0.5f);
	m.mVertices[23].Set(0.5f,-0.5f, 0.5f);

	m.mNormals[20].Set(1,0,0);
	m.mNormals[21].Set(1,0,0);
	m.mNormals[22].Set(1,0,0);
	m.mNormals[23].Set(1,0,0);

	m.mColors[20].Set(1,1,1,1);
	m.mColors[21].Set(1,1,1,1);
	m.mColors[22].Set(1,1,1,1);
	m.mColors[23].Set(1,1,1,1);

	m.mUV[20].Set(0,1);
	m.mUV[21].Set(1,1);
	m.mUV[22].Set(1,0);
	m.mUV[23].Set(0,0);

	m.mTriangles[30] = 20;//add 6
	m.mTriangles[31] = 23;
	m.mTriangles[32] = 22;

	m.mTriangles[33] = 20;
	m.mTriangles[34] = 22;
	m.mTriangles[35] = 21;

	return m.mGameID;
	
}

////////////////////////////////////////////////////////////////////////////////
/*GameID RendererObjectFactory::CloneMaterial()
{
	assert("CloneMaterial() Not Implemented" && false);
	return NULL;
}*/
////////////////////////////////////////////////////////////////////////////////
GameID RendererObjectFactory::CreateMaterial(std::string name)
{
	GameID id = RendererObjectFactory::GetMaterial(name);
	if(id != (GameID)NULL)
	{
		return id;
	}
	Renderer& r = Game::GetInstance().GetRenderer();
	Material& m = r.CreateMaterial(name.c_str());
	m.mAmbient.Set(0.5,0.5,0.5,1);
	m.mDiffuse.Set(1,1,1,1);
	m.mTextureCount = 0;

	RendererObjectFactory& rf = RendererObjectFactory::GetInstance();
	rf.mMaterialsDB[name] = m.mGameID;
	return m.mGameID;

	
	

}
////////////////////////////////////////////////////////////////////////////////
GameID RendererObjectFactory::GetMaterial(std::string name)
{
	std::map<std::string,GameID>::iterator it;
	RendererObjectFactory& rf = RendererObjectFactory::GetInstance();
	it = rf.mMaterialsDB.find(name);
	if(it != rf.mMaterialsDB.end())//Material with desired name already exists
	{
		return (*it).second;
	}
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
void RendererObjectFactory::RemoveMaterial(std::string name)
{
	std::map<std::string,GameID>::iterator it;
	RendererObjectFactory& rf = RendererObjectFactory::GetInstance();
	it = rf.mMaterialsDB.find(name);
	if(it != rf.mMaterialsDB.end())//Material with desired name already exists
	{
		rf.mMaterialsDB.erase(it);
	}
	

	it = rf.mFontsDB.find(name);
	if(it != rf.mFontsDB.end())//Material with desired name already exists
	{
		rf.mFontsDB.erase(it);
	}

}
////////////////////////////////////////////////////////////////////////////////
GameID RendererObjectFactory::GetDefaultMaterial()
{
	GameID id = RendererObjectFactory::GetMaterial(QUOTEME(DEFAULT_MATERIAL));
	if(id != (GameID)NULL)
	{
		return id;
	}
	id = RendererObjectFactory::CreateMaterial(QUOTEME(DEFAULT_MATERIAL));
	Game::GetInstance().GetRenderer().Retain(0,id);
	return id;
}

////////////////////////////////////////////////////////////////////////////////
Mesh& RendererObjectFactory::GetMesh(GameID meshID)
{
	return Game::GetInstance().GetRenderer().GetMesh(meshID);
}

////////////////////////////////////////////////////////////////////////////////
Material& RendererObjectFactory::GetMaterial(GameID matID)
{
	return Game::GetInstance().GetRenderer().GetMaterial(matID);
}
////////////////////////////////////////////////////////////////////////////////
GameID RendererObjectFactory::CreateTexture(std::string name,std::string filename, bool delayLoad)
{
	Renderer& r = Game::GetInstance().GetRenderer();
	Texture& t = r.CreateTexture(name,filename,delayLoad);

	//This should be in the RenderObjectFactory... the loading part.  The uploading to GPU should be in the renderer.
	if(!delayLoad)
		RendererObjectFactory::ReadTextureFromDisk(filename,t);

	RendererObjectFactory::GetInstance().mTexturesDB[t.mName.c_str()] = t.mGameID;
	return t.mGameID;
}
////////////////////////////////////////////////////////////////////////////////
GameID RendererObjectFactory::GetTexture(std::string name)
{
	std::map<std::string,GameID>::iterator it;
	RendererObjectFactory& rf = RendererObjectFactory::GetInstance();
	it = rf.mTexturesDB.find(name);
	if(it != rf.mTexturesDB.end())
	{
		return (*it).second;
	}
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
void RendererObjectFactory::RemoveTexture(std::string name)
{
	//printf("Removing %s\n",name.c_str());
	std::map<std::string,GameID>::iterator it;
	RendererObjectFactory& rf = RendererObjectFactory::GetInstance();
	it = rf.mTexturesDB.find(name);
	if(it != rf.mTexturesDB.end())
	{
		rf.mTexturesDB.erase(it);
	}
}
////////////////////////////////////////////////////////////////////////////////
Texture& RendererObjectFactory::GetTexture(GameID id)
{
	return Game::GetInstance().GetRenderer().GetTexture(id);
}
////////////////////////////////////////////////////////////////////////////////
void RendererObjectFactory::ReadTextureFromDisk(std::string filename, Texture& tex)
{
	//printf("Loading %s\n",tex.mName.c_str());
	std::string fullpath;
	std::string file = filename;
	GetFullFilePathFromResource(file,fullpath);

	tex.mSourceFileName = fullpath;
	tex.mShortFileName = filename;
#ifndef TARGET_OS_IPHONE
	read_png_file(fullpath.c_str(),tex);
	/*
    SDL_Surface* surface = IMG_Load( fullpath.c_str() );  
    if( !surface )
    {
        printf("Could not load %s\n", filename );
        return;
    }    

	
	// Check that the imageís width is a power of 2
	if ( (surface->w & (surface->w - 1)) != 0 ) {
		printf("warning: image.bmpís width is not a power of 2\n");
	}

	// Also check if the height is a power of 2
	if ( (surface->h & (surface->h - 1)) != 0 ) {
		printf("warning: image.bmpís height is not a power of 2\n");
	}

	tex.mNumberOfColors = surface->format->BytesPerPixel;
	tex.mHasAlpha = tex.mNumberOfColors == 4?true:false;
		
	//contains an alpha channel
	if(tex.mNumberOfColors==4)
	{
		
		if(surface->format->Rmask==0x000000ff)
			printf("texture_format=GL_RGBA;");
		else
			printf("texture_format=GL_BGRA;");
	}
	else if(tex.mNumberOfColors==3) //no alpha channel
	{
		if(surface->format->Rmask==0x000000ff)
			printf("texture_format=GL_RGB;");
		else
			printf("texture_format=GL_BGR;");
	}
	else
	{
		printf("warning: the image is not truecolorÖthis will break");
	}

    //int texture_wh = surface->w;      
	
   
	
	SDL_LockSurface(surface);
	int pixelCount = surface->w * surface->h;
	tex.mPixelData = new unsigned int[pixelCount];
	memcpy(tex.mPixelData,surface->pixels,tex.mNumberOfColors * pixelCount);
	
	SDL_UnlockSurface(surface);
	
	tex.mWidth = surface->w;
	tex.mHeight = surface->h;
	tex.mBitsPerPixel = tex.mNumberOfColors*8;

	//SDL_FreeSurface(conv);        
    SDL_FreeSurface(surface);
	*/
#else
	//tex.mNumberOfColors = 4;
	//tex.mHasAlpha = true;
	//tex.mBitsPerPixel = tex.mNumberOfColors*8;
	//::GetTexture((char*)tex.mShortFileName.c_str(), &(tex.mPixelData), (int*)&(tex.mWidth), (int*)&(tex.mHeight));
	
	//TODO: Detect if texture is PVR!!!  
	bool isPVR = false;
	std::string extension = GetFileExtension(tex.mShortFileName);
	if(ToUpper(extension) == "PVR")
		isPVR = true;
	if(isPVR)
	{
		::LoadTextureDataCompressed((char*)tex.mSourceFileName.c_str(),tex);
	}
	else
	{	
		//::LoadTextureData((char*)tex.mShortFileName.c_str(),tex);
		read_png_file(fullpath.c_str(),tex);
	}
#endif

}
////////////////////////////////////////////////////////////////////////////////
GameID RendererObjectFactory::CreateTextureFont(std::string name)
{
	GameID id = RendererObjectFactory::GetTextureFont(name);
	if(id != 0)
	{
		return id;
	}
	Renderer& r = Game::GetInstance().GetRenderer();
	TextureFont& font = r.CreateTextureFont(name);


	RendererObjectFactory& rf = RendererObjectFactory::GetInstance();
	rf.mFontsDB[name] = font.mGameID;
	return font.mGameID;
}
////////////////////////////////////////////////////////////////////////////////
GameID RendererObjectFactory::GetTextureFont(std::string name)
{
	std::map<std::string,GameID>::iterator it;
	RendererObjectFactory& rf = RendererObjectFactory::GetInstance();
	it = rf.mFontsDB.find(name);
	if(it != rf.mFontsDB.end())//Material with desired name already exists
	{
		return (*it).second;
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////
TextureFont& RendererObjectFactory::GetTextureFont(GameID fontID)
{
	return Game::GetInstance().GetRenderer().GetTextureFont(fontID);
}
////////////////////////////////////////////////////////////////////////////////
GameID RendererObjectFactory::CreateGridMesh(std::string name, int widthInCells, int heightInCells, float width, float height)
{
	int gridCellsCount = widthInCells*heightInCells;

	GameID rid = RendererObjectFactory::CreateMesh(name);
	Mesh& m = RendererObjectFactory::GetMesh(rid);

	m.mVertexCount = gridCellsCount;
	m.mNormalCount = gridCellsCount;
	m.mColorCount = gridCellsCount;
	m.mUVCount = gridCellsCount;
	m.mUV2Count = gridCellsCount;
	m.mTriangleCount = ((widthInCells-1)*(heightInCells-1))*2;//the 2 is for 2 tris per cell
	
	m.mVertices = new IvVector3[m.mVertexCount];
	m.mNormals = new IvVector3[m.mNormalCount];
	m.mColors = new IvColor[m.mColorCount];
	m.mUV = new IvVector2[m.mUVCount];
	m.mUV2 = new IvVector2[m.mUV2Count];
	m.mTriangles = new unsigned short[m.mTriangleCount*3];//the 3 is for 3 indices per triangle.. we aren't using any struct or class just a straight array of ints

	
	float dx = 1.0f/(widthInCells-1);
	float dy = 1.0f/(heightInCells-1);

	float currX,currY;
	currX = currY = 0.0f; 
	int index = 0;
	for(int y = 0; y < heightInCells; y++)
	{
		for(int x = 0; x < widthInCells; x++)
		{		
			index = y*widthInCells + x;
			m.mVertices[index].Set(currX*width,currY*height,0.0f);
			m.mColors[index].Set(1.0f,1.0f,1.0f,1.0f);
			m.mUV[index].Set(currX/1.0f,currY/1.0f);
			m.mUV2[index].Set(currX/1.0f,currY/1.0f);
			currX = currX + dx;				
		}
		currY = currY + dy;
		currX = 0.0f;
	}

	
	int id = 0;
	for(int y = 0; y < heightInCells-1; y++)
	{
		for(int x = 0; x < widthInCells-1; x++)
		{
			unsigned short i0 = (y + 0)*widthInCells + (x + 0);
			unsigned short i1 = (y + 0)*widthInCells + (x + 1);
			unsigned short i2 = (y + 1)*widthInCells + (x + 1);
			unsigned short i3 = (y + 1)*widthInCells + (x + 0);
						
			//3 2
			//0 1

			//CCW Vertex Ordering
			m.mTriangles[id++] = i0;
			m.mTriangles[id++] = i2;
			m.mTriangles[id++] = i3;
			
			m.mTriangles[id++] = i0;
			m.mTriangles[id++] = i1;
			m.mTriangles[id++] = i2;
			
		}
	}
	return m.mGameID;
	 
}

////////////////////////////////////////////////////////////////////////////////
GameID RendererObjectFactory::CreateQuadPoolMesh(std::string name, int quadCount, const IvVector2& quadSize)
{   
	GameID rid = RendererObjectFactory::CreateMesh(name);
	Mesh& m = RendererObjectFactory::GetMesh(rid);
    
	m.mVertexCount = quadCount*4;
	m.mNormalCount = quadCount*4;
	m.mColorCount = quadCount*4;
	m.mUVCount = quadCount*4;
	//m.mUV2Count = quadCount*4;
	m.mTriangleCount = quadCount*2;//the 2 is for 2 tris per cell
	
	m.mVertices = new IvVector3[m.mVertexCount];
	m.mNormals = new IvVector3[m.mNormalCount];
	m.mColors = new IvColor[m.mColorCount];
	m.mUV = new IvVector2[m.mUVCount];
	//m.mUV2 = new IvVector2[m.mUV2Count];
	m.mTriangles = new unsigned short[m.mTriangleCount*3];//the 3 is for 3 indices per triangle.. we aren't using any struct or class just a straight array of ints
    
	
	float sx = quadSize.x * 0.5f;
    float sy = quadSize.y * 0.5f;
    
 
	for(int quadIndex = 0; quadIndex < quadCount; quadIndex++)
	{
        int index =  RendererObjectFactory::QuadPoolMeshDataStartIndex(quadIndex);

        m.mVertices[index + 0].Set(-sx,-sy, 0.0f);
        m.mVertices[index + 1].Set( sx,-sy, 0.0f);
        m.mVertices[index + 2].Set( sx, sy, 0.0f);
        m.mVertices[index + 3].Set(-sx, sy, 0.0f);
        
        m.mNormals[index + 0].Set(0,0,1);
        m.mNormals[index + 1].Set(0,0,1);
        m.mNormals[index + 2].Set(0,0,1);
        m.mNormals[index + 3].Set(0,0,1);
        
        m.mColors[index + 0].Set(1.0f,1.0f,1.0f,1.0f);
        m.mColors[index + 1].Set(1.0f,1.0f,1.0f,1.0f);
        m.mColors[index + 2].Set(1.0f,1.0f,1.0f,1.0f);
        m.mColors[index + 3].Set(1.0f,1.0f,1.0f,1.0f);
        
        m.mUV[index + 0].Set(0,1);
        m.mUV[index + 1].Set(1,1);
        m.mUV[index + 2].Set(1,0);
        m.mUV[index + 3].Set(0,0);
        
        int triIndex = RendererObjectFactory::QuadPoolMeshTriangleStartIndex(quadIndex);
        m.mTriangles[triIndex + 0] = index + 0;
        m.mTriangles[triIndex + 1] = index + 2;
        m.mTriangles[triIndex + 2] = index + 3;
        
        m.mTriangles[triIndex + 3] = index + 0;
        m.mTriangles[triIndex + 4] = index + 1;
        m.mTriangles[triIndex + 5] = index + 2;
	}
	return m.mGameID;
    
}

void RendererObjectFactory::RegisterMesh(GameID id,std::string name)
{
	RendererObjectFactory::GetInstance().mMeshesDB[name] = id;
}

void RendererObjectFactory::RegisterMaterial(GameID id,std::string name)
{
	RendererObjectFactory::GetInstance().mMaterialsDB[name] = id;
}

void RendererObjectFactory::RegisterTexture(GameID id,std::string name)
{
	RendererObjectFactory::GetInstance().mTexturesDB[name] = id;
}
void RendererObjectFactory::RegisterTextureFont(GameID id,std::string name)
{
	RendererObjectFactory::GetInstance().mFontsDB[name] = id;
}



