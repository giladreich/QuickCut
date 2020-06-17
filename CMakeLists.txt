
cmake_minimum_required(VERSION 3.12)

##################################################################################################################
# Setup
##################################################################################################################

project(QuickCut
    LANGUAGES C CXX
    DESCRIPTION "QuickCut is the way to master your keyboard."
)

include(CMakeOptions.cmake)

if (USE_CONAN_PM)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/conan.cmake)
    conan_cmake_run(REQUIRES boost/1.73.0 
        BASIC_SETUP 
        BUILD missing)

    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
endif()

if (MSVC)
    if (MSVC_VERSION GREATER_EQUAL 1910)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive-")
    endif()
endif()


##################################################################################################################
# IDE Representation
##################################################################################################################

# Groups CMake pre-defined projects
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(MSVC)
    set_property (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY 
        VS_STARTUP_PROJECT QuickCut
    )
endif()

##################################################################################################################
# Submodules
##################################################################################################################

add_subdirectory(source)


##################################################################################################################
# clang-format
##################################################################################################################
if (USE_CLANG_FORMAT)
    set(DIRS_TO_FORMAT
        ${CMAKE_CURRENT_SOURCE_DIR}/source/QuickCut/*
        ${CMAKE_CURRENT_SOURCE_DIR}/source/QuickCutConsole/*
        ${CMAKE_CURRENT_SOURCE_DIR}/source/QuickCutService/*
        ${CMAKE_CURRENT_SOURCE_DIR}/source/QuickCutShared/*
    )
    file(GLOB_RECURSE FILES_TO_FORMAT ${DIRS_TO_FORMAT})
    list(FILTER FILES_TO_FORMAT INCLUDE REGEX "^.*\.(cpp|h|hpp|cc|cxx)$")
    list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
    include(ClangFormat)
    clangformat_setup(${FILES_TO_FORMAT})
endif()

