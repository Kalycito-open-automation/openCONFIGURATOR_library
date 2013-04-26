/**
 *****************************************************************************
 \file		validation.h

 \brief		This file contains the non API declarations that are used in checking existence of datatypes, SubIndices in collection list and version number of tool
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

#ifndef Validation_h
#define Validation_h

/****************************************************************************/
/* Includes */

#include "../Include/openCONFIGURATOR.h"
#include "../Include/Internal.h"

/****************************************************************************/
/* Function Declarations */

/*****************************************************************************/
/**
 \brief		This function shall be used to check if the object of given Node ID and datatype is present in the NodeCollection list

 \param		dataValue		Character pointer to hold the datavalue
 \param		nodeId			Integer Pointer to hold the value of node id

 \return	BOOL
 \retval	TRUE			if the object of given Node ID and datatype is present in the NodeCollection list
 \retval	FALSE			if the object of given Node ID and datatype is not present in the NodeCollection list
 */
/*****************************************************************************/
bool CheckIfDataTypeExists(char* dataValue, INT32 nodeId);
/*****************************************************************************/
/**
 \brief		This function shall be used to check if the object of given Node ID and datatype is present in the NodeCollection list

 \param		dtName		Character pointer to the datatype name
 \param		nodeId		Integer pointer to the value of node id

 \return	BOOL
 \retval	TRUE		if the object of given Node ID and datatype is present in the NodeCollection list
 \retval	FALSE		if the object of given Node ID and datatype is not present in the NodeCollection list
 */
/*****************************************************************************/
bool CheckIfDataTypeByNameExists(char* dtName, INT32 nodeId);
/*****************************************************************************/
/**
 \brief		This API shall be used to check for the presence of SubIndex in Index of given node ID and type in the collection list

 \param		nodeId			Integer to hold the value of node id
 \param		nodeType		Enum variable of NodeType to hold the value of Node type
 \param		indexID			Character pointer to the value of Index Id
 \param		subIndexId		Character pointer to the value of SubIndex Id

 \return	BOOL
 \retval	TRUE			if the required SubIndex exists
 \retval	FALSE			if the required SubIndex does not exist
 */
/*****************************************************************************/
bool CheckIfSubIndexExists(INT32 nodeId, NodeType nodeType, char* indexID,
		char* subIndexId);
/*****************************************************************************/
/**
 \brief		This function shall be used to retrieve the version number of the tool that was used during generation from the XML file contents and checks if it matches with the tool version

 \param		reader		xml Variable of xmlTextReaderPtr

 \return	BOOL
 \retval	TRUE		if retrieved tool version matches with the tool version
 \retval	FALSE		if retrieved tool version does not match with the tool version
 */
/*****************************************************************************/
bool IfVersionNumberMatches(xmlTextReaderPtr reader);

#endif // Validation_h
