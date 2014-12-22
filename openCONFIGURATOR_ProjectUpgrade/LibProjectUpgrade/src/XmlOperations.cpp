/*******************************************************************************
* \file   XmlOperations.cpp
*
* \author Ramakrishnan Periyakaruppan, Kalycito Infotech Private Limited.
*
* \copyright (c) 2014, Kalycito Infotech Private Limited
*                    All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the
*     documentation and/or other materials provided with the distribution.
*   * Neither the name of the copyright holders nor the
*     names of its contributors may be used to endorse or promote products
*     derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

#include "../include/XmlOperations.h"
#include <libxml/xmlschemas.h>
#include <libxml/xpathInternals.h>
#include <boost/format.hpp>

PROJECT_UPGRADE_OPEN_NAMESPACE
/* Global variables */
std::string* xddValidationError = NULL;
char xddErrorBuf[1024];

xmlXPathObjectPtr GetNodeSet(const xmlDocPtr doc, const std::string & xpath)
{
	xmlXPathContextPtr context = xmlXPathNewContext(doc);
	if (context == NULL)
		return NULL;

	//Evaluate xpath expression.
	xmlXPathObjectPtr result = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), context);
	xmlXPathFreeContext(context);
	if (result == NULL)
		return NULL;

	//No nodes found by the xpath expression.
	if (xmlXPathNodeSetIsEmpty(result->nodesetval))
	{
		xmlXPathFreeObject(result);
		return NULL;
	}

	return result;
}

xmlXPathObjectPtr GetXdcNodeSet(const xmlDocPtr doc, const std::string & xpath)
{
	xmlXPathContextPtr context = xmlXPathNewContext(doc);
	if (context == NULL)
		return NULL;

	//Add namespaces to the xpath expression.
	if (xmlXPathRegisterNs(context,  BAD_CAST "plk", BAD_CAST "http://www.ethernet-powerlink.org") != 0)
		return NULL;

	if (xmlXPathRegisterNs(context,  BAD_CAST "co", BAD_CAST "http://sourceforge.net/projects/openconf/configuration") != 0)
		return NULL;

	//Evaluate xpath expression.
	xmlXPathObjectPtr result = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), context);
	xmlXPathFreeContext(context);
	if (result == NULL)
		return NULL;

	//No nodes found by the xpath expression.
	if (xmlXPathNodeSetIsEmpty(result->nodesetval))
	{
		xmlXPathFreeObject(result);
		return NULL;
	}

	return result;
}

Result ValidateXMLFile(const xmlDocPtr doc, const std::string & schema_filename)
{
	if (schema_filename.empty())
	{
		// The schema cannot be loaded or is not well-formed.
		boost::format formatter(kMsgSchemaNotFound);
		formatter % schema_filename;

		return Result(ErrorCode::XDD_SCHEMA_NOT_FOUND, formatter.str());
	}

	xmlDocPtr schema_doc = xmlReadFile(schema_filename.c_str(), NULL, XML_PARSE_NONET);
	if (schema_doc == NULL)
	{
		// The schema cannot be loaded or is not well-formed.
		boost::format formatter(kMsgSchemaNotFound);
		formatter % schema_filename;

		return Result(ErrorCode::XDD_SCHEMA_NOT_FOUND, formatter.str());
	}

	xmlSchemaParserCtxtPtr parser_ctxt = xmlSchemaNewDocParserCtxt(schema_doc);
	if (parser_ctxt == NULL)
	{
		xmlFreeDoc(schema_doc);
		// Unable to create a parser context for the schema.
		boost::format formatter(kMsgSchemaParserContextError);
		formatter % schema_filename;

		return Result(ErrorCode::XDD_SCHEMA_PARSER_CONTEXT_ERROR, formatter.str());
	}

	xmlSchemaPtr schema = xmlSchemaParse(parser_ctxt);
	if (schema == NULL)
	{
		// The schema itself is not valid.
		xmlSchemaFreeParserCtxt(parser_ctxt);
		xmlFreeDoc(schema_doc);
		boost::format formatter(kMsgSchemaInvalid);
		formatter % schema_filename;

		return Result(ErrorCode::XDD_SCHEMA_NOT_VALID, formatter.str());
	}

	xmlSchemaValidCtxtPtr valid_ctxt = xmlSchemaNewValidCtxt(schema);
	if (valid_ctxt == NULL)
	{
		// Unable to create a validation context for the schema.
		xmlSchemaFree(schema);
		xmlSchemaFreeParserCtxt(parser_ctxt);
		xmlFreeDoc(schema_doc);
		boost::format formatter(kMsgSchemaValidationContextError);
		formatter % schema_filename;

		return Result(ErrorCode::XDD_SCHEMA_VALIDATION_CONTEXT_ERROR, formatter.str());
	}

	//Set error handler for validation errors.
	xmlSchemaSetValidErrors(valid_ctxt,
							(xmlSchemaValidityErrorFunc) HandleSchemaValidationError,
							(xmlSchemaValidityWarningFunc) HandleSchemaValidationError,
							stderr);

	int retVal = xmlSchemaValidateDoc(valid_ctxt, doc);

	xmlSchemaFreeValidCtxt(valid_ctxt);
	xmlSchemaFree(schema);
	xmlSchemaFreeParserCtxt(parser_ctxt);
	xmlFreeDoc(schema_doc);

	if (retVal)
	{
		return Result(ErrorCode::XDD_SCHEMA_VALIDATION_FAILED, *xddValidationError);
	}

	return Result(ErrorCode::SUCCESS);
}

void HandleSchemaValidationError(void*, const char* msg, ...)
{
	va_list args;
	va_start(args, msg);
	int len = vsprintf(xddErrorBuf, msg, args);
	va_end(args);

	//FIXME: Check memory leak?
	if (len == 0)
	{
		xddValidationError = new std::string("Error could not be retrieved.");
	}
	else
	{
		xddValidationError = new std::string(xddErrorBuf);
	}
}

PROJECT_UPGRADE_CLOSE_NAMESPACE
