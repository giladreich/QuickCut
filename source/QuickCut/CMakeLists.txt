
cmake_minimum_required(VERSION 3.16)

##################################################################################################################
# Setup
##################################################################################################################

project(QuickCut)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(VIEWS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Views)
set(WIDGETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Widgets)
set(RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Resources)

add_definitions(
    -DUNICODE
    -D_UNICODE
    -DBOOST_BIND_GLOBAL_PLACEHOLDERS
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SOURCE_ROOT_DIR}
    ${PROJECT_BINARY_DIR} # generated "ui_*.h" files.
)

##################################################################################################################
# Project Files
##################################################################################################################

# Sources
file(GLOB ROOT_CXX_FILES
    ${ROOT_DIR}/*.h
    ${ROOT_DIR}/*cpp
)
file(GLOB VIEWMODELS_CXX_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/ViewModels/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ViewModels/*.cpp
)
file(GLOB VIEWS_CXX_FILES
    ${VIEWS_DIR}/*.h
    ${VIEWS_DIR}/*cpp
)
file(GLOB WIDGETS_CXX_FILES
    ${WIDGETS_DIR}/*.h
    ${WIDGETS_DIR}/*cpp
)
set(SHARED_ROOT_CXX_FILES
    ${QUICKCUT_SHARED_CXX_FILES}
)
set(CXX_FILES
    ${ROOT_CXX_FILES}
    ${VIEWMODELS_CXX_FILES}
    ${VIEWS_CXX_FILES}
    ${WIDGETS_CXX_FILES}
    # Shared sources:
    ${SHARED_ROOT_CXX_FILES}
    ${QUICKCUT_HOOKS_CXX_FILES}
    ${QUICKCUT_MANAGERS_CXX_FILES}
    ${QUICKCUT_MODELS_CXX_FILES}
    ${QUICKCUT_PARSERS_CXX_FILES}
    ${QTSINGLEAPP_CXX_FILES}
    ${QUICKCUT_UTILS_CXX_FILES}
    ${QTSERVICE_CXX_FILES}
)

# Resources
set(QRC_FILES
    ${RESOURCES_DIR}/QuickCut.qrc
    ${RESOURCES_DIR}/ui/styles/stylesheet.qrc
)
file(GLOB QUI_FILES ${RESOURCES_DIR}/ui/*.ui)
file(GLOB QSS_FILES ${RESOURCES_DIR}/ui/styles/*.qss)

set(RESOURCE_FILES
    # ${QRC_FILES}
    # ${QUI_FILES}
    ${QSS_FILES}
)

if(WIN32)
    set(BIN_INFO_RC
        ${RESOURCES_DIR}/platform/windows/QuickCut.rc
    )
elseif(UNIX)
    set(BIN_INFO_RC
        "" # TODO(Gilad): create resource file for linux.
    )
endif()

set(RESOURCE_FILES
    ${RESOURCE_FILES}
    ${BIN_INFO_RC}
)

##################################################################################################################
# Qt Framework
##################################################################################################################

set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)
# set(CMAKE_AUTOUIC ON)

# qt5_wrap_cpp()
qt5_add_resources(QRCC_FILES ${QRC_FILES})
qt5_wrap_ui(QUIC_FILES ${QUI_FILES})

set(RESOURCE_FILES
    ${RESOURCE_FILES}
    ${QRCC_FILES}
    ${QUIC_FILES}
)

##################################################################################################################
# IDE Representation
##################################################################################################################

source_group("Source Files\\ViewModels" FILES
    ${VIEWMODELS_CXX_FILES}
)
source_group("Source Files\\Views" FILES
    ${VIEWS_CXX_FILES}
)
source_group("Source Files\\Widgets" FILES
    ${WIDGETS_CXX_FILES}
)

source_group("Source Files\\Shared" FILES
    ${SHARED_ROOT_CXX_FILES}
)
source_group("Source Files\\Shared\\Hooks" FILES
    ${QUICKCUT_HOOKS_CXX_FILES}
)
source_group("Source Files\\Shared\\Managers" FILES
    ${QUICKCUT_MANAGERS_CXX_FILES}
)
source_group("Source Files\\Shared\\Models" FILES
    ${QUICKCUT_MODELS_CXX_FILES}
)
source_group("Source Files\\Shared\\Parsers" FILES
    ${QUICKCUT_PARSERS_CXX_FILES}
)
source_group("Source Files\\Shared\\QtService" FILES
    ${QTSERVICE_CXX_FILES}
)
source_group("Source Files\\Shared\\Utils" FILES
    ${QUICKCUT_UTILS_CXX_FILES}
)
source_group("Source Files\\Shared\\Utils\\QSingleApp" FILES
    ${QTSINGLEAPP_CXX_FILES}
)

source_group("Resources" FILES
    ${BIN_INFO_RC}
)
source_group("Resources\\QtUI" FILES
    ${QUI_FILES}
)
source_group("Resources\\QtRC" FILES
    ${QRC_FILES}
)
source_group("Resources\\QtRC\\Themes" FILES
    ${QSS_FILES}
)

# Auto generated files after build is triggered
# These files only gets generated at compile time, but adding this anyway in case someone
# doing config->build->config->OpenIDE which should filter them all right.
file(GLOB_RECURSE CMAKE_GENERATED_FILES ${PROJECT_BINARY_DIR}/*)
list(FILTER CMAKE_GENERATED_FILES INCLUDE REGEX "^.*\.(hxx|cxx)$")
source_group("_GeneratedFiles\\CMake" FILES
    ${CMAKE_GENERATED_FILES}
)
file(GLOB_RECURSE QT_GENERATED_FILES ${PROJECT_BINARY_DIR}/*)
list(FILTER QT_GENERATED_FILES INCLUDE REGEX "^.*(mocs_.*|ui_.*|qrc_.*)\.(h|cpp)$")
source_group("_GeneratedFiles\\Qt" FILES
    ${QRCC_FILES}
    ${QUIC_FILES}
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}_autogen/mocs_compilation.cpp
    ${QT_GENERATED_FILES}
)

##################################################################################################################
# Target
##################################################################################################################

add_executable(${PROJECT_NAME}
    ${CXX_FILES}
    ${RESOURCE_FILES}
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC
        Qt5::WinMain
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Network
    )
else() # UNIX
    target_link_libraries(${PROJECT_NAME} PUBLIC
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Network
    )
endif()

target_precompile_headers(${PROJECT_NAME} PRIVATE ${QUICKCUT_SHARED_DIR}/QuickCutPCH.h)

if(WIN32)
    set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS " /MANIFESTUAC:\"level='requireAdministrator'\"")
    set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS " /SUBSYSTEM:WINDOWS")
    set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()
