/**
 ******************************************************************************
 \file		APIOperations.h

 \brief		This file contains the non API function declarations that are used to support the API functions in APIOperations.cpp
 ******************************************************************************
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

 *****************************************************************************/
#ifndef APIOperations_h
#define APIOperations_h

/*****************************************************************************/
/* Includes */

/*****************************************************************************/
/* Function Declarations */

/*****************************************************************************/
/**
 \brief		This function resets the values in all the PDO(0x14XX, 0x16XX, 0x18XX, 0x1AXX) Indices and Sub Indices to 0

 \param		nodeId		Integer to hold node id
 \param		nodeType	Enum of type NodeType to hold node type

 \return	void
 */
/*****************************************************************************/
void ResetAllPdos(INT32 nodeId, NodeType nodeType);
/*****************************************************************************/
/**
 \brief		This function shall write the buffer(char pointer) with the index data(id value, datatype value, actual value) of the node to be used for CDC generation

 \param		indexObj		Class pointer of Index for node
 \param		cdcBuffer		Character pointer to the buffer data

 \return	void
 */
/*****************************************************************************/
void GetIndexData(Index* indexObj, char* cdcBuffer);
/*****************************************************************************/
/**
 \brief		This function shall write all the indices of the CNs into CDC file(mnobd.txt)

 \param		fileName	Character pointer to the full path of the file (mnobd.txt)

 \return	void
 */
/*****************************************************************************/
void WriteCNsData(char* fileName);
/*****************************************************************************/
/**
 \brief		This function shall process the 'Complex' datatype collection and generate the process image for the node

 \param		cdtObj				Class pointer of ComplexDataType for node
 \param		appProcessObj		Class pointer of ApplicationProcess for node
 \param		nodeObj				Class pointer of Node for node
 \param		parameterObj		Struct pointer of Parameter for node
 \param		pdoType				Enumeration flag for PDO type
 \param		moduleName			Character pointer to the Module name
 \param		moduleIndexId		Character pointer to the Module Index value

 \return	INT32				Total bytes mapped for that module
 */
/*****************************************************************************/
INT32 ProcessCDT(ComplexDataType* cdtObj, ApplicationProcess* appProcessObj,
		Node* nodeObj, Parameter* parameterObj, PDOType pdoType,
		char* moduleName, char* moduleIndexId);
/*****************************************************************************/
/**
 \brief		This function shall decode the unique ID reference & generate the relevant MN PDO variable collection

 \param		uniquedIdref	Character pointer to the uniquedId reference
 \param		nodeObj			Class pointer of Node for node
 \param		pdoType			Enumeration flag for PDO type
 \param		moduleName		Character pointer to the Module name
 \param		moduleIndex		Character pointer to the Module Index value

 \return	INT32			Returns total bytes mapped for that module
 */
/******************************************************************************/
INT32 DecodeUniqueIDRef(char* uniquedIdref, Node* nodeObj, PDOType pdoType,
		char* moduleName, char* moduleIndex);
/*****************************************************************************/
/**
 \brief		This function shall set the MN PDO actual value calculated from the MNPdoVariable struct generated by Processpdonodes and the iPrevSubIndex value is updated

 \param		mnPdoVarObj		Struct of type MNPdoVariable
 \param		prevSubIndex	Integer to update the previous subindex used
 \param		indexObj		Class pointer of the class Index of the CN's PDO index
 \param		indexId			Character pointer to the MN index value
 \param		prevSize		Integer to update the previous size used

 \return	void
 */
/*****************************************************************************/
void GetMNPDOSubIndex(MNPdoVariable mnPdoVarObj, INT32& prevSubIndex,
		Index* indexObj, char* indexId, INT32 prevSize);
/*****************************************************************************/
/**
 \brief		This function shall add the specified attribute for each Sub Index

 \param		indexId				Character pointer to the index id
 \param		indexCollObj		Class pointer of the class IndexCollection
 \param		nodeId				Integer to hold the node id of the MN
 \param		value				Character pointer to the value set to subindex
 \param		setDefaultValue		Boolean flag to set for Default Value

 \return	void
 */
/*****************************************************************************/
void AddForEachSIdx(char *indexId, IndexCollection *indexCollObj, INT32 nodeId,
		char *value, bool setDefaultValue);
/*****************************************************************************/
/**
 \brief		This function shall check whether the offset is used or not

 \param		iOffset			Integer to hold the offset value
 \param		varPdoType		Enum of type PDOType to hold PDO type

 \return	void
 */
/*****************************************************************************/
void CheckIfOffsetUsed(INT32& iOffset, PDOType varPdoType);

#endif // APIOperations_h
