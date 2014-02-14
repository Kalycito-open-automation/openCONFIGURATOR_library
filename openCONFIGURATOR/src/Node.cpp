/**
 *****************************************************************************
 \file		Node.cpp

 \brief		This file contains the class member definitions to handle the functional properties of the Node such as datatype collection,Index collection, network management collection
 *****************************************************************************
 */

/*
 © Kalycito Infotech Private Limited

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

/****************************************************************************/
/* Includes */

#include <stdio.h>
#include <sstream>

#include "../Include/Node.h"
#include "../Include/Internal.h"
#include "../Include/Logging.h"

using namespace std;

//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

/*************************************************************************/
/* Constructor */

/**

 */

Node::Node(void) :
	// MN
	transmitsPRes(), 
	// CN	
	isMandatory(),
	autostartNode(),
	resetInOperational(),	
	verifyAppSwVersion(),
	autoAppSwUpdateAllowed(),
	verifyDeviceType(),
	verifyVendorId(),
	verifyRevisionNumber(),
	verifyProductCode(),
	verifySerialNumber(),
	// MN & CN
	isAsyncOnly(),
	isType1Router(),
	isType2Router()
{
	dtCollObj = new DataTypeCollection();
	indexCollObj = new IndexCollection();
	appProcessObj = new ApplicationProcess();
	nmtObj = new NetworkManagement();
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

void Node::SetNodeName(const char* nodeNameStr)
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

void Node::AddProcessImage(ProcessImage piObj)
{
	PICollection.push_back(piObj);
}

void Node::AddNETProcessImage(NETProcessImage netPIobj)
{
	NETPIColl.push_back(netPIobj);
}

void Node::AddMNPDOvar(MNPdoVariable pdoVarObj, PDOType pdotype)
{
	if (PDO_RPDO == pdotype)
	{
		MNPDOOUTVarCollection.push_back(pdoVarObj);
	}
	else if (PDO_TPDO == pdotype)
	{
		MNPDOINVarCollection.push_back(pdoVarObj);
	}
}

IndexCollection* Node::GetPDOIndexCollection(PDOType pdotype)
{
	IndexCollection* objPdoIndexCollection = new IndexCollection();
	Index* objIndex = NULL;
	for (INT32 iLoopCount = 0; iLoopCount < indexCollObj->GetNumberofIndexes();
	        iLoopCount++)
	{
		objIndex = indexCollObj->GetIndexByPosition(iLoopCount);

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

	for (INT32 indexLC = 0; indexLC < indexCollObj->GetNumberofIndexes();
	        indexLC++)
	{
		indexObj = indexCollObj->GetIndexByPosition(indexLC);
		if ((indexObj->GetPDOType() == PDO_TPDO))
		{
			if ((0 == strncmp(indexObj->GetIndexValue(), "1A", 2))
			        || 0 == strncmp(indexObj->GetIndexValue(), "1a", 2))
			{
				*tpdoCount = *tpdoCount + 1;
			}
			pdoIndexCollObj->AddIndex(*indexObj);
		}
		else if ((indexObj->GetPDOType() == PDO_RPDO))
		{
			if (0 == strncmp(indexObj->GetIndexValue(), "16", 2))
			{
				*rpdoCount = *rpdoCount + 1;
			}
			pdoIndexCollObj->AddIndex(*indexObj);
		}
	}
	return pdoIndexCollObj;
}

IndexCollection* Node::GetIndexCollectionWithoutPDO()
{
	IndexCollection* indexCollObject = new IndexCollection();
	Index* indexObj = NULL;
	for (INT32 indexLC = 0; indexLC < indexCollObj->GetNumberofIndexes();
	        indexLC++)
	{
		indexObj = indexCollObj->GetIndexByPosition(indexLC);
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

	for (UINT32 piLC = 0; piLC < PICollection.size(); piLC++)
	{
		piObj = &PICollection[piLC];

		if (piObj->parameterPos == paramerterPos)
		{
			return piObj;
		}
	}
	return piObj;
}

void Node::DeleteCollectionsForPI()
{
	MNPDOINVarCollection.clear();
	MNPDOOUTVarCollection.clear();
	PICollection.clear();

}

void Node::DeleteCollectionsForNETPI()
{

	NETPIColl.clear();
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

ocfmRetCode Node::SetForcedCycle(const char* tempForcedCycleVal)
{
	//add or update 1f9b
	ocfmRetCode exObj;
	ocfmRetCode indexNodeExObj;
	ocfmRetCode indexMNstoreExObj;

	INT32 idxPos = 0;
	INT32 sIdxPos = 0;

	ostringstream errorString;

	char* sidxId = new char[SUBINDEX_LEN];
	char indexId[] = MULTIPL_CYCLE_ASSIGN_OBJECT;

	exObj.setErrorCode(OCFM_ERR_SUCCESS);

	sidxId = IntToAscii(this->GetNodeId(), sidxId, 16);
	sidxId = PadLeft(sidxId, '0', 2);

	indexMNstoreExObj = IfSubIndexExists(MN_NODEID, MN, indexId, sidxId, &sIdxPos,
	                                     &idxPos);
	indexNodeExObj = IfSubIndexExists(this->GetNodeId(), CN, indexId, sidxId,
	                                  &sIdxPos, &idxPos);
	if ((OCFM_ERR_SUCCESS != indexMNstoreExObj.getErrorCode())
	        && (OCFM_ERR_SUCCESS != indexNodeExObj.getErrorCode()))
	{
		if (OCFM_ERR_INDEXID_NOT_FOUND == indexMNstoreExObj.getErrorCode())
		{
			errorString<<"The Index 1F9B does not exist in MN.\n";
		}
		if (OCFM_ERR_INDEXID_NOT_FOUND == indexNodeExObj.getErrorCode())
		{
			errorString<<"The Index 1F9B does not exist in CN node id:"<<this->GetNodeId()<<".\n";
		}
		if (OCFM_ERR_SUBINDEXID_NOT_FOUND == indexMNstoreExObj.getErrorCode())
		{
			errorString<<"The Subindex "<<sidxId<<" in Index 1F9B does not exist in MN.\n";
		}
		if (OCFM_ERR_SUBINDEXID_NOT_FOUND == indexNodeExObj.getErrorCode())
		{
			errorString<<"The Subindex "<<sidxId<<" in Index 1F9B does not exist in CN node id:"<<this->GetNodeId()<<".\n";
		}
		errorString<<"Unable to assign the multiplexing cycle.\n";

		exObj.setErrorCode(OCFM_ERR_MULTIPLEX_ASSIGN_ERROR);
		exObj.setErrorString(errorString.str());

		return exObj;
	}

	char* sidxName = new char[50];
	sidxName[0] = 0;

	GetSubIndexAttributes(240, MN, indexId, sidxId, NAME, sidxName);
	SetBasicSubIndexAttributes(240, MN, indexId, sidxId, tempForcedCycleVal,
	                           sidxName, true);

	Index* idxObj1 = NULL;
	idxObj1 = GetMNIndexValues(indexId);
	if (NULL != idxObj1)
	{
		//UpdateNumberOfEnteriesSIdx(objMN1F92Index, MN);
		idxObj1->SetFlagIfIncludedCdc(true);
		if (NULL != idxObj1->GetSubIndexbyIndexValue((char*) "00"))
		{
			idxObj1->GetSubIndexbyIndexValue((char*) "00")->SetFlagIfIncludedCdc(
			    true);
		}
	}

	strcpy(sidxName, "");
	GetSubIndexAttributes(this->GetNodeId(), CN, indexId, sidxId, NAME,
	                      sidxName);
	SetBasicSubIndexAttributes(this->GetNodeId(), CN, indexId, sidxId,
	                           tempForcedCycleVal, sidxName, true);

	Index* idxObj;
	idxObj = this->indexCollObj->GetIndexbyIndexValue(indexId);
	if (NULL != idxObj)
	{
		idxObj->SetFlagIfIncludedCdc(true);
		if (NULL != idxObj->GetSubIndexbyIndexValue((char*) "00"))
		{
			idxObj->GetSubIndexbyIndexValue((char*) "00")->SetFlagIfIncludedCdc(
			    true);
		}
	}

	forcedCycle = new char[strlen(tempForcedCycleVal) + ALLOC_BUFFER];
	strcpy((char*) forcedCycle, tempForcedCycleVal);

	delete[] sidxName;
	return exObj;
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

	SetSubIndexAttribute(MN_NODEID, MN, (char *) MULTIPL_CYCLE_ASSIGN_OBJECT,
	                     sidxId, ACTUALVALUE, forcedCycle);
	SetSubIndexAttribute(nodeId, nodeType, (char *) MULTIPL_CYCLE_ASSIGN_OBJECT,
	                     sidxId, ACTUALVALUE, forcedCycle);

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

	stErrorInfo = IfSubIndexExists(MN_NODEID, MN, indexId, sidxId, &sidxPos,
	                               &idxPos);
	if (OCFM_ERR_SUCCESS != stErrorInfo.getErrorCode())
	{
		return;
	}

	char* subIndName = new char[50];
	subIndName[0] = 0;

	GetSubIndexAttributes(240, MN, indexId, sidxId, NAME, subIndName);
	SetBasicSubIndexAttributes(MN_NODEID, MN, indexId, sidxId, presTimoutVal,
	                           subIndName, true);

	Index* idxObj = NULL;
	idxObj = GetMNIndexValues(indexId);
	if (NULL != idxObj)
	{
		UpdateNumberOfEnteriesSIdx(idxObj, MN);
		idxObj->SetFlagIfIncludedCdc(true);
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

boost::optional<bool> Node::TransmitsPRes(void)
{
	return this->transmitsPRes;
}

void Node::SetTransmitsPRes(bool transmitsPRes)
{
	this->transmitsPRes = transmitsPRes;
}

boost::optional<bool> Node::GetAutostartNode(void)
{
	return this->autostartNode;
}
void Node::SetAutostartNode(bool autostartNode)
{
	this->autostartNode = autostartNode;
}

boost::optional<bool> Node::GetResetInOperational(void)
{
	return this->resetInOperational;
}

void Node::SetResetInOperational(bool resetInOperational)
{
	this->resetInOperational = resetInOperational;
}

boost::optional<bool> Node::GetVerifyAppSwVersion(void)
{
	return this->verifyAppSwVersion;
}

void Node::SetVerifyAppSwVersion(bool verifyAppSwVersion)
{
	this->verifyAppSwVersion = verifyAppSwVersion;
}

boost::optional<bool> Node::GetAutoAppSwUpdateAllowed(void)
{
	return this->autoAppSwUpdateAllowed;
}

void Node::SetAutoAppSwUpdateAllowed(bool autoAppSwUpdateAllowed)
{
	this->autoAppSwUpdateAllowed = autoAppSwUpdateAllowed;
}

boost::optional<bool> Node::GetVerifyDeviceType(void)
{
	return this->verifyDeviceType;
}

void Node::SetVerifyDeviceType(bool verifyDeviceType)
{
	this->verifyDeviceType = verifyDeviceType;
}

boost::optional<bool> Node::GetVerifyVendorId(void)
{
	return this->verifyVendorId;
}

void Node::SetVerifyVendorId(bool verifyVendorId)
{
	this->verifyVendorId = verifyVendorId;
}

boost::optional<bool> Node::GetVerifyRevisionNumber(void)
{
	return this->verifyRevisionNumber;
}

void Node::SetVerifyRevisionNumber(bool verifyRevisionNumber)
{
	this->verifyRevisionNumber = verifyRevisionNumber;
}

boost::optional<bool> Node::GetVerifyProductCode(void)
{
	return this->verifyProductCode;
}

void Node::SetVerifyProductCode(bool verifyProductCode)
{
	this->verifyProductCode = verifyProductCode;
}

boost::optional<bool> Node::GetVerifySerialNumber(void)
{
	return this->verifySerialNumber;
}

void Node::SetVerifySerialNumber(bool verifySerialNumber)
{
	this->verifySerialNumber = verifySerialNumber;
}

boost::optional<bool> Node::IsMandatory(void)
{
	return this->isMandatory;
}

void Node::SetIsMandatory(bool isMandatory)
{
	this->isMandatory = isMandatory;
}

boost::optional<bool> Node::IsAsyncOnly(void)
{
	return this->isAsyncOnly;
}

void Node::SetIsAsyncOnly(bool isAsyncOnly)
{
	this->isAsyncOnly =isAsyncOnly;
}

boost::optional<bool> Node::IsType1Router(void)
{
	return this->isType1Router;
}

void Node::SetIsType1Router(bool isType1Router)
{
	this->isType1Router = isType1Router;
}

boost::optional<bool> Node::IsType2Router(void)
{
	return this->isType2Router;
}

void Node::SetIsType2Router(bool isType2Router)
{
	this->isType2Router= isType2Router;
}

template<typename T>
boost::optional<T> Node::SetActualValue(const UINT32 index, const UINT32 subIndex, const T value)
{
	LOG_DEBUG() 
		<< std::hex 
		<< std::showbase
		<< std::boolalpha
		<< "Setting actualValue for " << index << "/" << subIndex
		<< " on node " << this->nodeId
		<< " to '" << value << "'.";
	Index& indexObj = this->indexCollObj->GetIndexRef(index);
	std::ostringstream convertToString;
	convertToString << std::boolalpha << value;
	std::stringstream convertFromString;
	boost::optional<T> oldActualValue;
	const char* oldActualValuePtr = NULL;

	// Retrieve old actual value and store it
	if (subIndex == 0 && !indexObj.HasSubIndices())
	{
		// Client intends to write to Index		
		oldActualValuePtr = indexObj.GetActualValue();
	} 
	else
	{
		// Client intends to write to SubIndex
		SubIndex& subIndexObj = indexObj.GetSubIndexRef(subIndex);
		oldActualValuePtr = subIndexObj.GetActualValue();
	}
	if (oldActualValuePtr)
	{
		std::string oldActualValueStr = oldActualValuePtr;
		LOG_DEBUG() << "Old actualValue: '" << oldActualValueStr << "'";
		if (boost::algorithm::starts_with(oldActualValueStr, "0x")) 
			convertFromString << std::hex << oldActualValueStr.substr(2);
		else
			convertFromString << std::boolalpha << oldActualValueStr;
		T temporary;
		convertFromString >> temporary;
		oldActualValue = temporary;
	}
	else
	{
		LOG_DEBUG() << "Old actualValue: NULL";
	}

	// Write new value
	if (subIndex == 0 && !indexObj.HasSubIndices())
	{
		indexObj.SetActualValue(convertToString.str().c_str());
	} 
	else
	{
		SubIndex& subIndexObj = indexObj.GetSubIndexRef(subIndex);
		subIndexObj.SetActualValue(convertToString.str().c_str());
	}
	LOG_DEBUG() << "New actualValue: '" << convertToString.str().c_str() << "'";
	return oldActualValue;
}

template<typename T>
boost::optional<T> Node::GetActualValue(const UINT32 index, const UINT32 subIndex)
{
	Index& indexObj = this->indexCollObj->GetIndexRef(index);
	std::stringstream convertFromString;
	boost::optional<T> actualValue;
	const char* actualValuePtr = NULL;

	if (subIndex == 0 && !indexObj.HasSubIndices())
	{
		// Client intends to read to Index		
		actualValuePtr = indexObj.GetActualValue();
	} 
	else
	{
		// Client intends to read SubIndex
		SubIndex& subIndexObj = indexObj.GetSubIndexRef(subIndex);
		actualValuePtr = subIndexObj.GetActualValue();
	}
	if (actualValuePtr != NULL && strlen(actualValuePtr) != 0)
	{
		std::string actualValueStr = actualValuePtr;
		if (boost::algorithm::starts_with(actualValueStr, "0x")) 
			convertFromString << std::hex << actualValueStr.substr(2);
		else
			convertFromString << std::boolalpha << actualValueStr;
		T temporary;
		convertFromString >> temporary;
		actualValue = temporary;
	}
	return actualValue;
}

template boost::optional<UINT32> Node::SetActualValue<UINT32>(const UINT32 index, const UINT32 subIndex, const UINT32 value);
template boost::optional<bool> Node::SetActualValue<bool>(const UINT32 index, const UINT32 subIndex, const bool value);
template boost::optional<std::string> Node::SetActualValue<std::string>(const UINT32 index, const UINT32 subIndex, const std::string value);

template boost::optional<UINT32> Node::GetActualValue<UINT32>(const UINT32 index, const UINT32 subIndex);
template boost::optional<bool> Node::GetActualValue<bool>(const UINT32 index, const UINT32 subIndex);
template boost::optional<std::string> Node::GetActualValue<std::string>(const UINT32 index, const UINT32 subIndex);

#ifndef __GNUC__
#pragma endregion Properties
#endif
