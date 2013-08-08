######################################################################
# Copyright (C) 2013 Esteban Tovagliari. All Rights Reserved.        #
# Licensed under the terms of the CDDL License.                      #
# See CDDL_LICENSE.txt for a copy of the license.                    #
######################################################################

CMAKE_MINIMUM_REQUIRED( VERSION 2.8.0 FATAL_ERROR)

######################################################
# project

PROJECT( ramen)

######################################################
# cmake stuff

SET( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/ramenlibs/cmake/Modules)

#disable in source builds
IF( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
        MESSAGE(FATAL_ERROR "CMake generation for Ramen is not allowed within the source directory!")
ENDIF()

# Set the default built type
IF( NOT CMAKE_BUILD_TYPE)
  SET( CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
ENDIF()

######################################################
# version

SET( RAMEN_VERSION_MAJOR "0")
SET( RAMEN_VERSION_MINOR "9")
SET( RAMEN_VERSION_PATCH "5")
SET( RAMEN_VERSION ${RAMEN_VERSION_MAJOR}.${RAMEN_VERSION_MINOR}.${RAMEN_VERSION_PATCH})

####################################################################
# options

SET( RAMEN_SIMD_TARGET "SSE4" CACHE STRING "SIMD target")
SET_PROPERTY( CACHE RAMEN_SIMD_TARGET PROPERTY STRINGS SSE2 SSE4 AVX AVX2)
SET( RAMEN_ISPC_EXTRA_FLAGS "" CACHE STRING "ispc extra flags")

######################################################
# platform specific

IF( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    # OSX
ELSEIF( ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    ADD_DEFINITIONS( -pthread)
    FIND_PACKAGE( Threads REQUIRED)
    SET( RAMEN_ALL_LIBS ${CMAKE_THREAD_LIBS_INIT})
ELSEIF( ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    SET( RAMEN_ALL_LIBS Userenv)
    MESSAGE( FATAL_ERROR "Windows not supported yet")
ELSE()
    MESSAGE( FATAL_ERROR "Platform ${CMAKE_SYSTEM_NAME} not supported yet")
ENDIF()

####################################################################
# compiler flags

IF( CMAKE_COMPILER_IS_GNUCXX)
    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -fvisibility=hidden -fvisibility-inlines-hidden")

    IF( RAMEN_SIMD_TARGET STREQUAL "SSE2")
        SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2")
    ELSEIF( RAMEN_SIMD_TARGET STREQUAL "SSE4")
        SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.2")
    ELSEIF( RAMEN_SIMD_TARGET STREQUAL "AVX")
        SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx")
    ELSEIF( RAMEN_SIMD_TARGET STREQUAL "AVX2")
        SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2")
    ENDIF()
ELSEIF( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe")
ELSE()
    MESSAGE( FATAL_ERROR "unknown compiler is not supported yet")
ENDIF()

####################################################################
# dependencies

# GL
FIND_PACKAGE( OpenGL REQUIRED)
FIND_PACKAGE( GLEW REQUIRED)

# boost
SET( Boost_ADDITIONAL_VERSIONS "1.53.0")
SET( BOOST_NEEDED_LIBS  filesystem
                        thread
                        system
                        date_time
                        )

FIND_PACKAGE( Boost COMPONENTS ${BOOST_NEEDED_LIBS} REQUIRED)

FIND_PACKAGE( TBB REQUIRED)

# Qt
SET( QT_MT_REQUIRED false)
SET( QT_MIN_VERSION "4.8.0")
FIND_PACKAGE( Qt4 REQUIRED)
SET( NEEDED_QT_INCLUDES ${QT_INCLUDE_DIR} ${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR} ${QT_QTOPENGL_INCLUDE_DIR})
SET( NEEDED_QT_LIBS	${QT_QTCORE_LIBRARIES} ${QT_QTGUI_LIBRARIES} ${QT_QTOPENGL_LIBRARIES})

FIND_PACKAGE( Half REQUIRED)
FIND_PACKAGE( Imath REQUIRED)
FIND_PACKAGE( OpenEXR REQUIRED)

FIND_PACKAGE( JPEG REQUIRED)
FIND_PACKAGE( PNG REQUIRED)
FIND_PACKAGE( TIFF REQUIRED)
FIND_PACKAGE( OpenImageIO REQUIRED)

FIND_PACKAGE( OpenColorIO REQUIRED)

FIND_PACKAGE( ZLIB REQUIRED)

FIND_PROGRAM( ISPC_EXE NAME ispc)

SET( RAMEN_ISPC_FLAGS "--pic")

IF( CMAKE_BUILD_TYPE MATCHES "Debug" OR CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
    SET( RAMEN_ISPC_FLAGS ${RAMEN_ISPC_FLAGS} "-g")
ENDIF()

IF( CMAKE_BUILD_TYPE MATCHES "Release" OR CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
    SET( RAMEN_ISPC_FLAGS ${RAMEN_ISPC_FLAGS} "-O1")
ENDIF()

IF( RAMEN_SIMD_TARGET STREQUAL "SSE2")
    SET( RAMEN_ISPC_FLAGS ${RAMEN_ISPC_FLAGS} "--target=sse2")
ELSEIF( RAMEN_SIMD_TARGET STREQUAL "SSE4")
    SET( RAMEN_ISPC_FLAGS ${RAMEN_ISPC_FLAGS} "--target=sse4")
ELSEIF( RAMEN_SIMD_TARGET STREQUAL "AVX")
    SET( RAMEN_ISPC_FLAGS ${RAMEN_ISPC_FLAGS} "--target=avx1.1")
ELSEIF( RAMEN_SIMD_TARGET STREQUAL "AVX2")
    SET( RAMEN_ISPC_FLAGS ${RAMEN_ISPC_FLAGS} "--target=avx2")
ENDIF()

SET( RAMEN_ISPC_FLAGS ${RAMEN_ISPC_FLAGS} ${RAMEN_ISPC_EXTRA_FLAGS})

######################################################
# optional dependencies


######################################################
# ramen libs

ADD_SUBDIRECTORY( ramenlibs)

IF( CMAKE_SYSTEM_NAME MATCHES "Linux")
    SET_TARGET_PROPERTIES( ramen_arrays PROPERTIES LIBRARY_OUTPUT_DIRECTORY
                            ${PROJECT_BINARY_DIR}/ramen-${RAMEN_VERSION}/lib/)

    SET_TARGET_PROPERTIES( ramen_core PROPERTIES LIBRARY_OUTPUT_DIRECTORY
                            ${PROJECT_BINARY_DIR}/ramen-${RAMEN_VERSION}/lib/)

    SET_TARGET_PROPERTIES( ramen_math PROPERTIES LIBRARY_OUTPUT_DIRECTORY
                            ${PROJECT_BINARY_DIR}/ramen-${RAMEN_VERSION}/lib/)

    SET_TARGET_PROPERTIES( ramen_iterators PROPERTIES LIBRARY_OUTPUT_DIRECTORY
                            ${PROJECT_BINARY_DIR}/ramen-${RAMEN_VERSION}/lib/)

    SET_TARGET_PROPERTIES( ramen_gl PROPERTIES LIBRARY_OUTPUT_DIRECTORY
                            ${PROJECT_BINARY_DIR}/ramen-${RAMEN_VERSION}/lib/)

    SET_TARGET_PROPERTIES( ramen_os PROPERTIES LIBRARY_OUTPUT_DIRECTORY
                            ${PROJECT_BINARY_DIR}/ramen-${RAMEN_VERSION}/lib/)
ELSEIF( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET_TARGET_PROPERTIES( ramen_arrays PROPERTIES LIBRARY_OUTPUT_DIRECTORY
                            ${PROJECT_BINARY_DIR}/Ramen-${RAMEN_VERSION}.app/Contents/lib/)

    SET_TARGET_PROPERTIES( ramen_core PROPERTIES LIBRARY_OUTPUT_DIRECTORY
                            ${PROJECT_BINARY_DIR}/Ramen-${RAMEN_VERSION}.app/Contents/lib/)

    SET_TARGET_PROPERTIES( ramen_math PROPERTIES LIBRARY_OUTPUT_DIRECTORY
                            ${PROJECT_BINARY_DIR}/Ramen-${RAMEN_VERSION}.app/Contents/lib/)

    SET_TARGET_PROPERTIES( ramen_gl PROPERTIES LIBRARY_OUTPUT_DIRECTORY
                            ${PROJECT_BINARY_DIR}/Ramen-${RAMEN_VERSION}.app/Contents/lib/)

    SET_TARGET_PROPERTIES( ramen_iterators PROPERTIES LIBRARY_OUTPUT_DIRECTORY
                            ${PROJECT_BINARY_DIR}/Ramen-${RAMEN_VERSION}.app/Contents/lib/)

    SET_TARGET_PROPERTIES( ramen_os PROPERTIES LIBRARY_OUTPUT_DIRECTORY
                            ${PROJECT_BINARY_DIR}/Ramen-${RAMEN_VERSION}.app/Contents/lib/)
ELSEIF( ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    MESSAGE( FATAL_ERROR "Windows not supported yet")
ELSE()
    MESSAGE( FATAL_ERROR "Platform ${CMAKE_SYSTEM_NAME} not supported yet")
ENDIF()

SET( RAMENLIBS_LIBS ramen_core
                    ramen_math
                    ramen_iterators
                    ramen_gl
                    ramen_os
                    )

######################################################
# definitions

ADD_DEFINITIONS( -DBOOST_FILESYSTEM_VERSION=3)
ADD_DEFINITIONS( -DBOOST_FILESYSTEM_NO_DEPRECATED)

ADD_DEFINITIONS( -DQT_NO_KEYWORDS)

# looks like we need to set this manually...
SET( MOC_EXTRA_ARGS -DQT_MOC_RUN)

######################################################
# includes

INCLUDE_DIRECTORIES(    ${PROJECT_SOURCE_DIR}
                        ${PROJECT_SOURCE_DIR}/ramenlibs/ramen/arrays
                        ${PROJECT_SOURCE_DIR}/ramenlibs/ramen/algorithm
                        ${PROJECT_SOURCE_DIR}/ramenlibs/ramen/color
                        ${PROJECT_SOURCE_DIR}/ramenlibs/ramen/config
                        ${PROJECT_SOURCE_DIR}/ramenlibs/ramen/core
                        ${PROJECT_SOURCE_DIR}/ramenlibs/ramen/gl
                        ${PROJECT_SOURCE_DIR}/ramenlibs/ramen/iterators
                        ${PROJECT_SOURCE_DIR}/ramenlibs/ramen/math
                        ${PROJECT_SOURCE_DIR}/ramenlibs/ramen/os
                        ${PROJECT_SOURCE_DIR}/ramenlibs/ramen/string_algo
                        ${PROJECT_SOURCE_DIR}/ramen/

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

                        ${ZLIB_INCLUDE_DIRS}
                        )

######################################################
# libraries

SET( RAMEN_ALL_LIBS	${RAMENLIBS_LIBS}
                        ${NEEDED_QT_LIBS}

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

                        ${RAMEN_ALL_LIBS}
                        )

######################################################
# ramen

ADD_SUBDIRECTORY( ramen)

######################################################
# tests

#IF( WITH_TESTS)
    INCLUDE_DIRECTORIES(  ramenlibs/google_testing_libs/google
                          ramenlibs/google_testing_libs/google/gtest
                          ramenlibs/google_testing_libs/google/gmock
                          )

    ENABLE_TESTING()
    #ADD_SUBDIRECTORY( test)
#ENDIF()

######################################################
# docs

FIND_PACKAGE( Doxygen)

IF( DOXYGEN_FOUND)
        CONFIGURE_FILE( ${PROJECT_SOURCE_DIR}/docs/Doxyfile.in ${PROJECT_BINARY_DIR}/Doxyfile @ONLY)
        ADD_CUSTOM_TARGET( doc ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
                            WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
                            COMMENT "Generating API documentation with Doxygen" VERBATIM)
ENDIF()
