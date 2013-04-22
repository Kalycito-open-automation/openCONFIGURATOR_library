/**
 ************************************************************************************************
 \file			Node.cpp

 \brief			Handles information of node which inclues id,name,type
 ************************************************************************************************
 */

/*
 (c) Kalycito Infotech Private Limited

 License:

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 1. Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.

 3. Neither the name of Kalycito Infotech Private Limited nor the names of 
 its contributors may be used to endorse or promote products derived
 from this software without prior written permission. For written
 permission, please contact info@kalycito.com.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.

 Severability Clause:

 If a provision of this License is or becomes illegal, invalid or
 unenforceable in any jurisdiction, that shall not affect:
 1. the validity or enforceability in that jurisdiction of any other
 provision of this License; or
 2. the validity or enforceability in other jurisdictions of that or
 any other provision of this License.

 ****************************************************************************/

/****************************************************************************************************/
/* Includes */

#include <stdio.h>
#include "../Include/Node.h"
#include "../Include/Internal.h"

//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

/*************************************************************************/
/* Constructor */

/**
 
 */

Node::Node(void)
{
	indexCollObj = NULL;
	dtCollObj = NULL;
	indexCollObj = NULL;
	appProcessObj = NULL;
	nmtObj = NULL;
	nodeName = NULL;
	hasPdoObjects = false;
	stationType = NORMAL;
	forcedCycle = NULL;
	presTimeOut = NULL;
	isForcedCycle = false;
	nodeId = 0;
	nodePosition = 0;
	//VarNodeType = 0; //TODO: Review initialization
	presActualPayload = 0;
	preqActualPayload = 0;
	//cout<<"NodeClass: Constructor Executed"<<endl;
}

/*************************************************************************/
/* Destructor */

/**
 
 */

Node::~Node(void)
{
	//Add destrucor code here
}
#ifndef __GNUC__
#pragma region Properties
#endif



NodeType Node::GetNodeType()
{
	return nodeType;
}

void Node::SetNodeType(NodeType objNodeType)
{
	nodeType = objNodeType;
}



INT32 Node::GetNodeId()
{
	return nodeId;
}

void Node::SetNodeId(INT32 nodeID)
{
	nodeId = nodeID;

}


INT32 Node::GetNodeIndex()
{
	return nodePosition;
}

void Node::SetNodeIndex(INT32 nodePos)
{
	nodePosition = nodePos;
}



char* Node::GetNodeName()
{
	return nodeName;
}


void Node::SetNodeName(char* nodeNameStr)
{
	if (NULL != nodeName)
	{
		delete[] nodeName;
	}
	nodeName = new char[strlen(nodeNameStr) + STR_ALLOC_BUFFER];
	strcpy((char*) nodeName, nodeNameStr);
}


bool Node::HasPdoObjects()
{
	return hasPdoObjects;
}

void Node::SetFlagForPdoObjects(bool flag)
{
	hasPdoObjects = flag;
}


IndexCollection* Node::GetIndexCollection()
{
	return indexCollObj;
}


ApplicationProcess* Node::GetApplicationProcess()
{
	return appProcessObj;
}


NetworkManagement* Node::GetNetworkManagement()
{
	return nmtObj;
}


DataTypeCollection* Node::GetDataTypeCollection()
{
	return dtCollObj;
}


void Node::CreateIndexCollection()
{
	IndexCollection* objIndexCollection = NULL;
	objIndexCollection = new IndexCollection();
	indexCollObj = objIndexCollection;
}


void Node::CreateDataTypeCollection()
{
	DataTypeCollection* pobjDataTypeCollection = NULL;
	pobjDataTypeCollection = new DataTypeCollection();
	dtCollObj = pobjDataTypeCollection;
}

void Node::AddProcessImage(ProcessImage piObj)
{
	INT32 iItemPosition = PICollection.Add();
	PICollection[iItemPosition] = piObj;
}

void Node::AddNETProcessImage(NETProcessImage netPIobj)
{
	INT32 iItemPosition = NETPIColl.Add();
	NETPIColl[iItemPosition] = netPIobj;
}

void Node::AddMNPDOvar(MNPdoVariable pdoVarObj, PDOType pdotype)
{
	if (PDO_RPDO == pdotype)
	{
		INT32 iItemPosition = MNPDOOUTVarCollection.Add();
		MNPDOOUTVarCollection[iItemPosition] = pdoVarObj;
	}
	else if (PDO_TPDO == pdotype)
	{
		INT32 iItemPosition = MNPDOINVarCollection.Add();
		MNPDOINVarCollection[iItemPosition] = pdoVarObj;
	}
	else
	{
		//Nothing to be added.
	}
}


void Node::CreateApplicationProcess()
{
	ApplicationProcess* appProcess = NULL;

	appProcess = new ApplicationProcess();
	appProcessObj = appProcess;
}


void Node::CreateNetworkManagement()
{
	NetworkManagement* nmtObject = NULL;

	nmtObject = new NetworkManagement();
	nmtObj = nmtObject;
}

IndexCollection* Node::GetPDOIndexCollection(PDOType pdotype)
{
	IndexCollection* objPdoIndexCollection = new IndexCollection();
	Index* objIndex = NULL;
	for (INT32 iLoopCount = 0; iLoopCount < indexCollObj->GetNumberofIndexes(); iLoopCount++)
	{
		objIndex = indexCollObj->GetIndex(iLoopCount);

		if (objIndex->GetPDOType() == pdotype)
		{
			objPdoIndexCollection->AddIndex(*objIndex);
		}
	}
	return objPdoIndexCollection;
}

IndexCollection* Node::getPDOIndexCollection(INT32 *rpdoCount, INT32 *tpdoCount)
{
	IndexCollection* pdoIndexCollObj = new IndexCollection();
	Index* indexObj = NULL;

	for (INT32 indexLC = 0;	indexLC < indexCollObj->GetNumberofIndexes(); indexLC++)
	{
		indexObj = indexCollObj->GetIndex(indexLC);
		if ((indexObj->GetPDOType() == PDO_TPDO))
		{
			if ((0 == strncmp(indexObj->GetIndexValue(), "1A", 2)) || 0 == strncmp(indexObj->GetIndexValue(), "1a", 2))
			{
				*tpdoCount = *tpdoCount + 1;
			}
			pdoIndexCollObj->AddIndex(*indexObj);
		}
		else if((indexObj->GetPDOType() == PDO_RPDO))
		{
			if (0 == strncmp(indexObj->GetIndexValue(), "16", 2))
			{
				*rpdoCount = *rpdoCount + 1;
			}
			pdoIndexCollObj->AddIndex(*indexObj);
		}
		else
		{
			//other than TPDO or RPDO
		}

	}
	return pdoIndexCollObj;
}


IndexCollection* Node::GetIndexCollectionWithoutPDO()
{
	IndexCollection* indexCollObject = new IndexCollection();
	Index* indexObj = NULL;
	for (INT32 indexLC = 0;	indexLC < indexCollObj->GetNumberofIndexes(); indexLC++)
	{
		indexObj = indexCollObj->GetIndex(indexLC);
		if (CheckIfNotPDO((char*) indexObj->GetIndexValue()))
		{
			indexCollObject->AddIndex(*indexObj);
		}
	}
	return indexCollObject;
}


bool Node::IsNull()
{
	if (NULL == indexCollObj)
	{
		return true;
	}
	else
	{
		return false;
	}
}

ProcessImage* Node::GetPIbyParaIndex(INT32 paramerterPos)
{
	ProcessImage* piObj = NULL;

	for (INT32 piLC = 0; piLC < PICollection.Count();	piLC++)
	{
		piObj = PICollection.GetAddress(piLC);

		if (piObj->parameterPos == paramerterPos)
		{
			return piObj;
		}
	}
	return piObj;
}


void Node::DeleteCollectionsForPI()
{
	if (0 != MNPDOINVarCollection.Count())
	{
		MNPDOINVarCollection.Clear();
	}
	if (0 != MNPDOOUTVarCollection.Count())
	{
		MNPDOOUTVarCollection.Clear();
	}
	if (0 != PICollection.Count())
	{
		PICollection.Clear();
	}
}


void Node::DeleteCollectionsForNETPI()
{
	if (0 != NETPIColl.Count())
	{
		NETPIColl.Clear();
	}
}


StationType Node::GetStationType()
{
	return stationType;
}

void Node::SetStationType(StationType stationtype)
{
	stationType = stationtype;
}


char* Node::GetForcedCycleValue()
{
	return forcedCycle;
}

ocfmRetCode Node::SetForcedCycle(char* tempForcedCycleVal)
{
	//add or update 1f9b
	ocfmRetCode errReslt1;
	ocfmRetCode errReslt2;
	ocfmRetCode errReslt3;
	INT32 idxPos = 0;
	INT32 sIdxPos = 0;

	char customErr1[200];
	char customErr2[100];
	char customErr3[100];
	customErr1[0] = 0;
	customErr2[0] = 0;
	customErr3[0] = 0;

	char* sidxId = new char[SUBINDEX_LEN];
	char indexId[] = MULTIPL_CYCLE_ASSIGN_OBJECT;

	errReslt1.code = OCFM_ERR_SUCCESS;

	sidxId = IntToAscii(this->GetNodeId(), sidxId, 16);
	sidxId = PadLeft(sidxId, '0', 2);

	errReslt2 = IfSubIndexExists(MN_NODEID, MN, indexId, sidxId, &sIdxPos, &idxPos);
	errReslt3 = IfSubIndexExists(this->GetNodeId(), CN,	indexId, sidxId, &sIdxPos, &idxPos);
	if ((OCFM_ERR_SUCCESS == errReslt2.code) && (OCFM_ERR_SUCCESS == errReslt3.code))
	{
		// both the index and subindex are present continue
	}
	else
	{
		if (OCFM_ERR_INDEXID_NOT_FOUND == errReslt2.code)
		{
			strcpy((char*) customErr2, "The Index 1F9B does not exist in MN. ");
		}
		if (OCFM_ERR_INDEXID_NOT_FOUND == errReslt3.code)
		{
			sprintf((char*) customErr3, "The Index 1F9B does not exist in CN node id:%d. ", this->GetNodeId());
		}
		if (OCFM_ERR_SUBINDEXID_NOT_FOUND == errReslt2.code)
		{
			sprintf(customErr2,	"The Subindex %s in Index 1F9B does not exist in MN. ",	sidxId);
		}
		if (OCFM_ERR_SUBINDEXID_NOT_FOUND == errReslt3.code)
		{
			sprintf(customErr3,	"The Subindex %s in Index 1F9B does not exist in CN node id:%d. ", sidxId, this->GetNodeId());
		}
		strcpy(customErr1, customErr2);
		strcat(customErr1, customErr3);
		strcat(customErr1, "Unable to assign the multiplexing cycle");

		errReslt1.code = OCFM_ERR_MULTIPLEX_ASSIGN_ERROR;
		CopyCustomErrorString(&errReslt1, (char*) &customErr1);
		return errReslt1;
	}

	char* sidxName = new char[50];
	sidxName[0] = 0;

	GetSubIndexAttributes(240, MN, indexId, sidxId,	NAME, sidxName);
	SetBasicSubIndexAttributes(240, MN, indexId, sidxId,	tempForcedCycleVal, sidxName, TRUE);

	Index* idxObj1 = NULL;
	idxObj1 = GetMNIndexValues(indexId);
	if (NULL != idxObj1)
	{
		//UpdateNumberOfEnteriesSIdx(objMN1F92Index, MN);
		idxObj1->SetFlagIfIncludedCdc(TRUE);
		if (NULL != idxObj1->GetSubIndexbyIndexValue((char*) "00"))
		{
			idxObj1->GetSubIndexbyIndexValue((char*) "00")->SetFlagIfIncludedCdc(TRUE);
		}
	}

	strcpy(sidxName, "");
	GetSubIndexAttributes(this->GetNodeId(), CN, indexId, sidxId, NAME, sidxName);
	SetBasicSubIndexAttributes(this->GetNodeId(), CN, indexId, sidxId, tempForcedCycleVal, sidxName, TRUE);

	Index* idxObj;
	idxObj = this->indexCollObj->GetIndexbyIndexValue(indexId);
	if (NULL != idxObj)
	{
		idxObj->SetFlagIfIncludedCdc(TRUE);
		if (NULL != idxObj->GetSubIndexbyIndexValue((char*) "00"))
		{
			idxObj->GetSubIndexbyIndexValue((char*) "00")->SetFlagIfIncludedCdc(TRUE);
		}
	}

	forcedCycle = new char[strlen(tempForcedCycleVal) + ALLOC_BUFFER];
	strcpy((char*) forcedCycle, tempForcedCycleVal);

	delete[] sidxName;
	return errReslt1;
}


void Node::ResetForcedCycleValue()
{
	if (NULL == forcedCycle)
	{
		forcedCycle = new char[strlen("") + STR_ALLOC_BUFFER];
	}
	strcpy((char*) forcedCycle, "");

	char* sidxId = new char[SUBINDEX_LEN];
	sidxId = IntToAscii(nodeId, sidxId, 16);
	sidxId = PadLeft(sidxId, '0', 2);

	SetSubIndexAttribute(MN_NODEID, MN,	(char *) MULTIPL_CYCLE_ASSIGN_OBJECT, sidxId, ACTUALVALUE, forcedCycle);
	SetSubIndexAttribute(nodeId, nodeType, (char *) MULTIPL_CYCLE_ASSIGN_OBJECT, sidxId, ACTUALVALUE, forcedCycle);

	delete[] sidxId;
}

void Node::SetPollResponseTimeout(char* presTimoutVal)
{
	if (NULL != presTimeOut)
	{
		delete[] presTimeOut;
	}
	presTimeOut = new char[strlen(presTimoutVal) + STR_ALLOC_BUFFER];
	strcpy((char*) presTimeOut, presTimoutVal);

	//add or update 1f92 subobjects in MN
	ocfmRetCode stErrorInfo;
	INT32 idxPos = 0;
	INT32 sidxPos = 0;
	char* sidxId = new char[SUBINDEX_LEN];
	char indexId[] = MNCN_POLLRESPONSE_TIMEOUT_OBJECT;

	sidxId = IntToAscii(this->GetNodeId(), sidxId, 16);
	sidxId = PadLeft(sidxId, '0', 2);

	stErrorInfo = IfSubIndexExists(MN_NODEID, MN, indexId,	sidxId, &sidxPos, &idxPos);
	if (OCFM_ERR_SUCCESS != stErrorInfo.code)
	{
		return;
	}

	char* subIndName = new char[50];
	subIndName[0] = 0;

	GetSubIndexAttributes(240, MN, indexId,	sidxId, NAME, subIndName);
	SetBasicSubIndexAttributes(MN_NODEID, MN, indexId, sidxId, presTimoutVal, subIndName, TRUE);

	Index* idxObj = NULL;
	idxObj = GetMNIndexValues(indexId);
	if (NULL != idxObj)
	{
		UpdateNumberOfEnteriesSIdx(idxObj, MN);
		idxObj->SetFlagIfIncludedCdc(TRUE);
	}

	delete[] subIndName;
}


bool Node::GetForceCycleFlag()
{
	return isForcedCycle;
}

void Node::SetForceCycleFlag(bool forceCycleFlag)
{
	isForcedCycle = forceCycleFlag;
}

void Node::SetPResActPayloadValue(INT32 value)
{
	presActualPayload = value;
}

INT32 Node::GetPResActPayloadValue()
{
	return presActualPayload;
}

void Node::SetPReqActPayloadValue(INT32 value)
{
	preqActualPayload = value;

}


INT32 Node::GetPReqActPayloadValue()
{
	return preqActualPayload;
}
#ifndef __GNUC__
#pragma endregion Properties
#endif
