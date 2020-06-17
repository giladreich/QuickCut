
cmake_minimum_required(VERSION 3.12)

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
)

##################################################################################################################
# Project Files
##################################################################################################################
set(SHARED_CXX_FILES
    ${QUICKCUT_SHARED_DIR}/pch.h
    ${QUICKCUT_SHARED_DIR}/Profile.h
    ${QUICKCUT_SHARED_DIR}/Profile.cpp
    ${QUICKCUT_SHARED_DIR}/Action.h
    ${QUICKCUT_SHARED_DIR}/Action.cpp
    ${QTSINGLEAPP_DIR}/QSingleInstance.h
    ${QTSINGLEAPP_DIR}/QSingleInstance.cpp
)
set(QTSERVICE_CXX_FILES
    ${QTSERVICE_DIR}/qtservice.h
    ${QTSERVICE_DIR}/qtservice_p.h
    ${QTSERVICE_DIR}/qtservice.cpp
)
if (WIN32)
    set(QTSERVICE_CXX_FILES
        ${QTSERVICE_CXX_FILES}
        ${QTSERVICE_DIR}/qtservice_win.cpp
    )
elseif(UNIX)
    set(QTSERVICE_CXX_FILES
        ${QTSERVICE_CXX_FILES}
        ${QTSERVICE_DIR}/qtservice_unix.cpp
        ${QTSERVICE_DIR}/qtunixsocket.h
        ${QTSERVICE_DIR}/qtunixsocket.cpp
        ${QTSERVICE_DIR}/qtunixserversocket.h
        ${QTSERVICE_DIR}/qtunixserversocket.cpp
    )
endif()
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
    main.cpp
    Preferences.h
    Preferences.cpp
)
set(CXX_FILES
    ${MISC_CXX_FILES}
    ${UI_CXX_FILES}
    ${WIDGETS_CXX_FILES}
    ${QTSERVICE_CXX_FILES}
    ${SHARED_CXX_FILES}
)

file(GLOB QUI_FILES *.ui)
file(GLOB QRC_FILES 
    MainWindow.qrc
    Styles/stylesheet.qrc
)
file(GLOB QSS_FILES Styles/*.qss)

set(RESOURCE_FILES
    ${QUI_FILES}
    ${QRC_FILES}
    ${QSS_FILES}
)


if(WIN32)
    set(BIN_INFO_RC
        Resources/platform/windows/QuickCut.rc
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
# IDE Representation
##################################################################################################################
source_group("Source Files" FILES
    ${MISC_CXX_FILES}
)
source_group("Source Files\\Shared" FILES
    ${SHARED_CXX_FILES}
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
source_group("Resources" FILES
    ${BIN_INFO_RC}
)
source_group("Resources\\QtUI" FILES
    ${QUI_FILES}
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

if(WIN32)
    set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS " /MANIFESTUAC:\"level='requireAdministrator'\"")
endif()
