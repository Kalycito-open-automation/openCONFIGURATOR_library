################################################################################
#
# Project: openCONFIGURATOR
#
# (c) Bernecker + Rainer Industrie-Elektronik Ges.m.b.H.
#     B&R Strasse 1, A-5142 Eggelsberg
#     www.br-automation.com
#
# Description: Main cmake file of openCONFIGURATOR
#
# License:
#
#   Redistribution and use in source and binary forms, with or without
#   modification, are permitted provided that the following conditions
#   are met:
#
#   1. Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#
#   2. Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in the
#      documentation and/or other materials provided with the distribution.
#
#   3. Neither the name of the copyright holders nor the names of its
#      contributors may be used to endorse or promote products derived
#      from this software without prior written permission. For written
#      permission, please contact office@br-automation.com.
#
#   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
#   FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
#   COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
#   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
#   BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
#   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
#   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#   POSSIBILITY OF SUCH DAMAGE.
#
#   Severability Clause:
#
#       If a provision of this License is or becomes illegal, invalid or
#       unenforceable in any jurisdiction, that shall not affect:
#       1. the validity or enforceability in that jurisdiction of any other
#          provision of this License; or
#       2. the validity or enforceability in other jurisdictions of that or
#          any other provision of this License.
#
################################################################################
# ensure working support of VS.NET 2010 and version strings
CMAKE_MINIMUM_REQUIRED (VERSION 2.8.8)

PROJECT (OPENCONFIGURATOR) 
###############################################################################
# Set additional settings for MS Visual Studio Compiler
###############################################################################
IF (MSVC)
	SET(CMAKE_CXX_FLAGS "/Zi /nologo /W4 /WX- /Od /Oy- /Ob0 /D _CRT_SECURE_NO_WARNINGS /Gm- /EHsc /GS /fp:precise /Zc:wchar_t /Zc:forScope /Gd /analyze- /errorReport:queue /GR-")
	SET(CMAKE_CXX_FLAGS_RELEASE "/MD")
	SET(CMAKE_CXX_FLAGS_DEBUG "/MDd")
ELSEIF(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  	SET(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -Weffc++")
ENDIF(MSVC)
	
###############################################################################
# Set options
###############################################################################
IF (WIN32)
	OPTION (OPEN_CONFIGURATOR_DLL "Build openCONFIGURATOR static library" ON)
ELSE(WIN32)	
ENDIF (WIN32)
OPTION (OPEN_CONFIGURATOR_DOCUMENTATION "Build openCONFIGURATOR source code documentation." OFF)
OPTION (OPEN_CONFIGURATOR_TCL_WRAPPER "Build openCONFIGURATOR TCL/TK library" OFF)	

###############################################################################
# Add a target to generate API documentation with Doxygen
###############################################################################
FIND_PACKAGE(Doxygen)
IF (OPEN_CONFIGURATOR_DOCUMENTATION)
	IF(DOXYGEN_FOUND)
		CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/openCONFIGURATOR.doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
		ADD_CUSTOM_TARGET(
			DOCUMENTATION ALL ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Generating API documentation with Doxygen" VERBATIM
		)
	ENDIF(DOXYGEN_FOUND)
ENDIF()


###############################################################################
# Add subprojects
###############################################################################
ADD_SUBDIRECTORY(openCONFIGURATOR)
###############################################################################
# Add a target to generate TCL wrapper
###############################################################################
IF(OPEN_CONFIGURATOR_TCL_WRAPPER)
	ADD_SUBDIRECTORY(openCONFIGURATORWrapper)
ENDIF(OPEN_CONFIGURATOR_TCL_WRAPPER)

