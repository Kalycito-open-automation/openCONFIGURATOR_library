#include "../Include/NodeCollection.h"
#include "../Include/BaseIndex.h"
#include "../Include/Declarations.h"
#include "../Include/Internal.h"

CBaseIndex::CBaseIndex(void)
	{
		
		m_Index         =  NULL;
		m_Name          =  NULL;
		m_HighLimit     =  NULL;		
		m_dataType.Name =  NULL;
		m_DefaultValue  =  NULL;		
		m_ActualValue   =  NULL;		
		m_accessType    =  NULL;
		m_LowLimit	    =  NULL;
		m_UniqueIDRef	  =  NULL;

	}

CBaseIndex::~CBaseIndex(void)
	{
		//delete m_DefaultValue;
	}
#pragma region Properties
/**************************************************************************************************
	* Function Name: getName
    * Description: Returns the Name of the Index Object
/****************************************************************************************************/

const char* CBaseIndex::getName()
	{return m_Name;}

/**************************************************************************************************
	* Function Name: setName
    * Description: sets the Name of the Index Object
/****************************************************************************************************/
void CBaseIndex::setName(char* Name)
	{
		m_Name = new char[strlen(Name) + 1];
		strcpy((char*)m_Name, Name);
	}
/**************************************************************************************************
	* Function Name: getIndexValue
    * Description: Returns the Index of the Index Object
/****************************************************************************************************/

const char* CBaseIndex::getIndexValue()
	{return m_Index;}

/**************************************************************************************************
	* Function Name: setIndex
    * Description: sets the Index of the Index Object
/****************************************************************************************************/
void CBaseIndex::setIndexValue(char* Index)
	{
		m_Index = new char[strlen(Index)];
		strcpy((char*)m_Index, Index);
	}
/**************************************************************************************************
	* Function Name: getLowLimit
    * Description: Returns the LowLimit of the Index Object
/****************************************************************************************************/

const char* CBaseIndex::getLowLimit()
	{return m_LowLimit;}

/**************************************************************************************************
	* Function Name: setLowLimit
    * Description: sets the LowLimit of the Index Object
/****************************************************************************************************/
void CBaseIndex::setLowLimit(char* LowLimit)
	{
		m_LowLimit = new char[strlen(LowLimit)];
		strcpy((char*)m_LowLimit,LowLimit);
 }
/**************************************************************************************************
	* Function Name: getLowLimit
    * Description: Returns the LowLimit of the Index Object
/****************************************************************************************************/

const char* CBaseIndex::getHighLimit()
	{return m_HighLimit;}

/**************************************************************************************************
	* Function Name: setLowLimit
    * Description: sets the LowLimit of the Index Object
/****************************************************************************************************/
void CBaseIndex::setHighLimit(char* HighLimit)
	{
		m_HighLimit = new char[strlen(HighLimit)];
		strcpy((char*)m_HighLimit,HighLimit);
	}
/**************************************************************************************************
	* Function Name: getDefaultValue
    * Description: Returns the Default Value of the Index Object
/****************************************************************************************************/

const char* CBaseIndex::getDefaultValue()
	{return m_DefaultValue;}

/**************************************************************************************************
	* Function Name: setDefaultValue
    * Description: sets the Default Value of the Index Object
/****************************************************************************************************/
void CBaseIndex::setDefaultValue(char* Value)
	{				
		m_DefaultValue = new char[strlen(Value)];
		strcpy((char*)m_DefaultValue,Value);
	}
/**************************************************************************************************
	* Function Name: getActualValue
    * Description: Returns the Actual Value of the Index Object
/****************************************************************************************************/

const char* CBaseIndex::getActualValue()
	{
		if(m_ActualValue == NULL)
			m_ActualValue = NULL;
		return m_ActualValue;
	}

/**************************************************************************************************
	* Function Name: setActualValue
    * Description: sets the Actual Value of the Index Object
/****************************************************************************************************/
void CBaseIndex::setActualValue(char* Value)
	{
		m_ActualValue = new char[strlen(Value)];		
		strcpy((char*)m_ActualValue,Value);
	}
/**************************************************************************************************
	* Function Name: getAccessType
    * Description: Returns the Access Type of the Index Object
/****************************************************************************************************/

	const char* CBaseIndex::getAccessType()
	{return m_accessType;}

/**************************************************************************************************
	* Function Name: setAccessType
    * Description: sets the Access Type of the Index Object
/****************************************************************************************************/
void CBaseIndex::setAccessType(char* accessType)
	{	
		m_accessType = new char[strlen(accessType)];	
		strcpy((char*)m_accessType,accessType);
	}	
/**************************************************************************************************
	* Function Name: getObjectType
    * Description: Returns the Object Type of the Index Object
/****************************************************************************************************/

	const char* CBaseIndex::getObjectType ()
	{
		switch (m_objectType)
			{
			case 5:
				return "DFEAULT";
				break;
			case 6:
				return "DEFSTRUCT";
				break;
			case 7:
				return "VAR";
				break;
			case 8:
				return "ARRAY";
				break;
			case 9:
				return "RECORD";
				break;
			default:
				return NULL;
				break;
			}
	
	;}

/**************************************************************************************************
	* Function Name: setObjectType
    * Description: sets the Object of the Index Object
/****************************************************************************************************/
	void CBaseIndex::setObjectType(int objectType)
	{	
	
		switch (objectType)
			{
			case 5:
				m_objectType=DEFTYPE;
				break;
			case 6:
				m_objectType=DEFSTRUCT;
				break;
			case 7:
				m_objectType=VAR;
				break;
			case 8:
				m_objectType=ARRAY;
				break;
			case 9:
				m_objectType=RECORD;
				break;
			default:				
				break;
			}

		
	}	
/**************************************************************************************************
	* Function Name: getPDOMapping
    * Description: Returns the PDOMapping of the Index Object
/****************************************************************************************************/

EPDOMapping CBaseIndex::getPDOMapping ()
	{return m_pdoMapping;}

/**************************************************************************************************
	* Function Name: setPDOMapping
    * Description: sets the PDOMapping of the Index Object
/****************************************************************************************************/
void CBaseIndex::setPDOMapping(char* pdoMapping)
	{		
		if(strcmp(ConvertToUpper((char*)pdoMapping), "DEFAULT")==0)
		m_pdoMapping=DEFAULT;
		
		else if(strcmp(ConvertToUpper((char*)pdoMapping), "NO")==0)
		m_pdoMapping = NO;		 
		else if(strcmp(ConvertToUpper((char*)pdoMapping), "OPTIONAL")==0)
		m_pdoMapping = OPTIONAL;		 

		
	}
/**************************************************************************************************
	* Function Name: getDataType
    * Description: Returns the DataType of the Index Object
/****************************************************************************************************/

DataType CBaseIndex::getDataType ()
	{return m_dataType;}

/**************************************************************************************************
	* Function Name: setDataType
    * Description: sets the DataType of the Index Object
/****************************************************************************************************/
void CBaseIndex::setDataType(char* dataTypeName)
	{	
		DataType dt;
		CNodeCollection* objNodeCol;
		objNodeCol= CNodeCollection::getNodeColObjectPointer();
		CNode objNode = objNodeCol->getNode(m_NodeID);
		CDataTypeCollection* dtcol;
		dtcol=objNode.getDataTypeCollection();
		dt = dtcol->getDataType(dataTypeName);
		m_dataType= dt;
		/*m_dataType.DataTypeValue =dt.DataTypeValue;
		m_dataType.Name =dt.Name ;*/
	}
/**************************************************************************************************
	* Function Name: getNodeID
    * Description: Returns the NodeID of the Node of the Index Object
/****************************************************************************************************/

int CBaseIndex::getNodeID()
	{return m_NodeID;}

/**************************************************************************************************
	* Function Name: setNodeID
    * Description: sets the NodeID of the Index Object
/****************************************************************************************************/
void CBaseIndex::setNodeID(int NodeID)
	{	
		m_NodeID = NodeID;
	}
	/**************************************************************************************************
	* Function Name: getNodeID
    * Description: Returns the NodeID of the Node of the Index Object
/****************************************************************************************************/

char* CBaseIndex::getUniqueIDRef()
	{return m_UniqueIDRef;}

/**************************************************************************************************
	* Function Name: setNodeID
    * Description: sets the NodeID of the Index Object
/****************************************************************************************************/
void CBaseIndex::setUniqueIDRef(char* UniqueID)
	{	
		m_UniqueIDRef = new char[strlen(UniqueID)];
		strcpy((char*)m_UniqueIDRef,UniqueID);
	}
#pragma endregion Properties

#pragma region MemberFunctions
/**************************************************************************************************
	* Function Name: SaveChanges
    * Description: Saves the changes of the Index properties
/****************************************************************************************************/
void CBaseIndex::SaveChanges(char* Index,char* Name)
	{
		m_Index = Index;
		m_Name  = Name;
	}
/**************************************************************************************************
	* Function Name: ValidateIndex
    * Description: Saves the changes of the Index properties
/****************************************************************************************************/
void CBaseIndex::ValidateIndex()
	{
		
	}
#pragma endregion MemberFunctions
