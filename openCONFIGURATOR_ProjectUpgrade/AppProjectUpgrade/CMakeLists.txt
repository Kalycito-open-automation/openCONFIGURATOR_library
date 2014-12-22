################################################################################
#
# Main CMake file for building console application that is used to upgrade pre 
# v1.4.0 openCONFIGURATOR projects to v1.4.0
#
# Copyright (c) 2014, Kalycito Infotech Private Limited
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the copyright holders nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
# ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
################################################################################

PROJECT(AppProjectUpgrade)

CMAKE_MINIMUM_REQUIRED (VERSION 2.8.8)

###############################################################################
# Add additional compiler flags
###############################################################################
IF(UNIX)
	FIND_PACKAGE(LibXml2 REQUIRED)
	ADD_DEFINITIONS(${LIBXML2_DEFINITIONS})
	IF(${LIBXML2_VERSION_STRING} STRLESS "2.7.2")
		MESSAGE(FATAL_ERROR "Libxml2 minimum required version '2.7.2' not found on the system.")
	ENDIF()
ELSEIF(WIN32)
     FIND_LIBRARY(LIBXML2 NAMES libxml2.lib PATHS ${CMAKE_SOURCE_DIR}/libxml2/lib)
ENDIF(UNIX)

### Find Boost Libraries BEGIN ###
SET(Boost_DEBUG TRUE)
SET(Boost_USE_STATIC_RUNTIME OFF)
FIND_PACKAGE(Boost 1.54.0 COMPONENTS log log_setup date_time filesystem system thread REQUIRED)
# Necessary for Auto-Linking under VS, ignored under Unix
ADD_DEFINITIONS(/DBOOST_ALL_DYN_LINK)
### Find Boost Libraries END ###

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../LibProjectUpgrade/include)
IF(WIN32)
	INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/libxml2/include
						${CMAKE_SOURCE_DIR}/libxml2/iconv/include
						${Boost_INCLUDE_DIRS})
ELSE(WIN32)
	INCLUDE_DIRECTORIES(${LIBXML2_INCLUDE_DIR})
	# This will cause the Boost-Headers to be included via GNU GCC's "-isystem"
	# option and hence will treat the Boost-Headers as system headers
	# (and suppress warnings for them).
	INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})
ENDIF(WIN32)

###############################################################################
# Link library directories
###############################################################################
IF(WIN32)
	ADD_DEFINITIONS(-DWIN32)
	LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/libxml2/lib ${Boost_LIBRARY_DIRS})
ENDIF(WIN32)

###############################################################################
# Link libxml2 and boost to the project_upgrade_app
###############################################################################
UNSET(ADDITIONAL_DEPENDENCIES)
IF (WIN32)
	SET(ADDITIONAL_DEPENDENCIES ${ADDITIONAL_DEPENDENCIES} ${LIBXML2})
ELSE(WIN32)
	SET(ADDITIONAL_DEPENDENCIES ${ADDITIONAL_DEPENDENCIES} ${LIBXML2_LIBRARIES})
ENDIF(WIN32)


ADD_EXECUTABLE(project_upgrade_app ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp )
TARGET_LINK_LIBRARIES(project_upgrade_app project_upgrade)

INSTALL(TARGETS project_upgrade_app
		RUNTIME DESTINATION ${INSTALL_DIR}
		ARCHIVE DESTINATION ${INSTALL_DIR}
		)
