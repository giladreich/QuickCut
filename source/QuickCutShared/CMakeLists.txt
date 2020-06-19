
cmake_minimum_required(VERSION 3.12)

set(QTSINGLEAPP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/QSingleApplication CACHE STRING "")
set(QTSERVICE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/QtService CACHE STRING "")

file(GLOB QUICKCUT_SHARED_CXX_FILES
    *.h
    *.cpp
)
set(QUICKCUT_SHARED_CXX_FILES
    ${QUICKCUT_SHARED_CXX_FILES}
    CACHE STRING "" FORCE
)

add_subdirectory(Models)
add_subdirectory(QSingleApplication)
add_subdirectory(QtService)
