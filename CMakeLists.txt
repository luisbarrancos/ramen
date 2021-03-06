######################################################################
# Copyright (C) 2013 Esteban Tovagliari. All Rights Reserved.        #
# Licensed under the terms of the CDDL License.                      #
# See CDDL_LICENSE.txt for a copy of the license.                    #
######################################################################

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0 FATAL_ERROR)

######################################################
# project

PROJECT(ramen)

######################################################
# cmake stuff

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/Modules)

#disable in source builds
IF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	MESSAGE(FATAL_ERROR "CMake generation for Ramen is not allowed within the source directory!")
ENDIF()

# Set the default built type
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
ENDIF()

######################################################
# version

SET(RAMEN_VERSION_MAJOR "1")
SET(RAMEN_VERSION_MINOR "0")
SET(RAMEN_VERSION_PATCH "0")
SET(RAMEN_VERSION ${RAMEN_VERSION_MAJOR}.${RAMEN_VERSION_MINOR}.${RAMEN_VERSION_PATCH})

######################################################
# platform specific

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    # OSX
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    ADD_DEFINITIONS(-pthread)
    FIND_PACKAGE(Threads REQUIRED)
    SET(RAMEN_ALL_LIBS ${CMAKE_THREAD_LIBS_INIT})
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    SET(RAMEN_ALL_LIBS Userenv)
    MESSAGE(FATAL_ERROR "Windows not supported yet")
ELSE()
    MESSAGE(FATAL_ERROR "Platform ${CMAKE_SYSTEM_NAME} not supported yet")
ENDIF()

####################################################################
# compiler flags

IF(CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -std=c++11 -fvisibility=hidden -fvisibility-inlines-hidden -Wno-deprecated-declarations")
ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe")
ELSE()
    MESSAGE(FATAL_ERROR "unknown compiler is not supported yet")
ENDIF()

####################################################################
# dependencies

# GL
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLEW REQUIRED)

# boost
SET(Boost_ADDITIONAL_VERSIONS "1.61.0")
SET(BOOST_NEEDED_LIBS  filesystem thread system date_time)

FIND_PACKAGE(Boost COMPONENTS ${BOOST_NEEDED_LIBS} REQUIRED)

FIND_PACKAGE(TBB REQUIRED)

# Qt
SET(QT_MT_REQUIRED false)
SET(QT_MIN_VERSION "4.8.0")
FIND_PACKAGE(Qt4 REQUIRED)
SET(NEEDED_QT_INCLUDES ${QT_INCLUDE_DIR} ${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR} ${QT_QTOPENGL_INCLUDE_DIR})
SET(NEEDED_QT_LIBS	${QT_QTCORE_LIBRARIES} ${QT_QTGUI_LIBRARIES} ${QT_QTOPENGL_LIBRARIES})

FIND_PACKAGE(Half REQUIRED)
FIND_PACKAGE(Imath REQUIRED)
FIND_PACKAGE(OpenEXR REQUIRED)

FIND_PACKAGE(JPEG REQUIRED)
FIND_PACKAGE(PNG REQUIRED)
FIND_PACKAGE(TIFF REQUIRED)
FIND_PACKAGE(OpenImageIO REQUIRED)

FIND_PACKAGE(OpenColorIO REQUIRED)

#FIND_PACKAGE(Agg REQUIRED)
FIND_PACKAGE(ZLIB REQUIRED)

######################################################
# optional dependencies

######################################################
# definitions

ADD_DEFINITIONS(-DBOOST_FILESYSTEM_VERSION=3)
ADD_DEFINITIONS(-DBOOST_FILESYSTEM_NO_DEPRECATED)

ADD_DEFINITIONS(-DQT_NO_KEYWORDS)
SET(MOC_EXTRA_ARGS )

######################################################
# includes

INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR}
                     ${PROJECT_SOURCE_DIR}/ramen/

                     ${PROJECT_SOURCE_DIR}/extern/antigrain-2.4/include
                     ${PROJECT_SOURCE_DIR}/extern/yaml-cpp-0.3.0/include

                     ${NEEDED_QT_INCLUDES}

                     ${Boost_INCLUDE_DIR}
                     ${TBB_INCLUDE_DIRS}

                     ${OPENGL_INCLUDE_DIR}
                     ${GLEW_INCLUDE_DIRS}

                     ${HALF_INCLUDE_DIRS}
                     ${IMATH_INCLUDE_DIRS}
                     ${OPENEXR_INCLUDE_DIR}
                     ${OPENEXR_INCLUDE_DIR}/OpenEXR/

                     ${JPEG_INCLUDE_DIR}
                     ${PNG_INCLUDE_DIR}
                     ${TIFF_INCLUDE_DIR}
                     ${OPENIMAGEIO_INCLUDE_DIRS}

                     ${OPENCOLORIO_INCLUDE_DIRS}

                     ${ZLIB_INCLUDE_DIRS})

######################################################
# libraries

SET(RAMEN_ALL_LIBS	${NEEDED_QT_LIBS}

                    ${OPENCOLORIO_LIBRARIES}
                    ${OPENIMAGEIO_LIBRARIES}

                    ${HALF_LIBRARIES}
                    ${IMATH_LIBRARIES}
                    ${OPENEXR_LIBRARIES}

                    ${JPEG_LIBRARIES}
                    ${PNG_LIBRARIES}
                    ${TIFF_LIBRARIES}

                    ${GLEW_LIBRARIES}
                    ${OPENGL_LIBRARIES}

                    ${Boost_LIBRARIES}
                    ${TBB_LIBRARIES}
                    ${ZLIB_LIBRARIES}

                    ${RAMEN_ALL_LIBS})

######################################################
# ramen

ADD_SUBDIRECTORY(ramen)

######################################################
# docs

FIND_PACKAGE(Doxygen)

IF(DOXYGEN_FOUND)
	CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/docs/Doxyfile.in ${PROJECT_BINARY_DIR}/Doxyfile @ONLY)
        ADD_CUSTOM_TARGET(doc ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
                            WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
                            COMMENT "Generating API documentation with Doxygen" VERBATIM)
ENDIF()
