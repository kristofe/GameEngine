#ifndef _SIMPLE_PARTICLE_SYSTEM_H
#define _SIMPLE_PARTICLE_SYSTEM_H

#include "RenderObject.h"



class SimpleParticleSystem : public RenderObject{
public:
    struct SimpleParticle{
        unsigned int    mParticlePositionID;
        unsigned int    mParticlePoolID;
		IvVector2		mVelocity;
    };
protected:
	int                         mPoolSize;
    IvVector2                   mMinParticleSize;
    IvVector2                   mMaxParticleSize;
	IvVector2					mCurrentParticleSize;
    std::vector<SimpleParticle> mParticles;
    std::vector<IvVector3>      mParticlePositions;
	std::string					mDebugTextTag;
	unsigned int				mLastUpdateTime;
	float						mElapsedSecondsSinceLastUpdate;
public:
	
	SimpleParticleSystem(PersistentData& storageData);
	~SimpleParticleSystem();
    
    void Update();
    void LevelLoaded();
	void MouseDown(float u, float v, int button);
    
   
	

	void Serialize(PersistentData& storageData);
	void DeSerialize(PersistentData& storageData);
	
	
private:
	//Remove Copy Constructor and assignment operator
	SimpleParticleSystem		&operator =  (const SimpleParticleSystem &m) { printf("SimpleParticleSystem = THIS ASSIGNMENT OPERATOR SHOULDN'T BE GETTING CALLED"); return *this;};
	SimpleParticleSystem(const SimpleParticleSystem &m){printf("SimpleParticleSystem COPY CTOR SHOULDN'T BE GETTING CALLED");};;

	void UpdateDebugText();
	void InitParticles();
    void UpdateMesh();
    void UpdateParticles();
	
	virtual std::string GetTypeString()
	{
		return "SimpleParticleSystem";
	}
	
	virtual std::string DebugOutput()
	{
		char buf[256];
		sprintf(buf, "\tmPoolSize:%d",(int)mPoolSize);
		std::string superInfo = RenderObject::DebugOutput();
		std::string s = superInfo + buf;
		return s;
	}
	
};

#endif //_SIMPLE_PARTICLE_SYSTEM_H