/************************************************************************
\file NodeApi.h
\brief Functions to add/remove/modify POWERLINK-Nodes.
\author Christoph Ruecker, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
************************************************************************/

#ifndef NODE_API_H_
#define NODE_API_H_

#include <string>
#include "Exception.h"
#include "Exports.h"
#include "Node.h"
#include "Result.h"

namespace openCONFIGURATOR 
{
	namespace Library
	{
		namespace API
		{
			/** \addtogroup nodeapi
			 * @{
			 */
			/**
			\brief	Add a new node to the network.

			\param[in] nodeId Supply 240 for an MN, 1..239 for CNs.
			\param[in] nodeName	Name of the node.
			\param[in] xddFile The node's XDD to import (default CN-XDD if empty).

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result AddNode(const UINT32 nodeId, const std::string nodeName, const std::string xddFile = "");

			/**
			\brief	Delete a node from the network.

			\param[in] nodeId NodeID for the node to be deleted.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result DeleteNode(const UINT32 nodeId);

			/*DLLEXPORT Result GetNode(const UINT32 nodeId, Node& node);*/
			/**
			\brief	Replace the XDD of a node.

			\param [in] nodeId	NodeID to identify the node to replace the XDD.
			\param [in] path	Path to new XDD.
			\param [in] xddFile Filename of new XDD.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result ReplaceXdd(const UINT32 nodeId, const std::string path, const std::string xddFile);

			/**
			\brief	Check if a node exists in the POWERLINK network.

			\param [in] nodeId	NodeID to be checked for existence.
			\param [out] exists <code>True</code> if the node exists, <code>false</code> otherwise.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result IsExistingNode(const UINT32 nodeId, bool& exists);

			/**
			\brief	Return the number of nodes in the network.

			\param[out] nodeCount Overall no. of nodes in the POWERLINK network.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetNodeCount(UINT32& nodeCount);
			/** @} */
		}
	}
}

#endif