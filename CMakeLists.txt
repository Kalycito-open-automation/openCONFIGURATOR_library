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
# Ensure working support of VS.NET 2010 and version strings
CMAKE_MINIMUM_REQUIRED (VERSION 2.8.8)

INCLUDE(CMakeDependentOption)

PROJECT (openCONFIGURATOR) 
###############################################################################
# Compiler flags for MS Visual Studio
###############################################################################
IF(MSVC)
	SET(CMAKE_CXX_FLAGS "/Zi /nologo /W4 /WX- /Od /Oy- /Ob0 /D_CRT_SECURE_NO_WARNINGS /D_SCL_SECURE_NO_WARNINGS /Gm- /EHsc /GS /fp:precise /Zc:wchar_t /Zc:forScope /Gd /analyze- /errorReport:queue /GR")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /DDEBUG")
	# \${BUILD_TYPE} will only be correctly evaluated by the INSTALL command!
	SET(INSTALL_DIR "${CMAKE_BINARY_DIR}/\${BUILD_TYPE}")
	# This variable will be used by BR.FS.CdcBuilder
	SET(INSTALL_DIR_VS "${CMAKE_BINARY_DIR}/$(Configuration)")
ELSEIF(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Weffc++ -std=c++03")
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
	SET(INSTALL_DIR "${CMAKE_BINARY_DIR}/bin")
ENDIF(MSVC)

###############################################################################
# Set options
###############################################################################
IF(WIN32)
	OPTION (OPEN_CONFIGURATOR_NET_WRAPPER "Build openCONFIGURATOR C# wrapper library." OFF)
	IF(MSVC11 OR MSVC10)
		CMAKE_DEPENDENT_OPTION(OPEN_CONFIGURATOR_NET_EXE "Build openCONFIGURATOR C# executable." OFF 
			"OPEN_CONFIGURATOR_NET_WRAPPER" ON)
		OPTION (OPEN_CONFIGURATOR_UNIT_TESTS "Build openCONFIGURATOR unit tests." OFF)
	ENDIF()	
#	OPTION (OPEN_CONFIGURATOR_TCL_WRAPPER "Build openCONFIGURATOR TCL/TK wrapper library." OFF)
ELSE()
	OPTION (OPEN_CONFIGURATOR_NET_WRAPPER "Build openCONFIGURATOR C# wrapper library." OFF)
#	OPTION (OPEN_CONFIGURATOR_TCL_WRAPPER "Build openCONFIGURATOR TCL/TK wrapper library." OFF)
ENDIF(WIN32)
OPTION (OPEN_CONFIGURATOR_DOCUMENTATION "Build openCONFIGURATOR source code documentation." OFF)

###############################################################################
# Add a target to generate API documentation with Doxygen
###############################################################################
IF(OPEN_CONFIGURATOR_DOCUMENTATION)
	FIND_PACKAGE(Doxygen REQUIRED)
	IF(DOXYGEN_FOUND)
		CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/resources/openCONFIGURATOR.doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
		ADD_CUSTOM_TARGET(
			DOCUMENTATION ALL ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Generating API documentation with Doxygen" VERBATIM
		)
	ENDIF(DOXYGEN_FOUND)
ENDIF()

###############################################################################
# Add a target to always generate the openCONFIGURATOR library
###############################################################################
ADD_SUBDIRECTORY(openCONFIGURATOR)

###############################################################################
# Add a target to generate .NET wrapper
###############################################################################
IF(OPEN_CONFIGURATOR_NET_WRAPPER)
	ADD_SUBDIRECTORY(openCONFIGURATORCSHARPWrapper)
ENDIF()

###############################################################################
# Add a target to generate TCL wrapper
###############################################################################
#IF(OPEN_CONFIGURATOR_TCL_WRAPPER)
#	ADD_SUBDIRECTORY(openCONFIGURATORWrapper)
#ENDIF()

###############################################################################
# Add subprojects
###############################################################################
IF(WIN32)
	IF(OPEN_CONFIGURATOR_UNIT_TESTS)
		ADD_SUBDIRECTORY(openCONFIGURATORTest)
	ENDIF()
	IF(OPEN_CONFIGURATOR_NET_WRAPPER AND OPEN_CONFIGURATOR_NET_EXE)
		CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/BR.FS.CdcBuilder/BR.FS.CdcBuilder.csproj ${CMAKE_SOURCE_DIR}/BR.FS.CdcBuilder/BR.FS.CdcBuilder_cmake.csproj)
		INCLUDE_EXTERNAL_MSPROJECT(BR.FS.CdcBuilder ${CMAKE_SOURCE_DIR}/BR.FS.CdcBuilder/BR.FS.CdcBuilder_cmake.csproj
			TYPE BB3CF1A9-E8A2-40A9-B628-6983D6C5724E
			PLATFORM "Any CPU"
			openCONFIGURATORcsharpWRAPPER)
	ENDIF()
ENDIF(WIN32)	
