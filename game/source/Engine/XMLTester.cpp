/*
 *  XMLTester.cpp
 *  Mac
 *
 *  Created by Kristofer Schlachter on 7/15/09.
 *  Copyright 2009 Gotham Wave Games, Inc. All rights reserved.
 *
 */

#include "XMLTester.h"
#include "Platform.h"

// ----------------------------------------------------------------------
// STDOUT dump and indenting utility functions
// ----------------------------------------------------------------------
void XMLTester::Test(){
	std::string fullpath;
	std::string file = "Config.xml";
	GetFullFilePathFromResource(file,fullpath);
	XMLTester::dump_to_stdout(fullpath.c_str());
	
}

const char * XMLTester::xml_getIndent( unsigned int numIndents )
{
	static const char * pINDENT="                                      + ";
	static const unsigned int LENGTH=strlen( pINDENT );
	unsigned int n=numIndents*NUM_INDENTS_PER_SPACE;
	if ( n > LENGTH ) n = LENGTH;
	
	return &pINDENT[ LENGTH-n ];
}

// same as getIndent but no "+" at the end
const char *XMLTester::getIndentAlt( unsigned int numIndents )
{
	static const char * pINDENT="                                        ";
	static const unsigned int LENGTH=strlen( pINDENT );
	unsigned int n=numIndents*NUM_INDENTS_PER_SPACE;
	if ( n > LENGTH ) n = LENGTH;
	
	return &pINDENT[ LENGTH-n ];
}

int XMLTester::dump_attribs_to_stdout(TiXmlElement* pElement, unsigned int indent)
{
	if ( !pElement ) return 0;
	
	TiXmlAttribute* pAttrib=pElement->FirstAttribute();
	int i=0;
	int ival;
	double dval;
	const char* pIndent=xml_getIndent(indent);
	printf("\n");
	while (pAttrib)
	{
		printf( "%s%s: value=[%s]", pIndent, pAttrib->Name(), pAttrib->Value());
		
		if (pAttrib->QueryIntValue(&ival)==TIXML_SUCCESS)    printf( " int=%d", ival);
		if (pAttrib->QueryDoubleValue(&dval)==TIXML_SUCCESS) printf( " d=%1.1f", dval);
		printf( "\n" );
		i++;
		pAttrib=pAttrib->Next();
	}
	return i;	
}

void XMLTester::dump_to_stdout( TiXmlNode* pParent, unsigned int indent)
{
	if ( !pParent ) return;
	
	TiXmlNode* pChild;
	TiXmlText* pText;
	int t = pParent->Type();
	printf( "%s", xml_getIndent(indent));
	int num;
	
	switch ( t )
	{
		case TiXmlNode::DOCUMENT:
			printf( "Document" );
			break;
			
		case TiXmlNode::ELEMENT:
			printf( "Element [%s]", pParent->Value() );
			num=dump_attribs_to_stdout(pParent->ToElement(), indent+1);
			switch(num)
		{
			case 0:  printf( " (No attributes)"); break;
			case 1:  printf( "%s1 attribute", getIndentAlt(indent)); break;
			default: printf( "%s%d attributes", getIndentAlt(indent), num); break;
		}
			break;
			
		case TiXmlNode::COMMENT:
			printf( "Comment: [%s]", pParent->Value());
			break;
			
		case TiXmlNode::UNKNOWN:
			printf( "Unknown" );
			break;
			
		case TiXmlNode::TEXT:
			pText = pParent->ToText();
			printf( "Text: [%s]", pText->Value() );
			break;
			
		case TiXmlNode::DECLARATION:
			printf( "Declaration" );
			break;
		default:
			break;
	}
	printf( "\n" );
	for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
	{
		dump_to_stdout( pChild, indent+1 );
	}
}

int XMLTester::dump_attribs_to_multimap(TiXmlElement* pElement, std::string docpath)
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
		printf( "%s@%s=[%s]\n", docpath.c_str(), pAttrib->Name(), pAttrib->Value());
		
		//if (pAttrib->QueryIntValue(&ival)==TIXML_SUCCESS)    printf( " int=%d", ival);
		//if (pAttrib->QueryDoubleValue(&dval)==TIXML_SUCCESS) printf( " d=%1.1f", dval);
		//printf( "\n" );
		i++;
		pAttrib=pAttrib->Next();
	}

	return i;	
}

void XMLTester::dump_to_multimap( TiXmlNode* pParent, std::string docpath)
{
	if ( !pParent ) return;
	
	//printf("%s\n",docpath.c_str());
	TiXmlNode* pChild;
	TiXmlText* pText;
	int t = pParent->Type();
	//printf( "%s", xml_getIndent(indent));
	int num;
	
	switch ( t )
	{
		case TiXmlNode::DOCUMENT:
			//printf( "Document" );
			break;
			
		case TiXmlNode::ELEMENT:
			
			//printf( "Element [%s]", pParent->Value() );
			num=dump_attribs_to_multimap(pParent->ToElement(), docpath);
			/*switch(num)
		{
			case 0:  printf( " (No attributes)"); break;
			case 1:  printf( "%s1 attribute", ""); break;
			default: printf( "%s%d attributes", "", num); break;
		}*/
			 
			break;
			
		case TiXmlNode::COMMENT:
			//printf( "Comment: [%s]", pParent->Value());
			break;
			
		case TiXmlNode::UNKNOWN:
			//printf( "Unknown" );
			break;
			
		case TiXmlNode::TEXT:
			pText = pParent->ToText();
			printf( "%s=%s\n",docpath.c_str(), pText->Value() );
			break;
			
		case TiXmlNode::DECLARATION:
			//printf( "Declaration" );
			break;
		default:
			break;
	}
	//printf( "\n" );
	for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
	{
		std::string newPath = docpath;
		if(pChild && (pChild->Type() == TiXmlNode::ELEMENT))
		{
			if(newPath.size() > 0)
				newPath += ".";
			newPath += pChild->Value();
		}
		
		dump_to_multimap( pChild, newPath);
	}
}


// load the named file and dump its structure to STDOUT
void XMLTester::dump_to_stdout(const char* pFilename)
{
	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();
	if (loadOkay)
	{
		//printf("\n%s:\n", pFilename);
		dump_to_multimap( &doc ,""); // defined later in the tutorial
	}
	else
	{
		printf("Failed to load file \"%s\"\n", pFilename);
	}
}
