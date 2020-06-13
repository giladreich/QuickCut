
cmake_minimum_required(VERSION 3.12)

##################################################################################################################
# Setup
##################################################################################################################
project(QuickCutConsole)

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
set(MISC_CXX_FILES
    main.cpp
    QuickCutConsole.h
    QuickCutConsole.cpp
)

if(WIN32)
    set(MISC_CXX_FILES
        ${MISC_CXX_FILES}
        QuickCutConsoleWindows.h
        QuickCutConsoleWindows.cpp
    )
    set(RESOURCE_FILES
        Resources/QuickCutConsole.rc
    )
elseif(UNIX)
    set(MISC_CXX_FILES
        ${MISC_CXX_FILES}
        QuickCutConsoleUnix.h
        QuickCutConsoleUnix.cpp
    )
    set(RESOURCE_FILES
        "" # TODO(Gilad): Create resource for unix systems.
    )
endif()

set(CXX_FILES
    ${MISC_CXX_FILES}
    ${SHARED_CXX_FILES}
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
source_group("Resources" FILES
    ${RESOURCE_FILES}
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
    )
else() # UNIX
    target_link_libraries(${PROJECT_NAME} PUBLIC
        Qt5::Core
    )
endif()

if(WIN32)
    set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS " /MANIFESTUAC:\"level='requireAdministrator'\"")
endif()
