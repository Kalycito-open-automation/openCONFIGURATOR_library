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

			/*DLLEXPORT Result AddNode(Node const& node, const std::string path, const std::string xddFile);*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result AddNode(const UINT32 nodeId, const std::string nodeName, const std::string xddFile = "");
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result DeleteNode(const UINT32 nodeId);
			/*DLLEXPORT Result GetNode(const UINT32 nodeId, Node& node);*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result ReplaceXdd(const UINT32 nodeId, const std::string path, const std::string xddFile);
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result IsExistingNode(const UINT32 nodeId, bool& exists);
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetNodeCount(UINT32& nodeCount);
		}
	}
}

#endif