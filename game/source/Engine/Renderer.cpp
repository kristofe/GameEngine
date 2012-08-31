#include "Renderer.h"
#include "Game.h"
#include "Mesh.h"
#include "Model.h"
#include "Material.h"
#include "Texture.h"
#include "TextureFont.h"
#include "Platform.h"
#include "RenderObject.h"
#include "Camera.h"
////////////////////////////////////////////////////////////////////////////////
Renderer::~Renderer()
{

	//Make Sure to delete the MeshToROLists in the mMeshesPerMaterial map.
}

////////////////////////////////////////////////////////////////////////////////
void Renderer::AddRenderObject(GameID id)
{
	RenderObjectSetIt it;
	it = mRenderObjects.find(id);
	if(it != mRenderObjects.end())
	{
		return;
	}

	mRenderObjects.insert(id);
	RenderObject& ro = (RenderObject&)Game::GetGameObjectStatic(id);
	if(ro.IsTransparent())
	   mTranparentRenderObjects.push_back(&ro);
	else
	   mOpaqueRenderObjects.push_back(&ro);	
}
////////////////////////////////////////////////////////////////////////////////
void Renderer::RemoveRenderObject(GameID id)
{
	mPendingRenderObjectRemovals.push_back(id);
	RenderObject& ro = (RenderObject&)Game::GetGameObjectStatic(id);
	mPendingRenderObjectRemovalPointers.push_back(&ro);
}
////////////////////////////////////////////////////////////////////////////////
void Renderer::HandlePendingRenderObjectRemovals()
{

	std::list<GameID>::iterator pendIt = mPendingRenderObjectRemovals.begin();
	while(pendIt != mPendingRenderObjectRemovals.end())
	{
		GameID id = *pendIt;

		RenderObjectSetIt roIT;
		roIT = mRenderObjects.find(id);
		if(roIT != mRenderObjects.end())
		{
			mRenderObjects.erase(id);
		}
		++pendIt;
	}

	std::list<RenderObject*>::iterator ptrIt = mPendingRenderObjectRemovalPointers.begin();
	while(ptrIt != mPendingRenderObjectRemovalPointers.end())
	{
		RenderObject* ro = *ptrIt;
	   
		for(int idx = 0; idx < ObjectsArrayCount; idx++){
			RenderObjectVectorIt it = mObjectVectors[idx]->begin();
			while(it != mObjectVectors[idx]->end())
			{
				if(*it == ro)
				{
					mObjectVectors[idx]->erase(it);
					break;
				}
				++it;
			}
		}
		++ptrIt;
	}
	mPendingRenderObjectRemovalPointers.clear();
	mPendingRenderObjectRemovals.clear();
	/*
	for(unsigned int i = 0; i < ro.mMeshes.size(); i++)
	{
		Mesh* mesh = ro.mMeshes[i];
		Material* mat = ro.mMaterials[i];
		
		std::map<RendererMaterialID,MeshToROList>::iterator it;
		it = mMeshesPerMaterial.find(mat->mGameID);
		if(it == mMeshesPerMaterial.end())
		{
			return;
		}
		MeshToRenderObject m2ro(ro,mesh);

		MeshToROList m2ROList = (*it).second; 

		MeshToROList::iterator m2roIt;
		m2roIt = m2ROList.find(m2ro);
		if(m2roIt != meshROList.end())
		{
			m2ROList.erase(m2roIt);
		}
		
		if(m2ROList.size() == 0)
		{
			mMeshesPerMaterial.erase(it);
		}
	}
	*/
}
////////////////////////////////////////////////////////////////////////////////
void Renderer::AddCamera(Camera* c)
{
	for(unsigned int i = 0; i < mCameras.size(); ++i)
	{
		if(mCameras[i] == c)
			return;
	}
	if(c->mIsMain)
		mMainCamera = c;
	
	mCameras.push_back(c);
	
}
////////////////////////////////////////////////////////////////////////////////
void Renderer::RemoveCamera(Camera* c)
{
	mPendingCameraRemovals.push_back(c);
	
}
////////////////////////////////////////////////////////////////////////////////
void Renderer::HandlePendingCameraRemovals()
{
	
	std::list<Camera*>::iterator pendIt = mPendingCameraRemovals.begin();
	while(pendIt != mPendingCameraRemovals.end())
	{
		Camera* rt = *pendIt;
		//RenderTarget* rt = (RenderTarget*)Game::GetInstance().GetGameObject(id);
		//Game::GetInstance().DeleteGameObject(id);
		std::vector<Camera*>::iterator it = mCameras.begin();
		while(it != mCameras.end())
		{
			if(*it == rt)
			{
				mCameras.erase(it);
				break;
			}
			++it;
		}
		
		++pendIt;
	}
	mPendingCameraRemovals.clear();
}
////////////////////////////////////////////////////////////////////////////////
void Renderer::AddRenderTarget(RenderTarget* rt)
{
	for(unsigned int i = 0; i < mRenderTargets.size(); ++i)
	{
		if(mRenderTargets[i] == rt)
			return;
	}
	
	mRenderTargets.push_back(rt);
	
}
////////////////////////////////////////////////////////////////////////////////
void Renderer::RemoveRenderTarget(RenderTarget* renderTarget)
{
	mPendingRenderTargetRemovals.push_back(renderTarget);

}
////////////////////////////////////////////////////////////////////////////////
void Renderer::HandlePendingRenderTargetRemovals()
{
	
	std::list<RenderTarget*>::iterator pendIt = mPendingRenderTargetRemovals.begin();
	while(pendIt != mPendingRenderTargetRemovals.end())
	{
		RenderTarget* rt = *pendIt;
		//RenderTarget* rt = (RenderTarget*)Game::GetInstance().GetGameObject(id);
		//Game::GetInstance().DeleteGameObject(id);
		RenderTargetVectorIt it = mRenderTargets.begin();
		while(it != mRenderTargets.end())
		{
			if(*it == rt)
			{
				mRenderTargets.erase(it);
				break;
			}
			++it;
		}
		
		++pendIt;
	}
	mPendingRenderTargetRemovals.clear();
}


////////////////////////////////////////////////////////////////////////////////
GameID Renderer::AddMesh(Mesh* m)
{
	GameID id = Game::GetNextGameID();
	mMeshes[id] = m;
	m->mGameID = id;
	return id;
	
}

////////////////////////////////////////////////////////////////////////////////
void Renderer::DeleteMesh(GameID id)
{
	Mesh* m = mMeshes[id];
	SAFE_DELETE(m);
	mMeshes.erase(id);
}

////////////////////////////////////////////////////////////////////////////////
Mesh& Renderer::GetMesh(GameID id)
{
	return *(mMeshes[id]);
}

////////////////////////////////////////////////////////////////////////////////
Mesh& Renderer::CreateMesh(std::string name)
{
	Mesh* m = new Mesh(name);
	//printf("Creating Mesh %s\n",name.c_str());
	AddMesh(m);
	return *m;
}

////////////////////////////////////////////////////////////////////////////////
Mesh& Renderer::LoadMesh(std::string name,std::string filename)
{
	//Implement later
	return CreateMesh(name);
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
GameID Renderer::AddMaterial(Material* m)
{
	GameID id = Game::GetNextGameID();
	mMaterials[id] = m;
	m->mGameID = id;
	return id;
	
}

////////////////////////////////////////////////////////////////////////////////
void Renderer::DeleteMaterial(GameID id)
{
	Material* m = mMaterials[id];
	//printf("Deleting material: %s\n",m->mName);
	SAFE_DELETE(m);
	mMaterials.erase(id);
}

////////////////////////////////////////////////////////////////////////////////
Material& Renderer::GetMaterial(GameID id)
{
	return *(mMaterials[id]);
}

////////////////////////////////////////////////////////////////////////////////
Material& Renderer::CreateMaterial(std::string name)
{
	Material* m = new Material(name);
	//printf("Creating Material %s\n",name.c_str());
	AddMaterial(m);
	return *m;
}

////////////////////////////////////////////////////////////////////////////////
Material& Renderer::LoadMaterial(std::string name, std::string filename)
{
	//Implement later
	return CreateMaterial(name);
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*RendererModelID Renderer::AddModel(Model* m)
{
	mModels[++mLastModelID] = m;
	m->mGameID = mLastModelID;
	return mLastModelID;
	
}

////////////////////////////////////////////////////////////////////////////////
void Renderer::DeleteModel(RendererModelID id)
{
	Model* m = mModels[id];
	SAFE_DELETE(m);
	mModels.erase(id);
}

////////////////////////////////////////////////////////////////////////////////
Model& Renderer::GetModel(RendererModelID id)
{
	return *(mModels[id]);
}

////////////////////////////////////////////////////////////////////////////////
Model& Renderer::CreateModel()
{
	Model* m = new Model();
	AddModel(m);
	return *m;
}

////////////////////////////////////////////////////////////////////////////////
Model& Renderer::LoadModel(char* filename)
{
	//Implement later
	return CreateModel();
}
*/
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
GameID Renderer::AddTexture(Texture* m)
{
	GameID id = Game::GetNextGameID();
	mTextures[id] = m;
	m->mGameID = id;
	return id;
	
}

////////////////////////////////////////////////////////////////////////////////
void Renderer::DeleteTexture(GameID id)
{
	Texture* m = mTextures[id];
	//printf("Deleting texture: %s\n",m->mName);
	SAFE_DELETE(m);
	mTextures.erase(id);
}

////////////////////////////////////////////////////////////////////////////////
Texture& Renderer::GetTexture(GameID id)
{
	return *(mTextures[id]);
}

////////////////////////////////////////////////////////////////////////////////
Texture& Renderer::CreateTexture(std::string name,std::string filename,bool delayLoad)
{
	Texture* m = new Texture(name,filename,delayLoad);
	//printf("Creating Texture %s\n",name.c_str());
	AddTexture(m);
	return *m;
}

////////////////////////////////////////////////////////////////////////////////
Texture& Renderer::LoadTexture(std::string filename, bool delayLoad)
{
	//Implement later
	return CreateTexture(filename, filename,delayLoad);
}



////////////////////////////////////////////////////////////////////////////////
void Renderer::Retain(GameID referencer, GameID referencee)
{
	RendererObjectRefCounter refCounter;

	ReferenceCounterDBIt dbIt = mRendererRefCountDB.find(referencee);
	if(dbIt != mRendererRefCountDB.end())
	{
		refCounter = (*dbIt).second;
		refCounter.referencingObjects.insert(referencer);
	}
	else
	{
		refCounter.rendererID = referencee;
		refCounter.referencingObjects.insert(referencer);
		mRendererRefCountDB[refCounter.rendererID] = refCounter;
	}
}
////////////////////////////////////////////////////////////////////////////////
void Renderer::Release(GameID referencer, GameID referencee)
{
	ReferenceCounterDBIt dbIt = mRendererRefCountDB.find(referencee);
	if(dbIt != mRendererRefCountDB.end())
	{
		RendererObjectRefCounter refCounter = (*dbIt).second;
		std::set<GameID>::iterator it = refCounter.referencingObjects.find(referencer);
		if(it != refCounter.referencingObjects.end())
		{
			refCounter.referencingObjects.erase(it);
		}
		if(refCounter.referencingObjects.empty()){
			mRendererRefCountDB.erase(dbIt);
			DeleteRendererObject(referencee);

		}
	}
}
////////////////////////////////////////////////////////////////////////////////
void Renderer::DeleteRendererObject(GameID id)
{
	mPendingRendererObjectDeletions.push_back(id);
}
////////////////////////////////////////////////////////////////////////////////
void Renderer::HandlePendingRendererObjectDeletions()
{
	std::list<GameID>::iterator pendIt = mPendingRendererObjectDeletions.begin();
	while(pendIt != mPendingRendererObjectDeletions.end())
	{
		GameID id = *pendIt;
		MaterialObjectMapIt	matIt = mMaterials.find(id);
		if(matIt != mMaterials.end())
		{
			Material* mat = (*matIt).second;
			//printf("Deleting Material %s\n",mat->mName.c_str());
			mMaterials.erase(matIt);
			RendererObjectFactory::RemoveMaterial(mat->mName);
			SAFE_DELETE(mat);
		}
		
		MeshObjectMapIt	meshIt = mMeshes.find(id);
		if(meshIt != mMeshes.end())
		{
			Mesh* mesh = (*meshIt).second;
			//printf("Deleting Mesh %s\n",mesh->mName.c_str());
			mMeshes.erase(meshIt);
			RendererObjectFactory::RemoveMesh(mesh->mName);
			SAFE_DELETE(mesh);
		}
		
		TextureObjectMapIt	texIt = mTextures.find(id);
		if(texIt != mTextures.end())
		{
			Texture* tex = (*texIt).second;
			//printf("Deleting Texture %s\n",tex->mName.c_str());
			mTextures.erase(texIt);
			DeleteTextureFromGPU(*tex);
			RendererObjectFactory::RemoveTexture(tex->mName);
			SAFE_DELETE(tex);
		}
		
		/*TextureFontMapIt	fontIt = mFonts.find(id);
		if(fontIt != mFonts.end())
		{
			TextureFont* font = (*fontIt).second;
			printf("Deleting Font %s\n",font->mName.c_str());
			RendererObjectFactory::RemoveTextureFont(font->mName)
			delete font;
		}*/
		
		++pendIt;
	}
	mPendingRendererObjectDeletions.clear();
}
////////////////////////////////////////////////////////////////////////////////
GameID Renderer::AddTextureFont(TextureFont* m)
{
	GameID id = Game::GetNextGameID();
	mFonts[id] = m;
	m->mGameID = id;
	return id;
	
}
////////////////////////////////////////////////////////////////////////////////
void Renderer::DeleteTextureFont(GameID id)
{
	TextureFont* m = mFonts[id];
	//printf("Deleting font: %s\n",m->mName);
	SAFE_DELETE(m);
	mFonts.erase(id);
}

////////////////////////////////////////////////////////////////////////////////
TextureFont& Renderer::GetTextureFont(GameID id)
{
	return *(mFonts[id]);
}

////////////////////////////////////////////////////////////////////////////////
TextureFont& Renderer::CreateTextureFont(std::string name)
{
	std::string filename;
	std::string path;
	//GetResourcePath(path);
	//filename.reserve(512);
	//filename = path;
	//printf("Creating Font %s\n",name.c_str());
	TextureFont* font = new TextureFont(name);
	AddTextureFont(font);
	filename += font->mFileName;

	Texture& texture = LoadTexture(filename.c_str());
	RendererObjectFactory::ReadTextureFromDisk(filename.c_str(),texture);
	GameID textureID = texture.mGameID;
	
	Material& mat = CreateMaterial(name);
	mat.mIsFont = true;
	GameID materialID = mat.mGameID;
	mat.SetMainTexture(textureID);
	
	font->materialID = materialID;
	font->textureID = textureID;

	return *font;
}
////////////////////////////////////////////////////////////////////////////////
void Renderer::Serialize(PersistentData& storageData)
{
	mGameEngine = &(Game::GetInstance());
	
	storageData.SetName("Renderer");
	storageData.SetProperty("Width",mWidth);
	storageData.SetProperty("Height",mHeight);
	//storageData.SetProperty("LastGameID",mLastGameID);
	
	PersistentData* node;
	PersistentData* c;
	
	node = new PersistentData("RenderObjectList");
	storageData.AddChild(node);
	RenderObjectSetIt roIt = mRenderObjects.begin();
	while (roIt != mRenderObjects.end()) {
		c = new PersistentData("RenderObject");
		c->SetProperty("GameID",*roIt);
		node->AddChild(c);
		++roIt;
	}

	node = new PersistentData("Materials");
	storageData.AddChild(node);
	MaterialObjectMapIt matIt = mMaterials.begin();
	while (matIt != mMaterials.end()) {
		c = new PersistentData("Material");
		matIt->second->Serialize(*c);
		node->AddChild(c);
		++matIt;
	}
	
	node = new PersistentData("Meshes");
	storageData.AddChild(node);
	MeshObjectMapIt meshIt = mMeshes.begin();
	while (meshIt != mMeshes.end()) {
		c = new PersistentData("Mesh");
		meshIt->second->Serialize(*c);
		node->AddChild(c);
		++meshIt;
	}
	
	node = new PersistentData("Textures");
	storageData.AddChild(node);
	TextureObjectMapIt texIt = mTextures.begin();
	while (texIt != mTextures.end()) {
		c = new PersistentData("Texture");
		texIt->second->Serialize(*c);
		node->AddChild(c);
		++texIt;
	}
	
	node = new PersistentData("Fonts");
	storageData.AddChild(node);
	TextureFontMapIt fontIt = mFonts.begin();
	while (fontIt != mFonts.end()) {
		c = new PersistentData("Font");
		fontIt->second->Serialize(*c);
		node->AddChild(c);
		++fontIt;
	}
	
	node = new PersistentData("RefCounters");
	storageData.AddChild(node);
	ReferenceCounterDBIt refIt = mRendererRefCountDB.begin();
	while (refIt != mRendererRefCountDB.end()) {
		refIt->second.Serialize(*node);	
		++refIt;
	}
	
}
////////////////////////////////////////////////////////////////////////////////
void Renderer::DeSerialize(PersistentData& storageData)
{
	
	
}
////////////////////////////////////////////////////////////////////////////////
unsigned int Renderer::GetEnumForString(std::string str)
{
	std::map<std::string, unsigned int>::iterator it = mStringToEnumDB.find(str);
	if(it != mStringToEnumDB.end())
		return it->second;
	
	printf("ERROR!!! Renderer::GetEnumForString() couldn't find value for string %s\n", str.c_str());
	return 0;
	
}
////////////////////////////////////////////////////////////////////////////////
void Renderer::DumpDebugData()
{	

	RenderObjectSetIt roIt = mRenderObjects.begin();
	while (roIt != mRenderObjects.end()) {
		printf("%d\tRenderObject\n",*roIt);
		++roIt;
	}
	
	MaterialObjectMapIt matIt = mMaterials.begin();
	while (matIt != mMaterials.end()) {
		printf("%s\n",matIt->second->DebugOutput().c_str());
		++matIt;
	}
	
	MeshObjectMapIt meshIt = mMeshes.begin();
	while (meshIt != mMeshes.end()) {
		printf("%s\n",meshIt->second->DebugOutput().c_str());
		++meshIt;
	}
	
	
	TextureObjectMapIt texIt = mTextures.begin();
	while (texIt != mTextures.end()) {
		Texture* tex = static_cast<Texture*>(texIt->second);
		if(tex->mGameID == 0)
		{
			printf("0\tTexture\tWHAT THE HELL?\n");
		}
		else
		{
			printf("%s\n",texIt->second->DebugOutput().c_str());
		}
		++texIt;
	}
	
	TextureFontMapIt fontIt = mFonts.begin();
	while (fontIt != mFonts.end()) {
		printf("%s\n",fontIt->second->DebugOutput().c_str());
		++fontIt;
	}
	
	ReferenceCounterDBIt refIt = mRendererRefCountDB.begin();
	while (refIt != mRendererRefCountDB.end()) {
		printf("%d\t%s\n",refIt->first,refIt->second.DebugOutput().c_str());	
		++refIt;
	}
	
	/*
	std::list<GameID>::iterator pendIt = mPendingRendererObjectDeletions.begin();
	while(pendIt != mPendingRendererObjectDeletions.end())
	{
		GameID id = *pendIt;
		MaterialObjectMapIt	matIt = mMaterials.find(id);
		if(matIt != mMaterials.end())
		{
			//Material* mat = (*matIt).second;
			printf("%d\tPendingDeletion\t%s\n",id,(*matIt).second->DebugOutput().c_str());
		}
		
		MeshObjectMapIt	meshIt = mMeshes.find(id);
		if(meshIt != mMeshes.end())
		{
			//Mesh* mesh = (*meshIt).second;
			printf("%d\tPendingDeletion\t%s\n",id,(*meshIt).second->DebugOutput().c_str());
		}
		
		TextureObjectMapIt	texIt = mTextures.find(id);
		if(texIt != mTextures.end())
		{
			//Texture* tex = (*texIt).second;
			printf("%d\tPendingDeletion\t%s\n",id,(*texIt).second->DebugOutput().c_str());
		}
		
		TextureFontMapIt	fontIt = mFonts.find(id);
		if(fontIt != mFonts.end())
		{
			//TextureFont* tex = (*fontIt).second;
			printf("%d\tPendingDeletion\t%s\n",id,(*fontIt).second->DebugOutput().c_str());
		}
		++pendIt;
	}
	 */
	
}
////////////////////////////////////////////////////////////////////////////////

