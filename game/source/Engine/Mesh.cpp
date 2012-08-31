/*
 *  Mesh.cpp
 *  XPlatformGL
 *
 *  Created by Kristofer Schlachter on 1/6/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Mesh.h"
#include "Globals.h"


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
Mesh::Mesh(std::string name):mName(name){
	mVertices			= NULL;
	mNormals			= NULL;
	mColors				= NULL;
	mUV					= NULL;
	mUV2				= NULL;
	mTriangles			= NULL;
	
	mVertexCount		= 0;
	mNormalCount		= 0;
	mColorCount			= 0;
	mUVCount			= 0;
	mUV2Count			= 0;
	mTriangleCount		= 0;
	mDirtyFlags			= MESH_STATE_CLEAN;
	mSerialize			= true;
}
////////////////////////////////////////////////////////////////////////////////

Mesh::~Mesh(){
	Clear();
}

////////////////////////////////////////////////////////////////////////////////
void Mesh::Serialize(PersistentData& storageData)
{
	if(!mSerialize)
		return;
		
	storageData.SetName("Mesh");
	storageData.SetProperty("LoadedFromFile",mLoadedFromFile);
	storageData.SetProperty("GameID",mGameID);
	storageData.SetProperty("Name",mName.c_str());
	
	if(mLoadedFromFile)
	{
	   storageData.SetProperty("PathToFile",mPathToFile.c_str());
	}
	else
	{
		
		//char buff[128];
		//unsigned int objID;
		PersistentData* node;
		
		node = new PersistentData("Vertices");
		storageData.AddChild(node);
		storageData.SetProperty("VertexCount",mVertexCount);
		for(unsigned int i = 0; i < mVertexCount; ++i)
		{
			node->AddVector3Child(mVertices[i],"Vertex");
		}
		
		node = new PersistentData("Normals");
		storageData.AddChild(node);
		storageData.SetProperty("NormalCount",mNormalCount);
		for(unsigned int i = 0; i < mNormalCount; ++i)
		{
			node->AddVector3Child(mNormals[i],"Normal");
		}
		
		node = new PersistentData("Colors");
		storageData.AddChild(node);
		storageData.SetProperty("ColorCount",mColorCount);
		for(unsigned int i = 0; i < mColorCount; ++i)
		{
			node->AddColorChild(mColors[i],"Color");
		}
		
		node = new PersistentData("UVs");
		storageData.AddChild(node);
		storageData.SetProperty("UVCount",mUVCount);
		for(unsigned int i = 0; i < mUVCount; ++i)
		{
			node->AddVector2Child(mUV[i],"UV");
		}
		
		node = new PersistentData("UV2s");
		storageData.AddChild(node);
		storageData.SetProperty("UV2Count",mUV2Count);
		for(unsigned int i = 0; i < mUV2Count; ++i)
		{
			node->AddVector2Child(mUV2[i],"UV");
		}
		
		std::string triangleList;
		triangleList.reserve(10240);
		node = new PersistentData("Triangles");
		storageData.AddChild(node);
		storageData.SetProperty("TriangleCount",mTriangleCount);
		for(unsigned int i = 0; i < mTriangleCount*3; ++i)
		{
			char buff[32]; 
			sprintf(buff, "%d",mTriangles[i]);
			triangleList += buff;
			if(i != mTriangleCount*3 - 1)
				triangleList +=",";
			
		}
		node->SetTextProperty(triangleList);
	}
	
}
////////////////////////////////////////////////////////////////////////////////
void Mesh::DeSerialize(PersistentData& storageData)
{
	mLoadedFromFile = storageData.GetPropertyB("LoadedFromFile");
	mPathToFile = storageData.GetProperty("PathToFile");
	mGameID = static_cast<GameID> (storageData.GetPropertyU("GameID"));
	mName = storageData.GetProperty("Name");
	RendererObjectFactory::RegisterMesh(mGameID,mName.c_str());
	if(mLoadedFromFile)
		return;/////////THIS SHOULD THEN GO AHEAD AND LOAD THE FILE>>> BUT THAT FUNCTIONALITY DOESN'T EXIST YET
	
	mVertexCount =  storageData.GetPropertyU("VertexCount");
	mNormalCount =  storageData.GetPropertyU("NormalCount");
	mColorCount =  storageData.GetPropertyU("ColorCount");
	mUVCount =  storageData.GetPropertyU("UVCount");
	mUV2Count =  storageData.GetPropertyU("UV2Count");
	mTriangleCount =  storageData.GetPropertyU("TriangleCount");
	
	mVertices = new IvVector3[mVertexCount];
	mNormals = new IvVector3[mNormalCount];
	mColors = new IvColor[mColorCount];
	mUV = new IvVector2[mUVCount];
	mUV2 = new IvVector2[mUV2Count];
	mTriangles = new unsigned short[mTriangleCount];
	
	
	PersistentData* c;
	unsigned int i = 0;
	std::list<PersistentData*> children;
	std::list<PersistentData*>::iterator it;
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// I Don't think that these are going to work... they are going to keep pulling
	// the same child and not go through all of the different ones.  for instance the mesh 
	// will just be copies of the first child in each type...  so the same vertex repeated over
	// and over.  same with uvs etc.
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	storageData.GetNodesFromPath("Vetices",children);
	it = children.begin();
	while(it != children.end() && i < mVertexCount)
	{
		c = (*it);
		mVertices[i] = c->GetVector3Child("Vertex");
		++it;
		++i;
	}

	i = 0;
	storageData.GetNodesFromPath("Normals",children);
	it = children.begin();
	while(it != children.end() && i < mNormalCount)
	{
		c = (*it);
		mNormals[i] = c->GetVector3Child("Normal");
		++it;
		++i;
	}
	
	i = 0;
	storageData.GetNodesFromPath("Colors",children);
	it = children.begin();
	while(it != children.end() && i < (int)mColorCount)
	{
		c = (*it);
		mColors[i] = c->GetColorChild("Color");
		++it;
		++i;
	}
	
	i = 0;
	storageData.GetNodesFromPath("UVs",children);
	it = children.begin();
	while(it != children.end() && i < (int)mUVCount)
	{
		c = (*it);
		mUV[i] = c->GetVector2Child("UV");
		++it;
		++i;
	}
	
	i = 0;
	storageData.GetNodesFromPath("UV2s",children);
	it = children.begin();
	while(it != children.end() && i < (int)mUV2Count)
	{
		c = (*it);
		mUV2[i] = c->GetVector2Child("UV");
		++it;
		++i;
	}
	
	std::string s;
	s.reserve(10240);
	s = storageData.GetProperty("Triangles");
	std::vector<std::string> indices;
	SplitString(s, ',', indices);
	if((int)indices.size() == mTriangleCount*3)
	{
		for(i = 0;i < (int)mTriangleCount*3; ++i)
		{
			mTriangles[i] = atoi(indices[i].c_str());
		}
	}
	else
	{
		printf("Mesh::DeSerialize() - MeshID %d :Triangle index count does not match specified count. indexCount = %d !=  mTriangleCount =%d",mGameID, (int)indices.size(),mTriangleCount);
		
	}
			
	
}
////////////////////////////////////////////////////////////////////////////////
void Mesh::Clone(Mesh& m)
{
	m.Clear();
	m.mVertexCount		= mVertexCount;
	m.mNormalCount		= mNormalCount;
	m.mColorCount		= mColorCount;
	m.mUVCount			= mUVCount;
	m.mUV2Count			= mUV2Count;
	m.mTriangleCount	= mTriangleCount;
	
	m.mVertices = new IvVector3[mVertexCount];
	m.mNormals = new IvVector3[mNormalCount];
	m.mColors = new IvColor[mColorCount];
	m.mUV = new IvVector2[mUVCount];
	m.mUV2 = new IvVector2[mUV2Count];
	m.mTriangles = new unsigned short[mTriangleCount*3];
	
	for(unsigned int i = 0; i < mVertexCount; ++i)
		m.mVertices[i] = mVertices[i];
	
	for(unsigned int i = 0; i < mNormalCount; ++i)
		m.mNormals[i] = mNormals[i];
	
	for(unsigned int i = 0; i < mColorCount; ++i)
		m.mColors[i] = mColors[i];
	
	for(unsigned int i = 0; i < mUVCount; ++i)
		m.mUV[i] = mUV[i];
	
	for(unsigned int i = 0; i < mUV2Count; ++i)
		m.mUV2[i] = mUV2[i];
	
	for(unsigned int i = 0; i < mTriangleCount*3; ++i)
		m.mTriangles[i] =mTriangles[i];
}
////////////////////////////////////////////////////////////////////////////////

void Mesh::Clear(){
	SAFE_DELETE_ARRAY(mVertices);
	SAFE_DELETE_ARRAY(mNormals);
	SAFE_DELETE_ARRAY(mColors);
	SAFE_DELETE_ARRAY(mUV);
	SAFE_DELETE_ARRAY(mUV2);
	SAFE_DELETE_ARRAY(mTriangles);
	
	
	mVertexCount		= 0;
	mNormalCount		= 0;
	mColorCount			= 0;
	mUVCount			= 0;
	mUV2Count			= 0;
	mTriangleCount		= 0;
	mDirtyFlags			= MESH_STATE_CLEAN;
}
////////////////////////////////////////////////////////////////////////////////
/*
void Mesh::Sync(Mesh* md){
	if(md->mDirtyFlags & MESH_STATE_VERTICES){
		CopyInVertices(md->mVertexCount, md->mVertices);
	}
	if(md->mDirtyFlags & MESH_STATE_NORMALS){
		CopyInNormals(md->mNormalCount, md->mNormals);
	}
	if(md->mDirtyFlags & MESH_STATE_COLORS){
		CopyInColors(md->mColorCount, md->mColors);
	}
	if(md->mDirtyFlags & MESH_STATE_UV || md->mDirtyFlags & MESH_STATE_UV2){
		CopyInUVs(md->mUVCount, md->mUV, md->mUV2Count, md->mUV2);
	}
	if(md->mDirtyFlags & MESH_STATE_TRIANGLES){
		CopyInTriangles(md->mTriangleCount, md->mTriangles);
	}
}
////////////////////////////////////////////////////////////////////////////////

void Mesh::CopyInVertices(unsigned int vertexCount, IvVector3 vertices[]){
	mDirtyFlags |= MESH_STATE_VERTICES;
	
	if(vertexCount != mVertexCount){
		SAFE_DELETE_ARRAY(mVertices);
		mVertexCount = vertexCount;
		mVertices = new IvVector3[mVertexCount];
	}
	
	
	for(int i = 0; i < mVertexCount; ++i){
		mVertices[i] = vertices[i];
	}
}
////////////////////////////////////////////////////////////////////////////////

void Mesh::CopyInNormals(unsigned int normalCount, IvVector3 normals[]){
	mDirtyFlags |= MESH_STATE_NORMALS;
	
	if(normalCount != mNormalCount){
		SAFE_DELETE_ARRAY(mNormals);
		mNormalCount = normalCount;
		mNormals = new IvVector3[mNormalCount];
	}
	
	
	for(int i = 0; i < mNormalCount; ++i){
		mNormals[i] = normals[i];
	}
}
////////////////////////////////////////////////////////////////////////////////

void Mesh::CopyInColors(unsigned int IvColorCount, IvColor IvColors[]){
	mDirtyFlags |= MESH_STATE_COLORS;
	if(IvColorCount != mColorCount){
		SAFE_DELETE_ARRAY(mColors);
		mColorCount = IvColorCount;
		mColors = new IvColor[mColorCount];
	}	
	
	for(int i = 0; i < mColorCount; ++i){
		mColors[i] = IvColors[i];
	}
}
////////////////////////////////////////////////////////////////////////////////

void Mesh::CopyInUVs(unsigned int uv1Count, IvVector2 uv[], unsigned int uv2Count, IvVector2 uv2[]){
	mDirtyFlags |= MESH_STATE_UV;
	if(uv1Count != mUVCount){
		SAFE_DELETE_ARRAY(mUV);
		mUVCount = uv1Count;
		mUV = new IvVector2[mUVCount];
	}

	for(int i = 0; i < mUVCount; ++i){
		mUV[i] = mUV[i];
	}
	if(uv2Count == 0) return;
	mDirtyFlags |= MESH_STATE_UV2;
	if(uv2Count != mUV2Count){
		SAFE_DELETE_ARRAY(mUV2);
		mUV2Count = uv2Count;
		mUV2 = new IvVector2[mUV2Count];
	}

	for(int i = 0; i < mUV2Count; ++i){
		mUV2[i] = uv2[i];
	}
}
////////////////////////////////////////////////////////////////////////////////

void Mesh::CopyInTriangles(unsigned int tricount, unsigned int triangles[]){
	mDirtyFlags |= MESH_STATE_TRIANGLES;
	if(tricount != mTriangleCount){
		SAFE_DELETE_ARRAY(mTriangles);
		mTriangleCount = tricount;
		mTriangles = new unsigned int[mTriangleCount];
	}

	for(int i = 0; i < mTriangleCount; ++i){
		mTriangles[i] = triangles[i];
	}
}
////////////////////////////////////////////////////////////////////////////////

void Mesh::CopyOutVertices(unsigned int& vertexCount, IvVector3 vertices[]){
	if(mVertexCount == 0) return;
	vertexCount = mVertexCount;
	vertices = new IvVector3[mVertexCount];
	for(int i = 0; i < mVertexCount; ++i){
		vertices[i] = mVertices[i];
	}
}
////////////////////////////////////////////////////////////////////////////////

void Mesh::CopyOutNormals(unsigned int& normalCount, IvVector3 normals[]){
	if(mNormalCount == 0) return;
	normalCount = mNormalCount;
	normals = new IvVector3[mNormalCount];
	for(int i = 0; i < mNormalCount; ++i){
		normals[i] = mNormals[i];
	}
}
////////////////////////////////////////////////////////////////////////////////

void Mesh::CopyOutIvColors(unsigned int& IvColorCount, IvColor IvColors[]){
	if(mColorCount == 0) return;
	IvColorCount = mColorCount;
	IvColors = new IvColor[mColorCount];
	for(int i = 0; i < mColorCount; ++i){
		IvColors[i] = mColors[i];
	}
}
////////////////////////////////////////////////////////////////////////////////

void Mesh::CopyOutUVs(unsigned int& uv1Count, IvVector2 uv[], unsigned int& uv2Count, IvVector2 uv2[]){
	if(mUVCount != 0) {
		uv1Count = mUVCount;
		uv = new IvVector2[mUVCount];
		for(int i = 0; i < mUVCount; ++i){
			uv[i] = mUV[i];
		}
	}
	
	if(mUV2Count != 0) {
		uv2Count = mUV2Count;
		uv = new IvVector2[mUV2Count];
		for(int i = 0; i < mUV2Count; ++i){
			uv2[i] = mUV2[i];
		}
	}
	
}
////////////////////////////////////////////////////////////////////////////////

void Mesh::CopyOutTriangles(unsigned int& tricount, unsigned int triangles[]){
	if(mTriangleCount == 0) return;
	tricount = mTriangleCount;
	triangles = new unsigned int[mTriangleCount];
	for(int i = 0; i < mTriangleCount; ++i){
		triangles[i] = mTriangles[i];
	}
}
*/
/*
	TAKING OUT POINTERS - WANT TO BE REALLY PARANOID ABOUT THREAD SAFETY
////////////////////////////////////////////////////////////////////////////////
void Mesh::SetVerticesPointer(unsigned int vertexCount, IvVector3 vertices[]){
	mDirtyFlags |= MD_STATE_VERTICES;
	SAFE_DELETE_ARRAY(mVertices);
	mVertexCount = vertexCount;
	mVertices = vertices;
}

////////////////////////////////////////////////////////////////////////////////
void Mesh::SetNormalsPointer(unsigned int normalCount, IvVector3 normals[]){
	mDirtyFlags |= MD_STATE_NORMALS;
	SAFE_DELETE_ARRAY(mNormals);
	mNormalCount = normalCount;
	mNormals = normals;
}


////////////////////////////////////////////////////////////////////////////////
void Mesh::SetIvColorsPointer(unsigned int IvColorCount, IvColor IvColors[]){
	mDirtyFlags |= MD_STATE_IvColorS;
	SAFE_DELETE_ARRAY(mColors);
	mColorCount = IvColorCount;
	mColors = IvColors;
}


////////////////////////////////////////////////////////////////////////////////
void Mesh::SetUVPointers(unsigned int uv1Count, IvVector2 uv[], unsigned int uv2Count, IvVector2 uv2[]){
	if(uv != NULL){
		mDirtyFlags |= MD_STATE_UV;
		SAFE_DELETE_ARRAY(mUV);
		mUVCount = uv1Count;
		mUV = uv;
	}
	if(uv2 != NULL){
		mDirtyFlags |= MD_STATE_UV2;
		SAFE_DELETE_ARRAY(mUV2);
		mUV2Count = uv2Count;
		mUV2 = uv2;
	}
}


////////////////////////////////////////////////////////////////////////////////
void Mesh::SetTrianglesPointer(unsigned int tricount, unsigned int triangles[]){
	mDirtyFlags |= MD_STATE_TRIANGLES;
	SAFE_DELETE_ARRAY(mTriangles);
	mTriangleCount = tricount;
	mTriangles = triangles;
}
*/
/*
	GETTING THESE POINTERS IS TOO DANGEROUS IN A MULTITHREADED ENVIRONMENT
////////////////////////////////////////////////////////////////////////////////

void Mesh::GetVerticesPointer(unsigned int& vertexCount, IvVector3 vertices[]){
	vertexCount = mVertexCount;
	vertices = mVertices;
}

////////////////////////////////////////////////////////////////////////////////
void Mesh::GetNormalsPointer(unsigned int& normalCount, IvVector3 normals[]){
	normalCount = mNormalCount;
	normals = mNormals;
}


////////////////////////////////////////////////////////////////////////////////
void Mesh::GetIvColorsPointer(unsigned int& IvColorCount, IvColor IvColors[]){
	IvColorCount = mColorCount;
	IvColors = mColors;
}


////////////////////////////////////////////////////////////////////////////////
void Mesh::GetUVPointers(unsigned int& uv1Count, IvVector2 uv[], unsigned int& uv2Count, IvVector2 uv2[]){
	uv1Count = mUVCount;
	uv = mUV;
	
	uv2Count = mUV2Count;
	uv2 = mUV2;
}


////////////////////////////////////////////////////////////////////////////////
void Mesh::GetTrianglesPointer(unsigned int& tricount, unsigned int triangles[]){
	tricount = mTriangleCount;
	triangles  = mTriangles;
}

*/
////////////////////////////////////////////////////////////////////////////////
void Mesh::CopyInUVs(IvVector2* uv){
	memcpy(mUV,uv,sizeof(IvVector2)*mUVCount);
}
////////////////////////////////////////////////////////////////////////////////
IvVector2* Mesh::SwapUVPointer(IvVector2* uv){
	IvVector2* ret = mUV;
	mUV = uv;
	
	return ret;
}
////////////////////////////////////////////////////////////////////////////////
IvVector2* Mesh::SwapUV2Pointer(IvVector2* uv){
	IvVector2* ret = mUV2;
	mUV2 = uv;
	
	return ret;
}
////////////////////////////////////////////////////////////////////////////////
IvColor* Mesh::SwapColorPointer(IvColor* c){
	IvColor* ret = mColors;
	mColors = c;
	
	return ret;
}
////////////////////////////////////////////////////////////////////////////////
