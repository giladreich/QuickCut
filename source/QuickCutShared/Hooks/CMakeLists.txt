
set(QUICKCUT_HOOKS_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "" FORCE)

set(HOOKS_CXX_FILES
        ${QUICKCUT_HOOKS_DIR}/KeyboardHook.h
        ${QUICKCUT_HOOKS_DIR}/KeyboardHook.cpp
)
if(WIN32)
    set(HOOKS_CXX_FILES
        ${HOOKS_CXX_FILES}
        ${QUICKCUT_HOOKS_DIR}/KeyboardHookWindows.h
        ${QUICKCUT_HOOKS_DIR}/KeyboardHookWindows.cpp
    )
else()
    set(HOOKS_CXX_FILES
        ${HOOKS_CXX_FILES}
        ${QUICKCUT_HOOKS_DIR}/KeyboardHookUnix.h
        ${QUICKCUT_HOOKS_DIR}/KeyboardHookUnix.cpp
    )
endif()

set(QUICKCUT_HOOKS_CXX_FILES
    ${HOOKS_CXX_FILES}
    CACHE STRING "" FORCE
)
