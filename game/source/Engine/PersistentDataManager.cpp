#include "PersistentDataManager.h"
#include "Platform.h"
#include "Globals.h"
#include <iostream>

void PersistentDataTree::SetRoot(PersistentData* d)
{ 
	mRoot = d;

}

PersistentData* PersistentDataTree::GetRoot()
{
	return mRoot;
}

PersistentData* PersistentDataTree::GetNode(std::string& path)
{
	return mRoot->GetNodeFromPath(path);
}

PersistentData* PersistentDataTree::GetNode(const char* path)
{
	std::string p = path;
	return mRoot->GetNodeFromPath(p);
}

void PersistentDataTree::GetNodes(const char* path,std::list<PersistentData*>& nodes)
{
	std::string p = path;
	mRoot->GetNodesFromPath(p, nodes);	
}

void PersistentDataTree::GetNodes(std::string& path,std::list<PersistentData*>& nodes)
{
	mRoot->GetNodesFromPath(path, nodes);	
}

void PersistentDataTree::AddNode(PersistentData* parent, PersistentData* node)
{

		if(parent == NULL)
		{
			SetRoot(node);
		}
		else
		{
			parent->AddChild(node);
		}

}

void PersistentData::SetProperty(std::string& key, std::string& val)
{
	mProperties[key] = val;
}

void PersistentData::SetTextProperty(std::string& val)
{
	mProperties[QUOTEME(_DBDTEXT_)] = val;
}

void PersistentData::AddTextOnlyChild(const char* key, int val)
{
	PersistentData* child = new PersistentData(key,key);
	std::string k = QUOTEME(_DBDTEXT_);
	char s[128];
	sprintf(s, "%d",val);
	std::string v = s;
	child->mProperties[k] = v;
	AddChild(child);
}

void PersistentData::AddTextOnlyChild(const char* key, unsigned int val)
{
	PersistentData* child = new PersistentData(key,key);
	std::string k = QUOTEME(_DBDTEXT_);
	char s[128];
	sprintf(s, "%d",val);
	std::string v = s;
	child->mProperties[k] = v;
	AddChild(child);
}

void PersistentData::AddTextOnlyChild(const char* key, long val)
{
	PersistentData* child = new PersistentData(key,key);
	std::string k = QUOTEME(_DBDTEXT_);
	char s[128];
	sprintf(s, "%d",(int)val);
	std::string v = s;
	child->mProperties[k] = v;
	AddChild(child);
}

void PersistentData::AddTextOnlyChild(const char* key, float val)
{
	PersistentData* child = new PersistentData(key,key);
	std::string k = QUOTEME(_DBDTEXT_);
	char s[128];
	sprintf(s, "%3.6f",val);
	std::string v = s;
	child->mProperties[k] = v;
	AddChild(child);
}

void PersistentData::AddTextOnlyChild(const char* key, double val)
{
	PersistentData* child = new PersistentData(key,key);
	std::string k = QUOTEME(_DBDTEXT_);
	char s[128];
	sprintf(s, "%3.6f",val);
	std::string v = s;
	child->mProperties[k] = v;
	AddChild(child);	
}

void PersistentData::AddTextOnlyChild(const char* key, const char* val)
{
	PersistentData* child = new PersistentData(key,key);
	std::string k = QUOTEME(_DBDTEXT_);

	std::string v = val;
	child->mProperties[k] = v;
	AddChild(child);
}

void PersistentData::AddTextOnlyChild(const char* key, std::string val)
{
	PersistentData* child = new PersistentData(key,key);
	std::string k = QUOTEME(_DBDTEXT_);
	child->mProperties[k] = val;
	AddChild(child);
}

void PersistentData::SetProperty(const char* key, const char* val)
{
	std::string k = key;
	std::string v = val;
	mProperties[k] = v;
}

void PersistentData::SetProperty(const char* key, float val)
{
	std::string k = key;
	char s[128];
	sprintf(s, "%3.6f",val);
	std::string v = s;
	mProperties[k] = v;
}

void PersistentData::SetProperty(const char* key, int val)
{
	char buff[64];
	sprintf(buff,"%d", val);
	std::string k = key;
	std::string v = buff;
	mProperties[k] = v;
}

void PersistentData::SetProperty(const char* key, bool val)
{
	std::string k = key;
	std::string v = val?"1":"0";
	mProperties[k] = v;
}

void PersistentData::SetProperty(const char* key, unsigned int val)
{
	char buff[64];
	sprintf(buff,"%d", val);
	std::string k = key;
	std::string v = buff;
	mProperties[k] = v;
}

std::string PersistentData::ToString()
{
	std::string str = mPath;
	str += ":";
	char s[32];
	sprintf(s, "%x", (unsigned int)this);

	str += s;
	str += "\n";

	std::map<std::string,std::string>::iterator it = mProperties.begin();
	while(it != mProperties.end())
	{
		str += "   ";
		str += it->first + ":" + it->second;
		str += "\n";
		++it;
	}
	//str += "\n";
	return str;
}

std::string PersistentData::GetProperty(std::string& key)
{
	std::string returnedString;
	std::map<std::string,std::string>::iterator it = mProperties.find(key);
	if(it != mProperties.end())
	{
		returnedString = (*it).second;
	}
	return returnedString;
}

std::string PersistentData::GetProperty(const char* key)
{
	std::string k = key;
	return GetProperty(k);
}

int PersistentData::GetPropertyI(const char* key)
{
	std::string k = key;
	std::string v =  GetProperty(k);
	return atoi(v.c_str());
}

bool PersistentData::GetPropertyB(const char* key)
{
	std::string k = key;
	std::string v =  GetProperty(k);
	int val =  atoi(v.c_str());
	return val != 0;
}

unsigned int PersistentData::GetPropertyU(const char* key)
{
	std::string k = key;
	std::string v =  GetProperty(k);
	long vv = atoi(v.c_str());
	return (unsigned int)vv;
}

long PersistentData::GetPropertyL(const char* key)
{
	std::string k = key;
	std::string v =  GetProperty(k);
	return atol(v.c_str());
}

float PersistentData::GetPropertyF(const char* key)
{
	std::string k = key;
	std::string v =  GetProperty(k);
	return (float)atof(v.c_str());
}

double PersistentData::GetPropertyD(const char* key)
{
	std::string k = key;
	std::string v =  GetProperty(k);
	return atof(v.c_str());
}

std::string PersistentData::GetTextOnlyChildAsText(const char* name)
{	
	std::string n = name;
	PersistentData* c = GetUniqueChild(n);
	if(c != NULL)
	{
		return c->GetText();
	}
	std::string r = "";
	return r;
}

float PersistentData::GetTextOnlyChildAsFloat(const char* name)
{
	std::string n = name;
	PersistentData* c = GetUniqueChild(n);
	if(c != NULL)
	{
		return c->GetTextAsFloat();
	}

	return 0.0f;
}

double PersistentData::GetTextOnlyChildAsDouble(const char* name)
{
	std::string n = name;
	PersistentData* c = GetUniqueChild(n);
	if(c != NULL)
	{
		return c->GetTextAsDouble();
	}

	return 0.0;
}

unsigned int PersistentData::GetTextOnlyChildAsUInt(const char* name)
{
	std::string n = name;
	PersistentData* c = GetUniqueChild(n);
	if(c != NULL)
	{
		return c->GetTextAsUInt();
	}

	return 0;
}

int PersistentData::GetTextOnlyChildAsInt(const char* name)
{
	std::string n = name;
	PersistentData* c = GetUniqueChild(n);
	if(c != NULL)
	{
		return c->GetTextAsInt();
	}

	return 0;
}

long PersistentData::GetTextOnlyChildAsLong(const char* name)
{
	std::string n = name;
	PersistentData* c = GetUniqueChild(n);
	if(c != NULL)
	{
		return c->GetTextAsLong();
	}

	return 0L;
}

bool PersistentData::GetTextOnlyChildAsBool(const char* name)
{
	std::string n = name;
	PersistentData* c = GetUniqueChild(n);
	if(c != NULL)
	{
		return c->GetTextAsBool();
	}

	return false;
}

std::string PersistentData::GetText()
{
	std::string k = QUOTEME(_DBDTEXT_);
	return GetProperty(k);
}

float PersistentData::GetTextAsFloat()
{
	return GetPropertyF(QUOTEME(_DBDTEXT_));
}

float PersistentData::GetTextAsDouble()
{
	return GetPropertyD(QUOTEME(_DBDTEXT_));
}

unsigned int PersistentData::GetTextAsUInt()
{
	return GetPropertyU(QUOTEME(_DBDTEXT_));
}

int PersistentData::GetTextAsInt()
{
	return GetPropertyI(QUOTEME(_DBDTEXT_));
}

long PersistentData::GetTextAsLong()
{
	return GetPropertyL(QUOTEME(_DBDTEXT_));
}

bool PersistentData::GetTextAsBool()
{
	return GetPropertyB(QUOTEME(_DBDTEXT_));
}

void PersistentData::AddChild(PersistentData* d)
{	
	mChildrenMap.insert(std::pair<std::string, PersistentData*>( d->mName, d));
	mChildren.push_back(d);
}

void PersistentData::RemoveChild(PersistentData* c)
{
	std::multimap<std::string, PersistentData*>::iterator mapIt = mChildrenMap.find(c->mName);

	mChildrenMap.erase(mapIt);
	
	std::list<PersistentData*>::iterator it =  mChildren.begin();
	while(it != mChildren.end())
	{
		if(*it == c)
		{
			mChildren.erase(it);
			break;
		}
	}
		
}

void PersistentData::AddVector2Child(IvVector2& v, const char* name)
{
	PersistentData* c = new PersistentData(name);
	c->SetProperty("x",v.x);
	c->SetProperty("y",v.y);
	AddChild(c);
}

IvVector2 PersistentData::GetVector2Child(const char* name)
{
	IvVector2 v;
	std::string n = name;
	PersistentData* c = GetUniqueChild(n);
	if(c != NULL)
	{
		v.x = c->GetPropertyF("x");
		v.y = c->GetPropertyF("y");
	}
	return v;
}

void PersistentData::AddVector3Child(IvVector3& v, const char* name)
{
	PersistentData* c = new PersistentData(name);
	c->SetProperty("x",v.x);
	c->SetProperty("y",v.y);
	c->SetProperty("z",v.z);
	AddChild(c);
}

IvVector3 PersistentData::GetVector3Child(const char* name)
{
	IvVector3 v;
	std::string n = name;
	PersistentData* c = GetUniqueChild(n);
	if(c != NULL)
	{
		v.x = c->GetPropertyF("x");
		v.y = c->GetPropertyF("y");
		v.z = c->GetPropertyF("z");
	}
	return v;
}

void PersistentData::AddVector4Child(IvVector4& v, const char* name)
{
	PersistentData* c = new PersistentData(name);
	c->SetProperty("x",v.x);
	c->SetProperty("y",v.y);
	c->SetProperty("z",v.z);
	c->SetProperty("w",v.w);
	AddChild(c);
}

IvVector4 PersistentData::GetVector4Child(const char* name)
{
	IvVector4 v;
	std::string n = name;
	PersistentData* c = GetUniqueChild(n);
	if(c != NULL)
	{
		v.x = c->GetPropertyF("x");
		v.y = c->GetPropertyF("y");
		v.z = c->GetPropertyF("z");
		v.w = c->GetPropertyF("w");
	}
	return v;
}


void PersistentData::AddColorChild(IvColor& v, const char* name)
{
	PersistentData* c = new PersistentData(name);
	c->SetProperty("r",v.r);
	c->SetProperty("g",v.g);
	c->SetProperty("b",v.b);
	c->SetProperty("a",v.a);
	AddChild(c);
}

IvColor PersistentData::GetColorChild(const char* name)
{
	IvColor v;
	std::string n = name;
	PersistentData* c = GetUniqueChild(n);
	if(c != NULL)
	{
		v.r = c->GetPropertyF("r");
		v.g = c->GetPropertyF("g");
		v.b = c->GetPropertyF("b");
		v.a = c->GetPropertyF("a");
	}
	return v;
}

void PersistentData::AddQuatChild(IvQuat& v, const char* name)
{
	PersistentData* c = new PersistentData(name);
	c->SetProperty("x",v.x);
	c->SetProperty("y",v.y);
	c->SetProperty("z",v.z);
	c->SetProperty("w",v.w);
	AddChild(c);
}

IvQuat PersistentData::GetQuatChild(const char* name)
{
	IvQuat v;
	std::string n = name;
	PersistentData* c = GetUniqueChild(n);
	if(c != NULL)
	{
		v.x = c->GetPropertyF("x");
		v.y = c->GetPropertyF("y");
		v.z = c->GetPropertyF("z");
		v.w = c->GetPropertyF("w");
	}
	return v;
}

void PersistentData::AddMatrix44Child(IvMatrix44& m, const char* name)
{
	//Stuff matrix into a comma separated list of 16 floats
	PersistentData* mtxNode = new PersistentData(name);
	std::string s;
	char buff[128];
	for(int i = 0; i < 15; ++i)
	{
		sprintf(buff,"%3.8f",m.mV[i]);
		s += buff;
		s+= ",";
	}
	sprintf(buff,"%3.8f",m.mV[15]);
	s += buff;
	mtxNode->SetTextProperty(s);
	AddChild(mtxNode);
}

IvMatrix44 PersistentData::GetMatrix44Child(const char* name)
{
	IvMatrix44 m;
	std::string n = name;
	PersistentData* c = GetUniqueChild(n);
	if(c != NULL)
	{
		std::string s = c->GetText();
		std::vector<std::string> vals = SplitString(s, ',');
		int elemCount = (int)vals.size();
		if(elemCount != 16)
		{
			printf("!!!! PersistentData::GetMatrix444Child --> Error Reading a Matrix44... incorrect number of entries.  Should be 16 found %d.\n",elemCount);
		}
		for(int i = 0; i < elemCount; ++i)
		{
			std::string sVal = vals[i];
			float val = (float)atof(sVal.c_str());
			m.mV[i] = val;
		}
	}
	 
	return m;
}

PersistentData* PersistentData::GetUniqueChild(std::string& key)
{
	std::multimap<std::string,PersistentData*>::iterator itr = mChildrenMap.find(key);
	if(itr == mChildrenMap.end())
	{
		return NULL;
	}
	return itr->second;
}

PersistentData* PersistentData::GetNodeFromPath(std::string& path)
{
	std::vector<std::string> nodeNames = SplitString(path, '.');
	PersistentData* currNode = this;
	for(int i = 0; i < (int)nodeNames.size(); ++i)
	{
		std::string nodeName = nodeNames[i];
		currNode = currNode->GetUniqueChild(nodeName);
		if(currNode == NULL)
			break;
	}
	return currNode;

}

PersistentData* PersistentData::GetNodeFromPath(const char* path)
{
	std::string p = path;
	return GetNodeFromPath(p);
}

void PersistentData::GetNodesFromPath(std::string& path,std::list<PersistentData*>& nodes)
{
	std::vector<std::string> nodeNames = SplitString(path, '.');
	PersistentData* currNode = this;
	int lastIndex = (int)nodeNames.size() - 1;
	if( lastIndex < 0)
		return;
	for(int i = 0; i < lastIndex; ++i)
	{
		std::string nodeName = nodeNames[i];
		currNode = currNode->GetUniqueChild(nodeName);
		if(currNode == NULL)
			return;
	}
	currNode->GetChildren(nodeNames[lastIndex],nodes);
	
}

void PersistentData::GetNodesFromPath(const char* path,std::list<PersistentData*>& nodes)
{
	std::string p = path;
	GetNodesFromPath(p,nodes);
}

std::list<PersistentData*>& PersistentData::GetChildren()
{
	return mChildren;
	/*std::list<PersistentData*>::iterator it = mChildren.begin();
	while(it != mChildren.end())
	{
		children.push_back(*it);
		++it;
	}*/
}

void PersistentData::GetChildren(std::string& key, std::list<PersistentData*>& children)
{
	std::multimap<std::string,PersistentData*>::iterator lastElement;
	std::multimap<std::string,PersistentData*>::iterator itr = mChildrenMap.find(key);
	if(itr == mChildrenMap.end())
	{
		return;
	}
	
	lastElement = mChildrenMap.upper_bound(key);
	
	for ( ; itr != lastElement; ++itr)
		children.push_back(itr->second);
	
}

void PersistentData::GetChildren(const char* key,std::list<PersistentData*>& children)
{
	std::string k = key;
	return GetChildren(k,children);
}

void PersistentData::SaveStateToXMLNode(TiXmlElement* node)
{
	std::string str;
	std::map<std::string,std::string>::iterator it = mProperties.begin();
	while(it != mProperties.end())
	{
		if(it->first == QUOTEME(_DBDTEXT_))
		{
			node->LinkEndChild( new TiXmlText(it->second.c_str())); 
		}
		else
		{
			node->SetAttribute(it->first.c_str(),it->second.c_str());
		}
		++it;
	}
}


PersistentDataTree::~PersistentDataTree()
{
	DeleteSubTree(mRoot);
}

void PersistentDataTree::DeleteSubTree(PersistentData* node)
{
	
	std::list<PersistentData*> & children = node->GetChildren();

	
	std::list<PersistentData*> ::iterator it = children.begin();
	while(it != children.end())
	{
		DeleteSubTree(*it);
		++it;
	}

	SAFE_DELETE(node);
	
}

void PersistentDataTree::DumpToStdout(PersistentData* node)
{
	if(node == NULL)
		node = mRoot;
	
	std::cout << node->ToString() << std::endl;
	std::list<PersistentData*> children;
	children = node->GetChildren();
	
	std::list<PersistentData*> ::iterator it = children.begin();
	while(it != children.end())
	{
		DumpToStdout(*it);
		++it;
	}
			
	
}

int PersistentDataManager::LoadXMLAttributesToMultiMap(TiXmlElement* pElement, std::string& docpath, PersistentData* node)
{
	if ( !pElement ) return 0;
	
	TiXmlAttribute* pAttrib=pElement->FirstAttribute();
	int i=0;
	//int ival;
	//double dval;
	//const char* pIndent=xml_getIndent(indent);
	//printf("\n");
	while (pAttrib)
	{
		//printf( "%s@%s=[%s]\n", docpath.c_str(), pAttrib->Name(), pAttrib->Value());
		node->SetProperty(pAttrib->Name(),pAttrib->Value());
		//if (pAttrib->QueryIntValue(&ival)==TIXML_SUCCESS)    printf( " int=%d", ival);
		//if (pAttrib->QueryDoubleValue(&dval)==TIXML_SUCCESS) printf( " d=%1.1f", dval);
		//printf( "\n" );
		i++;
		pAttrib=pAttrib->Next();
	}
	
	return i;	
}

void PersistentDataManager::LoadXMLToMultiMap( TiXmlNode* pParent, std::string& docpath, PersistentData* parentNode)
{
	if ( !pParent ) return;
	
	TiXmlNode* pChild;
	TiXmlText* pText;
	std::string str;
	int t = pParent->Type();
	int num;
	
	switch ( t )
	{
		case TiXmlNode::DOCUMENT:
			break;
			
		case TiXmlNode::ELEMENT:
			num=LoadXMLAttributesToMultiMap(pParent->ToElement(), docpath, parentNode);			
			break;
			
		case TiXmlNode::COMMENT:
			break;
			
		case TiXmlNode::UNKNOWN:
			break;
			
		case TiXmlNode::TEXT:
			pText = pParent->ToText();
			str = pText->Value();
			parentNode->SetProperty(QUOTEME(_DBDTEXT_),str.c_str());
			break;
			
		case TiXmlNode::DECLARATION:
			break;
		default:
			break;
	}
	for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
	{
		PersistentData* node = parentNode;
		std::string newPath = docpath;
		if(pChild && (pChild->Type() == TiXmlNode::ELEMENT))
		{
			if(newPath.size() > 0)
				newPath += ".";
			newPath += pChild->Value();
			PersistentData* newNode = new PersistentData(newPath.c_str(),pChild->Value());
			mDataTree.AddNode(parentNode, newNode);
			node = newNode;
		}		
		LoadXMLToMultiMap( pChild, newPath, node);
	}
}

PersistentData* PersistentDataManager::GetNode(std::string& path)
{
	return mDataTree.GetNode(path);	
}

void PersistentDataManager::GetNodes(std::string& path,std::list<PersistentData*>& nodes)
{
	mDataTree.GetNodes(path,nodes);		
}

// load the named file and dump its structure to STDOUT
void PersistentDataManager::LoadXMLFile(const char* pFilename)
{
	std::string fullpath;
	std::string file = pFilename;
	GetFullFilePathFromResource(file,fullpath);
	
	TiXmlDocument doc(fullpath.c_str());
	bool loadOkay = doc.LoadFile();
	if (loadOkay)
	{
		PersistentData* node = new PersistentData("root","root");
		mDataTree.AddNode(NULL, node);
		std::string docpath = "";
		LoadXMLToMultiMap( &doc ,docpath,node); // defined later in the tutorial
	}
	else
	{
		printf("Failed to load file \"%s\"\n", pFilename);
	}
	
	//mDataTree.DumpToStdout();
	/*
	PersistentData* targetNode = NULL;
	
	targetNode = mDataTree.GetNode("Sim.FluidGrid");
	std::cout << targetNode->ToString() << std::endl;
	
	std::cout << mDataTree.GetNode("Sim.FluidGrid.gridDimension")->GetText() << std::endl;
	std::cout << targetNode->GetNodeFromPath("textureRepeatU")->GetText() << std::endl;
	std::cout << targetNode->GetNodeFromPath("textureRepeatV")->GetText() << std::endl;
	
	std::list<PersistentData*> soundEffectList;
	mDataTree.GetNodes("Sim.SoundEffects.SoundEffect",soundEffectList);
	std::list<PersistentData*>::iterator it = soundEffectList.begin();
	while(it != soundEffectList.end())
	{
		std::cout << (*it)->ToString() << std::endl;
		++it;
	}*/
	
	
	//std::cout << "MultiMap Statistics:" << std::endl;
	//std::list<std::string>::iterator it = mKeys.begin();
	//while(it != mKeys.end()){
	//	std::string key = *it;
	//	std::cout << "  [" << key << "] = " << mKeyCounter.count(key) << "" << std::endl;
	//	++it;
	//}
	
}

void PersistentDataManager::SaveNodeToXMLDoc(PersistentData* node, TiXmlElement* parentXMLNode)
{
	
	node->SaveStateToXMLNode(parentXMLNode);
	std::list<PersistentData*>& children = node->GetChildren();
	std::list<PersistentData*>::iterator it =  children.begin();
	while(it != children.end())
	{
		PersistentData* child = *it;
		TiXmlElement * newXMLNode = new TiXmlElement(child->GetName().c_str());  
		parentXMLNode->LinkEndChild( newXMLNode ); 
		SaveNodeToXMLDoc(child,newXMLNode);
		++it;
	}
	
}

void PersistentDataManager::SaveXMLFile(const char* pFilename, PersistentData* rootNode)
{
	 TiXmlDocument doc;  
	 TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
	 doc.LinkEndChild( decl ); 
	 
	PersistentData* root = rootNode; 
	if(rootNode == NULL)
		root = mDataTree.GetRoot();
	
	std::list<PersistentData*>& children = root->GetChildren();
	std::list<PersistentData*>::iterator it =  children.begin();
	while(it != children.end())
	{
		TiXmlElement * node = new TiXmlElement((*it)->GetName().c_str());  
		doc.LinkEndChild( node ); 
		SaveNodeToXMLDoc(*it,node);
		++it;
	}

	std::string fullpath;
	std::string file = pFilename;
	GetFullFilePathFromDocuments(file,fullpath);
	 doc.SaveFile(fullpath.c_str());  
 }


