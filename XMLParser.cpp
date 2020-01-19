
#include "XMLParser.h"
#include <stdlib.h>

XMLParser::XMLParser()
{

}

XMLParser::~XMLParser()
{
}

void XMLParser::traverse_document(xmlNode* a_node, int depth) {
        xmlNode *cur_node = NULL;
        int i=0;
        for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
                if (cur_node->type == XML_ELEMENT_NODE) {
                        for (i=0;i<depth;i++)
                                printf("----");
                        printf("<%s ", cur_node->name);
                        if ( cur_node->properties != NULL)  {
                                _xmlAttr* pAttr=cur_node->properties;
                                while (pAttr != NULL) {
                                        printf(" %s=\"%s\"", pAttr->name, pAttr->children->content);
                                        pAttr=pAttr->next;
                                }
                        }
                        printf("/>");
                        if (cur_node->children != NULL )
                                printf("%s", cur_node->children->content);
                        printf("\n");
                }
                traverse_document(cur_node->children, depth + 1);
        }
}

xmlNodePtr XMLParser::findNodeByName(xmlNodePtr rootnode, const xmlChar * nodename)
{
    xmlNodePtr node = rootnode;
    if(node == NULL){
        printf("Document is empty!\n");
        return NULL;
    }

    while(node != NULL){

        if(!xmlStrcmp(node->name, nodename)){
            return node;
        }
        else if (node->children != NULL) {
                xmlNodePtr intNode =  findNodeByName(node->children, nodename);
                if(intNode != NULL) {
                        return intNode;
                }
        }
        node = node->next;
    }
    return NULL;
}

xmlDocPtr XMLParser::getServerDirectory_8_2(Messenger::Message const& message)
{
        void xmlInitParser(void);	//initialize xmlInitParser
	char itoaBuf[256];		//The buffer for conversion int to char*
        xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
        xmlNodePtr root = xmlNewNode(NULL, BAD_CAST "DDS_request");
/*
	snprintf(itoaBuf, sizeof(itoaBuf), "%d", message.subject_id);
	xmlNewProp(root, (const xmlChar*)"id", (const xmlChar*) itoaBuf);
	xmlNewProp(root, (const xmlChar*)"from", (const xmlChar*)message.from.in());
	xmlNewProp(root, (const xmlChar*)"to", (const xmlChar*)message.to.in());
	xmlNewProp(root, (const xmlChar*)"type", (const xmlChar*)message.type.in());
*/
    xmlDocSetRootElement(doc, root);

        xmlNodePtr AssociationContext = xmlNewTextChild(root, NULL, (const xmlChar*)"AssociationContext", NULL);
	xmlNewProp(AssociationContext, (const xmlChar*)"id", (const xmlChar*) "32:78");

        xmlNodePtr SecurityRequest_PDU = xmlNewTextChild(AssociationContext, NULL, (const xmlChar*)"SecurityRequest-PDU", NULL);

	xmlNodePtr signedData = xmlNewTextChild(SecurityRequest_PDU, NULL, (const xmlChar*)"signedData", NULL);

	xmlNodePtr userData = xmlNewTextChild( signedData, NULL, (const xmlChar*)"userData", NULL);
	
	xmlNodePtr confirmed_RequestPDU  = xmlNewTextChild( userData, NULL, (const xmlChar*)"confirmed-RequestPDU", NULL);

	xmlNodePtr invokeId  = xmlNewTextChild(confirmed_RequestPDU, NULL, (const xmlChar*)"invokeId", (const xmlChar*) itoaBuf);

	xmlNodePtr ConfirmedServiceRequest  = xmlNewTextChild( confirmed_RequestPDU, NULL, (const xmlChar*)"ConfirmedServiceRequest", NULL);

	xmlNodePtr getNameList  = xmlNewTextChild(ConfirmedServiceRequest, NULL, (const xmlChar*)"getNameList", NULL);

	xmlNodePtr extendedObjectClass  = xmlNewTextChild(getNameList, NULL, (const xmlChar*)"extendedObjectClass", NULL);
	
	xmlNodePtr objectClass  = xmlNewTextChild(extendedObjectClass, NULL, (const xmlChar*)"objectClass", (const xmlChar*) "domain");

	xmlNodePtr objectScope  = xmlNewTextChild(extendedObjectClass, NULL, (const xmlChar*)"objectScope", NULL);

	xmlNodePtr vmdSpecific  = xmlNewTextChild(objectScope , NULL, (const xmlChar*)"vmdSpecific", NULL);

        //xmlSaveFormatFile("testXML.xml", doc, 1);

        return doc;
}

xmlDocPtr XMLParser::getServerDirectory_resp_8_2(Messenger::Message const& message)
{
        void xmlInitParser(void);       //initialize xmlInitParser
        char itoaBuf[256];              //The buffer for conversion int to char*
        xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
        xmlNodePtr root = xmlNewNode(NULL, BAD_CAST "DDS_response");
/*
        snprintf(itoaBuf, sizeof(itoaBuf), "%d", message.subject_id);
        xmlNewProp(root, (const xmlChar*)"id", (const xmlChar*) itoaBuf);
        xmlNewProp(root, (const xmlChar*)"from", (const xmlChar*)message.from.in());
        xmlNewProp(root, (const xmlChar*)"to", (const xmlChar*)message.to.in());
        xmlNewProp(root, (const xmlChar*)"type", (const xmlChar*)message.type.in());
 */ 
        xmlDocSetRootElement(doc, root);

        xmlNodePtr AssociationContext = xmlNewTextChild(root, NULL, (const xmlChar*)"AssociationContext", NULL);
        xmlNewProp(AssociationContext, (const xmlChar*)"id", (const xmlChar*) "32:78");

        xmlNodePtr SecurityRequest_PDU = xmlNewTextChild(AssociationContext, NULL, (const xmlChar*)"SecurityRequest-PDU", NULL);

        xmlNodePtr signedData = xmlNewTextChild(SecurityRequest_PDU, NULL, (const xmlChar*)"signedData", NULL);

        xmlNodePtr userData = xmlNewTextChild( signedData, NULL, (const xmlChar*)"userData", NULL);

        xmlNodePtr confirmed_RequestPDU  = xmlNewTextChild( userData, NULL, (const xmlChar*)"confirmed-ResponsePDU", NULL);

        xmlNodePtr invokeId  = xmlNewTextChild(confirmed_RequestPDU, NULL, (const xmlChar*)"invokeId", (const xmlChar*) itoaBuf);

        xmlNodePtr ConfirmedServiceRequest  = xmlNewTextChild( confirmed_RequestPDU, NULL, (const xmlChar*)"ConfirmedServiceResponse", NULL);

        xmlNodePtr getNameList  = xmlNewTextChild(ConfirmedServiceRequest, NULL, (const xmlChar*)"getNameList", NULL);

        xmlNodePtr extendedObjectClass  = xmlNewTextChild(getNameList, NULL, (const xmlChar*)"listOfIdentifier", NULL);

        xmlNodePtr objectClass  = xmlNewTextChild(extendedObjectClass, NULL, (const xmlChar*)"Identifier", (const xmlChar*) "MUMEAS");

        //xmlSaveFormatFile("testXML.xml", doc, 1);

        return doc;
}

xmlDocPtr XMLParser::getDataValue_8_2(Messenger::Message const& message, vector<string> itemIds)
{
        void xmlInitParser(void);	//initialize xmlInitParser
	char itoaBuf[256];		//The buffer for conversion int to char*
        xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
        xmlNodePtr root = xmlNewNode(NULL, BAD_CAST "DDS_request");
/*
	snprintf(itoaBuf, sizeof(itoaBuf), "%d", message.subject_id);
	xmlNewProp(root, (const xmlChar*)"id", (const xmlChar*) itoaBuf);
	xmlNewProp(root, (const xmlChar*)"from", (const xmlChar*)message.from.in());
	xmlNewProp(root, (const xmlChar*)"to", (const xmlChar*)message.to.in());
	xmlNewProp(root, (const xmlChar*)"type", (const xmlChar*)message.type.in());
*/  
    xmlDocSetRootElement(doc, root);

	xmlNodePtr confirmed_RequestPDU  = xmlNewTextChild(root, NULL, (const xmlChar*)"confirmed-RequestPDU", NULL);

	xmlNodePtr Read  = xmlNewTextChild(confirmed_RequestPDU, NULL, (const xmlChar*)"Read", NULL);

	xmlNodePtr VARIABLEACCESSSPECIFICATN = xmlNewTextChild(Read, NULL, (const xmlChar*)"VARIABLEACCESSSPECIFICATN", NULL);

	xmlNodePtr LISTOFVARIABLE  = xmlNewTextChild(VARIABLEACCESSSPECIFICATN, NULL, (const xmlChar*)"LISTOFVARIABLE", NULL);

	xmlNodePtr SEQUENCE = xmlNewTextChild(LISTOFVARIABLE, NULL, (const xmlChar*)"SEQUENCE", NULL);
	
	xmlNodePtr VARIABLESPECIFICATION  = xmlNewTextChild(SEQUENCE, NULL, (const xmlChar*)"VARIABLESPECIFICATION", NULL);

	xmlNodePtr NAME  = xmlNewTextChild(VARIABLESPECIFICATION, NULL, (const xmlChar*)"NAME", NULL);

	xmlNodePtr DOMAIN_SPECIFIC  = xmlNewTextChild(NAME, NULL, (const xmlChar*)"DOMAIN-SPECIFIC", NULL);

	xmlNodePtr DOMAINID  = xmlNewTextChild(DOMAIN_SPECIFIC, NULL, (const xmlChar*)"DOMAINID", (const xmlChar*)"SOLAR01");

	for(int u=0; u < itemIds.size(); u++)
	{
		xmlNewTextChild(DOMAIN_SPECIFIC, NULL, (const xmlChar*)"DOMAINID", (const xmlChar*) itemIds[u].c_str());
	}
	return doc;
}

xmlDocPtr XMLParser::getDataValue_resp_8_2(Messenger::Message const& message, vector<string> items)
{
        void xmlInitParser(void);	//initialize xmlInitParser
	char itoaBuf[256];		//The buffer for conversion int to char*
        xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
        xmlNodePtr root = xmlNewNode(NULL, BAD_CAST "DDS_response");
/*
	snprintf(itoaBuf, sizeof(itoaBuf), "%d", message.subject_id);
	xmlNewProp(root, (const xmlChar*)"id", (const xmlChar*) itoaBuf);
	xmlNewProp(root, (const xmlChar*)"from", (const xmlChar*)message.from.in());
	xmlNewProp(root, (const xmlChar*)"to", (const xmlChar*)message.to.in());
	xmlNewProp(root, (const xmlChar*)"type", (const xmlChar*)message.type.in());
*/  
    xmlDocSetRootElement(doc, root);

	xmlNodePtr confirmed_ResponsePDU = xmlNewTextChild(root, NULL, (const xmlChar*)"confirmed-ResponsePDU", NULL);

	xmlNodePtr ConfirmedServiceResponse  = xmlNewTextChild(confirmed_ResponsePDU, NULL, (const xmlChar*)"ConfirmedServiceResponse", NULL);

	xmlNodePtr Read  = xmlNewTextChild(ConfirmedServiceResponse, NULL, (const xmlChar*)"Read", NULL);

	xmlNodePtr listOfAccessResult  = xmlNewTextChild(Read, NULL, (const xmlChar*)"listOfAccessResult", NULL);

	xmlNodePtr success = xmlNewTextChild(listOfAccessResult, NULL, (const xmlChar*)"success", NULL);
	
	for(int u=0; u < items.size(); u++)
	{
		xmlNewTextChild(success, NULL, (const xmlChar*)"float", (const xmlChar*) items[u].c_str());
	}

	return doc;
}
xmlDocPtr XMLParser::setDataValue_8_2(Messenger::Message const& message, vector<string> itemId1s)
{
	void xmlInitParser(void);	//initialize xmlInitParser
	char itoaBuf[256];		//The buffer for conversion int to char
	xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
	xmlNodePtr root = xmlNewNode(NULL, BAD_CAST "DDS_Setresponse");
	
	/*	snprintf(itoaBuf, sizeof(itoaBuf), "%d", message.subject_id);
		xmlNewProp(root, (const xmlChar*)"id", (const xmlChar*) itoaBuf);
		xmlNewProp(root, (const xmlChar*)"from", (const xmlChar*)message.from.in());
		xmlNewProp(root, (const xmlChar*)"to", (const xmlChar*)message.to.in());
		xmlNewProp(root, (const xmlChar*)"type", (const xmlChar*)message.type.in());
	*/
	xmlDocSetRootElement(doc, root);

	xmlNodePtr confirmed_RequestPDU = xmlNewTextChild(root, NULL, (const xmlChar*)"confirmed-RequestPDU", NULL);

	xmlNodePtr Write = xmlNewTextChild(confirmed_RequestPDU, NULL, (const xmlChar*)"Write", NULL);

	xmlNodePtr VARIABLEACCESSSPECIFICATN = xmlNewTextChild(Write, NULL, (const xmlChar*)"VARIABLEACCESSSPECIFICATN", NULL);

	xmlNodePtr LISTOFVARIABLE = xmlNewTextChild(VARIABLEACCESSSPECIFICATN, NULL, (const xmlChar*)"LISTOFVARIABLE", NULL);

	xmlNodePtr SEQUENCE = xmlNewTextChild(LISTOFVARIABLE, NULL, (const xmlChar*)"SEQUENCE", NULL);

	xmlNodePtr VARIABLESPECIFICATION = xmlNewTextChild(SEQUENCE, NULL, (const xmlChar*)"VARIABLESPECIFICATION", NULL);

	xmlNodePtr NAME = xmlNewTextChild(VARIABLESPECIFICATION, NULL, (const xmlChar*)"NAME", NULL);

	xmlNodePtr DOMAIN_SPECIFIC = xmlNewTextChild(NAME, NULL, (const xmlChar*)"DOMAIN-SPECIFIC", NULL);

	xmlNodePtr DOMAINID = xmlNewTextChild(DOMAIN_SPECIFIC, NULL, (const xmlChar*)"DOMAINID", (const xmlChar*)"SOLAR01");

	for (int u = 0; u < itemId1s.size(); u++)
	{
		xmlNewTextChild(DOMAIN_SPECIFIC, NULL, (const xmlChar*)"DOMAINID", (const xmlChar*)itemId1s[u].c_str());
	}
	return doc;
}
xmlDocPtr XMLParser::setDataValue_resp_8_2(Messenger::Message const& message, vector<string> item1s)
{
	void xmlInitParser(void);	//initialize xmlInitParser
	char itoaBuf[256];		//The buffer for conversion int to char*
	xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
	xmlNodePtr root = xmlNewNode(NULL, BAD_CAST "DDS_setrequest");
	/*
		snprintf(itoaBuf, sizeof(itoaBuf), "%d", message.subject_id);
		xmlNewProp(root, (const xmlChar*)"id", (const xmlChar*) itoaBuf);
		xmlNewProp(root, (const xmlChar*)"from", (const xmlChar*)message.from.in());
		xmlNewProp(root, (const xmlChar*)"to", (const xmlChar*)message.to.in());
		xmlNewProp(root, (const xmlChar*)"type", (const xmlChar*)message.type.in());
	*/
	xmlDocSetRootElement(doc, root);

	xmlNodePtr confirmed_ResponsePDU = xmlNewTextChild(root, NULL, (const xmlChar*)"confirmed-ResponsePDU", NULL);

	xmlNodePtr ConfirmedServiceResponse = xmlNewTextChild(confirmed_ResponsePDU, NULL, (const xmlChar*)"ConfirmedServiceResponse", NULL);

	xmlNodePtr Write = xmlNewTextChild(ConfirmedServiceResponse, NULL, (const xmlChar*)"Write", NULL);

	xmlNodePtr CHOICE = xmlNewTextChild(Write, NULL, (const xmlChar*)"CHOICE", NULL);

	xmlNodePtr success = xmlNewTextChild(CHOICE, NULL, (const xmlChar*)"success", NULL);

	for (int u = 0; u < item1s.size(); u++)
	{
		xmlNewTextChild(success, NULL, (const xmlChar*)"float", (const xmlChar*)item1s[u].c_str());
	}

	return doc;
}

void XMLParser::freeXMLDoc(xmlDocPtr doc)
{
	if(doc != NULL) {
        	xmlFreeDoc(doc);
                doc = NULL;
    	}
}

