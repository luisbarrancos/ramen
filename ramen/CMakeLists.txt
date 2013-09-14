######################################################################
# Copyright (C) 2013 Esteban Tovagliari. All Rights Reserved.        #
# Licensed under the terms of the CDDL License.                      #
# See CDDL_LICENSE.txt for a copy of the license.                    #
######################################################################

######################################################
# ramen lib sources

IF( CMAKE_SYSTEM_NAME MATCHES "Linux")
    SET( RAMEN_GENERATED_HEADERS_DIR ${PROJECT_BINARY_DIR}/ramen-${RAMEN_VERSION}/include)
ELSEIF( ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    MESSAGE( FATAL_ERROR "Windows not supported yet")
ELSE()
    MESSAGE( FATAL_ERROR "Platform ${CMAKE_SYSTEM_NAME} not supported yet")
ENDIF()

CONFIGURE_FILE( ${PROJECT_SOURCE_DIR}/ramen/version.hpp.in
                ${RAMEN_GENERATED_HEADERS_DIR}/ramen/version.hpp)

INCLUDE_DIRECTORIES( ${RAMEN_GENERATED_HEADERS_DIR})

SET( RAMEN_MOC_CLASSES	${PROJECT_SOURCE_DIR}/ramenlibs/ramen/qwidgets/ramen/qwidgets/label_separator.hpp
                        ${PROJECT_SOURCE_DIR}/ramenlibs/ramen/qwidgets/ramen/qwidgets/time_scale.hpp
                        ${PROJECT_SOURCE_DIR}/ramenlibs/ramen/qwidgets/ramen/qwidgets/time_slider.hpp
                        ${PROJECT_SOURCE_DIR}/ramenlibs/ramen/qwidgets/ramen/qwidgets/ocio_combo.hpp
                        ${PROJECT_SOURCE_DIR}/ramenlibs/ramen/qwidgets/ramen/qwidgets/ocio_colorspace_combo.hpp
                        ${PROJECT_SOURCE_DIR}/ramenlibs/ramen/qwidgets/ramen/qwidgets/ocio_display_combo.hpp
                        ${PROJECT_SOURCE_DIR}/ramenlibs/ramen/qwidgets/ramen/qwidgets/ocio_view_combo.hpp

                        ui/main_window.hpp
                        ui/time_controls.hpp
                        ui/user_interface.hpp

                        ui/anim/anim_curves_view.hpp
                        ui/anim/anim_editor.hpp
                        ui/anim/track_model.hpp
                        ui/anim/anim_editor_toolbar.hpp

                        ui/dialogs/smooth_anim_curves_dialog.hpp

                        ui/inspector/inspector.hpp
                        ui/inspector/panel.hpp

                        ui/nodes_view/nodes_view.hpp
                        ui/nodes_view/nodes_view_tabs.hpp

                        ui/viewer/viewer_tabs.hpp

                        ui/widgets/color_button.hpp
                        ui/widgets/color_picker.hpp
                        ui/widgets/color_swatch.hpp
                        ui/widgets/exposure_picker.hpp
                        ui/widgets/hue_picker.hpp
                        ui/widgets/saturation_value_picker.hpp
                        ui/widgets/eyedropper_button.hpp
                        )

SET( RAMEN_SOURCES      assert.hpp
                        config.hpp

                        anim/any_curve.hpp
                        anim/any_curve.cpp
                        anim/clipboard.hpp
                        anim/clipboard.cpp
                        anim/curve_fwd.hpp
                        anim/curve.hpp
                        anim/curve_algorithm.hpp
                        anim/curve_algorithm.cpp
                        anim/curve_to_half_lut.hpp
                        anim/curve_to_half_lut.cpp
                        anim/float_curve.hpp
                        anim/float_curve.cpp
                        anim/float_key.hpp
                        anim/float_key.cpp
                        anim/fwd_diff_cubic_evaluator.hpp
                        anim/keyframe.hpp
                        anim/keyframe.cpp
                        anim/keyframe_vector.hpp
                        anim/shape_curve.hpp
                        anim/shape_key.hpp
                        anim/track.hpp
                        anim/track.cpp
                        anim/util.hpp
                        anim/util.cpp

                        app/application_fwd.hpp
                        app/application.hpp
                        app/application.cpp
                        app/command_line_parser_fwd.hpp
                        app/command_line_parser.hpp
                        app/command_line_parser.cpp
                        app/document_fwd.hpp
                        app/document.hpp
                        app/document.cpp
                        app/preferences.hpp
                        app/preferences.cpp

                        depgraph/dgraph_fwd.hpp
                        depgraph/dgraph.hpp
                        depgraph/dgraph.cpp
                        depgraph/dnode_fwd.hpp
                        depgraph/dnode.hpp
                        depgraph/dnode.cpp

                        manipulators/manipulable_fwd.hpp
                        manipulators/manipulable.hpp
                        manipulators/manipulable.cpp
                        manipulators/manipulator_fwd.hpp
                        manipulators/manipulator.hpp
                        manipulators/manipulator.cpp

                        memory/manager.cpp

                        nodes/composite_node.hpp
                        nodes/composite_node.cpp
                        nodes/node_fwd.hpp
                        nodes/node.hpp
                        nodes/node.cpp
                        nodes/node_factory.hpp
                        nodes/node_factory.cpp
                        nodes/node_graph_modifier_fwd.hpp
                        nodes/node_graph_modifier.hpp
                        nodes/node_graph_modifier.cpp
                        nodes/node_info.hpp
                        nodes/node_info.cpp
                        nodes/null_node.hpp
                        nodes/null_node.cpp
                        nodes/viewer_node.hpp
                        nodes/viewer_node.cpp
                        nodes/world_node.hpp
                        nodes/world_node.cpp

                        ocio/manager.cpp

                        params/composite_param.hpp
                        params/composite_param.cpp
                        params/param_fwd.hpp
                        params/param.hpp
                        params/param.cpp
                        params/param_set.hpp
                        params/param_set.cpp

                        system/system_fwd.hpp
                        system/system.hpp
                        system/system.cpp

                        ${PROJECT_SOURCE_DIR}/ramenlibs/ramen/qwidgets/ramen/qwidgets/label_separator.cpp
                        ${PROJECT_SOURCE_DIR}/ramenlibs/ramen/qwidgets/ramen/qwidgets/time_scale.cpp
                        ${PROJECT_SOURCE_DIR}/ramenlibs/ramen/qwidgets/ramen/qwidgets/time_slider_fwd.hpp
                        ${PROJECT_SOURCE_DIR}/ramenlibs/ramen/qwidgets/ramen/qwidgets/time_slider.hpp
                        ${PROJECT_SOURCE_DIR}/ramenlibs/ramen/qwidgets/ramen/qwidgets/time_slider.cpp
                        ${PROJECT_SOURCE_DIR}/ramenlibs/ramen/qwidgets/ramen/qwidgets/ocio_combo.hpp
                        ${PROJECT_SOURCE_DIR}/ramenlibs/ramen/qwidgets/ramen/qwidgets/ocio_combo.cpp
                        ${PROJECT_SOURCE_DIR}/ramenlibs/ramen/qwidgets/ramen/qwidgets/ocio_colorspace_combo.hpp
                        ${PROJECT_SOURCE_DIR}/ramenlibs/ramen/qwidgets/ramen/qwidgets/ocio_colorspace_combo.cpp
                        ${PROJECT_SOURCE_DIR}/ramenlibs/ramen/qwidgets/ramen/qwidgets/ocio_display_combo.hpp
                        ${PROJECT_SOURCE_DIR}/ramenlibs/ramen/qwidgets/ramen/qwidgets/ocio_display_combo.cpp
                        ${PROJECT_SOURCE_DIR}/ramenlibs/ramen/qwidgets/ramen/qwidgets/ocio_view_combo.hpp
                        ${PROJECT_SOURCE_DIR}/ramenlibs/ramen/qwidgets/ramen/qwidgets/ocio_view_combo.cpp

                        ui/events_fwd.hpp
                        ui/events.hpp
                        ui/events.cpp
                        ui/events_util.hpp
                        ui/events_util.cpp
                        ui/main_window.cpp
                        ui/palette.cpp
                        ui/time_controls.cpp
                        ui/user_interface_fwd.hpp
                        ui/user_interface.hpp
                        ui/user_interface.cpp

                        ui/anim/anim_curves_view.cpp
                        ui/anim/anim_editor.cpp
                        ui/anim/anim_editor_command.cpp
                        ui/anim/anim_editor_select_tool.cpp
                        ui/anim/anim_editor_tool.cpp
                        ui/anim/anim_editor_toolbar.cpp
                        ui/anim/drag_keys_command.cpp
                        ui/anim/draw_curves_visitors.cpp
                        ui/anim/edit_keys_visitors.cpp
                        ui/anim/edit_tangents_visitors.cpp
                        ui/anim/pick_curves_visitors.cpp
                        ui/anim/track_model.cpp

                        ui/dialogs/smooth_anim_curves_dialog.cpp
                        ui/dialogs/splash_screen.cpp

                        ui/inspector/inspector_fwd.hpp
                        ui/inspector/inspector.hpp
                        ui/inspector/inspector.cpp
                        ui/inspector/panel_fwd.hpp
                        ui/inspector/panel.hpp
                        ui/inspector/panel.cpp
                        ui/inspector/panel_factory.cpp

                        ui/nodes_view/nodes_view_fwd.hpp
                        ui/nodes_view/nodes_view.hpp
                        ui/nodes_view/nodes_view.cpp
                        ui/nodes_view/nodes_view_tabs_fwd.hpp
                        ui/nodes_view/nodes_view_tabs.hpp
                        ui/nodes_view/nodes_view_tabs.cpp

                        ui/viewer/viewer_tabs_fwd.hpp
                        ui/viewer/viewer_tabs.hpp
                        ui/viewer/viewer_tabs.cpp

                        ui/widgets/color.cpp
                        ui/widgets/color_button.cpp
                        ui/widgets/color_picker.cpp
                        ui/widgets/color_swatch.cpp
                        ui/widgets/exposure_picker.cpp
                        ui/widgets/hue_picker.cpp
                        ui/widgets/saturation_value_picker.cpp
                        ui/widgets/eyedropper_button.cpp

                        undo/command_fwd.hpp
                        undo/command.hpp
                        undo/command.cpp
                        undo/stack_fwd.hpp
                        undo/stack.hpp
                        undo/stack.cpp
                        )

######################################################
# nodes

SET( RAMEN_MOC_CLASSES ${RAMEN_MOC_CLASSES})

#SET( RAMEN_SOURCES ${RAMEN_SOURCES} nodes/generator_node.hpp
#                                    nodes/generator_node.cpp
#                                    nodes/pointop_node.hpp
#                                    nodes/pointop_node.cpp
#                                    nodes/color/channel_mixer_node.cpp
#                                    nodes/generate/color_bars_node.cpp
#                                    nodes/generate/color_node.cpp
#                                    )

######################################################
# ispc

#MACRO( ISPC_COMPILE OUT_LIST SRC_LOCATION)
#
#    GET_FILENAME_COMPONENT( ABS_ISPC_SRC ${SRC_LOCATION} ABSOLUTE)
#
#    GET_FILENAME_COMPONENT( ISPC_SRC_FILENAME ${SRC_LOCATION} NAME_WE)
#    SET( ISPC_OBJ_FILENAME "${ISPC_SRC_FILENAME}.obj")
#    SET( ISPC_HDR_FILENAME "${ISPC_SRC_FILENAME}_ispc.h")
#
#    SET( ISPC_OBJ ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${ISPC_OBJ_FILENAME})
#    SET( ISPC_HDR ${RAMEN_GENERATED_HEADERS_DIR}/ramen/image/${ISPC_HDR_FILENAME})
#    ADD_CUSTOM_COMMAND( OUTPUT ${ISPC_OBJ}
#                        COMMAND ${ISPC_EXE} ${RAMEN_ISPC_FLAGS} ${ABS_ISPC_SRC} -o ${ISPC_OBJ} -h ${ISPC_HDR}
#                        IMPLICIT_DEPENDS C ${SRC_LOCATION}
#                        DEPENDS ${SRC_LOCATION}
#                        MAIN_DEPENDENCY  ${SRC_LOCATION}  # this gives the ispc file in MSVC a 'compile' option on the source file
#                        )
#    SET_SOURCE_FILES_PROPERTIES( ${ISPC_OBJ} PROPERTIES GENERATED TRUE EXTERNAL_OBJECT TRUE)
#    SET_SOURCE_FILES_PROPERTIES( ${ISPC_HDR} PROPERTIES GENERATED TRUE EXTERNAL_OBJECT TRUE)
#    LIST( APPEND ${OUT_LIST} ${SRC_LOCATION} ${HDR_LOCATION})
#ENDMACRO()

# create generated headers directory
#FILE( MAKE_DIRECTORY ${RAMEN_GENERATED_HEADERS_DIR}/ramen/image)
#ISPC_COMPILE( RAMEN_ISPC_SOURCES image/simple.ispc)

######################################################
# platform specific

# platform sources
IF( ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # Linux
ELSEIF( ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    # Windows
ELSE()
    MESSAGE( FATAL_ERROR "Platform ${CMAKE_SYSTEM_NAME} not supported yet")
ENDIF()

######################################################
# ui files

SET( RAMEN_DESIGNER_FILES   ../designer/smooth_anim_curves.ui
                            )

######################################################
# QT

QT4_WRAP_UI( RAMEN_DESIGNER_SOURCES ${RAMEN_DESIGNER_FILES})
QT4_WRAP_CPP( RAMEN_MOC_SOURCES	${RAMEN_MOC_CLASSES} OPTIONS ${MOC_EXTRA_ARGS})
QT4_ADD_RESOURCES( RAMEN_RESOURCES ${PROJECT_SOURCE_DIR}/resources/resources.qrc)

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR})

######################################################
# ramen lib

ADD_LIBRARY( ramen SHARED   ${RAMEN_SOURCES}
                            ${RAMEN_ISPC_SOURCES}
                            ${RAMEN_MOC_SOURCES}
                            ${RAMEN_DESIGNER_SOURCES}
                            ${RAMEN_PLATFORM_SOURCES}
                            ${RAMEN_RESOURCES}
                            )

SET_TARGET_PROPERTIES( ramen PROPERTIES
                             VERSION ${RAMEN_VERSION_MAJOR}.${RAMEN_VERSION_MINOR}.${RAMEN_VERSION_PATCH}
                             SOVERSION ${RAMEN_VERSION_MAJOR})

TARGET_LINK_LIBRARIES( ramen ${RAMEN_ALL_LIBS})

######################################################
# ramen binary

SET( RAMEN_APP_SOURCES	main.cpp)

ADD_EXECUTABLE( ramen.bin ${RAMEN_APP_SOURCES})
TARGET_LINK_LIBRARIES( ramen.bin ramen ${RAMEN_ALL_LIBS})

IF( CMAKE_SYSTEM_NAME MATCHES "Linux")
    SET_TARGET_PROPERTIES( ramen PROPERTIES LIBRARY_OUTPUT_DIRECTORY
                            ${PROJECT_BINARY_DIR}/ramen-${RAMEN_VERSION}/lib/)

    SET_TARGET_PROPERTIES( ramen.bin PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                            ${PROJECT_BINARY_DIR}/ramen-${RAMEN_VERSION}/bin/)
ELSEIF( ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    MESSAGE( FATAL_ERROR "Windows not supported yet")
ELSE()
    MESSAGE( FATAL_ERROR "Platform ${CMAKE_SYSTEM_NAME} not supported yet")
ENDIF()
