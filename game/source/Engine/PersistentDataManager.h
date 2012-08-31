#ifndef _PERSISTENT_DATA_MGR_H
#define _PERSISTENT_DATA_MGR_H
#ifdef _WIN32
#include "xml/tinyxml.h"
#else
#include "tinyxml.h"
#endif

#include <string>
#include <map>
#include <list>

#define _DBDTEXT_ 0

/*
 unsigned long
 hash(unsigned char *str)
 {
 unsigned long hash = 5381;
 int c;
 
 while (c = *str++)
 hash = ((hash << 5) + hash) + c; // hash * 33 + c 

return hash;
}

 */

class IvVector2;
class IvVector3;
class IvVector4;
class IvQuat;
class IvColor;
class IvMatrix44;
struct AnimationData2D;
class PersistentData  {
	friend class PersistentDataTree;
public:
	PersistentData(std::string &path, std::string& name) 
	{
		mPath = path;
		mName = name;
		mProperties.clear();
		mChildrenMap.clear();
		mChildren.clear();
		
	};
	
	PersistentData(const char* path, const char* name) 
	{
		mPath = path;
		mName = name;
		mProperties.clear();
		mChildrenMap.clear();
		mChildren.clear();
	};
	
	PersistentData(const char* name) 
	{
		mPath = name;
		mName = name;
		mProperties.clear();
		mChildrenMap.clear();
		mChildren.clear();
	};
	
	~PersistentData()
	{
		mProperties.clear();
		mChildrenMap.clear();
		mChildren.clear();
	};
	void SetName(const char* name)
	{
		mName = name;
		mPath = name;
	};
	
	void SetProperty(std::string& key, std::string& val);
	void SetProperty(const char* key, const char* val);
	void SetProperty(const char* key, float val);
	void SetProperty(const char* key, int val);
	void SetProperty(const char* key, unsigned int val);
	void SetProperty(const char* key, bool val);
	void SetTextProperty(std::string& value);
	std::string GetProperty(std::string& key);
	std::string GetProperty(const char* key);
	int GetPropertyI(const char* key);
	bool GetPropertyB(const char* key);
	unsigned int GetPropertyU(const char* key);
	long GetPropertyL(const char* key);
	float GetPropertyF(const char* key);
	double GetPropertyD(const char* key);

	std::string GetTextOnlyChildAsText(const char* name);
	float GetTextOnlyChildAsFloat(const char* name);
	double GetTextOnlyChildAsDouble(const char* name);
	unsigned int GetTextOnlyChildAsUInt(const char* name);
	int GetTextOnlyChildAsInt(const char* name);
	long GetTextOnlyChildAsLong(const char* name);
	bool GetTextOnlyChildAsBool(const char* name);

	std::string GetText();
	float GetTextAsFloat();
	float GetTextAsDouble();
	unsigned int GetTextAsUInt();
	int GetTextAsInt();
	long GetTextAsLong();
	bool GetTextAsBool();
	
	void RemoveChild(PersistentData* c);
	
	std::list<PersistentData*>& GetChildren();
	void GetChildren(std::string& key, std::list<PersistentData*>& children);
	void GetChildren(const char* key, std::list<PersistentData*>& children);
	PersistentData* GetUniqueChild(std::string& key);
	PersistentData* GetNodeFromPath(std::string& path);
	PersistentData* GetNodeFromPath(const char* path);
	void GetNodesFromPath(std::string& path,std::list<PersistentData*>& nodes);
	void GetNodesFromPath(const char* path,std::list<PersistentData*>& nodes);
	std::string ToString();
	std::string& GetName(){return mName;};
	std::string& GetPath(){return mPath;};
	void SaveStateToXMLNode(TiXmlElement* node);
	void AddChild(PersistentData* d);

	void AddTextOnlyChild(const char* key, int val);
	void AddTextOnlyChild(const char* key, unsigned int val);
	void AddTextOnlyChild(const char* key, long val);
	void AddTextOnlyChild(const char* key, float val);
	void AddTextOnlyChild(const char* key, double val);
	void AddTextOnlyChild(const char* key, const char* val);
	void AddTextOnlyChild(const char* key, std::string val);
	
	void AddVector2Child(IvVector2& v, const char* name);
	IvVector2 GetVector2Child(const char* name);
	void AddVector3Child(IvVector3& v, const char* name);
	IvVector3 GetVector3Child(const char* name);
	void AddVector4Child(IvVector4& v, const char* name);
	IvVector4 GetVector4Child(const char* name);
	void AddQuatChild(IvQuat& v, const char* name);
	IvColor GetColorChild(const char* name);
	void AddColorChild(IvColor& v, const char* name);
	IvQuat GetQuatChild(const char* name);
	void AddMatrix44Child(IvMatrix44& m, const char* name);
	IvMatrix44 GetMatrix44Child(const char* name);
	
	
	virtual std::string GetTypeString()
	{
		return "PersistentData";
	}
	
protected:
	std::string	mPath;
	std::string mName;
	std::map<std::string, std::string>  mProperties;
	std::multimap<std::string, PersistentData*> mChildrenMap;
	std::list<PersistentData*> mChildren;
	
};

class PersistentDataTree {
protected:
	PersistentData*				mRoot;

public:
	PersistentDataTree():mRoot(NULL){};
	~PersistentDataTree();
	
	void DeleteSubTree(PersistentData* node);
	void SetRoot(PersistentData* d);
	PersistentData* GetRoot();
	void AddNode(PersistentData* parent, PersistentData* node);
	PersistentData* GetNode(std::string& path);
	PersistentData* GetNode(const char* path);
	void GetNodes(std::string& path,std::list<PersistentData*>& nodes);
	void GetNodes(const char* path,std::list<PersistentData*>& nodes);
	void DumpToStdout(PersistentData* node = NULL);
	
	virtual std::string GetTypeString()
	{
		return "PersistentDataTree";
	}
	
};

class PersistentDataManager
{
protected:
	std::multimap<std::string, std::map<std::string, std::string> >  mDataDB;
	std::multimap<std::string, std::string>  mKeyCounter;
	std::list<std::string> mKeys;

	PersistentDataTree	mDataTree;

	
public:
	PersistentDataManager(){};
	void LoadXMLFile(const char* filename);
	void SaveXMLFile(const char* filename, PersistentData* rootNode = NULL);
	PersistentData* GetNode(std::string& path);
	void GetNodes(std::string& path,std::list<PersistentData*>& nodes);
	PersistentDataTree& GetDataTree(){ return mDataTree;};
	
	virtual std::string GetTypeString()
	{
		return "PersistentDataManager";
	}
	
private:
	void LoadXMLToMultiMap( TiXmlNode* pParent, std::string& docpath, PersistentData* parentNode);
	void SaveNodeToXMLDoc(PersistentData* node, TiXmlElement* xmlNode);
	int LoadXMLAttributesToMultiMap(TiXmlElement* pElement, std::string& docpath,PersistentData* node);
	
};
#endif

