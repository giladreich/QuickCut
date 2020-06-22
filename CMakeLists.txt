
cmake_minimum_required(VERSION 3.16)

include(cmake/Utility.cmake)


##################################################################################################################
# Setup
##################################################################################################################

project(QuickCut
    LANGUAGES C CXX
    DESCRIPTION "QuickCut is the way to master your keyboard."
)

set(QUICKCUT_VERSION 2.0.0)

include(CMakeOptions.cmake)

if (USE_CONAN_PM)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Conan.cmake)
    conan_cmake_run(REQUIRES boost/1.73.0
        BASIC_SETUP
        BUILD missing
    )
    # conan_cmake_run(REQUIRES qt/5.15.0@bincrafters/stable
    #     BASIC_SETUP
    #     BUILD missing
    # )

    # if(GENERATE_DOCS)
    #     conan_cmake_run(REQUIRES doxygen/1.8.17
    #         BASIC_SETUP
    #         BUILD missing
    #     )
    # endif()

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
# set_property(GLOBAL PROPERTY AUTOGEN_TARGETS_FOLDER Qt_GeneratedFiles)

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
# Utils
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
    set_target_properties(clang-format PROPERTIES FOLDER Utils)
endif()

if(GENERATE_DOCS)
    find_package(Doxygen REQUIRED dot)

    if(Doxygen_doxygen_FOUND)
        message(STATUS "[INFO] Using Doxygen version ${DOXYGEN_VERSION}")
        message(STATUS "[INFO] Doxygen found -> ${DOXYGEN_EXECUTABLE}")
    else()
        message(ERROR "[ERROR] Doxygen not installed or could not be found.")
    endif()

    if(Doxygen_dot_FOUND)
        message(STATUS "[INFO] Doxygen dot (Graphviz) found -> ${DOXYGEN_DOT_EXECUTABLE}")
    else()
        message(ERROR "[ERROR] Doxygen dot (Graphviz) not installed or could not be found.")
    endif()

    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build_docs)
    set(DOXYGEN_EXCLUDE ./source/QuickCutInstaller)
    set(DOXYGEN_EXCLUDE_PATTERNS */build*/*)
    set(DOXYGEN_RECURSIVE YES)

    set(DOXYGEN_EXTRACT_ALL              YES)
    set(DOXYGEN_SOURCE_BROWSER           YES)
    set(DOXYGEN_SEARCHENGINE             YES)
    set(DOXYGEN_HAVE_DOT                 YES)
    set(DOXYGEN_DOT_MULTI_TARGETS        YES)
    set(DOXYGEN_CLASS_GRAPH              YES)
    set(DOXYGEN_COLLABORATION_GRAPH      YES)
    set(DOXYGEN_GROUP_GRAPHS             YES)
    set(DOXYGEN_UML_LOOK                 YES)
    set(DOXYGEN_INCLUDE_GRAPH            YES)
    set(DOXYGEN_INCLUDED_BY_GRAPH        YES)
    set(DOXYGEN_CALL_GRAPH               YES)
    set(DOXYGEN_CALLER_GRAPH             YES)
    set(DOXYGEN_GRAPHICAL_HIERARCHY      YES)
    set(DOXYGEN_DIRECTORY_GRAPH          YES)

    set(DOXYGEN_PROJECT_NAME ${PROJECT_NAME})
    set(DOXYGEN_PROJECT_NUMBER v${QUICKCUT_VERSION})
    set(DOXYGEN_PROJECT_BRIEF "\
            QuickCut is a shortcut keyboard manager that both facilitates key mapping \
            and activates specified processes. \
    ")
    set(DOXYGEN_PROJECT_LOGO ./media/logo.png)
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ./docs/README.md)

    doxygen_add_docs(build_docs
        ${CMAKE_CURRENT_SOURCE_DIR}
    )
    set_target_properties(build_docs PROPERTIES FOLDER Utils)
endif()

