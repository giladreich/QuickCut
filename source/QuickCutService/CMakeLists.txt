
cmake_minimum_required(VERSION 3.16)

##################################################################################################################
# Setup
##################################################################################################################

project(QuickCutService)

add_definitions(
    -DUNICODE
    -D_UNICODE
    -DBOOST_BIND_GLOBAL_PLACEHOLDERS
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SOURCE_ROOT_DIR}
)

##################################################################################################################
# Project Files
##################################################################################################################

# Sources
set(ROOT_CXX_FILES
    main.cpp
    QuickCutService.h
    QuickCutService.cpp
)

if(WIN32)
    set(ROOT_CXX_FILES
        ${ROOT_CXX_FILES}
        QuickCutServiceWindows.h
        QuickCutServiceWindows.cpp
    )
elseif(UNIX)
    set(ROOT_CXX_FILES
        ${ROOT_CXX_FILES}
        QuickCutServiceUnix.h
        QuickCutServiceUnix.cpp
    )
endif()

set(SHARED_CXX_FILES
    ${QUICKCUT_SHARED_DIR}/QuickCutPCH.h
    ${QUICKCUT_SHARED_DIR}/QuickCutPCH.cpp
)
set(UTILS_CXX_FILES
    ${QUICKCUT_UTILS_DIR}/Utility.h
    ${QUICKCUT_UTILS_DIR}/Utility.cpp
)
set(CXX_FILES
    ${ROOT_CXX_FILES}
    ${QTSERVICE_CXX_FILES}
    ${SHARED_CXX_FILES}
    ${UTILS_CXX_FILES}
)

# Resources
if(WIN32)
    set(RESOURCE_FILES
        Resources/QuickCutService.rc
    )
elseif(UNIX)
    set(RESOURCE_FILES
        "" # TODO(Gilad): Create resource for unix systems.
    )
endif()

##################################################################################################################
# IDE Representation
##################################################################################################################

source_group("Source Files" FILES
    ${ROOT_CXX_FILES}
)

source_group("Source Files\\Shared" FILES
    ${SHARED_CXX_FILES}
)
source_group("Source Files\\Shared\\QtService" FILES
    ${QTSERVICE_CXX_FILES}
)
source_group("Source Files\\Shared\\Utils" FILES
    ${UTILS_CXX_FILES}
)

source_group("Resources" FILES
    ${RESOURCE_FILES}
)

file(GLOB_RECURSE CMAKE_GENERATED_FILES ${PROJECT_BINARY_DIR}/*)
list(FILTER CMAKE_GENERATED_FILES INCLUDE REGEX "^.*\.(hxx|cxx)$")
source_group("_GeneratedFiles\\CMake" FILES
    ${CMAKE_GENERATED_FILES}
)
file(GLOB_RECURSE QT_GENERATED_FILES ${PROJECT_BINARY_DIR}/*)
list(FILTER QT_GENERATED_FILES INCLUDE REGEX "^.*(mocs_.*|ui_.*|qrc_.*)\.(h|cpp)$")
source_group("_GeneratedFiles\\Qt" FILES
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}_autogen/mocs_compilation.cpp
    ${QT_GENERATED_FILES}
)

##################################################################################################################
# Qt Framework
##################################################################################################################

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
#qt5_wrap_ui()
#qt5_wrap_cpp()

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
        Qt5::Network
    )
else() # UNIX
    target_link_libraries(${PROJECT_NAME} PUBLIC
        Qt5::Core
        Qt5::Network
    )
endif()

target_precompile_headers(${PROJECT_NAME} PRIVATE ${QUICKCUT_SHARED_DIR}/QuickCutPCH.h)
# target_precompile_headers(${PROJECT_NAME} REUSE_FROM QuickCut) # slow and some warnings.

if(WIN32)
    set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS " /MANIFESTUAC:\"level='requireAdministrator'\"")
    set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY VS_DEBUGGER_COMMAND_ARGUMENTS "-e")
    set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()
