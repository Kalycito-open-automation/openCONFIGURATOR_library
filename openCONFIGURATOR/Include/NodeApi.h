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
			\param[in] nodeName	Name of the node. Auto-generated as "MN" or "CN_<nodeId>" if empty.
			\param[in] xddFile The node's XDD to import (default XDD if empty).

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result AddNode(const UINT32 nodeId, const std::string nodeName, const std::string xddFile = "");

			/**
			\brief	Delete a node from the network.

			\param[in] nodeId NodeID of the node to be deleted.

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

			/**
			\brief	Set a POWERLINK parameter value of an existing node.

			\param[in] nodeId NodeID of the node to set the parameter for.
			\param[in] param openCONFIGURATOR::Library::NodeParameter::NodeParameter to set.
			\param[in] value Parameter value to set.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result SetNodeParameter(const UINT32 nodeId, const openCONFIGURATOR::Library::NodeParameter::NodeParameter param, const std::string value);

			/**
			\brief	Get a POWERLINK parameter value of an existing node.

			\param[in] nodeId NodeID for the node to be altered.
			\param[in] param openCONFIGURATOR::Library::NodeParameter::NodeParameter to be retrieved
			\param[out] value to be returned.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetNodeParameter(const UINT32 nodeId, const openCONFIGURATOR::Library::NodeParameter::NodeParameter param, std::string& value);

			/**
			\brief	Return the nodeIDs of all existing nodes in the POWERLINK network.

			\param[out] nodeIds of the existing nodes.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetNodes(std::vector<UINT32>& nodeIds); 
			/** @} */
		}
	}
}

#endif
