#ifndef NODE_API_H_
#define NODE_API_H_

#include <string>
#include "Exception.h"
#include "Exports.h"
#include "Node.h"

/*DLLEXPORT ocfmRetCode AddNode(Node const& node, const string path, const string xddFile);*/
// TODO: Remove nodeType since nodeId == 240 -> MN, all others CN.
DLLEXPORT ocfmRetCode AddNode(const UINT32 nodeId, const NodeType nodeType, const string nodeName, const string xddFile = "");
DLLEXPORT ocfmRetCode DeleteNode(const UINT32 nodeId);
/*DLLEXPORT ocfmRetCode GetNode(const UINT32 nodeId, Node& node);*/
DLLEXPORT ocfmRetCode ReplaceXdd(const UINT32 nodeId, const string path, const string xddFile);
DLLEXPORT bool IsExistingNode(const UINT32 nodeId);
DLLEXPORT UINT32 GetNodeCount();

#endif