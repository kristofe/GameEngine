#ifndef _RENDERER_OBJECT_FACTORY_H
#define _RENDERER_OBJECT_FACTORY_H

#include <map>
#include <string>
#include "Renderer.h"

enum RenderObjectPrimitiveTypes
{
	UNIT_PLANE_MESH,
	UNIT_SPHERE_MESH,
	UNIT_CUBE_MESH,
	DEFAULT_MATERIAL_ID
};

class RendererObjectFactory{
	friend class Renderer;

public:
	static RendererObjectFactory& GetInstance();
	RendererObjectFactory();
	~RendererObjectFactory();

	//const char* mPlaneDBName = "UNITPLANE";



	
private:
	//Remove Copy Constructor and assignment operator
	RendererObjectFactory		&operator =  (const RendererObjectFactory &m) { printf("RendererObjectFactory = THIS ASSIGNMENT OPERATOR SHOULDN'T BE GETTING CALLED"); return *this;};;
	RendererObjectFactory(const RendererObjectFactory &m){printf("RendererObjectFactory COPY CTOR SHOULDN'T BE GETTING CALLED");};
	
public:
	std::map<std::string,GameID>			mMaterialsDB;
	std::map<std::string,GameID>			mMeshesDB;
	//std::map<std::string,GameID>			mModelsDB;
	std::map<std::string,GameID>			mTexturesDB;
	std::map<std::string,GameID>			mFontsDB;


public:
	static GameID CloneMesh(GameID id, std::string name);
	static GameID GetUnitPlaneMesh();
	static GameID GetUnitCubeMesh();
	static GameID GetMesh(std::string name);
	static Mesh& GetMesh(GameID meshID);
	static void RemoveMesh(std::string name);
	static GameID CreateMesh(std::string name);
	static GameID CreateUnitPlaneMesh();
	static GameID CreateUnitCubeMesh();
	static GameID CreateGridMesh(std::string name, int widthInCells = 2.0f, int heightInCells = 2.0f, float width = 1.0f, float height = 1.0f);
    static GameID CreateQuadPoolMesh(std::string name, int quadCount, const IvVector2& quadSize);
    inline static int QuadPoolMeshDataStartIndex(int quad){return quad * 4;}
    inline static int QuadPoolMeshTriangleStartIndex(int quad){return quad * 6;}
  	//static GameID CloneMaterial();
	
	static GameID GetMaterial(std::string name);
	static GameID GetDefaultMaterial();
	static Material& GetMaterial(GameID matID);
	static void RemoveMaterial(std::string name);
	static GameID CreateMaterial(std::string name);
	//static RendererModelID GetUnitPlaneModel();
	//static RendererModelID CreateUnitPlaneModel();

	static void ReadTextureFromDisk(std::string filename, Texture& tex);
	static GameID CreateTexture(std::string name, std::string filename,bool delayLoad = false);
	static GameID GetTexture(std::string name);
	static void RemoveTexture(std::string name);
	static Texture& GetTexture(GameID id);

	static GameID CreateTextureFont(std::string name);
	static GameID GetTextureFont(std::string name);
	static TextureFont& GetTextureFont(GameID id);
	
	static void RegisterMesh(GameID id, std::string name);
	static void RegisterMaterial(GameID id, std::string name);
	static void RegisterTexture(GameID id, std::string name);
	static void RegisterTextureFont(GameID id, std::string name);
private:

	static RendererObjectFactory*	mInstance;
};


#endif //_RENDERER_OBJECT_FACTORY_H