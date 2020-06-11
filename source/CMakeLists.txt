
cmake_minimum_required(VERSION 3.12)

##################################################################################################################
# Setup
##################################################################################################################
string(TOLOWER ${CMAKE_CXX_COMPILER_ARCHITECTURE_ID} _ARCHITECTURE)
set(BUILD_DIR ${CMAKE_BINARY_DIR}/bin/${_ARCHITECTURE}-vc${MSVC_TOOLSET_VERSION})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})

set(SOURCE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "")
set(QUICKCUT_DIR ${SOURCE_ROOT_DIR}/QuickCut CACHE STRING "")
set(QUICKCUT_CONSOLE_DIR ${SOURCE_ROOT_DIR}/QuickCutConsole CACHE STRING "")
set(QUICKCUT_INSTALLER_DIR ${SOURCE_ROOT_DIR}/QuickCutInstaller CACHE STRING "")
set(QUICKCUT_SERVICE_DIR ${SOURCE_ROOT_DIR}/QuickCutService CACHE STRING "")
set(QUICKCUT_SHARED_DIR ${SOURCE_ROOT_DIR}/QuickCutShared CACHE STRING "")


##################################################################################################################
# Packages
##################################################################################################################
find_package(Qt5 REQUIRED COMPONENTS
    Core
    Widgets
    Gui
)
if (Qt5_FOUND)
    message(STATUS "[INFO] Qt5 Found -> ${Qt5_DIR}")
else()
    message(FATAL_ERROR "[ERROR] Failed to find Qt5. Did you add Qt installation kit bin directory to the system PATH?")
endif()

find_package(Boost REQUIRED)
if (Boost_FOUND)
    message(STATUS "[INFO] Boost Found.")
else()
    message(FATAL_ERROR "[ERROR] Failed to find Boost.")
endif()


##################################################################################################################
# Submodules
##################################################################################################################

add_subdirectory(QuickCutShared)
add_subdirectory(QuickCut)