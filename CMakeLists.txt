cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)

# Configure vcpkg


string( TOLOWER "${CMAKE_GENERATOR_PLATFORM}" CMAKE_GENERATOR_PLATFORM )
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)
if (${CMAKE_GENERATOR_PLATFORM} STREQUAL "win32")
    set(VCPKG_TARGET_TRIPLET x86-windows-static CACHE STRING "VCPKG Target Triplet to use")
elseif (${CMAKE_GENERATOR_PLATFORM} STREQUAL "x64")
    set(VCPKG_TARGET_TRIPLET x64-windows-static CACHE STRING "VCPKG Target Triplet to use")
else ()
    message(FATAL_ERROR "Unexpected argument for the platform generator")
endif ()

# Configure project

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(RES_DIR ${CMAKE_SOURCE_DIR}/res)

set(KOALABOX_SRC_DIR ${CMAKE_SOURCE_DIR}/KoalaBox/src)

file(STRINGS "${RES_DIR}/version.txt" PROJECT_VERSION_TEXT)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

project(Lyptus VERSION "${PROJECT_VERSION_TEXT}.0")

set(PROJECT_VERSION_SHORT ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
set(VERSION_SUFFIX $ENV{VERSION_SUFFIX})

set(GEN_DIR ${PROJECT_BINARY_DIR}/generated)

# Substitute variables in build and runtime config files

## Generate build config file
configure_file(${RES_DIR}/build_config.gen.h ${GEN_DIR}/build_config.h)

## Generate version resource file
configure_file(${RES_DIR}/version.gen.rc ${GEN_DIR}/version.rc)

# Build the library

add_library(
        ${CMAKE_PROJECT_NAME} SHARED
        ## Sources
        ${SRC_DIR}/main.cpp
        ${SRC_DIR}/config/config.cpp
        ${SRC_DIR}/patcher/patcher.cpp
        ${SRC_DIR}/lyptus/lyptus.cpp

        ## Dependencies
        ${KOALABOX_SRC_DIR}/koalabox/koalabox.cpp
        ${KOALABOX_SRC_DIR}/koalabox/logger/logger.cpp
        ${KOALABOX_SRC_DIR}/koalabox/win_util/win_util.cpp
        ${KOALABOX_SRC_DIR}/koalabox/util/util.cpp

        ## Resources
        ${GEN_DIR}/version.rc
)

math(EXPR BITS "8*${CMAKE_SIZEOF_VOID_P}")
set_target_properties(
        ${CMAKE_PROJECT_NAME}
        PROPERTIES
        RUNTIME_OUTPUT_NAME "Lyptus${BITS}"
)

target_include_directories(
        ${CMAKE_PROJECT_NAME} PRIVATE
        ${SRC_DIR}
        ${KOALABOX_SRC_DIR}
        ${GEN_DIR}
)

target_precompile_headers(
        ${CMAKE_PROJECT_NAME} PRIVATE
        "$<$<COMPILE_LANGUAGE:CXX>:${SRC_DIR}/pch.hpp>"
)

# Link dependencies

find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(
        ${CMAKE_PROJECT_NAME} PRIVATE
        spdlog::spdlog
        nlohmann_json
)
