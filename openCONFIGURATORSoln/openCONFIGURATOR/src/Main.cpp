///////////////////////////////////////////////////////////////////////////////////////////////
//
//  $Header: $
//
// NAME:  ClassNAME
//
// BASE  CLASSES: none
//  
// PURPOSE:  purpose description
//
// AUTHOR:  
//
//  COPYRIGHT NOTICE:
//
//	****************************************************************************

// (c) Kalycito Infotech Private Limited
//
//  Project:      openCONFIGURATOR 
//
//  Description:  
//
//
//  License:
//
//    Redistribution and use in source and binary forms, with or without
//    modification, are permitted provided that the following conditions
//    are met:
//
//    1. Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//    2. Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//
//    3. Neither the name of Kalycito Infotech Private Limited nor the names of 
//       its contributors may be used to endorse or promote products derived
//       from this software without prior written permission. For written
//       permission, please contact info@kalycito.com.
//
//    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
//    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
//    COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
//    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//    ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//    POSSIBILITY OF SUCH DAMAGE.
//
//    Severability Clause:
//
//        If a provision of this License is or becomes illegal, invalid or
//        unenforceable in any jurisdiction, that shall not affect:
//        1. the validity or enforceability in that jurisdiction of any other
//           provision of this License; or
//        2. the validity or enforceability in other jurisdictions of that or
//           any other provision of this License.
//
//****************************************************************************/
//
//  REVISION HISTORY:
// $Log:      $
///////////////////////////////////////////////////////////////////////////////////////////////

#include "../Include/openCONFIGURATOR.h"
#include "../Include/Internal.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <libxml/xmlreader.h>
#include <errno.h>
#include <string.h>

char* strerr;
int LastIndexParsed=0;

void LoadObjectDictionary()
	{
	
		//Parse the ObjectDictionary xml and creates the objectDictionary object
		
	}
	
static void setIndexAttributes(xmlTextReaderPtr reader, CIndex* objIndex)
	{
		const xmlChar* name, *value;
		//Retrieve the name and Value of an attribute
		value = xmlTextReaderConstValue(reader);
		name = xmlTextReaderConstName(reader);		

		if(strcmp(ConvertToUpper((char*)name), "INDEX")==0)
			{	
				// For holding temp data to print
				char* Buffer;
						
				// Setting the Index Value
				objIndex->setIndexValue((char*)value);
				
				// For Testing. 
				Buffer = (char *) malloc(sizeof(objIndex->getIndexValue()));
				strcpy(Buffer, objIndex->getIndexValue());
				printf("\tName:%s, objIndex->getIndexValue:%s\n", name, Buffer);
				
				/*$S Commented out the TPDO and RPDO grouping $S*/
				//TODO: Check point 1 
				char* data = (char*) value;
				if(strncmp(data,"14",2)==0 ||strncmp(data,"16",2) ==0)
					{
						objIndex->setPDOType(PDO_RPDO);
					}
				else if(strncmp(data,"18",2 )==0 ||strncmp(data,"1A",2) ==0)
					{
						objIndex->setPDOType(PDO_TPDO);
					}

			}

		else if( strcmp(ConvertToUpper((char*)name), "NAME") == 0 )
		{	
			objIndex->setName((char*)value);
			//objIndex->setName("CFM_VerifyConfiguration_REC12ksdhksdhk");
			// Test print
			printf("\tName:%s, objIndex->getName:%s\n", name, objIndex->getName());
		}

		else if(strcmp(ConvertToUpper((char*)name), "OBJECTTYPE")==0)					
		objIndex->setObjectType(atoi((const char*)value));

		else if(strcmp(ConvertToUpper((char*)name), "LOWLIMIT")==0)
		{
			objIndex->setLowLimit((char*)value);
			//objIndex->setLowLimit("123456789101112131415");
			//printf("\tName:%s, objIndex->getLowLimit:%s\n", name, objIndex->getLowLimit());
		}

		else if(strcmp(ConvertToUpper((char*)name), "HIGHLIMIT")==0)					
		objIndex->setHighLimit((char*)value);

		else if(strcmp(ConvertToUpper((char*)name), "ACCESSTYPE")==0)					
		objIndex->setAccessType((char*)value);

		else if(strcmp(ConvertToUpper((char*)name), "PDOMAPPING")==0)					
		objIndex->setPDOMapping((char*)value);
		

		else if(strcmp(ConvertToUpper((char*)name), "DEFAULTVALUE")==0)					
		objIndex->setDefaultValue((char*)value);

		else if(strcmp(ConvertToUpper((char*)name), "ACTUALVALUE")==0)					
		objIndex->setActualValue((char*)value);

		else if(strcmp(ConvertToUpper((char*)name), "DATATYPE")==0)					
		objIndex->setDataType((char*)value);							

		else if(strcmp(ConvertToUpper((char*)name), "UNIQUEIDREF")==0)					
		objIndex->setUniqueIDRef((char*)value);							
	
																	
	}
static void setSubIndexAttributes(xmlTextReaderPtr reader, CSubIndex* objSubIndex)
	{
		const xmlChar* name,*value;
		//Retrieve the name and Value of an attribute
		value = xmlTextReaderConstValue(reader);
		name =xmlTextReaderConstName(reader);							

		if(strcmp(ConvertToUpper((char*)name), "SUBINDEX")==0)						
		objSubIndex->setIndexValue((char*)value);

		else if(strcmp(ConvertToUpper((char*)name), "NAME")==0)					
		objSubIndex->setName((char*)value);

		else if(strcmp(ConvertToUpper((char*)name), "OBJECTTYPE")==0)					
		objSubIndex->setObjectType((int)value);

		else if(strcmp(ConvertToUpper((char*)name), "LOWLIMIT")==0)					
		objSubIndex->setLowLimit((char*)value);

		else if(strcmp(ConvertToUpper((char*)name), "HIGHLIMIT")==0)					
		objSubIndex->setHighLimit((char*)value);

		else if(strcmp(ConvertToUpper((char*)name), "ACCESSTYPE")==0)					
		objSubIndex->setAccessType((char*)value);

		else if(strcmp(ConvertToUpper((char*)name), "PDOMAPPING")==0)					
		objSubIndex->setPDOMapping((char*)value);

		else if(strcmp(ConvertToUpper((char*)name), "DEFAULTVALUE")==0)					
		objSubIndex->setDefaultValue((char*)value);

		else if(strcmp(ConvertToUpper((char*)name), "ACTUALVALUE")==0)					
		objSubIndex->setActualValue((char*)value);

		else if(strcmp(ConvertToUpper((char*)name), "DATATYPE")==0)					
		objSubIndex->setDataType((char*)value);		

		else if(strcmp(ConvertToUpper((char*)name), "UNIQUEIDREF")==0)					
		objSubIndex->setUniqueIDRef((char*)value);							
		
		
														
	}
	
static void setDataTypeAttributes(xmlTextReaderPtr reader ,DataType* objDataType)
	{
		const xmlChar* name,*value;
		int ret;

		objDataType->Name = NULL;
		objDataType->DataTypeValue = NULL;
		objDataType->DataSize = NULL;
		
		//printf("\nInside DataType");
		
		//Retrieve the name and Value of an attribute
		value = xmlTextReaderConstValue(reader);
		name =xmlTextReaderConstName(reader);
		//printf("Value_1:%s\n", value);							

		if(strcmp(ConvertToUpper((char*)name), "DATATYPE") == 0)						
		{
			objDataType->DataTypeValue =new char[strlen((char*)value) + 1];
			strcpy(objDataType->DataTypeValue, (char*)value);
			//printf("objDataType->DataTypeValue:%s\n",objDataType->DataTypeValue);
			//Read the Equivalent name of a datatype
			ret = xmlTextReaderRead(reader);
			ret = xmlTextReaderRead(reader);
			value = xmlTextReaderConstValue(reader);
			name =xmlTextReaderConstName(reader);
					
			/*objDataType->setName((char*)name);*/
			objDataType->Name = new char[strlen((char*)name) + 1];
			//printf("\n Size:%d",sizeof(objDataType->Name));
			//printf("\n Size of Name:%d", strlen((char*)name));
			strcpy(objDataType->Name, (char*)name);
			//printf("\n outsideName:%s", objDataType->Name);
			printf("objDataType->DataTypeValue:%s, objDataType->Name:%s\n", objDataType->DataTypeValue, objDataType->Name);
			objDataType->DataSize = new char[4];
			if((strcmp(ConvertToUpper(objDataType->Name),"UNSIGNED8")==0) ||
				(strcmp(ConvertToUpper(objDataType->Name),"BOOLEAN")==0 ) ||
				(strcmp(ConvertToUpper(objDataType->Name),"INTEGER8")==0 ))
					strcpy(objDataType->DataSize,"0001");

			if((strcmp(ConvertToUpper(objDataType->Name),"UNSIGNED16")==0) ||
				(strcmp(ConvertToUpper(objDataType->Name),"INTEGER16")==0 ))
				strcpy(objDataType->DataSize,"0002");

			if((strcmp(ConvertToUpper(objDataType->Name),"UNSIGNED32")==0) ||
				(strcmp(ConvertToUpper(objDataType->Name),"INTEGER32")==0 ) ||
				(strcmp(ConvertToUpper(objDataType->Name),"REAL32")==0 ))
				strcpy(objDataType->DataSize,"0004");
				
				if(strcmp(ConvertToUpper(objDataType->Name),"UNSIGNED64")==0)
				strcpy(objDataType->DataSize,"0008");
			
				}
				printf("\noutside DataType");
	}
/**************************************************************************************************
	* Function Name: AddIndexAttributes
    * Description: Adds the default attributes to the Index, when addded.
/****************************************************************************************************/
static void AddIndexAttributes(char* IndexID, CIndex* objIndex)
	{
			
				// Setting the Index Value
				objIndex->setIndexValue(IndexID);
				//$S The actual value for all the attributes has to come from ObjDict.xdd
				objIndex->setName("Test_Name");
				char* value = "Test_Values";
				objIndex->setObjectType(atoi((const char*)value));
				objIndex->setLowLimit((char*)value);
				objIndex->setHighLimit((char*)value);
				objIndex->setAccessType((char*)value);
				//objIndex->setPDOMapping((char*)value);				
				objIndex->setDefaultValue((char*)value);
				//objIndex->setActualValue((char*)value);
				objIndex->setDataType((char*)value);
	}

/**************************************************************************************************
	* Function Name: AddSubIndexAttributes
    * Description: Adds the default attributes to the Index, when addded.
/****************************************************************************************************/
static void AddSubIndexAttributes(char* SubIndexID, CSubIndex* objSubIndex)
	{
			
				// Setting the Index Value
				objSubIndex->setIndexValue(SubIndexID);
				//$S The actual value for all the attributes has to come from ObjDict.xdd
				objSubIndex->setName("Test_Name");
				char* value = "Test_Values";
				objSubIndex->setObjectType(atoi((const char*)value));
				objSubIndex->setLowLimit((char*)value);
				objSubIndex->setHighLimit((char*)value);
				objSubIndex->setAccessType((char*)value);
				//objIndex->setPDOMapping((char*)value);				
				objSubIndex->setDefaultValue((char*)value);
				//objIndex->setActualValue((char*)value);
				objSubIndex->setDataType((char*)value);
	}

static void setParameterAttributes(xmlTextReaderPtr reader, Parameter* stParameter)
	{
		const xmlChar* name,*value;
		//Retrieve the name and Value of an attribute
		value = xmlTextReaderConstValue(reader);
		name =xmlTextReaderConstName(reader);							

		if(strcmp(ConvertToUpper((char*)name), "UNIQUEID")==0)
		{						
			stParameter->UniqueID = new char[strlen((char*)name)];
			strcpy(stParameter->UniqueID,(char*)name);
		}

		else if(strcmp(ConvertToUpper((char*)name), "NAME")==0)					
		{						
			stParameter->Name = new char[strlen((char*)name)];
			strcpy(stParameter->Name,(char*)name);
		}

		/* TO DO: DATATYPE..There is no tag for it..need to check after how many reads datatype is define </label>
            <USINT/>*/					
			
	}
	
/**************************************************************************************************
	* Function Name: ImportXML
    * Description: Imports the XML file
/****************************************************************************************************/

int ImportXML(char* fileName, char* errorString, int NodeID, ENodeType NodeType)
	{
		/*char* fileLocation;
		fileLocation= fileName + filePath;*/
		//fileLocation ="E:\\Kalycito\\checkout\\Source\\SharedLibrary\\openCONFIGURATORSoln\\XMLParser\\12345678_MyName.xml";
		//parseFile("E:\\Kalycito\\checkout\\Source\\SharedLibrary\\openCONFIGURATORSoln\\XMLParser\\12345678_MyName_BitCoding.xml",(ENodeType)NodeType,NodeID);

		printf("Inside Importxml");
		parseFile(fileName, NodeID, NodeType);
		//Cleanup function for the XML library.
   
		/* Check if UniqueIDRefs are present, fetch the value from parameter and struct tags*/
		//ProcessUniqueIDRefs();

		/* Process PDO Objects*/
		//ProcessPDONodes(NodeID);
		
		printf("Parsing Done");
		 xmlCleanupParser();
    /*
     * this is to debug memory for regression tests
     */
    xmlMemoryDump();
    
		return(1);


	}
/**************************************************************************************************
	* Function Name: processNode
    * Description: Process the Node value,Name and its attributes
/****************************************************************************************************/
void processNode(xmlTextReaderPtr reader,ENodeType NodeType,int NodeIndex)
{
		const xmlChar *name, *value;
	
		CNodeCollection *objNodeCollection;
		CNode objNode;
	
    name = xmlTextReaderConstName(reader);
    if (name == NULL)
		printf("\n\n\n\nGot NULL for Name\n\n\n\n");
	//	name = BAD_CAST "--";		
    value = xmlTextReaderConstValue(reader);
    
	//If the NodeTYPE is ELEMENT
	if( xmlTextReaderNodeType(reader)==1)
		{
			if(strcmp(((char*)name),"defType")==0)
				{
					objNodeCollection= CNodeCollection::getNodeColObjectPointer();
					CDataTypeCollection* objDataTypeCollection;			
					//printf("createed Nodecol\n");
					DataType objDataType;
					if (xmlTextReaderHasAttributes(reader)==1)
						{						
							while(xmlTextReaderMoveToNextAttribute(reader))
								{
									//printf("Calling setDataTypeAttributes..\n ");
									setDataTypeAttributes(reader, &objDataType);
									//$Svalue = xmlTextReaderConstValue(reader);
									//$Sprintf("Value_1:%s\n", value);
									//$Sname =xmlTextReaderConstName(reader);																									
								}
						}

					objNode = objNodeCollection->getNode(NodeType,NodeIndex);							
					objDataTypeCollection = objNode.getDataTypeCollection();
					objDataTypeCollection->addDataType(objDataType);
			

				}
			if(strcmp(((char*)name),"Parameter")==0)
				{
					objNodeCollection= CNodeCollection::getNodeColObjectPointer();					
					Parameter stParameter;

					
					objNode = objNodeCollection->getNode(NodeType, NodeIndex);	
					
					if (xmlTextReaderHasAttributes(reader)==1)
						{						
							while(xmlTextReaderMoveToNextAttribute(reader))
								{
										setParameterAttributes(reader,&stParameter);																															
								}
						}
						
						// Add parameter to the parameter collection of a node
						objNode.addParameter(stParameter);				
					
				}
			if(strcmp(((char*)name),"Object")==0)
				{
					objNodeCollection= CNodeCollection::getNodeColObjectPointer();
					CIndexCollection* objIndexCollection;
					CIndex objIndex;

					
					objNode = objNodeCollection->getNode(NodeType, NodeIndex);	
						//Set the NodeID
						objIndex.setNodeID(objNode.getNodeId());

					if (xmlTextReaderHasAttributes(reader)==1)
						{						
							while(xmlTextReaderMoveToNextAttribute(reader))
								{
									//printf("Calling setIndexAttributes..\n ");
									setIndexAttributes(reader, &objIndex);	
									//$Svalue = xmlTextReaderConstValue(reader);
									//$Sprintf("\tValue_2:%s\n", value);
									//$Sname =xmlTextReaderConstName(reader);																								
								}
						}
						
						objIndexCollection = objNode.getIndexCollection();
						
						//Add Index object to the IndexCollection
						objIndexCollection->addIndex(objIndex);				
						LastIndexParsed = objIndexCollection->getNumberofIndexes()-1;
						
				}
			if(strcmp(((char*)name),"SubObject")==0 )
				{
					objNodeCollection = CNodeCollection::getNodeColObjectPointer();
					CIndexCollection* objIndexCollection;
					CSubIndex objSubIndex;
					objNode =objNodeCollection->getNode(NodeType,NodeIndex);
						//Set the NodeID
						objSubIndex.setNodeID(objNode.getNodeId());		

					if (xmlTextReaderHasAttributes(reader) ==1)
						{
							
							while(xmlTextReaderMoveToNextAttribute(reader))
								{
									setSubIndexAttributes(reader,&objSubIndex);
									//value = xmlTextReaderConstValue(reader);
									//printf("\t\tValue_3:%s\n", value);
									//name =xmlTextReaderConstName(reader);																									
								}
						}
						CIndex* objIndexPtr;
								
				
						objIndexCollection =objNode.getIndexCollection();
						objIndexPtr =objIndexCollection->getIndex(LastIndexParsed);
										
						if(strcmp(objIndexPtr->getIndexValue(),"2103")==0)
							{
								if(strcmp(objSubIndex.getIndexValue(),"17")==0)
									{	
									printf("%d",objIndexPtr->getNumberofSubIndexes());
									}
							}
						printf("LastIndexParsed: %d-%s\n",LastIndexParsed, objIndexPtr->getIndexValue());
						printf("SubIndex value: %s \n",objSubIndex.getIndexValue());
						objIndexPtr->addSubIndex(objSubIndex);

						////free memory
						//objIndexPtr->~CIndex();
						//delete objIndexPtr;
						/*printf("subindex deleted:%s",objIndexPtr->getIndexValue());*/
						//printf("Index added\n");
						
						
				}	
			}
		
			
}
/**************************************************************************************************
	* Function Name: parseFile
    * Description: Parses the XML file
/****************************************************************************************************/
void parseFile(char* filename, int NodeIndex, ENodeType  NodeType) 
{
    xmlTextReaderPtr reader;
    int ret;

		
    reader = xmlReaderForFile(filename, NULL, 0);
    if (reader != NULL)
				{
        ret = xmlTextReaderRead(reader);
        while (ret == 1)
								{		
										processNode(reader,NodeType,NodeIndex );
										ret = xmlTextReaderRead(reader);
								}
				}
				else
				{
					  fprintf(stderr, "Unable to open %s\n", filename);
				}
		
				printf("\n\n\nCalling xmlFreeTextReader\n\n\n\n");

	//				xmlFreeTextReader(reader);
 //       if (ret != 0)
	//				{
 //           fprintf(stderr, "%s : failed to parse\n", filename);
	//		/*strerr= strerror(errno);
	//		strerr =" Error:" +strerr;*/
	//				}
 //   }
	//else 
	//	{
	//		/*strerr=strerror(errno);
	//		strerr =" Error:" +strerr;*/
 //      fprintf(stderr, "Unable to open %s\n", filename);
	//	}
 }
 
/**************************************************************************************************
	* Function Name: ReImport
    * Description: Parses the XML file
	Return value Legend:
	Cannot ReImport 		- -1
	ReImport Success 		- 1
/****************************************************************************************************/
int ReImportXML(char* fileName, char* errorString, int NodeID, ENodeType NodeType)
{
	
	if((IfNodeExists(NodeID, NodeType, errorString)) >= 0)
	{
		CNode objNode;		
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;
		CDataTypeCollection *objDataTypeCollection;
		CIndex objIndex;

		objIndex.setNodeID(objNode.getNodeId());
		objNodeCollection= CNodeCollection::getNodeColObjectPointer();
		objNode = objNodeCollection->getNode(CN, NodeID);
		
		objDataTypeCollection = objNode.getDataTypeCollection();

		objIndexCollection = objNode.getIndexCollection();
		//cout<< "Number of DataType:" << objDataTypeCollection->getNumberOfDataTypes() << endl;
		// Delete IndexCollection
		objIndexCollection->DeleteIndexCollection();
		// Delete DataTypeCollection
		objDataTypeCollection->DeleteDataTypeCollection();
		//cout<< "Number of DataType:" << objDataTypeCollection->getNumberOfDataTypes() << endl;
		parseFile(fileName, NodeID, NodeType);
		return 1;
	}
	else
	{
		printf("\nCannot ReImport!!\n");
		return -1;
	}
} 
 
/**************************************************************************************************
	* Function Name: CreateTree
    * Description:
/****************************************************************************************************/
void CreateTree()
	{
		//objNodeCollection= CNodeCollection::getNodeColObject();
		
	}
/**************************************************************************************************
	* Function Name: CreateNode
    * Description:
/****************************************************************************************************/
void CreateNode(int NodeID, ENodeType NodeType)
	{
		ocfmRetCode Ret;
		CNode objNode;
		CNodeCollection *objNodeCollection;

		/* Check if the Node already exists with the same NodeID*/
		
		objNode.setNodeId(NodeID);
		objNode.setNodeType(NodeType);
		
		objNode.CreateIndexCollection();
		objNode.CreateDataTypeCollection();

		objNodeCollection = CNodeCollection::getNodeColObjectPointer();
		objNodeCollection->addNode(objNode);		
	}

/**************************************************************************************************
	* Function Name: DeleteNode
    * Description:
/****************************************************************************************************/
void DeleteNode(int NodePos)
	{
		int count;
		CNode objNode;		
		CNodeCollection *objNodeCollection;
		objNodeCollection= CNodeCollection::getNodeColObjectPointer();	
		cout<< "Inside DeleteNode: \n" <<objNodeCollection->getNumberOfNodes()<<endl;
		objNodeCollection = CNodeCollection::getNodeColObjectPointer();
		objNodeCollection->deleteNode(NodePos);							
		return;
	}
	
/**************************************************************************************************
	* Function Name: DeleteIndex
    * Description: Deletes the Index after performing check for Index Existanse
	Return value Legend:
	Cannot Delete Index		- -1
	Delete Index Success 		- 1
/****************************************************************************************************/
int DeleteIndex(int NodeID, ENodeType NodeType, char* IndexID, char* ErrStr)
	{
		
		int IndexPos = IfIndexExists(NodeID, NodeType, IndexID, ErrStr);
		if(IndexPos >= 0)
		{
			CNode objNode;		
			CNodeCollection *objNodeCollection;
			CIndexCollection *objIndexCollection;
			CIndex objIndex;

			objIndex.setNodeID(objNode.getNodeId());
			objNodeCollection= CNodeCollection::getNodeColObjectPointer();
			objNode = objNodeCollection->getNode(CN, NodeID);

			objIndexCollection = objNode.getIndexCollection();
			//cout<< "Inside DeleteIndex: \n" << atoi(IndexID) <<endl;
			cout<< "Inside DeleteIndex: \n" << IndexPos <<endl;
			objIndexCollection->deleteIndex(IndexPos);
			return 1;
		}
		else
			cout<< "Index cannot be Deleted!!" <<endl;
		return -1;

	}
	
/**************************************************************************************************
	* Function Name: DeleteSubIndex
    * Description: Deletes the Index after performing check for Index Existanse
	Return value Legend:
	Cannot Delete SubIndex		- -1
	Delete SubIndex Success 	- 1
/****************************************************************************************************/
int DeleteSubIndex(int NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID, char* ErrStr)
	{
		CNode objNode;		
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;
		CIndex objIndex;
		CIndex* objSubIndex;
		int SubIndexPos = IfSubIndexExists(NodeID, NodeType, IndexID, SubIndexID, ErrStr);
		
		if(SubIndexPos >= 0)
		{					
			int IndexPos = IfIndexExists(NodeID, NodeType, IndexID, ErrStr);
			objIndex.setNodeID(objNode.getNodeId());
			objNodeCollection= CNodeCollection::getNodeColObjectPointer();
			objNode = objNodeCollection->getNode(NodeType, NodeID);

			objIndexCollection = objNode.getIndexCollection();
			objSubIndex =objIndexCollection->getIndex(IndexPos);
			
			cout << "NumberofSubIndexes:" << objIndex.getNumberofSubIndexes()<< endl;
			objSubIndex->deleteSubIndex(SubIndexPos);
			return 1;
		}
		else
			cout<< "SubIndex cannot be Deleted!!" <<endl;
		return -1;

	}
/**************************************************************************************************
	* Function Name: AddSubIndex
    * Description:
/****************************************************************************************************/
void AddSubIndex(int NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID)
	{
		int count;
		CNode objNode;		
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;
		CIndex objIndex;
		
		cout<< "Inside AddSubIndex \n"<<endl;
		objNodeCollection = CNodeCollection::getNodeColObjectPointer();
		// Check for number of Nodes present
		if( objNodeCollection->getNumberOfNodes() > 0)
		{
			for(int count = 0; count < objNodeCollection->getNumberOfNodes(); count++)
			{
				printf("`");
				objNode = objNodeCollection->getNodebyCollectionIndex(count);
				// Check for the type of Node
				if (objNode.getNodeType() == CN)
				{
					// Check for corresponding Index
					if(objNode.getNodeId() == NodeID)
					{
						//Set the NodeID							
						objIndex.setNodeID(objNode.getNodeId());
						objNodeCollection= CNodeCollection::getNodeColObjectPointer();
						objNode = objNodeCollection->getNode(NodeType, NodeID);
						
						objIndexCollection = objNode.getIndexCollection();
						if(objIndexCollection->getNumberofIndexes() == 0)
						{
								printf("Cannot add SubIndex - Index not Found\n\n");
								return;
						}
						else
						{
							//Check for existance of the Index
							for(int tmpIndexcount = 0; tmpIndexcount < objIndexCollection->getNumberofIndexes(); tmpIndexcount++)
							{
								CIndex* objIndexPtr;
								objIndexPtr =objIndexCollection->getIndex(tmpIndexcount);						
								printf("IndexValue:%s-%s\n", objIndexPtr->getIndexValue(), IndexID);
								if((strcmp(objIndexPtr->getIndexValue(), IndexID) == 0))
								{
									
									CSubIndex objSubIndex;
									//Set the NodeID
									objSubIndex.setNodeID(objNode.getNodeId());
									AddSubIndexAttributes(SubIndexID, &objSubIndex);									
									objIndexPtr->addSubIndex(objSubIndex);
									printf("Added SubIndex \n\n");
									return;
								}
								else if(tmpIndexcount == (objIndexCollection->getNumberofIndexes() - 1))
								{
									printf("Cannot add SubIndex - Index not Found\n\n");
									return;
								}
							}
						}
					}
					else
					{
						printf("!");
						//return;
					}
				}					
			}
		}
		else
		{
				printf("Cannot add SubIndex - No Nodes found!\n");
				return;
		}

	}	

/**************************************************************************************************
	* Function Name: AddIndex
    * Description:
/****************************************************************************************************/
void AddIndex(int NodeID, ENodeType NodeType, char* IndexID)
	{
		int count;
		CNode objNode;		
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;
		CIndex objIndex;
		
		cout<< "Inside AddIndex \n"<<endl;
		objNodeCollection = CNodeCollection::getNodeColObjectPointer();
		// Check for number of Nodes present
		if( objNodeCollection->getNumberOfNodes() > 0)
		{
			for(int count = 0; count < objNodeCollection->getNumberOfNodes(); count++)
			{
				printf("`");
				objNode = objNodeCollection->getNodebyCollectionIndex(count);
				// Check for the type of Node
				if (objNode.getNodeType() == CN)
				{
					//int tmp_NodeID = objNode.getNodeId();
					//printf("NodeID:%d\n", tmp_NodeID);
					// Check for corresponding Index
					if(objNode.getNodeId() == NodeID)
					{
						//Set the NodeID							
						objIndex.setNodeID(objNode.getNodeId());
						objNodeCollection= CNodeCollection::getNodeColObjectPointer();
						objNode = objNodeCollection->getNode(NodeType, NodeID);
						
						objIndexCollection = objNode.getIndexCollection();
						if(objIndexCollection->getNumberofIndexes() == 0)
						{
							AddIndexAttributes(IndexID, &objIndex);
							objIndexCollection->addIndex(objIndex);	
							printf("Added Index \n\n");
							return;
						}
						else
						{
							//Check for existance of the Index
							for(int tmpIndexcount = 0; tmpIndexcount < objIndexCollection->getNumberofIndexes(); tmpIndexcount++)
							{
								CIndex* objIndexPtr;
								objIndexPtr =objIndexCollection->getIndex(tmpIndexcount);						
								printf("IndexValue:%s-%s\n", objIndexPtr->getIndexValue(), IndexID);
								if((strcmp(objIndexPtr->getIndexValue(), IndexID) == 0))
								{
									printf("Cannot Add - Index Already Exists!\n");
									return;
								}
								else if(tmpIndexcount == (objIndexCollection->getNumberofIndexes() - 1))
								{
									// Add the Index
									//objIndex.setIndexValue(IndexID);
									AddIndexAttributes(IndexID, &objIndex);
									// Add the default Index Attributes
									
									
									//Add Index object to the IndexCollection
									objIndexCollection->addIndex(objIndex);	
									printf("Added Index \n\n");
									return;
								}

							}
						}
					}
					else
					{
						printf("!");
						//return;
					}
				}						
			}
		}
		else
		{
				printf("No Nodes found!\n");
				return;
		}

	}	
	
/**************************************************************************************************
	* Function Name: DisplayNodeTree
    * Description:
/****************************************************************************************************/
void DisplayNodeTree()
	{
		int count;
		CNode objNode;		
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;
		
		objNodeCollection= CNodeCollection::getNodeColObjectPointer();
		objIndexCollection = objNode.getIndexCollection();
		
		if( objNodeCollection->getNumberOfNodes() > 0)
		{
			for(int count = 0; count < objNodeCollection->getNumberOfNodes(); count++)
			{
					objNode = objNodeCollection->getNodebyCollectionIndex(count);
					if (objNode.getNodeType() == CN)
					{						
						//int tmp_NodeID = objNode.getNodeId();
						//objNode =objNodeCollection->getNode(CN,tmp_NodeID);
						printf("NodePos:%d, NodeID:%d\n", count, objNode.getNodeId());
					}						
			}
		}
		else
		{
				printf("No Nodes found!\n");
				return;
		}
			
	}	
	
/**************************************************************************************************
	* Function Name: GetIndexData
   * Description: 
/****************************************************************************************************/
void GetIndexData(CIndex* objIndex, char* Buffer)
	{
			int len;
			
			//Get the Index Value		
			
			/*if(objIndex->getNumberofSubIndexes()==0 &&(objIndex->getDefaultValue()!= NULL ||
																																													 objIndex->getActualValue()!=NULL))*/
			strcpy(Buffer,"");
			if(objIndex->getNumberofSubIndexes()==0 && objIndex->getActualValue()!=NULL)
			
			{
					//Buffer = (char*)malloc(sizeof(objIndex->getIndexValue()+1));
					strcpy(Buffer,objIndex->getIndexValue());

					//Place a tab			
					len = strlen(Buffer);
					//Buffer =(char*)realloc(Buffer,2);
					strcat(Buffer,"\t");
					printf("%s",Buffer);

					//If subindexes are none, add "00"
					//	Buffer =(char*)realloc(Buffer,4);
						strcat(Buffer,"00");
						strcat(Buffer,"\t");

							//Add datatype
							DataType dt;		
							//Buffer =(char*)realloc(Buffer,6);	
							dt = objIndex->getDataType();
							int padLength=0;
							if(dt.Name != NULL)
							{
								strcat(Buffer ,dt.DataSize);				
								padLength = hex2int(dt.DataSize)*2;
							}
							else strcat(Buffer,"0000");
							
							strcat(Buffer,"\t");
						
							if (objIndex->getActualValue()!=NULL)
							{									
									//Buffer =(char*)realloc(Buffer,sizeof(objIndex->getActualValue()+1));	
									strcat(Buffer,padLeft((char*)objIndex->getActualValue(),'0',padLength));
							}
					
							//else 
							//{
							//	//Buffer =(char*)realloc(Buffer,sizeof(objIndex->getDefaultValue()+1));									
							//	strcat(Buffer,objIndex->getDefaultValue());
							//	//Buffer =(char*)realloc(Buffer,2);				
							//
							//}
								strcat(Buffer,"\n");
				}	
			
			else
			{
				int noOfSubIndexes = objIndex->getNumberofSubIndexes();
				bool Indexadded  = false;				
				for(int i=0; i<noOfSubIndexes ; i++)
				{
					CSubIndex* objSubIndex;
					objSubIndex = objIndex->getSubIndex(i);
				
					if(strcmp(objSubIndex->getIndexValue(),"00")!=0 && objSubIndex->getActualValue() != NULL)
					{
						if (Indexadded)
						strcat(Buffer,objIndex->getIndexValue());
						else
						{
							strcpy(Buffer,objIndex->getIndexValue());
							Indexadded = true;
						}
											
						//Place a tab			
						len = strlen(Buffer);
						strcat(Buffer,"\t");
						printf("%s",Buffer);

							/*if(objSubIndex->getActualValue() != NULL || objSubIndex->getDefaultValue()!=NULL)*/
								strcat(Buffer, objSubIndex->getIndexValue());
								strcat(Buffer,"\t");
									//Add datatype
								DataType dt;			
								dt = objSubIndex->getDataType();
								int padLength=0;
								if(dt.Name != NULL)
								{
									strcat(Buffer ,dt.DataSize);				
									padLength = hex2int(dt.DataSize)*2;
								}
								else strcat(Buffer,"0000");
								
								strcat(Buffer,"\t");
							
								if (objSubIndex->getActualValue()!=NULL)
								{			
										char* actvalue;
										actvalue = (char*)malloc(50);
										actvalue = strchr((char*)objSubIndex->getActualValue(),'x');
										if(actvalue!=NULL)
										{
											actvalue = subString(actvalue,1,strlen(actvalue)-1);
											strcat(Buffer,padLeft(actvalue,'0',padLength));
										}
										else	strcat(Buffer,padLeft((char*)objSubIndex->getActualValue(),'0',padLength));
								}
									/*else strcat(Buffer,objSubIndex->getDefaultValue());*/
								strcat(Buffer,"\n");
						}
					}
						
				}
	}
	
		
			
/**************************************************************************************************
	* Function Name: GenerateCNOBD
   * Description: 
/****************************************************************************************************/
char* GenerateCNOBD(CNodeCollection* objNodeCol)
	{
		CNode objNode;
		CIndexCollection* objIndexCollection;
		char* Buffer2;

		Buffer2 =  new char;
		for(int count=0; count<objNodeCol->getNumberOfNodes(); count++)
			{
				objNode = objNodeCol->getNode(CN,count+1);
				objIndexCollection = objNode.getIndexCollection();
				char* comment= new char;
				char* c;
				c= new char;
				itoa(count+1,c,10);
				strcpy(comment,"\\\\Configuration Data for CN-");
				comment = strcat(comment,c);
				comment = strcat(comment,"\n");
				char* NodeId;
				//comment = strcat(comment,(char*)count);
				/*strcpy(Buffer2, comment);*/
				int NumberOfIndexes = objIndexCollection->getNumberofIndexes();
				for(int i=0; i<NumberOfIndexes; i++)
					{
						CIndex* objIndex;
						objIndex = objIndexCollection->getIndex(i);
						const char* IndexValue = objIndex->getIndexValue();
						if(strcmp(IndexValue,"1006")==0 ||strcmp(IndexValue,"2000")==0)
							{
								//strcpy(Buffer2,GetIndexData(objIndex));
								GetIndexData(objIndex, Buffer2);
								
							}
					}				
						
			}
		return Buffer2;
	}
	/**************************************************************************************************
	* Function Name: WriteCNsData
 * Description: Writes all CNS data to CDC file
/****************************************************************************************************/

	void WriteCNsData(char* fileName)
{
		char* Buffer2;
		char* Buffer3;
		int len;
		CIndexCollection* objIndexCollection;
	

		//Buffer2 = NULL;
//		strcpy(Buffer3,"");
		char* c = (char*)malloc(4);	
		FILE* fileptr = new FILE();
		CNode objNode;	
		CNodeCollection* objNodeCollection;
		objNodeCollection = CNodeCollection::getNodeColObjectPointer();		
		
		int CNCount=0;
		if (( fileptr = fopen(fileName,"a+")) == NULL)
		{
			cout << "Problem" <<endl;
		}
		for(int count=0; count<objNodeCollection->getNumberOfNodes(); count++)
			{
				objNode = objNodeCollection->getNodebyCollectionIndex(count);
				if (objNode.getNodeType() == CN)
					{
						
							objIndexCollection = objNode.getIndexCollection();
							char* comment= (char*)malloc(30);
							itoa(CNCount+1,c,10);
							CNCount++;
							
							strcpy(comment,"\\\\Configuration Data for CN-");
							comment = strcat(comment,c);
							comment = strcat(comment,"\n");
							len = strlen(comment);
							if((fwrite(comment, sizeof(char),len,fileptr))!=NULL)
							{
								printf("\nComments for CN-%d",count);
								fclose(fileptr);
							}
							char* NodeId;
							//comment = strcat(comment,(char*)count);
							/*strcpy(Buffer2, comment);*/
							int NumberOfIndexes;
							bool firstBuffer = true;																								
							NumberOfIndexes = objIndexCollection->getNumberofIndexes();
							/*************WRITE 1006, PDO Data, Module Data *******************************/
								for(int i=0; i<NumberOfIndexes; i++)
								{
									CIndex* objIndex;
									char* Buffer4;
									objIndex = objIndexCollection->getIndex(i);
									const char* IndexValue = objIndex->getIndexValue();
									if(strcmp(IndexValue,"1006")==0 || (CheckIfNotPDO((char*)IndexValue)==false)  ||
											CheckIfManufactureSpecificObject((char*)IndexValue))
										{
											Buffer4 = (char*)malloc(300);							
											GetIndexData(objIndex, Buffer4);
											if(firstBuffer)
											{
												Buffer2 = (char*)malloc(sizeof(char)*strlen(Buffer4)+1);																				
												strcpy(Buffer2, Buffer4);
												firstBuffer = false;											
											}
											else
											{
												printf("length of Buffer1 %d",strlen(Buffer4));
												printf("\nlength of Buffer2 %d",strlen(Buffer2));
												strcat(Buffer2, Buffer4);
											}
											//free(Buffer1);
										}									
								}	
						/**************write module data********************************/
					/*	if(objNode.ProcessImageCollection.Count() !=0)
						{
							int count = objNode.ProcessImageCollection.Count();
							
							for(int i=0; i<count; i++)
							{
										char* Index = (char*)malloc(4);	
										CIndex* objIndex;										
										strcpy(Index,objNode.ProcessImageCollection[i].Index);
										objIndex = objIndexCollection->getIndexbyIndexValue(Index);
										
							}
						}*/
							
							//Convert CN NodeID to Hex
							itoa(count+1,c,16);	
							char CNDataLength[18];
							strcpy(CNDataLength, "1F22\t");
							c = padLeft(c, '0', 2);
							/*if (strlen(c)< 2)
								{
									char clen[2];
									strcpy(clen,c);
									strcpy(c,"0");
									strcat(c,clen);
								}
							*///write CN-n NodeID  in the next to 1F22
							strcat(CNDataLength, c);
							strcat(CNDataLength, "\t");
							
							//write the size of CN-n Buffer
							int len = lenOfCNBuffer(Buffer2);

							//Convert length to Hex
							itoa(len,c,16);
							printf("c%s",c);
							
							c = padLeft(c, '0', 4);
							strcat(CNDataLength, c);				
							
							// First write the IF22 data in a Buffer and then CN-ns Object Dictionary
							Buffer3 = (char*)malloc(strlen(Buffer2)+50);
							strcpy(Buffer3, CNDataLength);
							strcat(Buffer3, "\n");
							strcat(Buffer3, Buffer2);
															
						
							//strcpy(Buffer2,GenerateCNOBD(objNodeCollection));
						
							//write all CNs data in the file
							len = strlen(Buffer3);
							if (( fileptr = fopen(fileName,"a+")) == NULL)
							{
								cout << "Problem" <<endl;
							}
							if((fwrite(Buffer3, sizeof(char),len,fileptr))!=NULL)
								{
									printf("success");				
									fclose(fileptr);	
								}
					}
			}
	}
/**************************************************************************************************
	* Function Name: GenerateCDC
    * Description: Generates the CDC file
/****************************************************************************************************/
void GenerateCDC(char* fileName)
	{
		CNode objNode;	
		CIndexCollection* objIndexCollection;
		//ofstream file;
		char *Buffer1;
		int len;
				
		char* CNData;

		printf("Inside GenerateCDC");
				
		Buffer1 = NULL;		

		//get the MN Node object from the NodeCollection
		CNodeCollection* objNodeCollection;
		objNodeCollection = CNodeCollection::getNodeColObjectPointer();		
		
	/******************************* Write MN's Indexes ******************************************************************/
		objNode = objNodeCollection->getMNNode();

		if(objNode.isNull())
		{
			printf("No MN Node exists");
			exit(1);
		}
		printf("Node id %d",objNode.getNodeId());

		FILE* fileptr = new FILE();
		if (( fileptr = fopen(fileName,"w+")) == NULL)
			{
				printf ( "Cannot open file you have named...!\n" );
			
			}
		else
			{
			}

		objIndexCollection = objNode.getIndexCollection();

		//Get all the MN's Default Data in Buffer1
		for(int i=0;i < objIndexCollection->getNumberofIndexes ();i++)
			{
				CIndex* objIndex;
				objIndex = objIndexCollection->getIndex(i);
			/*	if(CheckIfNotPDO((char*)objIndex->getIndexValue()))
					{	*/					
						printf("Index%s",objIndex->getIndexValue());						
						Buffer1 = (char*)malloc(200);
						len = strlen(Buffer1);
						
						GetIndexData(objIndex,Buffer1);
						len = strlen(Buffer1);
						if((fwrite(Buffer1, sizeof(char),len,fileptr))!=NULL)
						{
							printf("Buffer1 written");
						
						}
						delete[] Buffer1;
				/*	}*/
						
			}
			fclose(fileptr);
			
		/*************************Write CN's Data in Buffer2***************************************************/
		WriteCNsData(fileName);
		
		//Get all the IF81 ENTRY in Buffer1
		if(objNodeCollection->getNumberOfNodes()!=0)
		{
			if (( fileptr = fopen(fileName,"a+")) == NULL)
			{
				printf ( "Cannot open file you have named...!\n" );
			
			}
		else
			{
			}
			for(int i=0;i < objNodeCollection->getNumberOfNodes();i++)
			{
					CNode objNode;
					objNode = objNodeCollection->getNodebyCollectionIndex(i);
					if(objNode.getNodeType() ==CN)
					{
							Buffer1 = (char*)malloc(100);
							len = strlen(Buffer1);
							strcpy(Buffer1, "//// NodeId Reassignment\n");
							strcat(Buffer1, "1F81");
							strcat(Buffer1, "\t");
							int NodeID = objNode.getNodeId();		
							char* hex = (char*)malloc(3);					
							hex = itoa(NodeID,hex,16);
							hex = padLeft(hex,'0',2);
							strcat(Buffer1, hex);
							strcat(Buffer1, "\t0004\t80000007\n");
							len = strlen(Buffer1);
							if((fwrite(Buffer1, sizeof(char),len,fileptr))!=NULL)
							{
								printf("Buffer1 written");
							
							}
							delete[] Buffer1;
					}
				}
				/*	}*/
						
			}
		fclose(fileptr);		
		
	}

/**************************************************************************************************
	* Function Name: GenerateMNOBD
   * Description: Generates the MN Object Dictionary
/****************************************************************************************************/
//void GenerateMNOBD()
//	{
//		CNodeCollection *objNodeCollection;
//		objNodeCollection= CNodeCollection::getNodeColObject();
//
//		//get the MN Node
//		CNode objMNNode;
//		
//		objMNNode=objNodeCollection->getNode(MN,249);
//
//		//get the Index collection of MN Object
//		CIndexCollection *objIndexCollection;
//		objIndexCollection = objMNNode->getIndexCollection();
//
//		//loop through the Indexes of IndexCollection
//		int count=0;
//		for (count=0;count < objIndexCollection->getNumberofIndexes(); count++)
//			{
//				CIndex objIndex;
//				objIndex = objIndexCollection->getIndex(count);
//				objIndex.getIndex();
//				
//			}
//
//
//
//
//	}

/**************************************************************************************************
	* Function Name: ProcessPDONodes
    * Description: Processes the Node
/****************************************************************************************************/
void ProcessPDONodes(int NodeID)
{
		CNodeCollection* objNodeCol;
		objNodeCol = CNodeCollection::getNodeColObjectPointer();
		CNode objNode;
		objNode = objNodeCol->getNode(CN,NodeID);
		CIndexCollection* objPDOCollection;
		CIndexCollection* objIndexCollection;
		/* Check RPDO Mapped objects*/
		objPDOCollection = objNode.getPDOIndexCollection(PDO_RPDO);
		if(objPDOCollection!= NULL)
		{
		
			objIndexCollection = objNode.getIndexCollection();
			for(int count = 0; count<objPDOCollection->getNumberofIndexes(); count++)
			{
					CIndex* objIndex;
					objIndex = objPDOCollection->getIndex(count);
					if (objIndex->getActualValue()!=NULL)
					{
							const char* value = objIndex->getActualValue();
							int len = strlen(value);
							char* reverseValue = (char*)malloc(len);
							/* Reverse the actual value to get Index / subindex*/
							reverseValue = reverse((char*)value);
							
							/* Get the Index*/
							char* strIndex = (char*)malloc(4);
							strIndex = strncpy(strIndex,reverseValue,4);
								/* Get the SubIndex*/
							char* strSubIndex = (char*)malloc(2);
							strSubIndex = subString(reverseValue,2,2);
							ProcessImage objProcessImage;
																		
							objProcessImage.Index = new char[4];
							strcpy(objProcessImage.Index, strIndex);
							
							objProcessImage.subindex = new char[2];
							strcpy(objProcessImage.subindex, (const char*)strSubIndex);
					}
				}
			}
	}

void ConvertToBinary(char* fileName)
{
	FILE *fin, *fout_cdc, *fout_Cfile;
	int count=0;
	int num=0;
	char ch=0;
	int iCtr;
	int iLength;
	unsigned char ca_cn1obd[10000];
	unsigned char tempCn1Obd[10000];

	fin = fopen(fileName, "r");
	fout_cdc = fopen("config_data.cdc", "wb");
	fout_Cfile = fopen("../../openPOWERLINK_v1.1.0/ObdConf.c", "wb");
	//if(argc<=1 || argc>2)
	//{
	//	printf("\n Invalid argument \n");
	//	goto Exit;
	//}
	//else
	//{
		while(ch != EOF)
		{
			ch = fgetc(fin);
			if(ch == '/') 
		    {
				while(fgetc(fin) != '\n');
		    }
			else if( ch != '\t' || ch != '\n' || ch != ' ')
			{
				// Convert to Upper case
				ch = toupper(ch);
				if(ch >= 65 && ch <= 70)
				{
					ca_cn1obd[num] = ch - 55;
					num++;
				}
				else if (( ch >=48 && ch <= 57))
				{
					ca_cn1obd[num] = ch - 48;
					num++;
				}
			}
		//}
		ca_cn1obd[num] = '\0';		
		// For testing purpose.
		for ( count = 0; count<num ; count++)
		{
			//printf("0X%x, ",ca_cn1obd[count]);
		}
		//
		//
		iLength = num;
		//printf( "\niLength = %d\n",iLength);
		fprintf(fout_Cfile,"static unsigned char tempBaMnObd[%d] = {",((iLength/2)-1));
		//For Byte Packing
		for (iCtr = 0 , count = 0; iCtr < iLength; iCtr++, count++ )
		{
			tempCn1Obd[count] = (unsigned char)( ( ca_cn1obd[ iCtr ] << 4 ) | ca_cn1obd[ ++iCtr ] );
			//printf("0x%x\t",tempCn1Obd[count]);
			fprintf(fout_Cfile,"0X%x, ",tempCn1Obd[count]);
		}
		//printf("Size : %d\n", count);
		fprintf(fout_Cfile,"};\n");		
		count = count -1;
		fprintf(fout_Cfile,"static unsigned int DataCount = %d;\n",count);
		// Write to Binary file
		fwrite(&tempCn1Obd,1,count,fout_cdc);
	}
	Exit :
	fclose(fin);
	fclose(fout_cdc);
	fclose(fout_Cfile);
}
void ProcessUniqueIDRefs(CNode* objNode)
 {
	 
	 
	
 }
 int lenOfCNBuffer(char* Buffer)
 {
		char tempchar;
		int len = strlen(Buffer);
		int counter =0;
		int actualLength=0;
		while(counter<len)
		{
			tempchar = *(Buffer+counter);
			if(tempchar!='\n' && tempchar != '\t')
			actualLength++;
			counter++;
		}
		return actualLength;
 }
