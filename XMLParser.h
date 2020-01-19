	

#pragma once
#include "MessengerTypeSupportC.h"
#include "MessengerTypeSupportImpl.h"

#include <stdio.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <vector>

using namespace std;
class XMLParser{
public:
	XMLParser();
	~XMLParser();

	void traverse_document(xmlNode*, int);

    xmlNodePtr findNodeByName(xmlNodePtr, const xmlChar *);
    //xmlNodePtr findNodeByName__(xmlNodePtr, const xmlChar *, char[][255]);
	
    xmlDocPtr getServerDirectory_8_2(Messenger::Message const &);
	
    xmlDocPtr getServerDirectory_resp_8_2(Messenger::Message const &);
	
    xmlDocPtr getDataValue_8_2(Messenger::Message const &, vector<string>);
	
    xmlDocPtr getDataValue_resp_8_2(Messenger::Message const &, vector<string>);

    xmlDocPtr setDataValue_8_2(Messenger::Message const& , vector<string>);
    
    xmlDocPtr setDataValue_resp_8_2(Messenger::Message const& , vector<string>);    
	
    void freeXMLDoc(xmlDocPtr );

private:
		

};
