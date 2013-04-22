/**
 *****************************************************************************************************
 \file			Node.h

 \brief			Handles each node's information on id,name,type present inside the network
 *****************************************************************************************************
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
#ifndef Node_h
#define Node_h

/****************************************************************************************************/
/* Includes */

#include "DataTypeCollection.h"
#include "IndexCollection.h"
#include "Declarations.h"
#include "ApplicationProcess.h"
#include "ProcessImage.h"
#include "NetworkManagement.h"

/**
 ******************************************************************************************************
 \class			Node
 \brief			This class includes each node's information on id,name,type present inside the network
 
 ******************************************************************************************************/
class Node
{
	public:
		Node(void);
		~Node(void);

		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the DataTypeCollection object of the Node

		 \return	DataTypeCollection*
		 */
		/*****************************************************************************/
		DataTypeCollection* GetDataTypeCollection();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the IndexCollection object of the Node
		 
		 \return	IndexCollection*
		 */
		/*****************************************************************************/
		IndexCollection* GetIndexCollection();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the ApplicatinoProcess object of the Node
		 
		 \return	ApplicationProcess*
		 */
		/*****************************************************************************/
		ApplicationProcess* GetApplicationProcess();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the NetworkManagement object of the Node
		 
		 \return	NetworkManagement*
		 */
		/*****************************************************************************/
		NetworkManagement *GetNetworkManagement();

		TCollection<ProcessImage> PICollection;
		TCollection<MNPdoVariable> MNPDOINVarCollection;
		TCollection<MNPdoVariable> MNPDOOUTVarCollection;
		TCollection<NETProcessImage> NETPIColl;

		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the Node ID of the Node
		 
		 \return	INT32
		 */
		/*****************************************************************************/
		INT32 GetNodeId();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to set the Node ID of the Node
		 
		 \param		nodeID		Integer to hold the Node ID of the Node

		 \return	void
		 */
		/*****************************************************************************/
		void SetNodeId(INT32 nodeID);

		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the position of the Node
		 
		 \return	INT32
		 */
		/*****************************************************************************/
		INT32 GetNodeIndex();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to set the position of the Node
		 
		 \param		nodePos		Integer to the position of the node

		 \return	void
		 */
		/*****************************************************************************/
		void SetNodeIndex(INT32 nodePos);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the Name of the Node

		 \return	char*		Node name as string
		 */
		/*****************************************************************************/
		char* GetNodeName();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to set the Name of the Node
		 
		 \param		nodeNameStr		Character pointer to the name of the node

		 \return	void
		 */
		/*****************************************************************************/
		void SetNodeName(char* nodeNameStr);

		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the NodeType of the Node

		 \return	NodeType
		 */
		/*****************************************************************************/
		NodeType GetNodeType();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to set the NodeType of the node
		 
		 \param		objNodeType 	Enum Variable of NodeType to hold the node type
		 
		 \return	void
		 */
		/*****************************************************************************/
		void SetNodeType(NodeType objNodeType);

		/*****************************************************************************/
		/**
		 \brief		This function shall be used to check if the Node has PDO objects
		 
		 \return	BOOL
		 
		 \retval	TRUE		if the node has PDO objects
		 \retval	FALSE		if the node does not have PDO objects	
		 */
		/*****************************************************************************/
		bool HasPdoObjects();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to set the hasPdoObjects flag of the Node to indicate that the Node has PDO objects

		 \param		flag	Boolean flag to hold the value 'true' if PDO objects are present
		 
		 \return	void
		 */
		/*****************************************************************************/
		void SetFlagForPdoObjects(bool flag);

		/*****************************************************************************/
		/**
		 \brief		This function shall be used to create an IndexCollection object for the node
		 
		 \return	void
		 */
		/*****************************************************************************/
		void CreateIndexCollection();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to create a new DataTypeCollection object
		 
		 \return	void
		 */
		/*****************************************************************************/
		void CreateDataTypeCollection();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to create a new ApplicationProcess object
		 
		 \return	void
		 */
		/*****************************************************************************/
		void CreateApplicationProcess();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to create a new NetworkManagement object
		 
		 \return	void
		 */
		/*****************************************************************************/
		void CreateNetworkManagement();

		/*****************************************************************************/
		/**
		 \brief		This function shall be used to add a ProcessImage object to the collection list 
		 
		 \param		piObj  Class variable of ProcessImage to hold the object
		 
		 \return	void
		 */
		/*****************************************************************************/
		void AddProcessImage(ProcessImage piObj);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to add a NETProcessImage object to the collection list
		 
		 \param		netPIobj  Class variable of NETProcessImage to hold the object
		 
		 \return	void
		 */
		/*****************************************************************************/
		void AddNETProcessImage(NETProcessImage netPIobj);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to add a MNPdoVariable object for the MN. It is added to the MNPDOOUTVarCollection list if given pdotype is PDO_RPDO / to the MNPDOINVarCollection list if given pdotype is PDO_TPDO
		 
		 \param		pdoVarObj      	Structure variable of MNPdoVariable
		 \param		pdotype			Enum variable of PDOType
		 
		 \return	void
		 */
		/*****************************************************************************/
		void AddMNPDOvar(MNPdoVariable pdoVarObj, PDOType pdotype);

		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return an object containing all the Index objects of PDO type from the IndexCollection list
		 
		 \param		pdotype		 Enum variable of PDOType hold the PDO type
		 
		 \return	IndexCollection*
		 */
		/*****************************************************************************/
		IndexCollection* GetPDOIndexCollection(PDOType pdotype);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return an object containing all the Index objects of non PDO type from the IndexCollection list
		 
		 \return	IndexCollection*
		 */
		/*****************************************************************************/
		IndexCollection* GetIndexCollectionWithoutPDO();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return an object containing all the Index objects of PDO type and also update the number of RPDO and TPDO indexes in the calling function
		 
		 \param		rpdoCount	Integer pointer to number of RPDOs
		 \param		tpdoCount	Integer pointer to number of TPDOs
		 
		 \return	IndexCollection*
		 */
		/*****************************************************************************/
		IndexCollection* getPDOIndexCollection(INT32 *rpdoCount,
				INT32 *tpdoCount);

		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return a ProcessImage object at given position in the PICollection list
		 
		 \param		paramerterPos		Integer to hold the value of parameter index
		 
		 \return	ProcessImage*
		 */
		/*****************************************************************************/
		ProcessImage* GetPIbyParaIndex(INT32 paramerterPos);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to delete the objects in the MNPDOINVarCollection, NPDOOUTVarCollection and PICollection lists
		 
		 \return	void
		 */
		/*****************************************************************************/
		void DeleteCollectionsForPI();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the StationType of the Node
		 
		 \return	StationType
		 */
		/*****************************************************************************/
		StationType GetStationType();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to set the StationType of the Node
		 
		 \param		stationtype		Enum variable of EStationType to hold value of Station type
		 
		 \return	void
		 */
		/*****************************************************************************/
		void SetStationType(StationType stationtype);

		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the force cycle value of the Node
		 
		 \return	char*
		 */
		/*****************************************************************************/
		char* GetForcedCycleValue();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to check if the Node is configured for forcecycle
		 
		 \return	BOOL
		 
		 \retval	TRUE		if the Node is configured for forcecycle
		 \retval	FALSE		if the Node is not configured for forcecycle	
		 */
		/*****************************************************************************/
		bool GetForceCycleFlag();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to set the force cycle value of the Node
		 
		 \param		tempForcedCycleVal     	Character pointer to the forced cycle value
		 
		 \return	ocfmRetCode		ConfiguratorErrors
		 */
		/*****************************************************************************/
		ocfmRetCode SetForcedCycle(char* tempForcedCycleVal);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to reset the force cycle value of the Node
		 
		 \return	void
		 */
		/*****************************************************************************/
		void ResetForcedCycleValue();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to enable/disable forcecylce for the Node
		 
		 \param		forceCycleFlag       Boolean flag to hold the value of forced cycle flag
		 
		 \return	void
		 */
		/*****************************************************************************/
		void SetForceCycleFlag(bool forceCycleFlag);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to set the poll response timeout value in MN
		 
		 \param		presTimoutVal      Character pointer to the value of poll response timeout
		 
		 \return	void
		 */
		/*****************************************************************************/
		void SetPollResponseTimeout(char* presTimoutVal);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to check if the IndexCollection object is NULL
		 
		 \return	BOOL
		 
		 \retval	TRUE		if IndexCollection object is NULL
		 \retval	FALSE		if IndexCollection object is not NULL
		 */
		/*****************************************************************************/
		bool IsNull();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to delete the objects in the NETPIColl list
		 
		 \return	void
		 */
		/*****************************************************************************/
		void DeleteCollectionsForNETPI();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to set the PRes Actual Payload value for the node
		 
		 \param		value      Integer to hold value of Pres Actual payload
		 
		 \return	void
		 */
		/*****************************************************************************/
		void SetPResActPayloadValue(INT32 value);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the PRes Actual Payload value for the node
		 
		 \return	INT32
		 */
		/*****************************************************************************/
		INT32 GetPResActPayloadValue();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to set the PReq Actual Payload value for the node
		 
		 \param		value		Integer to hold value of Preq Actual payload
		 
		 \return	void
		 */
		/*****************************************************************************/
		void SetPReqActPayloadValue(INT32 value);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the PReq Actual Payload value for the node
		 
		 \return	INT32
		 */
		/*****************************************************************************/
		INT32 GetPReqActPayloadValue();

	private:
		DataTypeCollection *dtCollObj;
		IndexCollection *indexCollObj;
		ApplicationProcess* appProcessObj;
		NetworkManagement *nmtObj;

		INT32 nodeId;
		INT32 nodePosition;
		INT32 presActualPayload;
		INT32 preqActualPayload;
		bool hasPdoObjects;
		bool isForcedCycle;
		char* nodeName;
		char* forcedCycle;
		char* presTimeOut;
		StationType stationType;
		NodeType nodeType;
};
#endif // Node_h
