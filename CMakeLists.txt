# mongoDBTest file for quick C++ project creation
# By Robert J. Gebis (oxoocoffee)

cmake_minimum_required(VERSION 3.7.0 FATAL_ERROR)

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( STATUS "In-source builds not allowed. Please make build directory")
    message( STATUS "and run CMake from there")
    message( STATUS "You may need to remove CMakeCache.txt and CmakeFiles/" )
    message( FATAL_ERROR )
endif()

#-----------------------------------------------------------------------
set(PROJ_NAME     "mongoDBTest")
set(COMPANY_NAME  "XYZ LLC")

# The version number is used.
set(APP_VERSION_MAJOR      0)
set(APP_VERSION_MINOR      1)
set(APP_VERSION_PATCH      1)
# This one will be set with git revision
set(APP_VERSION_REVISION   0)

# We set temporary version here since project() must be called
# first to initailize cmake subsystem but we will update version later when we test for git repo version
# In this project we only enabling C++ project (CXX). For C based project replace CXX with C or you can mix CXX C
project(${PROJ_NAME} VERSION "${APP_VERSION_MAJOR}.${APP_VERSION_MINOR}.${APP_VERSION_PATCH}.0" LANGUAGES CXX)

#-----------------------------------------------------------------------
add_definitions(-DQT_NO_KEYWORDS)

# Using cmake builtin CheckIncludeFileCXX micro tp
include(CheckIncludeFileCXX)

# Set this variables since they are not used so cmake will not give you warning like
# CMake Warning:
#   Manually-specified variables were not used by the project:
#
#    CMAKE_C_COMPILER
#    QT_QMAKE_EXECUTABLE

set(ignoreMe "${CMAKE_C_COMPILER} ${QT_QMAKE_EXECUTABLE}")

#-----------------------------------------------------------------------
# Some User defined Variables and default values. Also visible in Qt Creator UI
option(DEFINE_LIST_SOURCES
    "Print source files" OFF)               # Disabled by default

option(DEFINE_APP_TRACE_ENABLE
    "Enable Trace Calls" OFF)               # Disabled by default

# When enabled run: make doc
option(DEFINE_DOXYGEN_ENABLE
    "Enable Doxygen documentation" OFF)     # Disabled by default

option(DEFINE_DOXYGEN_INSTALL
    "Install Doxygen documentation" OFF)    # Disabled by default

option(DEFINE_STACK_PROTECTOR
    "Build with stact protector" OFF)       # Disabled by default

option(DEFINE_POPPE_STACK_TRACE
    "Enable Poppe Stack Trace" OFF)         # Disabled by default

option(DEFINE_POPPE_PERF_CONTING
    "Enable Poppe Performant Counters" OFF) # Disabled by default

if(DEFINE_DOXYGEN_INSTALL)
    set(DEFINE_DOXYGEN_ENABLE ON)
endif(DEFINE_DOXYGEN_INSTALL)

#-----------------------------------------------------------------------

if(UNIX)
    set(PATH_SEPARATOR         '/')
else(UNIX)
    set(PATH_SEPARATOR         '\\')
endif(UNIX)

macro(list_files list)
    if (DEFINE_LIST_SOURCES )
        foreach(infileName ${list})
            message(STATUS " ${infileName}")
        endforeach(infileName)
    endif(DEFINE_LIST_SOURCES)
endmacro()

#-----------------------------------------------------------------------
# Enable C++11 standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Always be pedantic about your code. Compilation warrnings should be treated as ERRORS!!!! Fix it...
set(CMAKE_C_FLAGS     "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra -Wunused-parameter -Wformat-security -Wunreachable-code -fdata-sections -ffunction-sections -Wcast-align")
set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -Wunused-parameter -Wformat-security -Wunreachable-code -fdata-sections -ffunction-sections -Wcast-align")

if(DEFINE_STACK_PROTECTOR)
    set(CMAKE_C_FLAGS     "${CMAKE_C_FLAGS} -fstack-protector-all")
    set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -fstack-protector-all")
endif(DEFINE_STACK_PROTECTOR)

# If LANGUAGES C enable above in project()
set(CMAKE_C_FLAGS_DEBUG   "-O0" CACHE INTERNAL "C debug compiler flags")
# If LANGUAGES CXX enable above in project()
set(CMAKE_CXX_FLAGS_DEBUG "-O0" CACHE INTERNAL "C++ debug compiler flags")
# If ASM files included in project
set(CMAKE_ASM_FLAGS_DEBUG "-g"  CACHE INTERNAL "ASM debug compiler flags")

# If LANGUAGES C enable above in project()
set(CMAKE_C_FLAGS_RELEASE   "-Os -flto" CACHE INTERNAL "C release compiler flags")
# If LANGUAGES CXX enable above in project()
set(CMAKE_CXX_FLAGS_RELEASE "-Os -flto" CACHE INTERNAL "C++ release compiler flags")
# If ASM files included in project
set(CMAKE_ASM_FLAGS_RELEASE ""          CACHE INTERNAL "ASM release compiler flags")

#-----------------------------------------------------------------------
# If cmake -DCMAKE_BUILD_TYPE=[DEBUG|RELEASE] ..
if( NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

string( TOUPPER ${CMAKE_BUILD_TYPE} DEFINE_DEBUG_BUILD)

# AB_BUILD_TYPE is used to flag source code DEBUG definition
if(DEFINE_DEBUG_BUILD STREQUAL DEBUG)
    set(DEFINE_DEBUG_BUILD "1")
    add_definitions(-DDEBUG)    # This is set to output in buildVersion.h.in
    if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
    endif()
else()
    set(DEFINE_DEBUG_BUILD "0") # This is set to output in buildVersion.h.in
    add_definitions(-DRELEASE)
endif()

#-----------------------------------------------------------------------
# Add user created cmake modules. Like FindGit in this case. This module
# replaces cmake own Git module which is missing some features
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

# Enable find_package local cmake modules extra output
set(LIBFIND_DEBUG ON)

find_package(LibBson)

if(LIBBSON_FOUND)
    include_directories( ${LIBBSON_INCLUDE_DIRS} )
else(LIBBSON_FOUND)
    message(STATUS "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
    # git clone https://github.com/mongodb/libbson.git -b 1.7.0 --depth 1
    #
    if(CMAKE_CROSSCOMPILING)
        message(FATAL_ERROR "You need to cross compile libbson library first")
    else(CMAKE_CROSSCOMPILING)
        message(FATAL_ERROR "You need to compile libbson library first")
    endif(CMAKE_CROSSCOMPILING)
endif(LIBBSON_FOUND)

find_package(LibBsonCXX)

if(LIBBSONCXX_FOUND)
    include_directories( ${LIBBSONCXX_INCLUDE_DIRS} )
else(LIBBSONCXX_FOUND)
    message(STATUS "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
    # git clone https://github.com/mongodb/mongo-cxx-driver.git --branch releases/stable --depth 1
    #
    if(CMAKE_CROSSCOMPILING)
        message(FATAL_ERROR "You need to cross compile mongocxx library first")
    else(CMAKE_CROSSCOMPILING)
        message(FATAL_ERROR "You need to compile mongocxx library first")
    endif(CMAKE_CROSSCOMPILING)
endif(LIBBSONCXX_FOUND)

find_package(LibMongoCXX)

if(LIBMONGOCXX_FOUND)
    include_directories( ${LIBMONGOCXX_INCLUDE_DIRS} )
else(LIBMONGOCXX_FOUND)
    message(STATUS "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
    # git clone https://github.com/mongodb/mongo-cxx-driver.git --branch releases/stable --depth 1
    #
    if(CMAKE_CROSSCOMPILING)
        message(FATAL_ERROR "You need to cross compile mongocxx library first")
    else(CMAKE_CROSSCOMPILING)
        message(FATAL_ERROR "You need to compile mongocxx library first")
    endif(CMAKE_CROSSCOMPILING)
endif(LIBMONGOCXX_FOUND)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/.git)
    find_package(Git)
    if(GIT_FOUND)
        find_program(GIT_EXECUTABLE git
                     DOC "git command line client")

        message( STATUS "Building from GIT")
        Git_WC_INFO(${PROJECT_SOURCE_DIR} Project)
        message( STATUS "Git Tools : ${GIT_VERSION_STRING}")
        message( STATUS "Revision  : ${Project_WC_REVISION}")
        message( STATUS "Creator   : ${Project_WC_LAST_CHANGED_AUTHOR}")
        message( STATUS "Date      : ${Project_WC_LAST_CHANGED_DATE}")

        #Git_WC_LOG(${PROJECT_SOURCE_DIR} Project)
        #message( STATUS "Log:  ${Project_LAST_CHANGED_LOG}")

        set( APP_VERSION_REVISION  ${Project_WC_REVISION})
    endif(GIT_FOUND)
elseif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/.svn)
    find_package(Subversion)
    if(SUBVERSION_FOUND)
        find_program(SVN_EXECUTABLE svn
                     DOC "subversion command line client")

        message( STATUS "Building from SVN")

        Subversion_WC_INFO(${PROJECT_SOURCE_DIR} Project)
        message( STATUS "SVN tools : ${SVN_VERSION_STRING}")
        message( STATUS "Revision  : ${Project_WC_REVISION}")
        message( STATUS "Creator   : ${Project_WC_LAST_CHANGED_AUTHOR}")
        message( STATUS "Date      : ${Project_WC_LAST_CHANGED_DATE}")

        #Subversion_WC_LOG( ${PROJECT_SOURCE_DIR} Project)
        #message( STATUS "Log:  ${Project_LAST_CHANGED_LOG}")

        set( APP_VERSION_REVISION  ${Project_WC_REVISION})
    endif(SUBVERSION_FOUND)
endif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/.git)

set(APP_BUILD_VERSION "${APP_VERSION_MAJOR}.${APP_VERSION_MINOR}.${APP_VERSION_PATCH}.${APP_VERSION_REVISION}")

#-----------------------------------------------------------------------
if(DEFINE_DOXYGEN_ENABLE)
    # add a target to generate API documentation with Doxygen
    # If found you can run: make
    find_package(Doxygen)
    if(NOT DOXYGEN_FOUND)
        message(STATUS "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
        if(APPLE)
            message(STATUS "brew install doxygen graphviz")
        else(UNIX)
            message(STATUS "sudo apt-get install doxygen-gui graphviz")
        endif()
        message(STATUS "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
        message(FATAL_ERROR " You need to install doxygen utility")
    endif(NOT DOXYGEN_FOUND)
endif(DEFINE_DOXYGEN_ENABLE)

if(DEFINE_POPPE_PERF_CONTING OR DEFINE_POPPE_STACK_TRACE)
    include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/include )
endif(DEFINE_POPPE_PERF_CONTING OR DEFINE_POPPE_STACK_TRACE)

#-----------------------------------------------------------------------
set(APP_CONFIG_BIN_PATH    ${CMAKE_CURRENT_BINARY_DIR}/bin/${CROSS_COMPILE_ARCH})
set(APP_CONFIG_SRC_PATH    ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(APP_CONFIG_3RD_PATH    ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty)
set(APP_CONFIG_CMAKE_PATH  ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(APP_CONFIG_BIN_INCLUDE ${APP_CONFIG_BIN_PATH}/include)

#-----------------------------------------------------------------------
message(STATUS "=========================================")

message(STATUS "  CMake Version: ${CMAKE_VERSION}")
message(STATUS "  Build Version  ${APP_BUILD_VERSION}")
message(STATUS "  System Version ${CMAKE_SYSTEM_VERSION}")
message(STATUS "  Force RPATH Install=${DEFINE_INSTALL_RPATH_ENABLE}")
message(STATUS " -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
message(STATUS " -DDEFINE_STACK_PROTECTOR=${DEFINE_STACK_PROTECTOR}")
message(STATUS " -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
message(STATUS " -DDEFINE_LIST_SOURCES=${DEFINE_LIST_SOURCES}")
message(STATUS " -DDEFINE_DOXYGEN_ENABLE=${DEFINE_DOXYGEN_ENABLE}")
message(STATUS " -DDEFINE_DOXYGEN_INSTALL=${DEFINE_DOXYGEN_INSTALL}")
message(STATUS " -DDEFINE_POPPE_STACK_TRACE=${DEFINE_POPPE_STACK_TRACE}")
message(STATUS " -DDEFINE_POPPE_PERF_CONTING=${DEFINE_POPPE_PERF_CONTING}")

message(STATUS "=========================================")

#-----------------------------------------------------------------------
# Remap above values which will be used to generate header file
if(DEFINE_APP_TRACE_ENABLE)
    set(DEFINE_APP_TRACE_ENABLE 1)
else()
    set(DEFINE_APP_TRACE_ENABLE 0)
endif()

if(DEFINE_POPPE_STACK_TRACE)
    set(DEFINE_POPPE_STACK_TRACE 1)
else()
    set(DEFINE_POPPE_STACK_TRACE 0)
endif()

if(DEFINE_POPPE_PERF_CONTING)
    set(DEFINE_POPPE_PERF_CONTING 1)
else()
    set(DEFINE_POPPE_PERF_CONTING 0)
endif()

#-----------------------------------------------------------------------
# process any "in" files
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/inConfig/buildVersion.h.in
    ${APP_CONFIG_SRC_PATH}/buildVersion.h
    IMMEDIATE @ONLY)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/inConfig/app_profile.sh.in
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/inConfig/app_profile.sh
    IMMEDIATE @ONLY)

#-----------------------------------------------------------------------
# These section enables QT Creator support
FILE(GLOB_RECURSE InFiles "${CMAKE_CURRENT_SOURCE_DIR}/cmake/inConfig/*.in")

add_custom_target(in-files SOURCES ${InFiles})

FILE(GLOB_RECURSE moduleFiles "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/*")

add_custom_target(modules-local SOURCES ${moduleFiles})

#-----------------------------------------------------------------------
# if cmake -DCMAKE_INSTALL_PREFIX=path ..
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "/usr/local"
        CACHE PATH "${PROJ_NAME} install prefix" FORCE)
endif()

message(STATUS "Building ${COMPANY_NAME} - Uninstaller")

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake/inConfig/cmake_uninstall.cmake)

if(DOXYGEN_FOUND)
    message(STATUS "Building ${COMPANY_NAME} - Doxygen documentation")

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/scripts/doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/doxyfile @ONLY)

    # Remove ALL if you do not want to auto create
    # doxygen on make. If you remove ALL then you will
    # have to do "make doxygen"
    #add_custom_target(doxygen ALL

    if(DEFINE_DOXYGEN_INSTALL)
        set(BUILD_ON_MAKE ALL)
    endif(DEFINE_DOXYGEN_INSTALL)

    add_custom_target(doxygen ${BUILD_ON_MAKE}
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM)

endif(DOXYGEN_FOUND)

if(DEFINE_DOXYGEN_ENABLE AND DEFINE_DOXYGEN_INSTALL)
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doxygen-html/ DESTINATION share/${COMPANY_NAME}/${PROJ_NAME})
endif(DEFINE_DOXYGEN_ENABLE AND DEFINE_DOXYGEN_INSTALL)

#-----------------------------------------------------------------------
set(SRC src)

# Targets to be build

message(STATUS "=====================================================")

add_subdirectory(${SRC}/mongoDBTest)

#-----------------------------------------------------------------------
# CPack
include(InstallRequiredSystemLibraries)

set(CPACK_SET_DESTDIR true)
set(CPACK_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_VENDOR "${COMPANY_NAME}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Useless Tool. Target ${CMAKE_SYSTEM_VERSION}")

set(CPACK_PACKAGE_VERSION_MAJOR "${APP_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${APP_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${APP_VERSION_PATCH}")
set(CPACK_PACKAGE_VERSION "${APP_BUILD_VERSION}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Copyright.txt")

set(CPACK_DEBIAN_FILE_NAME "${PROJ_NAME}-${APP_BUILD_VERSION}-${CMAKE_SYSTEM_PROCESSOR}.deb")
set(CPACK_DEBIAN_PACKAGE_SECTION "admin")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "required")
set(CPACK_DEBIAN_COMPRESSION_TYPE "xz")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "oxoocoffee")

set(CPACK_DEBIAN_PACKAGE_CONTROL_STRICT_PERMISSION TRUE)

include(CPack)
