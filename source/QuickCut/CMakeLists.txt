
cmake_minimum_required(VERSION 3.12)

##################################################################################################################
# Setup
##################################################################################################################
project(QuickCut)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${QUICKCUT_SHARED_DIR}
)

##################################################################################################################
# Project Files
##################################################################################################################
set(HEADERS_CXX_FILES
    MainWindow.h
    AboutWindow.h
    ActionEditWindow.h
    CheckUpdatesWindow.h
    ExamplesWindow.h
    Preferences.h
    QShortcutInput.h
    ${QUICKCUT_SHARED_DIR}/pch.h
    ${QUICKCUT_SHARED_DIR}/Action.h
    ${QUICKCUT_SHARED_DIR}/Profile.h
    ${QTSINGLEAPP_DIR}/QSingleInstance.h
    ${QTSERVICE_DIR}/qtservice.h
    ${QTSERVICE_DIR}/qtservice_p.h
)
set(SOURCES_CXX_FILES
    main.cpp
    MainWindow.cpp
    AboutWindow.cpp
    ActionEditWindow.cpp
    CheckUpdatesWindow.cpp
    ExamplesWindow.cpp
    Preferences.cpp
    QShortcutInput.cpp
    ${QUICKCUT_SHARED_DIR}/Action.cpp
    ${QUICKCUT_SHARED_DIR}/Profile.cpp
    ${QTSINGLEAPP_DIR}/QSingleInstance.cpp
    ${QTSERVICE_DIR}/qtservice.cpp
    ${QTSERVICE_DIR}/qtservice_win.cpp
)

file(GLOB UI_FILES *.ui)
file(GLOB QRC_FILES 
    MainWindow.qrc
    Styles/stylesheet.qrc
)
file(GLOB QSS_FILES Styles/*.qss)

set(RESOURCE_FILES
    ${UI_FILES}
    ${QRC_FILES}
    ${QSS_FILES}
)


##################################################################################################################
# IDE Representation
##################################################################################################################
source_group("Header Files" FILES
    ${HEADERS_CXX_FILES}
)
source_group("Source Files" FILES
    ${SOURCES_CXX_FILES}
)
source_group("Resources\\UI" FILES
    ${UI_FILES}
)
source_group("Resources\\RC" FILES
    ${QRC_FILES}
)
source_group("Resources\\RC\\Themes" FILES
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
add_executable(${PROJECT_NAME} WIN32
    ${HEADERS_CXX_FILES}
    ${SOURCES_CXX_FILES}
    ${RESOURCE_FILES}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)
