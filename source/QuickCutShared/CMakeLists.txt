
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

add_subdirectory(Hooks)
add_subdirectory(Managers)
add_subdirectory(Models)
add_subdirectory(Parsers)
add_subdirectory(QtService)
add_subdirectory(Utils)
