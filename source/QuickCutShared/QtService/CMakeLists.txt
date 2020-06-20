
set(QTSERVICE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "")

set(CXX_FILES
    ${QTSERVICE_DIR}/qtservice.h
    ${QTSERVICE_DIR}/qtservice_p.h
    ${QTSERVICE_DIR}/qtservice.cpp)
if (WIN32)
    set(CXX_FILES
        ${CXX_FILES}
        ${QTSERVICE_DIR}/qtservice_win.cpp
    )
elseif(UNIX)
    set(CXX_FILES
        ${CXX_FILES}
        ${QTSERVICE_DIR}/qtservice_unix.cpp
        ${QTSERVICE_DIR}/qtunixsocket.h
        ${QTSERVICE_DIR}/qtunixsocket.cpp
        ${QTSERVICE_DIR}/qtunixserversocket.h
        ${QTSERVICE_DIR}/qtunixserversocket.cpp
    )
endif()

set(QTSERVICE_CXX_FILES ${CXX_FILES} CACHE STRING "" FORCE)