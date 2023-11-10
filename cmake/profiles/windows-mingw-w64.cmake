include(${CMAKE_CURRENT_LIST_DIR}/common.cmake)

#
set(CMAKE_SYSTEM_NAME Windows)
set(TOOLCHAIN_PREFIX x86_64-w64-mingw32)
set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc-posix)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++-posix)
set(CMAKE_RC_COMPILER ${TOOLCHAIN_PREFIX}-windres)
set(CMAKE_FIND_ROOT_PATH /usr/${TOOLCHAIN_PREFIX})
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)

set(_rustTriple "x86_64-pc-windows-gnu")

set(GDK_LINK_OPTIONS "-static" "-static-libgcc")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # when debugging in windows try dwarf-2 or stabs formats
    add_compile_options("-gdwarf-2" "-fno-omit-frame-pointer") # winedbg flag
endif()
