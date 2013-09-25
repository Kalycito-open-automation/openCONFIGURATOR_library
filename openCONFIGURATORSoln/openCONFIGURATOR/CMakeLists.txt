################################################################################
#
# Project: openCONFIGURATOR
#
# (c) Bernecker + Rainer Industrie-Elektronik Ges.m.b.H.
#     B&R Strasse 1, A-5142 Eggelsberg
#     www.br-automation.com
#
# Description: cmake file for the openCONFIGURATOR library
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

PROJECT ( OPENCONFIGURATOR_LIB )


###############################################################################
# Add additional compiler flags
###############################################################################
IF (UNIX)
	FIND_PACKAGE(LibXml2 REQUIRED)
	ADD_DEFINITIONS(${LIBXML2_DEFINITIONS})
	IF(${LIBXML2_VERSION_STRING} STRLESS "2.7.2")
		MESSAGE(FATAL_ERROR "Libxml2 minimum required version '2.7.2' not found on the system.")
	ENDIF()
ELSEIF (WIN32)
	### Necessary for Boost.Log, only implemented under Windows ###
	SET(Boost_DEBUG TRUE)
	SET(Boost_USE_STATIC_RUNTIME OFF)
	FIND_PACKAGE(Boost 1.54.0 COMPONENTS log date_time filesystem system thread REQUIRED)
	# Necessary for Auto-Linking under VS
	ADD_DEFINITIONS(/DBOOST_ALL_DYN_LINK)
	######
	FIND_LIBRARY(LIBXML2 NAMES libxml2.lib PATHS ${CMAKE_SOURCE_DIR}/libxml2/lib)
ENDIF (UNIX)

###############################################################################
# Set general sources and header of openCONF library
###############################################################################
FILE ( GLOB_RECURSE LIB_SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp" )
FILE ( GLOB_RECURSE LIB_HEADERS "${PROJECT_SOURCE_DIR}/Include/*.h" )

###############################################################################
# Set library include directories
###############################################################################
IF(WIN32)
	INCLUDE_DIRECTORIES(
		${CMAKE_SOURCE_DIR}/libxml2/include 
		${CMAKE_SOURCE_DIR}/libxml2/iconv/include
		${Boost_INCLUDE_DIRS}
	)
ELSE(WIN32)
	INCLUDE_DIRECTORIES(${LIBXML2_INCLUDE_DIR})
ENDIF(WIN32)

###############################################################################
# Link library directories
###############################################################################
IF(WIN32)
	LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/libxml2/lib ${Boost_LIBRARY_DIRS})
ENDIF(WIN32)

###############################################################################
# If DLL option is chosen library type is shared
###############################################################################
SET(LIBRARY_TYPE SHARED)

###############################################################################
# Link libxml2 and boost to the openCONFIGURATOR library
###############################################################################
UNSET(ADDITIONAL_DEPENDENCIES)
IF (WIN32)
	SET(ADDITIONAL_DEPENDENCIES ${ADDITIONAL_DEPENDENCIES} ${LIBXML2})
ELSE(WIN32)
	SET(ADDITIONAL_DEPENDENCIES ${ADDITIONAL_DEPENDENCIES} ${LIBXML2_LIBRARIES})
ENDIF(WIN32)

###############################################################################
# Create openCONF library 
###############################################################################
ADD_LIBRARY(openCONFIGURATOR ${LIBRARY_TYPE} ${LIB_SOURCES} ${LIB_HEADERS})

###############################################################################
# Remove lib prefix under Linux
###############################################################################
IF(UNIX)
SET_TARGET_PROPERTIES(openCONFIGURATOR PROPERTIES PREFIX "")
ENDIF()

###############################################################################
# Create openCONF library 
###############################################################################
TARGET_LINK_LIBRARIES(openCONFIGURATOR ${ADDITIONAL_DEPENDENCIES})

###############################################################################
# Install openCONF library to build dir
###############################################################################
IF(WIN32)
FILE ( GLOB_RECURSE LIBS "${CMAKE_SOURCE_DIR}/resources/lib/*.dll" )
INSTALL(
FILES ${LIBS} DESTINATION ${CMAKE_SOURCE_DIR}/bin/
)
ENDIF()
FILE ( GLOB_RECURSE SCHEMA "${CMAKE_SOURCE_DIR}/resources/xddschema/*.xsd" )
INSTALL(
FILES ${SCHEMA} DESTINATION ${CMAKE_SOURCE_DIR}/bin/resources/xddschema
)
INSTALL(
TARGETS openCONFIGURATOR 
RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin
LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/bin
)




