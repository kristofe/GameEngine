#ifndef _XML_TESTER_H
#define _XML_TESTER_H
#include "tinyxml.h"
#include <string>

class XMLTester{
public:
	static const unsigned int NUM_INDENTS_PER_SPACE=2;
	static const char * xml_getIndent( unsigned int numIndents );
	static const char * getIndentAlt( unsigned int numIndents );
	static int dump_attribs_to_stdout(TiXmlElement* pElement, unsigned int indent);
	static int dump_attribs_to_multimap(TiXmlElement* pElement, std::string docpath);
	static void dump_to_stdout( TiXmlNode* pParent, unsigned int indent = 0 );
	static void dump_to_multimap( TiXmlNode* pParent, std::string docpath);
	static void dump_to_stdout(const char* pFilename);
	static void Test();
	
	
};

#endif

