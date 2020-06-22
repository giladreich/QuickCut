
##################################################################################################################
# Setup
##################################################################################################################
set(SOURCE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "")
set(QUICKCUT_DIR ${SOURCE_ROOT_DIR}/QuickCut CACHE STRING "")
set(QUICKCUT_CONSOLE_DIR ${SOURCE_ROOT_DIR}/QuickCutConsole CACHE STRING "")
set(QUICKCUT_INSTALLER_DIR ${SOURCE_ROOT_DIR}/QuickCutInstaller CACHE STRING "")
set(QUICKCUT_SERVICE_DIR ${SOURCE_ROOT_DIR}/QuickCutService CACHE STRING "")
set(QUICKCUT_SHARED_DIR ${SOURCE_ROOT_DIR}/QuickCutShared CACHE STRING "")


##################################################################################################################
# Packages
##################################################################################################################

# TODO(Gilad): Consider implementing a fall back scenarios, where if we could'nt find the packages, try finding
# if conan installed, if installed, do "conan install .." command and try to find again after conan setup.
find_package(Qt5 REQUIRED COMPONENTS
    Core
    Widgets
    Gui
    Network
)
if (Qt5_FOUND)
    message(STATUS "[INFO] Qt5 Found -> ${Qt5_DIR}")
else()
    message(FATAL_ERROR "[ERROR] Failed to find Qt5. Do you have the appropriate environment variables set?")
endif()

find_package(Boost REQUIRED)
if (Boost_FOUND)
    message(STATUS "[INFO] Boost Found -> ${FIND_PACKAGE_MESSAGE_DETAILS_Boost}")
else()
    message(FATAL_ERROR "[ERROR] Failed to find Boost.")
endif()


##################################################################################################################
# Submodules
##################################################################################################################

add_subdirectory(QuickCutShared)
add_subdirectory(QuickCut)
add_subdirectory(QuickCutConsole)
add_subdirectory(QuickCutService)