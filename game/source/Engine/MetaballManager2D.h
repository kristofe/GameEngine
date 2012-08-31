#ifndef _METABALL_MANAGER_2D_H
#define _METABALL_MANAGER_2D_H

#include "RenderObject.h"
#include <vector>
#include "IvAABB.h"

struct MetaballInfo
{
	IvVector3			mPosition;
	float				mRadius;
	/*int					mVertices[4];
	int					mUVs[4];
	int					mColors[4];
	unsigned short		mIndices[6];*/
};


class MetaballManager2D : public RenderObject
	{
	protected:
		std::vector<MetaballInfo*>	mMetaballs;
		std::vector<IvVector3>		mVertices;
		std::vector<IvVector2>		mUVs;
		std::vector<IvColor>		mColors;
		std::vector<unsigned short>	mIndices;
		bool						mDirtyMesh;
		
		GameID					mTextureID;
		std::string					mTexturePath;
		int							mTrianglePoolSize;
		
	public:
		//TextureFont* mFont;
		
		MetaballManager2D();
		MetaballManager2D(PersistentData& storageData);


		void PreAllocateMesh(int triangleCount);
		void ConstructMeshFromLocalData();
		void UpdateMeshFromLocalData();
		void Update();

		void SetMetaballPosition(const IvVector3& pos, const float radius);
		void Serialize(PersistentData& storageData);
		void DeSerialize(PersistentData& storageData);
		virtual std::string GetTypeString()
		{
			return "MetaballManager2D";
		}
	};

#endif