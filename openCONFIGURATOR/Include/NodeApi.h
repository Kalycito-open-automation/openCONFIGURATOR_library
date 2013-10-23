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

namespace openCONFIGURATOR 
{
	namespace Library
	{
		namespace API
		{

			/*DLLEXPORT ocfmRetCode AddNode(Node const& node, const std::string path, const std::string xddFile);*/
			// TODO: Remove nodeType since nodeId == 240 -> MN, all others CN.
			DLLEXPORT ocfmRetCode AddNode(const UINT32 nodeId, const NodeType nodeType, const std::string nodeName, const std::string xddFile = "");
			DLLEXPORT ocfmRetCode DeleteNode(const UINT32 nodeId);
			/*DLLEXPORT ocfmRetCode GetNode(const UINT32 nodeId, Node& node);*/
			DLLEXPORT ocfmRetCode ReplaceXdd(const UINT32 nodeId, const std::string path, const std::string xddFile);
			DLLEXPORT bool IsExistingNode(const UINT32 nodeId);
			DLLEXPORT UINT32 GetNodeCount();

		}
	}
}

#endif