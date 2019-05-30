cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME "display_manager")

project(${PROJECT_NAME})

################################################################################
# Options
################################################################################

OPTION(WITH_DOC "build with documenation" OFF)

message(STATUS)
message(STATUS "${PROJECT_NAME} Configuration:")
message(STATUS "CMAKE_BUILD_TYPE              = ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_INSTALL_PREFIX          = ${CMAKE_INSTALL_PREFIX}")
message(STATUS)
message(STATUS "WITH_DOC                      = ${WITH_DOC}")
message(STATUS)
message(STATUS "XT_LOG_INCLUDE_PATH           = ${XT_LOG_INCLUDE_PATH}")
message(STATUS)

################################################################################
# Compiler flags
################################################################################

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall -Wno-return-local-addr")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

################################################################################
# Definitions
################################################################################

################################################################################
# Includes
################################################################################

include_directories(
	${XT_LOG_INCLUDE_PATH}
)

################################################################################
# Link directories
################################################################################

################################################################################
# Sources
################################################################################

add_subdirectory(src)

################################################################################
# Docs
################################################################################

find_package(Doxygen)

if(WITH_DOC AND NOT DOXYGEN_FOUND)
	message(FATAL_ERROR "Doxygen not found. Required to generate docs.")
else()
	configure_file(
		${CMAKE_CURRENT_SOURCE_DIR}/doxygen.cfg
		${CMAKE_CURRENT_BINARY_DIR}/doxygen.cfg
		@ONLY
	)
	
	add_custom_target(
		doc ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxygen.cfg
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating API documentation with Doxygen" VERBATIM
	)
endif()