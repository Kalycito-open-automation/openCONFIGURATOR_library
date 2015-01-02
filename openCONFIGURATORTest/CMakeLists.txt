################################################################################
#
# Project: openCONFIGURATOR
#
# (c) Bernecker + Rainer Industrie-Elektronik Ges.m.b.H.
#     B&R Strasse 1, A-5142 Eggelsberg
#     www.br-automation.com
#
# Description: cmake file for the openCONFIGURATOR library CPPUNIT tests
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

PROJECT ( OPENCONFIGURATOR_TEST )


###############################################################################
# Add additional compiler flags
###############################################################################


IF(MSVC10)
	FIND_LIBRARY(CPPUNIT NAMES cppunit.lib PATHS ${CMAKE_SOURCE_DIR}/cppunit/lib/vs10/)
	FIND_LIBRARY(CPPUNITD NAMES cppunitd.lib PATHS ${CMAKE_SOURCE_DIR}/cppunit/lib/vs10/)
ENDIF()
	IF(MSVC11)
	FIND_LIBRARY(CPPUNIT NAMES cppunit.lib PATHS ${CMAKE_SOURCE_DIR}/cppunit/lib/vs11/)
	FIND_LIBRARY(CPPUNITD NAMES cppunitd.lib PATHS ${CMAKE_SOURCE_DIR}/cppunit/lib/vs11/)
ENDIF()
	
FIND_LIBRARY(LIBXML2 NAMES libxml2.lib PATHS ${CMAKE_SOURCE_DIR}/libxml2/lib)

### Find Boost Libraries BEGIN ###
SET(Boost_DEBUG TRUE)
SET(Boost_USE_STATIC_RUNTIME OFF)
FIND_PACKAGE(Boost 1.54.0 COMPONENTS log date_time filesystem system thread REQUIRED)
# Necessary for Auto-Linking under VS, ignored under Unix
ADD_DEFINITIONS(/DBOOST_ALL_DYN_LINK)
### Find Boost Libraries END ###

###############################################################################
# Set general sources and header of openCONF library
###############################################################################
FILE ( GLOB_RECURSE LIB_SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp" )
FILE ( GLOB_RECURSE LIB_HEADERS "${PROJECT_SOURCE_DIR}/Include/*.h" )
FILE ( GLOB_RECURSE OPENCONF_LIB "${CMAKE_SOURCE_DIR}/openCONFIGURATOR/src/*.cpp")
FILE ( GLOB_RECURSE OPENCONF_PROJECT_UPGRADE_LIB "${CMAKE_SOURCE_DIR}/openCONFIGURATOR_ProjectUpgrade/LibProjectUpgrade/src/*.cpp")

SOURCE_GROUP(openCONFIGURATOR_src FILES ${OPENCONF_LIB})
SOURCE_GROUP(openCONFIGURATOR_Project_Upgrade_src FILES ${OPENCONF_PROJECT_UPGRADE_LIB})

###############################################################################
# Set library include directories
###############################################################################
INCLUDE_DIRECTORIES(
	${CMAKE_SOURCE_DIR}/cppunit/include
	${CMAKE_SOURCE_DIR}/libxml2/include 
	${CMAKE_SOURCE_DIR}/libxml2/iconv/include
	${CMAKE_SOURCE_DIR}/openCONFIGURATOR/Include
	${CMAKE_SOURCE_DIR}/openCONFIGURATOR_ProjectUpgrade/LibProjectUpgrade/include
	${Boost_INCLUDE_DIRS}
)

###############################################################################
# Link library directories
###############################################################################
LINK_DIRECTORIES(
${CMAKE_SOURCE_DIR}/cppunit/lib
${CMAKE_SOURCE_DIR}/libxml2/lib
${Boost_LIBRARY_DIRS}
)

###############################################################################
# Create openCONFIGURATOR test executable 
###############################################################################
ADD_EXECUTABLE(openCONFIGURATORTest ${LIB_SOURCES} ${LIB_HEADERS} ${OPENCONF_LIB} ${OPENCONF_PROJECT_UPGRADE_LIB})

###############################################################################
# Link openCONFIGURATOR test executable to libraries 
###############################################################################
TARGET_LINK_LIBRARIES(openCONFIGURATORTest debug ${CPPUNITD} ${LIBXML2})
TARGET_LINK_LIBRARIES(openCONFIGURATORTest optimized ${CPPUNIT} ${LIBXML2})

###############################################################################
# Copy unit test dependency and execute the tests
###############################################################################

ADD_CUSTOM_COMMAND(TARGET openCONFIGURATORTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${Boost_LIBRARY_DIRS}"
        "${PROJECT_BINARY_DIR}/$<CONFIGURATION>")


ADD_CUSTOM_COMMAND(TARGET openCONFIGURATORTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/resources/"
        "${PROJECT_BINARY_DIR}/$<CONFIGURATION>/resources")

ADD_CUSTOM_COMMAND(TARGET openCONFIGURATORTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/resources/"
        "${PROJECT_BINARY_DIR}/resources")
		
ADD_CUSTOM_COMMAND(TARGET openCONFIGURATORTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/resources/lib"
        "${PROJECT_BINARY_DIR}/$<CONFIGURATION>")

ADD_CUSTOM_COMMAND(TARGET openCONFIGURATORTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/openCONFIGURATORTest/test_resources"
        "${PROJECT_BINARY_DIR}/")

ADD_CUSTOM_COMMAND(TARGET openCONFIGURATORTest POST_BUILD
                   COMMAND "$(TargetDir)$(TargetFileName)" > openCONFIGURATOR_unit_test_output.txt
                   COMMENT "Running openCONFIGURATOR CPPUNIT tests")
				   

				   




