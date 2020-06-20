
cmake_minimum_required(VERSION 3.16)

##################################################################################################################
# Setup
##################################################################################################################
project(QuickCut)

add_definitions(
    -DUNICODE
    -D_UNICODE
    -DBOOST_BIND_GLOBAL_PLACEHOLDERS
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${QUICKCUT_SHARED_DIR}
    ${PROJECT_BINARY_DIR} # generated "ui_*.h" files.
)

set(RESOURCE_DIR Resources)

##################################################################################################################
# Project Files
##################################################################################################################
set(SHARED_CXX_FILES
    ${QUICKCUT_SHARED_CXX_FILES}
    ${QTSINGLEAPP_CXX_FILES}
)
set(WIDGETS_CXX_FILES
    QShortcutInput.h
    QShortcutInput.cpp
)
set(UI_CXX_FILES
    MainWindow.h
    MainWindow.cpp
    AboutWindow.h
    AboutWindow.cpp
    ActionEditWindow.h
    ActionEditWindow.cpp
    CheckUpdatesWindow.h
    CheckUpdatesWindow.cpp
    ExamplesWindow.h
    ExamplesWindow.cpp
)
set(MISC_CXX_FILES
    ""
)
set(CXX_FILES
    main.cpp
    ${MISC_CXX_FILES}
    ${UI_CXX_FILES}
    ${WIDGETS_CXX_FILES}
    # Shared sources:
    ${SHARED_CXX_FILES}
    ${QUICKCUT_MODELS_CXX_FILES}
    ${QTSERVICE_CXX_FILES}
    ${QUICKCUT_UTILS_CXX_FILES}
)

set(QRC_FILES
    ${RESOURCE_DIR}/QuickCut.qrc
    ${RESOURCE_DIR}/ui/styles/stylesheet.qrc
)
file(GLOB QUI_FILES ${RESOURCE_DIR}/ui/*.ui)
file(GLOB QSS_FILES ${RESOURCE_DIR}/ui/styles/*.qss)

set(RESOURCE_FILES
    # ${QRC_FILES}
    # ${QUI_FILES}
    ${QSS_FILES}
)

if(WIN32)
    set(BIN_INFO_RC
        ${RESOURCE_DIR}/platform/windows/QuickCut.rc
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

# NOTE(Gilad): Only problem with this approach, that CMake will have to re-run the configure step after build and
# on VS also re-open the project, so filters taking effect, because these files gets generated during compilation.
# file(GLOB_RECURSE QT_GENERATED_FILES ${PROJECT_BINARY_DIR}/*)
# list(FILTER QT_GENERATED_FILES INCLUDE REGEX "^.*\.(cpp|h|hpp|cc|cxx)$")
# source_group("Qt_GeneratedFiles" FILES
#     ${QT_GENERATED_FILES}
# )

set(RESOURCE_FILES
    ${RESOURCE_FILES}
    ${QRCC_FILES}
    ${QUIC_FILES}
)


##################################################################################################################
# IDE Representation
##################################################################################################################
source_group("Source Files\\Misc" FILES
    ${MISC_CXX_FILES}
)
source_group("Source Files\\Shared" FILES
    ${SHARED_CXX_FILES}
)
source_group("Source Files\\Shared\\Models" FILES
    ${QUICKCUT_MODELS_CXX_FILES}
)
source_group("Source Files\\Shared\\QtService" FILES
    ${QTSERVICE_CXX_FILES}
)
source_group("Source Files\\Widgets" FILES
    ${WIDGETS_CXX_FILES}
)
source_group("Source Files\\Windows" FILES
    ${UI_CXX_FILES}
)
source_group("Source Files\\Utils" FILES
    ${QUICKCUT_UTILS_CXX_FILES}
)
source_group("${RESOURCE_DIR}" FILES
    ${BIN_INFO_RC}
)
source_group("${RESOURCE_DIR}\\QtUI" FILES
    ${QUI_FILES}
)
source_group("${RESOURCE_DIR}\\QtRC" FILES
    ${QRC_FILES}
)
source_group("${RESOURCE_DIR}\\QtRC\\Themes" FILES
    ${QSS_FILES}
)
source_group("Qt_GeneratedFiles" FILES
    ${QRCC_FILES}
    ${QUIC_FILES}
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}_autogen/mocs_compilation.cpp
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
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::WinMain
    )
else() # UNIX
    target_link_libraries(${PROJECT_NAME} PUBLIC
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Network
    )
endif()

target_precompile_headers(${PROJECT_NAME} PRIVATE ${QUICKCUT_SHARED_DIR}/pch.h)

if(WIN32)
    set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS " /MANIFESTUAC:\"level='requireAdministrator'\"")
endif()
