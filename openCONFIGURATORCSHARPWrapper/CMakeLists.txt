################################################################################
#
# Project: openCONFIGURATOR
#
# (c) Bernecker + Rainer Industrie-Elektronik Ges.m.b.H.
#     B&R Strasse 1, A-5142 Eggelsberg
#     www.br-automation.com
#
# Description: cmake file for the openCONFIGURATOR TCL wrapper
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
PROJECT (OPENCONFIGURATOR_CSHARPWRAPPER)

###############################################################################
# Ensure SWIG installation
###############################################################################
FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

###############################################################################
# Set wrapper include directories
###############################################################################
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

###############################################################################
# Set wrapper link directories
###############################################################################
LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/libxml2/lib)

###############################################################################
# Swig wrapper build options
###############################################################################
SET_SOURCE_FILES_PROPERTIES(openCONFIGURATORcsharp.i PROPERTIES CPLUSPLUS ON)
SWIG_ADD_MODULE(openCONFIGURATORcsharpWRAPPER csharp openCONFIGURATORcsharp.i)

###############################################################################
# Remove lib prefix under Linux
###############################################################################
IF(UNIX)
	SET_TARGET_PROPERTIES(openCONFIGURATORcsharpWRAPPER PROPERTIES PREFIX "")
ENDIF()

###############################################################################
# Create openCONF wrapper library 
###############################################################################
SWIG_LINK_LIBRARIES(openCONFIGURATORcsharpWRAPPER openCONFIGURATOR)

###############################################################################
# Install openCONF wrapper library to build dir
###############################################################################
INSTALL(
	TARGETS openCONFIGURATORcsharpWRAPPER
	ARCHIVE DESTINATION ${CMAKE_SOURCE_DIR}/bin
	LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/bin
)
