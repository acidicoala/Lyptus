cmake_minimum_required(VERSION 3.21)

include(KoalaBox/KoalaBox.cmake)

configure_globals(KoalaBox)

project(Lyptus VERSION 1.1.0)

configure_version_resource("https://github.com/acidicoala/Lyptus")

configure_build_config()

configure_library(
    SHARED

    ${SRC_DIR}/lyptus/lyptus.cpp
    ${SRC_DIR}/patcher/patcher.cpp
    ${SRC_DIR}/main.cpp

    ${KOALABOX_SRC_DIR}/koalabox/file_logger/file_logger.cpp
    ${KOALABOX_SRC_DIR}/koalabox/loader/loader.cpp
    ${KOALABOX_SRC_DIR}/koalabox/win_util/win_util.cpp
    ${KOALABOX_SRC_DIR}/koalabox/util/util.cpp
)

configure_precompile_headers(${CMAKE_PROJECT_NAME} ${SRC_DIR}/pch.hpp)

set_32_and_64(OUTPUT_NAME Lyptus32 Lyptus64)
configure_output_name(${OUTPUT_NAME})

configure_include_directories()

configure_dependencies(${CMAKE_PROJECT_NAME} nlohmann_json spdlog)
