#
# Copyright (C) 2017 Manh Tran
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
cmake_minimum_required(VERSION 2.8.9)
project (macao C)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        set(WINDOWS TRUE)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        set(LINUX TRUE)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        set(MACOSX TRUE)
elseif (CMAKE_SYSTEM_NAME MATCHES "CYGWIN")
        add_definitions(-Dlinux)
        set(LINUX TRUE)
endif()

add_definitions(-Wno-incompatible-pointer-types)

if (${USER_BUILD_IOS} MATCHES "true")
        set(CMAKE_OSX_SYSROOT "iphoneos")
        set(CMAKE_XCODE_EFFECTIVE_PLATFORMS "-iphoneos")
        set(CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD)")
endif()

if(${OPENGL} MATCHES "true")
        if(${BUILD_WEB} MATCHES "true")
                set(IMAGE_SDL TRUE)
                add_definitions(-DGFX=OGL -DIMAGE_TYPE=IMAGE_WEB)
        else()
                set(IMAGE_SDL TRUE)
                add_definitions(-DGFX=OGL -DIMAGE_TYPE=IMAGE_SDL2)
        endif()
else(${METAL} MATCHES "true")
        add_definitions(-DGFX=MTL -DIMAGE_TYPE=IMAGE_OBJC)
endif()

if(${APP_DEBUG} MATCHES "true")
        add_definitions(-DAPP_DEBUG=1)
else()
        add_definitions(-DAPP_DEBUG=0)
endif()

add_subdirectory(../../cherry/kernel ../${OUT}/kernel)
add_subdirectory(../../cherry/xml ../${OUT}/xml)
add_subdirectory(../../cherry/gfx ../${OUT}/gfx)
add_subdirectory(../../cherry/smartfox ../${OUT}/smartfox)
add_subdirectory(../../native_ui ../${OUT}/native_ui)

include_directories(../../cherry/include)
include_directories(../../cherry/smartfox/include)
include_directories(../../native_ui/include)
include_directories(../include)

set(PROJECT_LINK_LIBS kernel xml gfx native_ui smartfox)

file(GLOB_RECURSE SOURCES "../src/*.c")

set(CMAKE_EXECUTABLE_SUFFIX ".html")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s TOTAL_MEMORY=134217728 --use-preload-plugins --preload-file ../data@")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s EMTERPRETIFY=1 -s EMTERPRETIFY_ASYNC=1 -s ASSERTIONS=2 ")
add_executable(macao main.c ${SOURCES})
add_dependencies(macao ${PROJECT_LINK_LIBS})
target_link_libraries(macao ${PROJECT_LINK_LIBS})
