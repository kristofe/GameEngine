#ifndef _MESH_H
#define _MESH_H

#include "IvMathLib.h"
#include "Renderer.h"
#include "RendererObjectFactory.h"

class PersistentData;

enum MeshState{
	MESH_STATE_CLEAN		= 0,
	MESH_STATE_VERTICES	= 1,
	MESH_STATE_NORMALS	= 2,
	MESH_STATE_COLORS		= 4,
	MESH_STATE_UV			= 8,
	MESH_STATE_UV2		= 16,
	MESH_STATE_TRIANGLES	= 32,
	MESH_STATE_EVERYTHING = 0xFFFFFF
};

class Mesh{
	friend class Renderer;
	friend class RendererObjectFactory;

public:
	
	Mesh(std::string name);
	~Mesh();
	
private:
	//Remove Copy Constructor and assignment operator
	Mesh		&operator =  (const Mesh &m) { printf("Mesh = THIS ASSIGNMENT OPERATOR SHOULDN'T BE GETTING CALLED"); return *this;};;
	Mesh(const Mesh &m){printf("Mesh COPY CTOR SHOULDN'T BE GETTING CALLED");};

public:
	IvVector3*			mVertices;
	IvVector3*			mNormals;
	IvColor*			mColors;
	IvVector2*			mUV;
	IvVector2*			mUV2;
	unsigned short*		mTriangles;
	
	unsigned int		mVertexCount;
	unsigned int		mNormalCount;
	unsigned int		mColorCount;
	unsigned int		mUVCount;
	unsigned int		mUV2Count;
	unsigned int		mTriangleCount;
	
	unsigned int		mDirtyFlags;

	GameID			mGameID;
	
	bool				mSerialize;
	bool				mLoadedFromFile;
	std::string			mPathToFile;
	std::string			mName;
	
public:
	GameID GetID(){return mGameID;};
	void Serialize(PersistentData& storageData);
	void DeSerialize(PersistentData& storageData);
	virtual bool		Lock(){return true;};
	virtual bool		Unlock(){return true;};
	virtual void		Clear();
	virtual void		CopyInUVs(IvVector2* uv);
	virtual IvVector2*	SwapUVPointer(IvVector2* uv);
	virtual IvVector2*	SwapUV2Pointer(IvVector2* uv);
	virtual IvColor*	SwapColorPointer(IvColor* c);
	virtual void		Clone(Mesh& m);
	virtual IvVector2*	GetUVsForTextureIdx(int idx){if(idx == 0) return mUV; else return mUV2;}; //Right now meshes only support two sets of texture coords.
	/*
	virtual void		SetDirtyFlags(unsigned int flags) { mDirtyFlags = flags; };
	virtual void		Sync(Mesh* md);
	virtual void		CopyInVertices(unsigned int vertexCount, IvVector3 vertices[]);
	virtual void		CopyInNormals(unsigned int normalCount, IvVector3 normals[]);
	virtual void		CopyInColors(unsigned int IvColorCount, IvColor IvColors[]);
	virtual void		CopyInUVs(unsigned int uv1Count, IvVector2 uv[], unsigned int uv2Count = 0, IvVector2 uv2[] = NULL);
	virtual void		CopyInTriangles(unsigned int tricount, unsigned int triangles[]);
	
	virtual void		CopyOutVertices(unsigned int& vertexCount, IvVector3 vertices[]);
	virtual void		CopyOutNormals(unsigned int& normalCount, IvVector3 normals[]);
	virtual void		CopyOutIvColors(unsigned int& IvColorCount, IvColor IvColors[]);
	virtual void		CopyOutUVs(unsigned int& uv1Count, IvVector2 uv[], unsigned int& uv2Count, IvVector2 uv2[]);
	virtual void		CopyOutTriangles(unsigned int& tricount, unsigned int triangles[]);
	*/
	/*
	virtual void		SetVerticesPointer(unsigned int vertexCount, IvVector3 vertices[]);
	virtual void		SetNormalsPointer(unsigned int normalCount, IvVector3 normals[]);
	virtual void		SetIvColorsPointer(unsigned int IvColorCount, IvColor IvColors[]);
	virtual void		SetUVPointers(unsigned int uv1Count, IvVector2 uv[], unsigned int uv2Count = 0, IvVector2 uv2[] = NULL);
	virtual void		SetTrianglesPointer(unsigned int tricount, unsigned int triangles[]);
	
	virtual void		GetVerticesPointer(unsigned int& vertexCount, IvVector3 vertices[]);
	virtual void		GetNormalsPointer(unsigned int& normalCount, IvVector3 normals[]);
	virtual void		GetIvColorsPointer(unsigned int& IvColorCount, IvColor IvColors[]);
	virtual void		GetUVPointers(unsigned int& uv1Count, IvVector2 uv[], unsigned int& uv2Count, IvVector2 uv2[]);
	virtual void		GetTrianglesPointer(unsigned int& tricount, unsigned int triangles[]);	
	 */
	virtual std::string GetTypeString()
	{
		return "Mesh";
	}
	
	virtual std::string DebugOutput()
	{
		char buf[256];
		sprintf(buf, "%d\tMesh\tName:%s",mGameID,mName.c_str());
		std::string s = buf;
		return s;
	}
};


#endif //_Mesh_H